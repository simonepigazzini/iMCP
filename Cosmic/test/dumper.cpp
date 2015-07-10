#include <new>

#include "TString.h"
#include "TFile.h"
#include "TTree.h"

#include "../interface/CfgManager.h"
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
    string path=opts.GetOpt<string>("path2data");
    string run=opts.GetOpt<string>("run");
    if(argc == 3)
        run = string(argv[2]);
    int maxEvents=opts.GetOpt<int>("maxEvents");
    //---channels opts
    int nCh = opts.GetOpt<int>("nCh");
    int nSamples = opts.GetOpt<int>("nSamples");
    float tUnit = opts.GetOpt<float>("tUnit");
    int* chPolarity = new int[nCh];
    TString* nameMCP = new TString[nCh];    
    for(int jCh=1; jCh<=nCh; ++jCh)
    {
        nameMCP[jCh-1] = opts.GetOpt<string>("Ch"+to_string(jCh));
        chPolarity[jCh-1] = opts.GetOpt<int>("Ch"+to_string(jCh), 1);
    }

    //---definitions---
    int iCh=-1, iEvent=-1;
    string ls_command;
    //-----output setup-----
    TFile* outROOT = TFile::Open("ntuples/"+TString(run)+".root", "recreate");
    outROOT->cd();
    RecoTree outTree(nCh, nSamples, nameMCP);

    //-----read data-----
    ls_command = string("ls "+path+run+"/*txt > tmp/"+run+".list");
    system(ls_command.c_str());
    //---process WFs---
    ifstream waveList(string("tmp/"+run+".list").c_str(), ios::in);
    string file;    
    while(waveList >> file && (iEvent<maxEvents || maxEvents==-1))
    {
        iCh++;
        cout << file << endl;
        //---read WFs
        WFClass WF(chPolarity[iCh], tUnit);
        ifstream inputFile(file.c_str(), ios::in);
        float sample_val=0;    
        int iSample=0;
        while(inputFile >> sample_val && iSample<nSamples)
        {
            WF.AddSample(sample_val*1000);
            ++iSample;
        }
        inputFile.close();
        //---compute reco variables
        WF.SetBaselineWindow(10, 90);
        WF.SetSignalWindow(200, nSamples);
        outTree.baseline[iCh] = WF.SubtractBaseline();
        outTree.amp_max[iCh] = WF.GetAmpMax();
        int time_AM = (int)WF.GetTimeCF(1);
        int time_I1 = time_AM-30;
        int time_I2 = time_AM+50;
        outTree.time_CF[iCh] = WF.GetTimeCF(0.5);
        outTree.baseline[iCh] = WF.GetIntegral(100, 180);
        outTree.charge_tot[iCh] = WF.GetModIntegral(200, nSamples);
        if(time_I1 > 200 && time_I2 < nSamples)
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
        //---fill output tree---
        if(iCh == nCh-1)
        { 
            iEvent++;
            outTree.event_id = iEvent;	    
            outTree.Fill();
            //---reset
            iCh=-1;
        }
    }
    waveList.close();
    
    outTree.Write();
    outROOT->Close();
}
