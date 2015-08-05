#include <string>

#include "TString.h"
#include "TFile.h"
#include "TTree.h"

#include "../interface/CfgManager.h"
#include "../interface/TekFilesManager.h"
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
    for(int jCh=1; jCh<=nCh; ++jCh)
    {
        nameMCP[jCh-1] = opts.GetOpt<string>("global", string("Ch")+to_string(jCh));
        chPolarity[jCh-1] = opts.GetOpt<int>("global", string("Ch")+to_string(jCh), 1);
    }

    //---definitions---
    uint32 firstEvtTime=0;
    int iCh=-1, iEvent=-1;
    string ls_command;
    TekFilesManager sortedWaveList;
  
    //-----output setup-----
    TFile* outROOT = TFile::Open("ntuples/"+TString(run)+".root", "RECREATE");
    outROOT->cd();
    RecoTree outTree(nCh, nSamples, nameMCP);

    //-----read data + sort by date-----
    ls_command = string("ls "+path+run+" | grep 'txt' > tmp/"+run+".list");
    system(ls_command.c_str());
    ifstream waveList(string("tmp/"+run+".list").c_str(), ios::in);
    string file;    
    while(waveList >> file)
        sortedWaveList.PushBack(file);
        
    //---process WFs---
    map<uint32, vector<string> >::iterator evtFiles;
    while(iEvent<maxEvents || maxEvents==-1)
    {
        //---get next event files
        evtFiles = sortedWaveList.NextEvt();
        if(iEvent == -1)
            outTree.start_time = evtFiles->first;
        if(evtFiles == sortedWaveList.end())
            break;

        //---check input files
        if(evtFiles->second.size() != nCh)
            cout << "> Dumper --- WARNING: more than one event with the same time stamp" << endl;
        
        for(auto file : evtFiles->second)
        {
            ++iCh;
            file = path+run+"/"+file;
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
            //---fill the output tree
            if(iCh == nCh-1)
            {
                iEvent++;
                outTree.time_stamp = evtFiles->first-outTree.start_time;                
                outTree.event_id = iEvent;
                outTree.Fill();
                //---reset
                iCh = -1;
            }
        }
    }
    waveList.close();
    
    outTree.Write();
    outROOT->Close();
}
