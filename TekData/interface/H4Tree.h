#ifndef __H4_TREE__
#define __H4_TREE__

#include <string>

#include "TTree.h"
#include "TString.h"

using namespace std;

typedef unsigned long int uint32;
typedef unsigned long long int uint64;
 
//****************************************************************************************

class H4Tree
{
public: 
    //---ctors---
    H4Tree() {};
    H4Tree(TTree* tree);
    //---dtor---
    ~H4Tree() {};

    //---utils---
    bool NextEvt(int64_t entry=-1);

    TTree* tree_; 
    int    currentEntry_;
    
    uint64       evtTime;
    unsigned int spillNumber;
    unsigned int nDigiSamples;
    float*       digiSampleValue;
};

H4Tree::H4Tree(TTree* tree)
{
    tree_ = tree;
    currentEntry_=-1;

    nDigiSamples = 0;    
    tree_->SetBranchAddress("nDigiSamples", &nDigiSamples);
    tree_->GetEntry(0);

    evtTime = 0;
    spillNumber = 0;
    digiSampleValue = new float[nDigiSamples];
    tree_->SetBranchAddress("evtTime", &evtTime);
    tree_->SetBranchAddress("spillNumber", &spillNumber);
    tree_->SetBranchAddress("digiSampleValue", digiSampleValue);
}

bool H4Tree::NextEvt(int64_t entry)
{
    if(entry > -1)
        currentEntry_ = entry;
    
    ++currentEntry_;    
    if(currentEntry_ < tree_->GetEntriesFast())
    {
        tree_->GetEntry(currentEntry_);
        return true;
    }
    
    currentEntry_=-1;
    return false;
}

#endif
