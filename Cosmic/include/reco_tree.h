#ifndef __reco_tree__
#define __reco_tree__

#include <cstdlib>

#include <iostream>
#include <string>

#include "TTree.h"

#include "analysis_tools.h"

using namespace std;
 
//****************************************************************************************
//*** initialize all the pointer addressed by the outTree branches ***

int event_id=0;
int tot_samples=0;
float* time_CF;
float* amp_max;
float* charge_tot;
float* charge_sig;
float* baseline;
int* WF_ch; 
float* WF_time;
float* WF_val;

void SetOutTree(TTree* outTree, TString* nameMCP, int nCh)
{
    //---set total number of WF samples
    tot_samples = TOT_SAMPLES*nCh;
    //---allocate enough space for all channels
    time_CF = (float*)malloc(sizeof(float)*nCh);
    amp_max = (float*)malloc(sizeof(float)*nCh);
    charge_tot = (float*)malloc(sizeof(float)*nCh);
    charge_sig = (float*)malloc(sizeof(float)*nCh);
    baseline = (float*)malloc(sizeof(float)*nCh);
    WF_ch = (int*)malloc(sizeof(int)*nCh*TOT_SAMPLES);
    WF_time = (float*)malloc(sizeof(float)*nCh*TOT_SAMPLES);
    WF_val = (float*)malloc(sizeof(float)*nCh*TOT_SAMPLES);
    //---global branches
    outTree->Branch("event_id", &event_id, "event_id/I");
    outTree->Branch("tot_samples",&tot_samples, "tot_samples/I");
    outTree->Branch("WF_ch", WF_ch, "WF_ch[tot_samples]/I");
    outTree->Branch("WF_time", WF_time, "WF_time[tot_samples]/F");
    outTree->Branch("WF_val", WF_val, "WF_val[tot_samples]/F");
    //---channels branches
    for(int iCh=0; iCh<nCh; iCh++)
    {
	time_CF[iCh]=0;
	amp_max[iCh]=0;
	charge_tot[iCh]=0;
	charge_sig[iCh]=0;
	baseline[iCh]=0;
	outTree->Branch("time_"+nameMCP[iCh], &(time_CF[iCh]),"time_"+nameMCP[iCh]+"/F");
	outTree->Branch("amp_max_"+nameMCP[iCh], &(amp_max[iCh]), "amp_max_"+nameMCP[iCh]+"/F");
	outTree->Branch("charge_tot_"+nameMCP[iCh], &(charge_tot[iCh]), "charge_tot_"+nameMCP[iCh]+"/F");
	outTree->Branch("charge_sig_"+nameMCP[iCh], &(charge_sig[iCh]), "charge_sig_"+nameMCP[iCh]+"/F");
	outTree->Branch("baseline_"+nameMCP[iCh], &(baseline[iCh]), "baseline_"+nameMCP[iCh]+"/F");
    }
}

void SetInTree(TTree* inTree, TString* nameMCP, int nCh)
{

    //---set total number of WF samples
    tot_samples = TOT_SAMPLES*nCh;
    //---allocate enough space for all channels
    time_CF = (float*)malloc(sizeof(float)*nCh);
    amp_max = (float*)malloc(sizeof(float)*nCh);
    charge_tot = (float*)malloc(sizeof(float)*nCh);
    charge_sig = (float*)malloc(sizeof(float)*nCh);
    baseline = (float*)malloc(sizeof(float)*nCh);
    WF_ch = (int*)malloc(sizeof(int)*nCh*TOT_SAMPLES);
    WF_time = (float*)malloc(sizeof(float)*nCh*TOT_SAMPLES);
    WF_val = (float*)malloc(sizeof(float)*nCh*TOT_SAMPLES);
    //---global branches
    inTree->SetBranchAddress("event_id", &event_id);
    inTree->SetBranchAddress("tot_samples",&tot_samples);
    inTree->SetBranchAddress("WF_ch", WF_ch);
    inTree->SetBranchAddress("WF_time", WF_time);
    inTree->SetBranchAddress("WF_val", WF_val);
    //---channels branches
    for(int iCh=0; iCh<nCh; iCh++)
    {
	time_CF[iCh]=0;
	amp_max[iCh]=0;
	charge_tot[iCh]=0;
	charge_sig[iCh]=0;
	baseline[iCh]=0;
	inTree->SetBranchAddress("time_"+nameMCP[iCh], &(time_CF[iCh]));
	inTree->SetBranchAddress("amp_max_"+nameMCP[iCh], &(amp_max[iCh]));
	inTree->SetBranchAddress("charge_tot_"+nameMCP[iCh], &(charge_tot[iCh]));
	inTree->SetBranchAddress("charge_sig_"+nameMCP[iCh], &(charge_sig[iCh]));
	inTree->SetBranchAddress("baseline_"+nameMCP[iCh], &(baseline[iCh]));
    }
}

#endif
