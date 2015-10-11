/*******************************************************************************

    compile with --> c++ -o doAnalysis doAnalysis.cpp `root-config --cflags --glibs`
    run with --> ./doAnalysis Scan_*.dat Scan_number

*******************************************************************************/
#include <cstdio>
#include <cstdlib>
#include <stdint.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <sstream> 
#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include <iomanip>
#include <ctime>
#include <map>
#include <math.h>

#include "TROOT.h"
#include "TFile.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TChain.h"
#include "TF1.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TProfile.h"
#include "TMath.h"
#include "TString.h"
#include "TClass.h"
#include "TApplication.h"

#include "../include/analysis_tools.h"
#include "../include/init_tree_BTF.h"
#include "../include/histo_func.h"

//*******MAIN*******************************************************************
int main (int argc, char** argv)
{  
//    TApplication* app = new TApplication("app",0,0);
//-------Read data config files-----------------------------------------------
    std::string inputName = std::string(argv[1]);
    char split_char = '/';
    std::vector<std::string> tokens;
    std::istringstream split(inputName);
    for(std::string each; getline(split, each, split_char); 
        tokens.push_back(each));
    
    split_char = '_';
    std::vector<std::string> tokens_name;
    std::istringstream split_name(tokens.at(1));
    for(std::string each; getline(split_name, each, split_char); 
        tokens_name.push_back(each));

    const int Ch_ref1 = atoi((tokens_name.at(1)).c_str());
    const int Ch_ref2 = atoi((tokens_name.at(3)).c_str());
    const int Ch_1 = atoi((tokens_name.at(5)).c_str());
    const int Ch_2 = atoi((tokens_name.at(7)).c_str());
    const int Ch_3 = atoi((tokens_name.at(9)).c_str());

    std::vector<TString> nameMCP;
    nameMCP.push_back("MiB1");
    nameMCP.push_back("MiB2");
    if(argc > 4) nameMCP.at(1) = "Roma1";
    nameMCP.push_back("ScB");
    nameMCP.push_back("Planacon");
    nameMCP.push_back("MiB3");
    nameMCP.push_back("Roma2");

    std::vector<std::string> pcMCP;                                     
    for(unsigned int ii=0; ii<nameMCP.size(); ++ii) pcMCP.push_back("");
    pcMCP.at(Ch_ref1) = tokens_name.at(2);
    pcMCP.at(Ch_ref2) = tokens_name.at(4);
    pcMCP.at(Ch_1) = tokens_name.at(6);
    pcMCP.at(Ch_2) = tokens_name.at(8);
    pcMCP.at(Ch_3) = tokens_name.at(10);
    pcMCP.at(Ch_3).erase(pcMCP.at(Ch_3).size()-4, pcMCP.at(Ch_3).size()-1);
    
    //---treshold setup Scan-dependent
    init();
    const int iScan = atoi(argv[2])-1;
    th_3s_ref1 = th_3s[iScan][Ch_ref1];
    th_3s_ref2 = th_3s[iScan][Ch_ref2];
    th_3s_Ch1 = th_3s[iScan][Ch_1];
    th_3s_Ch2 = th_3s[iScan][Ch_2];
    th_3s_Ch3 = th_3s[iScan][Ch_3];
    th_5s_ref1 = th_5s[iScan][Ch_ref1];
    th_5s_ref2 = th_5s[iScan][Ch_ref2];
    th_5s_Ch1 = th_5s[iScan][Ch_1];
    th_5s_Ch2 = th_5s[iScan][Ch_2];
    th_5s_Ch3 = th_5s[iScan][Ch_3];
    th_SB_ref1 = th_SB[iScan][Ch_ref1];
    th_SB_ref2 = th_SB[iScan][Ch_ref2];
    th_SB_Ch1 = th_SB[iScan][Ch_1];
    th_SB_Ch2 = th_SB[iScan][Ch_2];
    th_SB_Ch3 = th_SB[iScan][Ch_3];

//--------Definition----------------------------------------------------------
    int nFiles=1;
    bool do_fit=0;
    if(argc > 3)
        do_fit = atoi(argv[3]);
    //---Get MS pulses
    TFile* inWave;
    TH1F* MS_histos[6];
    TH1F* digiChHistos[6];
    histoFunc* waves[6];
    TH1F* MS_cf_times;
    TH1F* MS_t1_times;
    TH1F* MS_t2_times;
    TF1* fitFunc_Ch1;
    TF1* fitFunc_Ch2;
    TF1* fitFunc_Ch3;
    TF1* fitFunc_ref1;
    TF1* fitFunc_ref2;
    //---MS times: cf, t1->.1*ampmax (low), t2->.1*ampmax (high)
    float MS_cf[6], MS_t1[6], MS_t2[6];
    if(do_fit == 1)
    {
	inWave = TFile::Open("tmp/"+(TString)tokens_name.at(0)+"_MS_func.root", "r");
	//--usefull times
	MS_cf_times = (TH1F*)inWave->Get("MS_cf_times");
	MS_t1_times = (TH1F*)inWave->Get("MS_t1_times");
	MS_t2_times = (TH1F*)inWave->Get("MS_t2_times");
	for(int iCh=0; iCh<6; iCh++)
	{
	    char tmp_name[20];
	    sprintf(tmp_name, "MS_fitfunc_Ch%d", iCh);
	    MS_histos[iCh] = (TH1F*)inWave->Get(tmp_name);
	    waves[iCh] = new histoFunc(MS_histos[iCh]);
	    sprintf(tmp_name, "histo_Ch%d", iCh);
	    digiChHistos[iCh] = new TH1F(tmp_name, tmp_name, 750, -50, 100);
	    MS_cf[iCh]=MS_cf_times->GetBinContent(iCh+1);
	    MS_t1[iCh]=MS_t1_times->GetBinContent(iCh+1);
	    MS_t2[iCh]=MS_t2_times->GetBinContent(iCh+1);
	}
	//---build fitFunc for the mcps channels
	fitFunc_Ch1 = GetFitFunc("Ch1", waves[Ch_1], MS_t1[Ch_1], MS_t2[Ch_1]);
	fitFunc_Ch2 = GetFitFunc("Ch2", waves[Ch_2], MS_t1[Ch_2], MS_t2[Ch_2]);
	fitFunc_Ch3 = GetFitFunc("Ch3", waves[Ch_3], MS_t1[Ch_3], MS_t2[Ch_3]);
	fitFunc_ref1 = GetFitFunc("ref1", waves[Ch_ref1], MS_t1[Ch_ref1], MS_t2[Ch_ref1]);
	fitFunc_ref2 = GetFitFunc("ref2", waves[Ch_ref2],  MS_t1[Ch_ref2], MS_t2[Ch_ref2]);
    }
    //---output tree
    TFile* outROOT = TFile::Open("ntuples/"+(TString)"reco_"+tokens_name.at(0)+".root","recreate");  
    outROOT->cd();
    TTree* outTree = new TTree("reco_tree", "reco_tree");
    outTree->SetDirectory(0);
    SetOutTree(outTree, &nameMCP, Ch_1, Ch_2, Ch_3, Ch_ref1, Ch_ref2);
    //---do runs loop
    ifstream log (argv[1], ios::in);
    while(log >> nFiles)
    {
        //-----Run dependend definition
        vector<float> digiCh[9];
        float timeCF[9], timeAM[9];
        float baseline[9];
        float intBase[9], intSignal[9], intSignalCF[9];
	float peakFitProb[9], ampMax[9];
	//int fibreX[8], hodoYchannels[8];
        int HV1=0, HV2=0, HV3=0, iRun=0, goodEvt=1;
        //---Chain
        TChain* chain = new TChain("eventRawData");
        InitTree(chain);
        //-----Read raw data tree-----------------------------------------------
        for(int iFiles=0; iFiles<nFiles; iFiles++)
        {
            log >> iRun;
            char iRun_str[100];
	    //sprintf(iRun_str, "/gwteray/users/marzocchi/iMCP/dataTrees/run_IMCP_%d_*.root", iRun);
	    sprintf(iRun_str, "/gwpool/users/pigazzini/iMCP/BTF_TB/raw_data/dataTrees/run_IMCP_%d_*.root", iRun);
            chain->Add(iRun_str);
            cout << "Reading: " << iRun_str <<  endl;
        }
        log >> HV1 >> HV2 >> HV3;
        //if(iRun != 261) continue; //analyze only one run
        //-----Data loop--------------------------------------------------------
        for(int iEntry=0; iEntry<chain->GetEntries(); iEntry++)
        {
	    if(iEntry % 1000 == 0)
		cout << "read  " << iEntry << "entries" << endl;
            //-----Unpack data--------------------------------------------------
            //---always clear the std::vector !!!
            for(int iCh=0; iCh<9; iCh++)
            {
                digiCh[iCh].clear();
            }
            //---Read the entry
            chain->GetEntry(iEntry);
            //---DAQ bug workaround
            if(iRun < 145) goodEvt = 10;
            else goodEvt = 1;
            if(evtNumber % goodEvt == 0)   
            {
                //---Read SciFront ADC value and set the e- multiplicity 
		for(int iCh=0; iCh<nAdcChannels; iCh++)
		{
                    if(adcBoard[iCh] == 1 && adcChannel[iCh] == 0) 
                        sci_front_adc = adcData[iCh];
		    if(adcBoard[iCh] == 1 && adcChannel[iCh] >= HODOX_ADC_START_CHANNEL &&
		       adcChannel[iCh] <= HODOX_ADC_END_CHANNEL)
			fibreX[(adcChannel[iCh]-HODOX_ADC_START_CHANNEL)] = adcData[iCh];
		    if(adcBoard[iCh] == 1 && adcChannel[iCh] >= HODOY_ADC_START_CHANNEL &&
		       adcChannel[iCh] <= HODOY_ADC_END_CHANNEL)
			fibreY[(adcChannel[iCh]-HODOY_ADC_START_CHANNEL)] = adcData[iCh];
		}
		//---Read digitizer samples
                for(int iSample=0; iSample<nDigiSamples; iSample++)
                    if(digiChannel[iSample] == 3)
			digiCh[digiChannel[iSample]].push_back(-digiSampleValue[iSample]);
                    else
                        digiCh[digiChannel[iSample]].push_back(digiSampleValue[iSample]);
                //---loop over MPC's channels
                for(int iCh=0; iCh<9; iCh++)
                {
                    baseline[iCh]=SubtractBaseline(5, 25, &digiCh[iCh]);
                    timeCF[iCh]=TimeConstFrac(47, 500, &digiCh[iCh], 0.5);
                    timeAM[iCh]=TimeConstFrac(47, 500, &digiCh[iCh], 1);
                    int t1 = (int)(timeCF[iCh]/0.2) - 3;
                    int t2 = (int)(timeCF[iCh]/0.2) + 17;
                    intBase[iCh] = ComputeIntegral(26, 46, &digiCh[iCh]);
                    if(t1 > 50 && t1 < 1024 && t2 > 50 && t2 < 1024)
                    {
                        ampMax[iCh] = InterpolateAmpMax(t1, t2, &digiCh[iCh], &peakFitProb[iCh]);
                        intSignal[iCh] = ComputeIntegral(t1, t2, &digiCh[iCh]);
			intSignalCF[iCh] = ComputeIntegral((int)(timeAM[iCh]/0.2)-5, (int)(timeAM[iCh]/0.2), &digiCh[iCh]);
                    }
                    else
                    {
                        ampMax[iCh] = InterpolateAmpMax(0, 1024, &digiCh[iCh], &peakFitProb[iCh]);
                        intSignal[iCh] = ComputeIntegral(50, 70, &digiCh[iCh]);
			intSignalCF[iCh] = 2000;
                    }
                }
                //-----Dump WaveForm Infos------------------------------------------------
                //---Multiplicity == 1 
		//---reset
		time_Ch1 = -100;
		time_Ch2 = -100;
		time_Ch3 = -100;
		//-----clock------------------------------------------------
                time_clock = timeCF[8];
                //-----ref1 MCP---------------------------------------------
		time_ref1 = timeCF[Ch_ref1];
		amp_max_ref1 = ampMax[Ch_ref1];
		charge_ref1 = intSignal[Ch_ref1];
		charge_CF_ref1 = intSignalCF[Ch_ref1];
		baseline_ref1 = intBase[Ch_ref1];
		f_prob_ref1 = peakFitProb[Ch_ref1];
		if(do_fit == 1)
		{
		    //---pulse fit variables ref1
		    for(int iSample=0; iSample<digiCh[Ch_ref1].size(); iSample++)
		    {
			digiChHistos[Ch_ref1]->SetBinContent(digiChHistos[Ch_ref1]->FindBin(iSample*0.2-timeCF[Ch_ref1]),
							     digiCh[Ch_ref1].at(iSample));
			digiChHistos[Ch_ref1]->SetBinError(digiChHistos[Ch_ref1]->FindBin(iSample*0.2-timeCF[Ch_ref1]), 4);
		    }
		    fitFunc_ref1->SetParameters(-intSignal[Ch_ref1], 1., 0.1, baseline[Ch_ref1]);
		    digiChHistos[Ch_ref1]->Fit(fitFunc_ref1,"RQ");
		    f_time_ref1 = (MS_cf[Ch_ref1]-fitFunc_ref1->GetParameter(2))*fitFunc_ref1->GetParameter(1);
		    f_amp_max_ref1 = -fitFunc_ref1->GetParameter(0);
		    f_charge_ref1 = 5*fitFunc_ref1->Integral(fitFunc_ref1->GetParameter(1)*
							     (MS_t1[Ch_ref1]-fitFunc_ref1->GetParameter(2)),
							     fitFunc_ref1->GetParameter(1)*
							     (MS_t2[Ch_ref1]-fitFunc_ref1->GetParameter(2)));
		    f_prob_ref1 = fitFunc_ref1->GetProb();
		}
		//-----Ch_1-------------------------------------------------
		time_Ch1 = timeCF[Ch_ref1] - timeCF[Ch_1];
		charge_Ch1 = intSignal[Ch_1];
		charge_CF_Ch1 = intSignalCF[Ch_1];
		amp_max_Ch1 = ampMax[Ch_1];
		baseline_Ch1 = intBase[Ch_1];      
		f_prob_Ch1 = peakFitProb[Ch_1];                  
		if(do_fit == 1)
		{
		    //---pulse fit variables Ch1
		    for(int iSample=0; iSample<digiCh[Ch_1].size(); iSample++)
		    {
			digiChHistos[Ch_1]->SetBinContent(digiChHistos[Ch_1]->FindBin(iSample*0.2-timeCF[Ch_ref1]),
							  digiCh[Ch_1].at(iSample));
			digiChHistos[Ch_1]->SetBinContent(digiChHistos[Ch_1]->FindBin(iSample*0.2-timeCF[Ch_ref1]), 4);
		    }
		    fitFunc_Ch1->SetParameters(-intSignal[Ch_1], 1., 0.1, baseline[Ch_1]);
		    digiChHistos[Ch_1]->Fit(fitFunc_Ch1,"RQ");
		    f_time_Ch1 = (MS_cf[Ch_1]-fitFunc_Ch1->GetParameter(2))*fitFunc_Ch1->GetParameter(1) - f_time_ref1;
		    f_amp_max_Ch1 = -fitFunc_Ch1->GetParameter(0);
		    f_charge_Ch1 = 5*fitFunc_Ch1->Integral(fitFunc_Ch1->GetParameter(1)*
							   (MS_t1[Ch_1]-fitFunc_Ch1->GetParameter(2)),
							   fitFunc_Ch1->GetParameter(1)*
							   (MS_t2[Ch_1]-fitFunc_Ch1->GetParameter(2)));
		    f_prob_Ch1 = fitFunc_Ch1->GetProb();
		}
		//-----Ch_2-------------------------------------------------
		time_Ch2 = timeCF[Ch_ref1] - timeCF[Ch_2];
		charge_Ch2 = intSignal[Ch_2];
		charge_CF_Ch2 = intSignalCF[Ch_2];
		amp_max_Ch2 = ampMax[Ch_2];
		baseline_Ch2 = intBase[Ch_2];      
		f_prob_Ch2 = peakFitProb[Ch_2];              
		if(do_fit == 1)
		{
		    //---pulse fit variables Ch2
		    for(int iSample=0; iSample<digiCh[Ch_2].size(); iSample++)
		    {
			digiChHistos[Ch_2]->SetBinContent(digiChHistos[Ch_2]->FindBin(iSample*0.2-timeCF[Ch_ref1]),
							  digiCh[Ch_2].at(iSample));
			digiChHistos[Ch_2]->SetBinContent(digiChHistos[Ch_2]->FindBin(iSample*0.2-timeCF[Ch_ref1]), 4);
		    }
		    fitFunc_Ch2->SetParameters(-intSignal[Ch_2], 1., 0.1, baseline[Ch_2]);
		    digiChHistos[Ch_2]->Fit(fitFunc_Ch2,"RQ");
		    f_time_Ch2 = (MS_cf[Ch_2]-fitFunc_Ch2->GetParameter(2))*fitFunc_Ch2->GetParameter(1) - f_time_ref1;
		    f_amp_max_Ch2 = -fitFunc_Ch2->GetParameter(0);
		    f_charge_Ch2 = 5*fitFunc_Ch2->Integral(fitFunc_Ch2->GetParameter(1)*
							   (MS_t1[Ch_2]-fitFunc_Ch2->GetParameter(2)),
							   fitFunc_Ch2->GetParameter(1)*
							   (MS_t2[Ch_2]-fitFunc_Ch2->GetParameter(2)));
		    f_prob_Ch2 = fitFunc_Ch2->GetProb();
		}

		//-----Ch_3-------------------------------------------------
		time_Ch3 = timeCF[Ch_ref1] - timeCF[Ch_3];
		charge_Ch3 = intSignal[Ch_3];
		charge_CF_Ch3 = intSignalCF[Ch_3];
		amp_max_Ch3 = ampMax[Ch_3];
		baseline_Ch3 = intBase[Ch_3];
		f_prob_Ch3 = peakFitProb[Ch_3];                    
		if(do_fit == 1)
		{
		    //---pulse fit variables Ch3
		    for(int iSample=0; iSample<digiCh[Ch_2].size(); iSample++)
		    {
			digiChHistos[Ch_3]->SetBinContent(digiChHistos[Ch_3]->FindBin(iSample*0.2-timeCF[Ch_ref1]),
							  digiCh[Ch_3].at(iSample));
			digiChHistos[Ch_3]->SetBinContent(digiChHistos[Ch_3]->FindBin(iSample*0.2-timeCF[Ch_ref1]), 4);
		    }
			fitFunc_Ch3->SetParameters(-intSignal[Ch_3], 1., 0.1, baseline[Ch_3]);
		    digiChHistos[Ch_3]->Fit(fitFunc_Ch3,"RQ");
		    f_time_Ch3 = (MS_cf[Ch_3]-fitFunc_Ch3->GetParameter(2))*fitFunc_Ch3->GetParameter(1) - f_time_ref1;
		    f_amp_max_Ch3 = -fitFunc_Ch3->GetParameter(0);
		    f_charge_Ch3 = 5*fitFunc_Ch3->Integral(fitFunc_Ch3->GetParameter(1)*
							   (MS_t1[Ch_3]-fitFunc_Ch3->GetParameter(2)),
							   fitFunc_Ch3->GetParameter(1)*
							   (MS_t2[Ch_3]-fitFunc_Ch3->GetParameter(2)));
		    f_prob_Ch3 = fitFunc_Ch3->GetProb();
		}
		//-----ref2 MCP---------------------------------------------
		time_ref2 = timeCF[Ch_ref2] -timeCF[Ch_ref1];
		amp_max_ref2 = ampMax[Ch_ref2];
		charge_ref2 = intSignal[Ch_ref2];
		charge_CF_ref2 = intSignalCF[Ch_ref2];
		baseline_ref2 = intBase[Ch_ref2];
		f_prob_ref2 = peakFitProb[Ch_ref2];
		//---pulse fit variables ref2
		if(do_fit == 1)
		{ 
		    for(int iSample=0; iSample<digiCh[Ch_ref2].size(); iSample++)
		    {
			digiChHistos[Ch_ref2]->SetBinContent(digiChHistos[Ch_ref2]->FindBin(iSample*0.2-timeCF[Ch_ref2]),
							     digiCh[Ch_ref2].at(iSample));
			digiChHistos[Ch_ref2]->SetBinContent(digiChHistos[Ch_ref2]->FindBin(iSample*0.2-timeCF[Ch_ref2]), 4);
		    }
		    fitFunc_ref2->SetParameters(-intSignal[Ch_ref2], 1., 0.1, baseline[Ch_ref2]);
		    digiChHistos[Ch_ref2]->Fit(fitFunc_ref2,"RQ");
		    f_time_ref2 = (MS_cf[Ch_ref2]-fitFunc_ref2->GetParameter(2))*fitFunc_ref2->GetParameter(1) - f_time_ref1;
		    f_amp_max_ref2 = -fitFunc_ref2->GetParameter(0);
		    f_charge_ref2 = 5*fitFunc_ref2->Integral(fitFunc_ref2->GetParameter(1)*
							     (MS_t1[Ch_ref2]-fitFunc_ref2->GetParameter(2)),
							     fitFunc_ref2->GetParameter(1)*
							     (MS_t2[Ch_ref2]-fitFunc_ref2->GetParameter(2)));
		    f_prob_ref2 = fitFunc_ref2->GetProb();
		}
		//-----Fill output tree-------------------------------------
		run_id = iRun;
		outTree->Fill();    
	    } 
        }
        //---Get ready for next run
        chain->Delete();
    }
    //-----close everything-----------------------------------------------------
    outTree->Write();
    outROOT->Close();
    
//---------Done-----------------------------------------------------------------
}

        
