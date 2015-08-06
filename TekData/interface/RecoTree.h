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
    int    WF_samples;
    int*   WF_ch; 
    float* WF_time;
    float* WF_val;
    int    n_channels;
    int*   channels;
    float* time;
    float* amp_max;
    float* charge_tot;
    float* charge_sig;
    float* baseline;
};

RecoTree::RecoTree(int nCh, int nSamples, TString* nameMCP)
{
    tree_ = new TTree();

    time_stamp=0;
    event_id=0;
    //---set total number of WF samples 
    WF_samples = nSamples*nCh;
    WF_ch = new int[WF_samples];
    WF_time = new float[WF_samples];
    WF_val = new float[WF_samples];
    //---allocate enough space for all channels
    n_channels = nCh;
    channels = new int[nCh];
    time = new float[nCh];
    amp_max = new float[nCh];
    charge_tot = new float[nCh];
    charge_sig = new float[nCh];
    baseline = new float[nCh];
    //---global branches
    tree_->Branch("start_time", &start_time, "start_time/i");
    tree_->Branch("time_stamp", &time_stamp, "time_stamp/i");
    tree_->Branch("event_id", &event_id, "event_id/I");
    tree_->Branch("WF_samples", &WF_samples, "WF_samples/I");
    tree_->Branch("WF_ch", WF_ch, "WF_ch[WF_samples]/I");
    tree_->Branch("WF_time", WF_time, "WF_time[WF_samples]/F");
    tree_->Branch("WF_val", WF_val, "WF_val[WF_samples]/F");
    tree_->Branch("n_channels", &n_channels, "n_channels/I");
    tree_->Branch("time", time, "time[n_channels]/F");
    tree_->Branch("amp_max", amp_max, "amp_max[n_channels]/F");
    tree_->Branch("charge_tot", charge_tot, "charge_tot[n_channels]/F");
    tree_->Branch("charge_sig", charge_sig, "charge_sig[n_channels]/F");
    tree_->Branch("baseline", baseline, "baseline[n_channels]/F");
    //---channels branches
    for(int iCh=0; iCh<nCh; iCh++)
    {
        channels[iCh]=iCh;
        tree_->Branch(nameMCP[iCh], &(channels[iCh]), nameMCP[iCh]+"/I");
    }
}

RecoTree::~RecoTree()
{
    delete[] WF_ch; 
    delete[] WF_time;
    delete[] WF_val;
    delete[] channels;
    delete[] time;
    delete[] amp_max;
    delete[] charge_tot;
    delete[] charge_sig;
    delete[] baseline;

    delete tree_;
}

#endif
