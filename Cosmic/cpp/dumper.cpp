#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<algorithm> 

#include"TString.h"
#include"TFile.h"
#include"TTree.h"

#include"../include/analysis_tools.h"

using namespace std;

int main(int argc, char* argv[])
{
    //-----definitions-----
    int iCh=-1, iEvent=-1;
    float sample_tmp=0;
    char ls_command[200];
    char* path=argv[1];
    char* run=argv[2];
    char list_file[200];
    //---
    vector<float> samples[3];
    TString nameMCP[3];
    string file;

    //-----output setup-----
    nameMCP[0] = "MiB1";
    nameMCP[1] = "MiB2";
    nameMCP[2] = "Planacon";
    TFile* outROOT = TFile::Open("ntuples/"+TString(run)+".root", "recreate");
    outROOT->cd();
    TTree* outTree = new TTree("reco_tree", "reco_tree");
    outTree->SetDirectory(0);
    SetOutTree(outTree, nameMCP, 3);

    //-----read data-----
    sprintf(ls_command, "ls %s%s/* > tmp/%s.list", path, run, run);
    system(ls_command);
    //---process WFs---
    sprintf(list_file, "tmp/%s.list", run);
    ifstream waveList(list_file, ios::in);
    while(waveList >> file)
    {
	iCh++;
	char current[200];
	sprintf(current, "%s", file.c_str());
	cout << current << endl;
	//---read WFs
	ifstream inputFile(current, ios::in);
	int sample_time=0;
	while(inputFile >> sample_tmp)
	{
	    sample_time++;
	    WF_ch[sample_time+iCh*TOT_SAMPLES] = iCh;
	    WF_time[sample_time+iCh*TOT_SAMPLES] = (float)sample_time*SAMPLING_UNIT;
	    if(iCh != 2)
	    {
		samples[iCh].push_back(sample_tmp);
		WF_val[sample_time+iCh*TOT_SAMPLES] = sample_tmp;
	    }
	    else
	    {
		samples[iCh].push_back(-sample_tmp);
		WF_val[sample_time+iCh*TOT_SAMPLES] = -sample_tmp;
	    } 
	}
	inputFile.close();
	//---compute reco variables
	baseline[iCh] = SubtractBaseline(100, 180, &samples[iCh]);
	amp_max[iCh] = AmpMax(200, samples[iCh].size(), &samples[iCh]);
	int time_AM = (int)TimeConstFrac(200, samples[iCh].size(), &samples[iCh], 1);
	int time_I1 = time_AM-30;
	int time_I2 = time_AM+50;
	time_CF[iCh] = TimeConstFrac(200, time_AM, &samples[iCh], 0.5);
	charge_tot[iCh] = ComputeModIntegral(200, samples[iCh].size(), &samples[iCh]);
	if(time_I1 > 200 && time_I2 < samples[iCh].size())
	    charge_sig[iCh] = ComputeIntegral(time_I1, time_I2, &samples[iCh]);
	else
	    charge_sig[iCh] = -1000;
	//---fill output tree---
	if(iCh == 2)
	{ 
	    iEvent++;
	    event_id = iEvent;	    
	    outTree->Fill();
	    //---reset
	    iCh=-1;
	    for(int i=0; i<3; i++)
		samples[i].clear();
	}
    }
    waveList.close();
    
    outTree->Write();
    outROOT->Close();
}
