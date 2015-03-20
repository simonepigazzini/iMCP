#include <stdlib.h>
#include <cmath>
#include <iostream>

//---ROOT
#include "TStyle.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TProfile.h"
#include "TMath.h"
#include "TF1.h"
#include "TCanvas.h"
//---RooFit
#include "RooRealVar.h"
#include "RooAbsReal.h"
#include "RooArgSet.h"
#include "RooArgList.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooUniform.h"
#include "RooAddPdf.h"
#include "RooExtendPdf.h"
#include "RooWorkspace.h"
#include "RooFitResult.h"
#include "RooPlot.h"

#include "../include/reco_tree.h"

//---define MCP ch
#define TRIG1 0
#define TRIG2 1
#define PLANACON 2
#define CUT_SPIKE 5    //2
#define CUT_PLANA 0.03 //0.03

using namespace std;
using namespace RooFit;

//****************************************************************************************
//*** analyze double coincidence ***

RooWorkspace* swTrigger(TTree* in_tree, float t_low=14, float t_high=17)
{
    //-----Setup RooFit Object-----
    //---variables
    RooRealVar rrv_time_diff("rrv_time_diff", "Triggers time difference",
			     t_low, t_high, "ns");
    RooRealVar G_mean("G_mean", "G_mean", (t_low+t_high)/2, t_low, t_high);
    RooRealVar G_sigma("G_sigma", "G_sigma", 0.02, 0.001, 1.);
    RooRealVar trig_num("trig_num", "trig_num", 5., 0., 10000.);
    RooRealVar fake_num("fake_num", "fake_num", 10., 0., 10000.);
    RooRealVar norm("norm", "global normalization", 10., 0., 10000.);
    //---dataset
    RooDataSet rds_double_coinc("rds_double_coinc", "Triggers time difference", 
				rrv_time_diff);
    //---PDFs
    RooGaussian G_trig("G_trig", "coincidece peak", rrv_time_diff, G_mean, G_sigma);
    RooUniform U_fake("U_fake", "fake rate", rrv_time_diff);
    RooAddPdf fit_func("fit_func", "combined fit function", 
		       RooArgList(G_trig, U_fake), RooArgList(trig_num, fake_num));
    //---Workspace
    RooWorkspace* rws_sw_trig = new RooWorkspace("rws_sw_trig");
    //-----Read tree-----
    for(int iEntry=0; iEntry<in_tree->GetEntriesFast(); iEntry++)
    {
	in_tree->GetEntry(iEntry);
	double time_diff = time_CF[TRIG2]-time_CF[TRIG1];
	if(charge_tot[PLANACON] < CUT_SPIKE && time_diff > t_low && time_diff < t_high)
	{
	    rrv_time_diff.setVal(time_diff);
	    rds_double_coinc.add(rrv_time_diff);
	}
    }
    //-----Fit and save-----
    fit_func.fitTo(rds_double_coinc, Extended(kTRUE));
    rws_sw_trig->import(rds_double_coinc);
    rws_sw_trig->import(fit_func);
    //---exit
    return rws_sw_trig;
}

//****************************************************************************************
//*** amplitude walk correction ***

void ampCorrection(TTree* in_tree, float c_cut_l, float c_cut_h, float* corr_par)
{
    TF1* fit_corr = new TF1("fit_corr", "pol1", 0, 2);
    TProfile* time_vs_amp = new TProfile("pr_t_vs_A", "pr_t_vs_A", 15, CUT_PLANA, 2);
    TH1F* backup_histo = new TH1F("backup", "backup", 1000, 5, 15);
    char cut[200];
    sprintf(cut, "charge_tot_Planacon < %f && (time_MiB2-time_MiB1) > %f && "
	    "(time_MiB2-time_MiB1) < %f && charge_sig_Planacon > %f", 
	    float(CUT_SPIKE), c_cut_l, c_cut_h, float(CUT_PLANA));
    char var[100];
    sprintf(var, "(time_MiB2-time_Planacon):charge_sig_Planacon>>pr_t_vs_A");
    in_tree->Draw(var, cut);
    sprintf(var, "(time_MiB2-time_Planacon)>>backup");
    in_tree->Draw(var, cut);
    time_vs_amp->Fit(fit_corr);
	
    //---store correction parameters
    corr_par[0] = -fit_corr->GetParameter(0);
    corr_par[1] = -fit_corr->GetParameter(1);

    if(fit_corr->GetParameter(1) <= fit_corr->GetParError(1) || 
	corr_par[0] != corr_par[0] && corr_par[1] != corr_par[1])
    {
	corr_par[0] = -backup_histo->GetMean();
	corr_par[1] = 0;
    }
    time_vs_amp->Delete();
}

//****************************************************************************************
//*** analyze triple coincidence ***

RooWorkspace* analyzeTestMCP(TTree* in_tree, float* amp_corr, 
			     float c_cut_l, float c_cut_h)
{
    //-----Setup RooFit Object-----
    //---variables
    float t_low=-3, t_high=3;
    RooRealVar rrv_time_mcp("rrv_time_mcp", "Test MCP and trig2 time difference",
			    t_low, t_high, "ns");
    RooRealVar G_mean_t("G_mean_t", "G_mean_t", (t_high+t_low)/2, t_low, t_high);
    RooRealVar G_sigma_t("G_sigma_t", "G_sigma_t", 0.05, 0.001, 1);
    RooRealVar G_norm("G_norm", "G_norm", 10, 0., 10000.);
    RooRealVar U_norm("U_norm", "U_norm", 10, 0., 10000.);
    //---dataset
    RooDataSet rds_triple_coinc("rds_triple_coinc", "Test MCP and trig2 time difference", 
				rrv_time_mcp);
    //---PDFs
    RooGaussian G_triple("G_triple", "coincidece peak", rrv_time_mcp, 
			 G_mean_t, G_sigma_t);
    RooUniform U_fake_t("U_fake_t", "U_fake_t", rrv_time_mcp);
    RooAddPdf fit_func_t("fit_func_T", "combined fit function", 
			 RooArgList(G_triple, U_fake_t), RooArgList(G_norm, U_norm));
    //RooExtendPdf eG_triple("eG_triple", "coincidece peak", G_triple, G_norm);  
    //---Workspace
    RooWorkspace* rws_test_mcp = new RooWorkspace("rws_test_mcp");
    //-----Read tree-----
    int count_t=0;
    for(int iEntry=0; iEntry<in_tree->GetEntriesFast(); iEntry++)
    {
	in_tree->GetEntry(iEntry);
	double time_trig = time_CF[TRIG2]-time_CF[TRIG1];
	double time_diff = time_CF[TRIG2]-time_CF[PLANACON];
	if(charge_tot[PLANACON] < CUT_SPIKE && charge_sig[PLANACON] > CUT_PLANA && 
	   time_trig > c_cut_l && time_trig < c_cut_h)
	{
	    float corr_val = time_diff+amp_corr[1]*charge_sig[PLANACON]+amp_corr[0];
	    if(corr_val > t_low && corr_val < t_high)
	    {
		rrv_time_mcp.setVal(corr_val);
		rds_triple_coinc.add(rrv_time_mcp);
		count_t++;
	    }
	}
    }
    //-----Fit and save-----
    fit_func_t.fitTo(rds_triple_coinc, Extended(kTRUE));
    rws_test_mcp->import(rds_triple_coinc);
    rws_test_mcp->import(fit_func_t);
    //---exit
    cout << "rough count:   " << count_t << endl; 
    return rws_test_mcp;
}

//****************************************************************************************
//*** Draw interesting plot ***

void drawPlots(TString& run_name, RooWorkspace* rws_sw_trig, RooWorkspace* rws_test_mcp)
{
    TCanvas* c1 = new TCanvas();
    RooPlot* d_frame = (rws_sw_trig->var("rrv_time_diff"))->frame();
    (rws_sw_trig->data("rds_double_coinc"))->plotOn(d_frame, Binning(80));
    (rws_sw_trig->pdf("fit_func"))->plotOn(d_frame);
    cout << d_frame->chiSquare(4) << endl;
    d_frame->Draw();
    c1->Print("./plots/"+run_name+"_trig.pdf","pdf");
    TCanvas* c2 = new TCanvas();
    RooPlot* t_frame = (rws_test_mcp->var("rrv_time_mcp"))->frame();
    (rws_test_mcp->data("rds_triple_coinc"))->plotOn(t_frame, Binning(50));
    (rws_test_mcp->pdf("G_triple"))->plotOn(t_frame);
    t_frame->Draw();
    cout << t_frame->chiSquare(4) << endl;
    c2->Print("./plots/"+run_name+"_test.pdf","pdf");
}

//****************************************************************************************
//*** compute efficiency and time resolution
void computeResults(RooWorkspace* rws_sw_trig, RooWorkspace* rws_test_mcp,
		    float c_cut_l, float c_cut_h)
{
    //---compute doubles & triples
    (rws_sw_trig->var("rrv_time_diff"))->setRange("trig_window", c_cut_l, c_cut_h);
    RooAbsReal* sig_frac_window = (rws_sw_trig->pdf("G_trig"))->
	createIntegral(*rws_sw_trig->var("rrv_time_diff"),
		       *rws_sw_trig->var("rrv_time_diff"), "trig_window");
    double n_trig = rws_sw_trig->var("trig_num")->getVal()*sig_frac_window->getVal();
    double n_fake = rws_sw_trig->var("fake_num")->getVal()*(c_cut_h-c_cut_l)/3;
    double n_triple = rws_test_mcp->var("G_norm")->getVal();
    double eff = n_triple/n_trig;
    double err_eff = sqrt(eff*(1-eff)/n_trig);
    //---compute res
    double trig_res = rws_sw_trig->var("G_sigma")->getVal()/sqrt(2);
    double test_res = sqrt(pow(rws_test_mcp->var("G_sigma_t")->getVal(),2) - 
			   pow(trig_res, 2));

    //---print results
    cout << "*************************" << endl
	 << "eff:       " << eff << "%" << endl
	 << "        +- " << err_eff <<"%" << endl
	 << "doppie:    " << n_trig << " ev" << endl
	 << "fake:      " << n_fake << " ev" << endl
	 << "triple:    " << n_triple << " ev" << endl
	 << "-------------------------" << endl
	 << "res_trig:  " << trig_res*1000 << " ps" << endl
	 << "res_Plana: " << test_res*1000 << " ps" << endl
	 << "*************************" << endl;
}

//****************************************************************************************
//*** MAIN ***

int main(int argc, char* argv[])
{
    //-----Startup check-----
    if(argc < 2)
    {
	cout << "ERROR! --- specify input file" << endl;
	return 0;
    }
    //-----Definitions-----
    int nCh=3;
    float d_sigma, d_mean, c_cut_l, c_cut_h;
    float amp_corr[2]={5,0};
    TString name_MCP[3]={"MiB1", "MiB2", "Planacon"};
    //---load data file
    TString run_name(argv[1]);
    TFile* in_file = TFile::Open("ntuples/"+run_name+".root");
    TTree* in_tree = (TTree*)in_file->Get("reco_tree");
    SetInTree(in_tree, name_MCP, nCh);
    //---RooFit workspace
    RooWorkspace* rws_sw_trig;
    RooWorkspace* rws_test_mcp;
    
    //---trigger
    rws_sw_trig = swTrigger(in_tree, atof(argv[2]), atof(argv[3]));
    d_mean = (rws_sw_trig->var("G_mean"))->getVal();
    d_sigma = (rws_sw_trig->var("G_sigma"))->getVal();
    c_cut_l = d_mean - 3*d_sigma;
    c_cut_h = d_mean + 3*d_sigma;
    ampCorrection(in_tree, c_cut_l, c_cut_h, amp_corr);
    //---triple
    rws_test_mcp = analyzeTestMCP(in_tree, amp_corr, c_cut_l, c_cut_h);
    //---draw plots
    drawPlots(run_name, rws_sw_trig, rws_test_mcp);
    //---print results
    computeResults(rws_sw_trig, rws_test_mcp, c_cut_l, c_cut_h);
}
			    
    
