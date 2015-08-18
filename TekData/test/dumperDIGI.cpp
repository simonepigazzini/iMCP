#include <string>

#include "TString.h"
#include "TFile.h"
#include "TTree.h"

#include "../interface/CfgManager.h"
#include "../interface/H4Tree.h"
#include "../interface/RecoTree.h"
#include "../interface/WFClass.h"

using namespace std;

//**********MAIN**************************************************************************

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        cout << argv[0] << " cfg file " << "[run]" << endl; 
        return -1;
    }

    //---load options---
    CfgManager opts;
    //---defaults
    opts.ParseConfigFile("cfg/dumper_default.cfg");
    //---current
    opts.ParseConfigFile(argv[1]);
    cout << opts << endl;
    //---data opts
    string path=opts.GetOpt<string>("global", "path2data");
    string run=opts.GetOpt<string>("global", "run");
    if(argc > 2)
        run = argv[2];
    //---channels opts
    int maxEvents = opts.GetOpt<int>("global", "maxEvents");
    int nCh = opts.GetOpt<int>("global", "nCh");
    int nSamples = opts.GetOpt<int>("global", "nSamples");
    float tUnit = opts.GetOpt<float>("global", "tUnit");
    int* chPolarity = new int[nCh];
    TString* nameMCP = new TString[nCh];    
    for(int jCh=0; jCh<nCh; ++jCh)
    {
        nameMCP[jCh] = opts.GetOpt<string>("global", string("Ch")+to_string(jCh));
        chPolarity[jCh] = opts.GetOpt<int>("global", string("Ch")+to_string(jCh), 1);
    }

    //---definitions---
    int iEvent=-1;
  
    //-----output setup-----
    TFile* outROOT = TFile::Open("ntuples/"+TString(run)+".root", "RECREATE");
    outROOT->cd();
    RecoTree outTree(nCh, nSamples, nameMCP);
  
    //-----input setup-----
    TFile* inROOT = TFile::Open(TString(path+run+".root"), "READ");
    inROOT->cd();
    TTree* inTree = (TTree*)inROOT->Get("H4tree");
    H4Tree h4Tree(inTree);

    cout << ">>> Processing H4DAQ run #" << run << " <<<" << endl
         << ">>> file: " << path << run << ".root" << endl;
    //---process WFs---
    while(h4Tree.NextEvt())
    {        
        for(int iCh=0; iCh<nCh; ++iCh)
        {
            //---read WFs
            WFClass WF(opts.GetOpt<int>("global", string("Ch")+to_string(iCh), 1), tUnit);
            for(int iSample=iCh*nSamples; iSample<iCh*nSamples+nSamples; ++iSample)
            {
                WF.AddSample(h4Tree.digiSampleValue[iSample]);
            }
            //---compute reco variables
            WF.SetBaselineWindow(10, 30);
            WF.SetSignalWindow(40, nSamples);
            WF.SubtractBaseline();
            outTree.amp_max[iCh] = WF.GetAmpMax();
            int time_AM = (int)WF.GetTimeCF(1);
            int time_I1 = time_AM-10;
            int time_I2 = time_AM+10;
            outTree.time[iCh] = WF.GetTimeCF(0.5);
            outTree.baseline[iCh] = WF.GetIntegral(10, 30);
            outTree.charge_tot[iCh] = WF.GetModIntegral(40, nSamples);
            if(time_I1 > 30 && time_I2 < nSamples)
                outTree.charge_sig[iCh] = WF.GetIntegral(time_I1, time_I2);
            else
                outTree.charge_sig[iCh] = -1000;
            //---WFs---
            vector<float>* analizedWF = WF.GetSamples();
            for(int jSample=0; jSample<analizedWF->size(); ++jSample)
            {
                outTree.WF_ch[jSample+iCh*nSamples] = iCh;
                outTree.WF_time[jSample+iCh*nSamples] = jSample*tUnit;
                outTree.WF_val[jSample+iCh*nSamples] = analizedWF->at(jSample);
            }
        }
        //---fill the output tree
        iEvent++;
        outTree.time_stamp = h4Tree.evtTime;
        outTree.event_id = h4Tree.spillNumber;
        outTree.Fill();
    }
    
    outROOT->cd();
    outTree.Write();
    outROOT->Close();
    inROOT->Close();
}
