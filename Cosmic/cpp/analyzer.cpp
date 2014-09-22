#include <stdlib.h>
#include <cmath>
#include <iostream>

//---ROOT
#include "TStyle.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TProfile.h"
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
#include "RooPlot.h"

#include "../include/reco_tree.h"

//---define MCP ch
#define TRIG1 0
#define TRIG2 1
#define PLANACON 2

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
    RooRealVar G_mean("G_mean", "G_mean", t_low+1.5, t_low+1, t_high-1);
    RooRealVar G_sigma("G_sigma", "G_sigma", 0.05, 0.001, 1);
    RooRealVar trig_num("trig_num", "trig_num", 10., 0., 10000.);
    RooRealVar fake_num("fake_num", "fake_num", 10., 0., 10000.);
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
	if(charge_tot[PLANACON] < 5 && time_diff > t_low && time_diff < t_high)
	{
	    rrv_time_diff.setVal(time_diff);
	    rds_double_coinc.add(rrv_time_diff);
	}
    }
    //-----Fit and save-----
    fit_func.fitTo(rds_double_coinc);
    rws_sw_trig->import(rds_double_coinc);
    rws_sw_trig->import(fit_func);
    //---exit
    return rws_sw_trig;
}

//****************************************************************************************
//*** analyze triple coincidence ***

RooWorkspace* analyzeTestMCP(TTree* in_tree, float c_cut_l, float c_cut_h,
			     float t_low=10, float t_high=13)
{
    //-----Setup RooFit Object-----
    //---variables
    RooRealVar rrv_time_mcp("rrv_time_mcp", "Test MCP and trig2 time difference",
			    t_low, t_high, "ns");
    RooRealVar G_mean_t("G_mean_t", "G_mean_t", t_low+1.5, t_low+1, t_high);
    RooRealVar G_sigma_t("G_sigma_t", "G_sigma_t", 0.05, 0.001, 1);
    RooRealVar G_norm("G_norm", "G_norm", 10, 0., 10000.);
    //---dataset
    RooDataSet rds_triple_coinc("rds_triple_coinc", "Test MCP and trig2 time difference", 
				rrv_time_mcp);
    //---PDFs
    RooGaussian G_triple("G_triple", "coincidece peak", rrv_time_mcp, 
			 G_mean_t, G_sigma_t);
    RooExtendPdf eG_triple("eG_triple", "coincidece peak", G_triple, G_norm);  
    //---Workspace
    RooWorkspace* rws_test_mcp = new RooWorkspace("rws_test_mcp");
    //-----Read tree-----
    for(int iEntry=0; iEntry<in_tree->GetEntriesFast(); iEntry++)
    {
	in_tree->GetEntry(iEntry);
	double time_trig = time_CF[TRIG2]-time_CF[TRIG1];
	double time_diff = time_CF[TRIG2]-time_CF[PLANACON];
	if(charge_tot[PLANACON] < 5 && charge_sig[PLANACON] > 0.04 && 
	   time_trig > c_cut_l && time_trig < c_cut_h &&
					      time_diff > t_low && time_diff < t_high)
	{
	    rrv_time_mcp.setVal(time_diff);
	    rds_triple_coinc.add(rrv_time_mcp);
	}
    }
    //-----Fit and save-----
    eG_triple.fitTo(rds_triple_coinc, Range(t_low+1.7, t_high-0.9));
    rws_test_mcp->import(rds_triple_coinc);
    rws_test_mcp->import(eG_triple);
    //---exit
    return rws_test_mcp;
}

//****************************************************************************************
//*** Draw interesting plot ***

void drawPlots(TString& run_name, RooWorkspace* rws_sw_trig, RooWorkspace* rws_test_mcp)
{
    TCanvas* c1 = new TCanvas();
    RooPlot* d_frame = (rws_sw_trig->var("rrv_time_diff"))->frame();
    (rws_sw_trig->data("rds_double_coinc"))->plotOn(d_frame, Binning(100));
    (rws_sw_trig->pdf("fit_func"))->plotOn(d_frame);
    d_frame->Draw();
    c1->Print("./plots/"+run_name+"_trig.pdf","pdf");
    TCanvas* c2 = new TCanvas();
    RooPlot* t_frame = (rws_test_mcp->var("rrv_time_mcp"))->frame();
    (rws_test_mcp->data("rds_triple_coinc"))->plotOn(t_frame, Binning(40));
    (rws_test_mcp->pdf("G_triple"))->plotOn(t_frame);
    t_frame->Draw();
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
    double eff = n_triple/(n_trig-n_fake);
    double err_eff = sqrt(eff*(1-eff)/(n_trig-n_fake));
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
    //---triple
    rws_test_mcp = analyzeTestMCP(in_tree, c_cut_l, c_cut_h, 
				  atof(argv[4]), atof(argv[5]));
    //---draw plots
    drawPlots(run_name, rws_sw_trig, rws_test_mcp);
    //---print results
    computeResults(rws_sw_trig, rws_test_mcp, c_cut_l, c_cut_h);
}
			    
    
