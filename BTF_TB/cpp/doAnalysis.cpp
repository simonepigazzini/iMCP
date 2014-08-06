/*****************************************************************************************
    This program compute charge or efficiency for all the run in a selected Scan for
    selected MCP under test. Also the hodoscope cut are selectable.

    compile with ---> c++ -o doAnalysis cpp/doAnalysis.cpp `root-config --cflags --glibs --libs`
    
    run with the proper python launcher

 ****************************************************************************************/

#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>

#include "TROOT.h"
#include "TSystem.h"
#include "TKey.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TString.h"
#include "TCut.h"
#include "TMath.h"
#include "TApplication.h"
#include "TCanvas.h"

using namespace std;

int main(int argc, char** argv)
{
    //-------Config-----------------------------------------------------------------------
    gSystem->Load("libTree");
    char *label, *thType, *MCP, *hodo_cut, *doWhat, ref1[10], ref2[10];
    int th_ref1=10000, th_ref2=10000, th_mcp=10000;
    thType = argv[1];
    hodo_cut = argv[2];
    MCP = argv[3];
    doWhat = argv[4];
    label = argv[5];
    sprintf(ref1, "MiB1");
    sprintf(ref2, "Roma2");

    //-------ROOT Object definitions------------------------------------------------------
    TCanvas* c1 = new TCanvas();
    c1->cd();
    TFile* inFile = TFile::Open("ntuples/reco_"+TString(label)+".root","r");
    TTree* nt = (TTree*)inFile->Get("reco_tree");
    TH1F* h_sig= new TH1F("h_sig","h_sig",500,-25000,5000);
    TH1F* h_base = new TH1F("h_base","h_base",500,-25000,5000);
    TH1F* h_trig1 = new TH1F("h_trig1","h_trig1",500,-25000,5000);
    TH1F* h_trig0 = new TH1F("h_trig0","h_trig0",500,-25000,5000);

    //-------Set Scan configuration-------------------------------------------------------
    int c=0;
    int rid[20];
    float step[20];
    //---CuLow
    if(strcmp(label,"CuLow") == 0)
    {
	rid[0]=141; rid[1]=179; rid[2]=186; rid[3]=191; rid[4]=196; rid[5]=197; rid[6]=200; rid[7]=202; rid[8]=206; rid[9]=208; rid[10]=210;
	step[0]=0; step[1]=1*0.64; step[2]=2*0.64; step[3]=3*0.64; step[4]=4*0.64; step[5]=5*0.64; 
	step[6]=6*0.64; step[7]=7*0.64; step[8]=8*0.64; step[9]=9*0.64; step[10]=11*0.64;
	c=11;
    }    
    //---CuHigh
    else if(strcmp(label,"CuHigh") == 0)
    {
	rid[0]=261; rid[1]=189; rid[2]=193; rid[3]=195; rid[4]=199; rid[5]=201; rid[6]=203; rid[7]=207; rid[8]=209; rid[9]=211;
	step[0]=0; step[1]=2*0.64; step[2]=3*0.64; step[3]=4*0.64; step[4]=5*0.64; 
	step[5]=6*0.64; step[6]=7*0.64; step[7]=8*0.64; step[8]=9*0.64; step[9]=11*0.64;
	c=10;
    }
    //---PbLow
    else if(strcmp(label,"PbLow") == 0)
    {    
	rid[0]=213; rid[1]=215; rid[2]=218; rid[3]=226; rid[4]=228; rid[5]=230; rid[6]=235; rid[7]=238;
	step[0]=0*0.71; step[1]=1*0.71; step[2]=2*0.71; step[3]=3*0.71; step[4]=4*0.71; step[5]=5*0.71; step[6]=7*0.71; step[7]=9*0.71;
	c=8;
    }
    //---PbHigh
    else if(strcmp(label,"PbHigh") == 0)
    {
	rid[0]=214; rid[1]=216; rid[2]=219; rid[3]=227; rid[4]=229; rid[5]=231; rid[6]=236; rid[7]=239;
	step[0]=0; step[1]=1*0.71; step[2]=2*0.71; step[3]=3*0.71; step[4]=4*0.71; step[5]=5*0.71; step[6]=7*0.71; step[7]=9*0.71;
	c=8;
    }
    //---Scan1
    else if(strcmp(label,"Scan1") == 0)
    {
	rid[0]=68; rid[1]=70; rid[2]=71; rid[3]=74; rid[4]=75; 
	rid[5]=76; rid[6]=77; rid[7]=78; rid[8]=79; rid[9]=80;
	rid[10]=89; rid[11]=90; rid[12]=91; rid[13]=92; rid[14]=93; rid[15]=94;
	step[0]=2800; step[1]=2700; step[2]=2600; step[3]=2500; step[4]=2400; step[5]=2300; 
	step[6]=2200; step[7]=2100; step[8]=2000; step[9]=2050; step[10]=2150; 
	step[10]=2250; step[11]=2350; step[12]=2450; step[13]=2550; step[14]=2650; 
	c=15;
    }
    //---Scan2
    else if(strcmp(label,"Scan2") == 0)
    {
	rid[0]=97; rid[1]=98; rid[2]=101; rid[3]=102; rid[4]=103; 
	rid[5]=104; rid[6]=105; rid[7]=106; rid[8]=107; rid[9]=108;
	step[0]=2750; step[1]=2650; step[2]=2550; step[3]=2450; step[4]=2350; 
	step[5]=2250; step[6]=2150; step[7]=2050; step[8]=1950; step[9]=1850;  
	c=10;
    }
    //---Scan3
    else if(strcmp(label,"Scan3") == 0)
    {
	rid[0]=141; rid[1]=144; rid[2]=145; rid[3]=147; rid[4]=148; 
	rid[5]=150; rid[6]=151; rid[7]=152; rid[8]=153; rid[9]=157;
	step[0]=2800; step[1]=2700; step[2]=2600; step[3]=2500; step[4]=2400; 
	step[5]=2300; step[6]=2200; step[7]=2100; step[8]=2000; step[9]=2750;  
	sprintf(ref2, "MiB3");
	c=10;
    }
    //---Scan4
    else if(strcmp(label,"Scan4") == 0)
    {
	rid[0]=247; rid[1]=248; rid[2]=249; rid[3]=250; rid[4]=251; rid[5]=252; 
	rid[6]=254; rid[7]=259; rid[8]=260; rid[9]=261; rid[10]=262;
	step[0]=2000; step[1]=2100; step[2]=2200; step[3]=2300; step[4]=2400; step[5]=2500; step[6]=2600; step[7]=2700;
	step[8]=2800; step[9]=2900; step[10]=3000;
	c=11;
    }

    //------Build TCut and draw variables--------------------------------------------------
    char str_cut_sig[200]="";
    char str_cut_sig_2D[200]="";
    char str_cut_trig0[200]="";
    char str_cut_trig1[200]="";
    char str_cut_hodoX[200]="";
    char str_cut_hodoY[200]="";
    char var_sig[100]="";
    char var_base[100]="";
    //-----Draw variables-----
    sprintf(var_sig, "charge_%s>>h_sig", MCP);
    sprintf(var_base, "charge_%s>>h_base", MCP);
    //-----Draw cut-----
    //---HV Scan
    if(TString(label).Contains("Scan") == 1)
    {  
	sprintf(str_cut_sig, "charge_%s < th_%s_%s", MCP, thType, MCP);
	if(strcmp(MCP, "MiB2") == 0)
	    sprintf(str_cut_sig_2D, "-charge_%s > -13.28*amp_max_%s - 350", MCP, MCP);
	sprintf(str_cut_trig0, "charge_%s > th_%s_%s  && charge_%s > th_%s_%s && sci_front_adc < 500",ref1, thType, ref1, ref2, thType, ref2);
	sprintf(str_cut_trig1, "charge_%s < th_%s_%s  && charge_%s < th_%s_%s && sci_front_adc > 500 && sci_front_adc < 1500",ref1, thType, ref1, ref2, thType, ref2);
    }
    //---Absorber Scan
    else if(TString(label).Contains("Cu") == 1 || TString(label).Contains("Pb") == 1)
    {
	sprintf(str_cut_sig, "charge_%s < th_%s_%s", MCP, thType, MCP);
	if(strcmp(MCP, "MiB2") == 0)
	    sprintf(str_cut_sig_2D, "-charge_%s > -13.28*amp_max_%s - 350", MCP, MCP);
	sprintf(str_cut_trig0, "charge_%s > th_%s_%s && sci_front_adc < 500", ref1, thType, ref1);
	sprintf(str_cut_trig1, "charge_%s < th_%s_%s && sci_front_adc > 500 && sci_front_adc < 1500", ref1, thType, ref1);
    }
    //---Pre-shower configuration
    /*
    else if(TString(label).Contains("PreSH") == 1)
    {
	sprintf(str_cut_sig, "charge_%s < th_%s_%s && charge_%s < th_%s_%s && -charge_%s > -13.28*amp_max_%s - 350 && sci_front_adc > 500 && sci_front_adc < 1500 && run_id == %d",ref1, thType, ref1, MCP, thType, MCP, MCP, MCP, rid[i]);
	sprintf(str_cut_charge, "charge_%s < th_%s_%s && -charge_%s > -13.28*amp_max_%s - 350 && sci_front_adc > 500 && sci_front_adc < 1500 && run_id == %d", ref1, thType, ref1, MCP, MCP, rid[i]);
	sprintf(str_cut_base, "charge_%s > th_%s_%s && charge_%s < th_%s_%s && -charge_%s > -13.28*amp_max_%s - 350 && sci_front_adc < 500 && run_id == %d", ref1, thType, ref1, MCP, thType, MCP, MCP, MCP, rid[i]);
	sprintf(str_cut_trig0, "charge_%s > th_%s_%s && sci_front_adc < 500 && run_id == %d",
		ref1, thType, ref1, rid[i]);
	sprintf(str_cut_trig1, "charge_%s < th_%s_%s && sci_front_adc > 500 && sci_front_adc < 1500 && run_id == %d",
		ref1, thType, ref1, rid[i]);
		}*/
    //---Hodoscope cut
    float thX[8]={113,95,127,118,94,134,133,160};
    float thY[8]={110,101,119,136,94,98,87,97};
    if(strcmp(hodo_cut,"all") == 0)
    {
	sprintf(str_cut_hodoX, "(fibreX[0] > %f || fibreX[1] > %f || fibreX[2] > %f || fibreX[3] > %f || fibreX[4] > %f || fibreX[5] > %f || fibreX[6] > %f || fibreX[7] > %f)", thX[0], thX[1], thX[2], thX[3], thX[4], thX[5], thX[6], thX[7]);
	sprintf(str_cut_hodoY, "(fibreY[0] > %f || fibreY[1] > %f || fibreY[2] > %f || fibreY[3] > %f || fibreY[4] > %f || fibreY[5] > %f || fibreY[6] > %f || fibreY[7] > %f)", thY[0], thY[1], thY[2], thY[3], thY[4], thY[5], thY[6], thY[7]);
    }
    else if(strcmp(hodo_cut,"tight") == 0)
    {
	sprintf(str_cut_hodoX, "(fibreX[3] > %f || fibreX[4] > %f)", thX[3], thX[4]);
	sprintf(str_cut_hodoY, "(fibreY[3] > %f || fibreY[4] > %f)", thY[3], thY[4]);
    }
    //-----construct TCut-----
    TCut cut_sig = str_cut_sig;
    TCut cut_sig_2D = str_cut_sig_2D;
    TCut cut_trig0 = str_cut_trig0;
    TCut cut_trig1 = str_cut_trig1;
    TCut cut_hodoX = str_cut_hodoX;
    TCut cut_hodoY = str_cut_hodoY;

    //-------Runs loop---------------------------------------------------------------------
    for(int i=0; i<c; i++)
    {
	//---Run cut
	char cut_run[20];
	sprintf(cut_run, "run_id == %d", rid[i]);
     	//-----Draw and print infos-----
	nt->Draw(var_sig, cut_trig1 && cut_sig && cut_sig_2D && cut_hodoX && cut_hodoY && cut_run);
	nt->Draw(var_base, cut_trig0 && cut_sig && cut_sig_2D && cut_hodoX && cut_hodoY && cut_run);
	nt->Draw("charge_MiB1>>h_trig0",cut_trig0 && cut_hodoX && cut_hodoY && cut_run);
	nt->Draw("charge_MiB1>>h_trig1",cut_trig1 && cut_hodoX && cut_hodoY && cut_run);
	float eff = (h_sig->GetEntries()-h_base->GetEntries()*h_trig1->GetEntries()/h_trig0->GetEntries())/h_trig1->GetEntries();
	//---Eff study
	if(strcmp(doWhat,"eff") == 0)
	{
	    char var_name[3] = "X0";
	    if(TString(label).Contains("Scan") == 1)
		sprintf(var_name, "HV");
	    if(i == 0)
	    {
		printf("---------Efficiency----------\n");
		printf(" %s\teff\te_%s\te_eff\n", var_name, var_name);
		printf("-----------------------------\n");
	    }
	    if(TString(label).Contains("Scan") == 1)
		printf("%.0f\t%.3f\t%.3f\t%.3f\n", step[i], eff, 0., TMath::Sqrt((eff*(1-eff))/h_trig1->GetEntries()));
	    else
		printf("%.3f\t%.3f\t%.3f\t%.3f\n", step[i], eff, 0., TMath::Sqrt((eff*(1-eff))/h_trig1->GetEntries()));
	    if(i == c-1)    
		printf("-----------------------------\n");
	}
	//---Charge study
	else if(strcmp(doWhat,"Q") == 0)
	{
	    char var_name[3] = "X0";
	    if(TString(label).Contains("Scan") == 1)
		sprintf(var_name, "HV");
	    if(i == 0)
	    {
		printf("---------Efficiency----------\n");
		printf(" %s\tQ\te_%s\te_Q\n", var_name, var_name);
		printf("-----------------------------\n");
	    }
	    nt->Draw(var_sig, cut_trig1 && cut_sig_2D && cut_hodoX && cut_hodoY && cut_run);
	    if(TString(label).Contains("Scan") == 1)
		printf("%.0f\t%.0f\t%.0f\t%.0f\n", step[i], -h_sig->GetMean(), 0., h_sig->GetMeanError());
	    else
		printf("%.3f\t%.0f\t%.0f\t%.0f\n", step[i], -h_sig->GetMean(), 0., h_sig->GetMeanError());
	    if(i == c-1)    
		printf("-----------------------------\n");
	}
	//---Time study 
	// TODO
    }
}
