#ifndef __reco_tree__
#define __reco_tree__

#include <string>

#include "TTree.h"
#include "TString.h"

using namespace std;

typedef unsigned long int uint32;
 
//****************************************************************************************

class RecoTree
{
public: 
    //---ctors---
    RecoTree(int nCh, int nSamples, TString* nameMCP);
    //---dtor---
    ~RecoTree();

    //---utils---
    void Fill() {tree_->Fill();};
    void Write(string name="reco_tree") {tree_->Write(name.c_str());}

    TTree* tree_; 

    uint32 start_time;
    uint32 time_stamp;
    int    event_id;   
    int    tot_samples;
    float* time_CF;
    float* amp_max;
    float* charge_tot;
    float* charge_sig;
    float* baseline;
    int*   WF_ch; 
    float* WF_time;
    float* WF_val;
};

RecoTree::RecoTree(int nCh, int nSamples, TString* nameMCP)
{
    tree_ = new TTree();

    time_stamp=0;
    event_id=0;
    //---set total number of WF samples
    tot_samples = nSamples*nCh;
    //---allocate enough space for all channels
    time_CF = new float[nCh];
    amp_max = new float[nCh];
    charge_tot = new float[nCh];
    charge_sig = new float[nCh];
    baseline = new float[nCh];
    WF_ch = new int[tot_samples];
    WF_time = new float[tot_samples];
    WF_val = new float[tot_samples];
    //---global branches
    tree_->Branch("start_time", &start_time, "start_time/i");
    tree_->Branch("time_stamp", &time_stamp, "time_stamp/i");
    tree_->Branch("event_id", &event_id, "event_id/I");
    tree_->Branch("tot_samples",&tot_samples, "tot_samplkes/I");
    tree_->Branch("WF_ch", WF_ch, "WF_ch[tot_samples]/I");
    tree_->Branch("WF_time", WF_time, "WF_time[tot_samples]/F");
    tree_->Branch("WF_val", WF_val, "WF_val[tot_samples]/F");
    //---channels branches
    for(int iCh=0; iCh<nCh; iCh++)
    {
	time_CF[iCh]=0;
	amp_max[iCh]=0;
	charge_tot[iCh]=0;
	charge_sig[iCh]=0;
	baseline[iCh]=0;
	tree_->Branch("time_"+nameMCP[iCh], &(time_CF[iCh]),"time_"+nameMCP[iCh]+"/F");
	tree_->Branch("amp_max_"+nameMCP[iCh], &(amp_max[iCh]), "amp_max_"+nameMCP[iCh]+"/F");
	tree_->Branch("charge_tot_"+nameMCP[iCh], &(charge_tot[iCh]), "charge_tot_"+nameMCP[iCh]+"/F");
	tree_->Branch("charge_sig_"+nameMCP[iCh], &(charge_sig[iCh]), "charge_sig_"+nameMCP[iCh]+"/F");
	tree_->Branch("baseline_"+nameMCP[iCh], &(baseline[iCh]), "baseline_"+nameMCP[iCh]+"/F");
    }
}

RecoTree::~RecoTree()
{
    delete[] time_CF;
    delete[] amp_max;
    delete[] charge_tot;
    delete[] charge_sig;
    delete[] baseline;
    delete[] WF_ch; 
    delete[] WF_time;
    delete[] WF_val;

    delete tree_;
}

#endif
