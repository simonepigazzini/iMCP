/*************************************************************

    tools for the BTF iMCP data.
    1-Charge treshold for every MCP
    2-Waveform manipulation function

*************************************************************/

#ifndef __analysis_tools__
#define __analysis_tools__

#include <cstdio>
#include <cstdlib>
#include <stdint.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

#include "TTree.h"
#include "TF1.h"

#include "histo_func.h"

using namespace std;

#define DIGITIZER_SAMPLING_UNIT 0.2 //digitizer samples width (ns)
#define MS_SAMPLING_UNIT 0.01 //mean signal samples width (ns)
#define MS_LOW_TIME -15
#define MS_HIGH_TIME 30

#define HODOX_ADC_START_CHANNEL 16
#define HODOX_ADC_MID_CHANNEL 19
#define HODOX_ADC_END_CHANNEL 23
#define HODOY_ADC_START_CHANNEL 24
#define HODOY_ADC_MID_CHANNEL 27
#define HODOY_ADC_END_CHANNEL 31

//------------------------------------------------------------------------------
//---initialize all the pointer addressed by the outTree branches
float time_Ch1=0, time_Ch2=0, time_Ch3=0, time_ref1=0, time_ref2=0;
float amp_max_Ch1=0, amp_max_Ch2=0, amp_max_Ch3=0, amp_max_ref1=0, amp_max_ref2=0;
float charge_Ch1=0, charge_Ch2=0, charge_Ch3=0, charge_ref1=0, charge_ref2=0;
float charge_CF_Ch1=0, charge_CF_Ch2=0, charge_CF_Ch3=0, charge_CF_ref1=0, charge_CF_ref2=0;
float baseline_Ch1=0, baseline_Ch2=0, baseline_Ch3=0, baseline_ref1=0, baseline_ref2=0;
//---fitted variables
float f_time_Ch1=0, f_time_Ch2=0, f_time_Ch3=0, f_time_ref1=0, f_time_ref2=0;
float f_amp_max_Ch1=0, f_amp_max_Ch2=0, f_amp_max_Ch3=0, f_amp_max_ref1=0, f_amp_max_ref2=0;
float f_charge_Ch1=0, f_charge_Ch2=0, f_charge_Ch3=0, f_charge_ref1=0, f_charge_ref2=0;
float f_prob_Ch1=0, f_prob_Ch2=0, f_prob_Ch3=0, f_prob_ref1=0, f_prob_ref2=0;
//---thresholds branches
float th_3s_Ch1=0, th_3s_Ch2=0, th_3s_Ch3=0, th_3s_ref1=0, th_3s_ref2=0;
float th_5s_Ch1=0, th_5s_Ch2=0, th_5s_Ch3=0, th_5s_ref1=0, th_5s_ref2=0;
float th_SB_Ch1=0, th_SB_Ch2=0, th_SB_Ch3=0, th_SB_ref1=0, th_SB_ref2=0;
//---global branches
int sci_front_adc=0, run_id=0;
//---hodoscope branches
int fibreX[8]={};
int fibreY[8]={}; 

//---Integral treshold th[iScan][Ch_n] Roma1 is on Ch=2
float _th[4][6];
float th_SB[5][6];
float th_3s[5][6];
float th_5s[5][6];
float MS_tCut[6];

void init()
{
    /*  soglia S/B = 100
    _th[0][0] = -180;   //Mib1 PC on
    _th[1][0] = -180;
    _th[2][0] = -180;
    _th[3][0] = -180;
    _th[4][0] = -180;

    _th[0][1] = -180;   //Mib2 PC on
    _th[1][1] = -180;   //-350;   //Mib2 PC off
    _th[2][1] = -125;   //-100;   //Roma 1 PC off
    _th[3][1] = -180;   //Mib2 PC off
    _th[4][1] = -180;   //Mib2 PC off

    _th[0][2] = -0;
    _th[1][2] = -0;
    _th[2][2] = -0;
    _th[3][2] = -0;
    _th[4][2] = -0;

    _th[0][3] = -70;   //Plana PC off and on
    _th[1][3] = -70;
    _th[2][3] = -70;
    _th[3][3] = -70;
    _th[4][3] = -70;

    _th[0][4] = -180;   //Mib3 PC off
    _th[1][4] = -180;   //Mib3 PC on
    _th[2][4] = -180;   //Mib3 PC on
    _th[3][4] = -180;   //Mib3 PC off
    _th[4][4] = -180;   //Mib3 PC off

    _th[0][5] = -125;   //Roma PC on
    _th[1][5] = -125;
    _th[2][5] = -125;
    _th[3][5] = -64;    //Roma PC on no AMPLI
    _th[4][5] = -125;   //Roma PC on no AMPLI
    */
    
    //soglia S/B = 1000
    th_SB[0][0] = -275;   //Mib1 PC on
    th_SB[1][0] = -275;
    th_SB[2][0] = -275;
    th_SB[3][0] = -275;
    th_SB[4][0] = -275;

    th_SB[0][1] = -275;   //Mib2 PC on
    th_SB[1][1] = -275;   //-350;   //Mib2 PC off
    th_SB[2][1] = -200;   //-100;   //Roma 1 PC off
    th_SB[3][1] = -275;   //Mib2 PC off
    th_SB[4][1] = -275;   //Mib2 PC off

    th_SB[0][2] = -0;
    th_SB[1][2] = -0;
    th_SB[2][2] = -0;
    th_SB[3][2] = -0;
    th_SB[4][2] = -0;

    th_SB[0][3] = -135;   //Plana PC off and on
    th_SB[1][3] = -135;
    th_SB[2][3] = -135;
    th_SB[3][3] = -135;
    th_SB[4][3] = -135;

    th_SB[0][4] = -275;   //Mib3 PC off
    th_SB[1][4] = -275;   //Mib3 PC on
    th_SB[2][4] = -275;   //Mib3 PC on
    th_SB[3][4] = -275;   //Mib3 PC off
    th_SB[4][4] = -275;   //Mib3 PC off

    th_SB[0][5] = -200;   //Roma PC on
    th_SB[1][5] = -200;
    th_SB[2][5] = -200;
    th_SB[3][5] = -125;   //Roma PC on no AMPLI
    th_SB[4][5] = -200;   //Roma PC on no AMPLI
    

    //soglia 3sigmaB 
    th_3s[0][0] = -220;   //Mib1 PC on
    th_3s[1][0] = -220;
    th_3s[2][0] = -220;
    th_3s[3][0] = -220;
    th_3s[4][0] = -220;

    th_3s[0][1] = -180;   //Mib2 PC on
    th_3s[1][1] = -180;   //-350;   //Mib2 PC off
    th_3s[2][1] = -125;   //-100;   //Roma 1 PC off
    th_3s[3][1] = -180;   //Mib2 PC off
    th_3s[4][1] = -180;   //Mib2 PC off

    th_3s[0][2] = -0;
    th_3s[1][2] = -0;
    th_3s[2][2] = -0;
    th_3s[3][2] = -0;
    th_3s[4][2] = -0;
        
    th_3s[0][3] = -85;    //Plana PC off and on
    th_3s[1][3] = -85;
    th_3s[2][3] = -85;
    th_3s[3][3] = -85;
    th_3s[4][3] = -85;

    th_3s[0][4] = -180;   //Mib3 PC off
    th_3s[1][4] = -180;   //Mib3 PC on
    th_3s[2][4] = -180;   //Mib3 PC on
    th_3s[3][4] = -180;   //Mib3 PC off
    th_3s[4][4] = -180;   //Mib3 PC off

    th_3s[0][5] = -125;   //Roma PC on
    th_3s[1][5] = -125;
    th_3s[2][5] = -125;
    th_3s[3][5] = -85;    //Roma PC on no AMPLI
    th_3s[4][5] = -125;   //Roma PC on

    //soglia 5sigmaB                                      
    th_5s[0][0] = -375;   //Mib1 PC on                      
    th_5s[1][0] = -375;
    th_5s[2][0] = -375;
    th_5s[3][0] = -375;
    th_5s[4][0] = -375;

    th_5s[0][1] = -314;   //Mib2 PC on                      
    th_5s[1][1] = -314;
    th_5s[2][1] = -210;   //Roma 1 PC off                    
    th_5s[3][1] = -314;   //Mib2 PC off                      
    th_5s[4][1] = -314;   //                                

    th_5s[0][2] = -0;
    th_5s[1][2] = -0;
    th_5s[2][2] = -0;
    th_5s[3][2] = -0;
    th_5s[4][2] = -0;

    th_5s[0][3] = -130;    //Plana PC off and on            
    th_5s[1][3] = -130;
    th_5s[2][3] = -130;
    th_5s[3][3] = -130;
    th_5s[4][3] = -130;

    th_5s[0][4] = -314;   //Mib3 PC off                      
    th_5s[1][4] = -314;   //Mib3 PC on                      
    th_5s[2][4] = -314;   //Mib3 PC on                      
    th_5s[3][4] = -314;   //Mib3 PC off                      
    th_5s[4][4] = -314;   //Mib3 PC off   
                   
    th_5s[0][5] = -210;   //Roma PC on                      
    th_5s[1][5] = -210;
    th_5s[2][5] = -210;
    th_5s[3][5] = -144;   //Roma PC on no AMPLI              
    th_5s[4][5] = -210;   //Roma PC on    

    //---Cut times for the DFT function
    MS_tCut[0] = 5;
    MS_tCut[1] = 7;
    MS_tCut[2] = -15;
    MS_tCut[3] = 5;
    MS_tCut[4] = 8;
    MS_tCut[5] = 0;
}

void SetOutTree(TTree* outTree, vector<TString>* nameMCP, 
                int Ch_1, int Ch_2, int Ch_3, int Ch_ref1=0, int Ch_ref2=5)
{
    //---standard analysis branches
    outTree->Branch("time_"+nameMCP->at(Ch_1),&time_Ch1,"time_"+nameMCP->at(Ch_1)+"/F");
    outTree->Branch("time_"+nameMCP->at(Ch_2),&time_Ch2,"time_"+nameMCP->at(Ch_2)+"/F");
    outTree->Branch("time_"+nameMCP->at(Ch_3),&time_Ch3,"time_"+nameMCP->at(Ch_3)+"/F");
    outTree->Branch("time_"+nameMCP->at(Ch_ref1),&time_ref1,"time_"+nameMCP->at(Ch_ref1)+"/F");
    outTree->Branch("time_"+nameMCP->at(Ch_ref2),&time_ref2,"time_"+nameMCP->at(Ch_ref2)+"/F");
    outTree->Branch("amp_max_"+nameMCP->at(Ch_1),&amp_max_Ch1,"amp_max_"+nameMCP->at(Ch_1)+"/F");
    outTree->Branch("amp_max_"+nameMCP->at(Ch_2),&amp_max_Ch2,"amp_max_"+nameMCP->at(Ch_2)+"/F");
    outTree->Branch("amp_max_"+nameMCP->at(Ch_3),&amp_max_Ch3,"amp_max_"+nameMCP->at(Ch_3)+"/F");
    outTree->Branch("amp_max_"+nameMCP->at(Ch_ref1),&amp_max_ref1,"amp_max_"+nameMCP->at(Ch_ref1)+"/F");
    outTree->Branch("amp_max_"+nameMCP->at(Ch_ref2),&amp_max_ref2,"amp_max_"+nameMCP->at(Ch_ref2)+"/F");
    outTree->Branch("charge_"+nameMCP->at(Ch_1),&charge_Ch1,"charge_"+nameMCP->at(Ch_1)+"/F");
    outTree->Branch("charge_"+nameMCP->at(Ch_2),&charge_Ch2,"charge_"+nameMCP->at(Ch_2)+"/F");
    outTree->Branch("charge_"+nameMCP->at(Ch_3),&charge_Ch3,"charge_"+nameMCP->at(Ch_3)+"/F");
    outTree->Branch("charge_"+nameMCP->at(Ch_ref1),&charge_ref1,"charge_"+nameMCP->at(Ch_ref1)+"/F");
    outTree->Branch("charge_"+nameMCP->at(Ch_ref2),&charge_ref2,"charge_"+nameMCP->at(Ch_ref2)+"/F");
    outTree->Branch("charge_CF_"+nameMCP->at(Ch_1),&charge_CF_Ch1,"charge_CF_"+nameMCP->at(Ch_1)+"/F");
    outTree->Branch("charge_CF_"+nameMCP->at(Ch_2),&charge_CF_Ch2,"charge_CF_"+nameMCP->at(Ch_2)+"/F");
    outTree->Branch("charge_CF_"+nameMCP->at(Ch_3),&charge_CF_Ch3,"charge_CF_"+nameMCP->at(Ch_3)+"/F");
    outTree->Branch("charge_CF_"+nameMCP->at(Ch_ref1),&charge_CF_ref1,"charge_CF_"+nameMCP->at(Ch_ref1)+"/F");
    outTree->Branch("charge_CF_"+nameMCP->at(Ch_ref2),&charge_CF_ref2,"charge_CF_"+nameMCP->at(Ch_ref2)+"/F");
    outTree->Branch("baseline_"+nameMCP->at(Ch_1),&baseline_Ch1,"baseline_"+nameMCP->at(Ch_1)+"/F");
    outTree->Branch("baseline_"+nameMCP->at(Ch_2),&baseline_Ch2,"baseline_"+nameMCP->at(Ch_2)+"/F");
    outTree->Branch("baseline_"+nameMCP->at(Ch_3),&baseline_Ch3,"baseline_"+nameMCP->at(Ch_3)+"/F");
    outTree->Branch("baseline_"+nameMCP->at(Ch_ref1),&baseline_ref1,"baseline_"+nameMCP->at(Ch_ref1)+"/F");
    outTree->Branch("baseline_"+nameMCP->at(Ch_ref2),&baseline_ref2,"baseline_"+nameMCP->at(Ch_ref2)+"/F");
    //---pulse fit branches
    outTree->Branch("f_time_"+nameMCP->at(Ch_1),&f_time_Ch1,"f_time_"+nameMCP->at(Ch_1)+"/F");
    outTree->Branch("f_time_"+nameMCP->at(Ch_2),&f_time_Ch2,"f_time_"+nameMCP->at(Ch_2)+"/F");
    outTree->Branch("f_time_"+nameMCP->at(Ch_3),&f_time_Ch3,"f_time_"+nameMCP->at(Ch_3)+"/F");
    outTree->Branch("f_time_"+nameMCP->at(Ch_ref1),&f_time_ref1,"f_time_"+nameMCP->at(Ch_ref1)+"/F");
    outTree->Branch("f_time_"+nameMCP->at(Ch_ref2),&f_time_ref2,"f_time_"+nameMCP->at(Ch_ref2)+"/F");
    outTree->Branch("f_amp_max_"+nameMCP->at(Ch_1),&f_amp_max_Ch1,"f_amp_max_"+nameMCP->at(Ch_1)+"/F");
    outTree->Branch("f_amp_max_"+nameMCP->at(Ch_2),&f_amp_max_Ch2,"f_amp_max_"+nameMCP->at(Ch_2)+"/F");
    outTree->Branch("f_amp_max_"+nameMCP->at(Ch_3),&f_amp_max_Ch3,"f_amp_max_"+nameMCP->at(Ch_3)+"/F");
    outTree->Branch("f_amp_max_"+nameMCP->at(Ch_ref1),&f_amp_max_ref1,"f_amp_max_"+nameMCP->at(Ch_ref1)+"/F");
    outTree->Branch("f_amp_max_"+nameMCP->at(Ch_ref2),&f_amp_max_ref2,"f_amp_max_"+nameMCP->at(Ch_ref2)+"/F");
    outTree->Branch("f_charge_"+nameMCP->at(Ch_1),&f_charge_Ch1,"f_charge_"+nameMCP->at(Ch_1)+"/F");
    outTree->Branch("f_charge_"+nameMCP->at(Ch_2),&f_charge_Ch2,"f_charge_"+nameMCP->at(Ch_2)+"/F");
    outTree->Branch("f_charge_"+nameMCP->at(Ch_3),&f_charge_Ch3,"f_charge_"+nameMCP->at(Ch_3)+"/F");
    outTree->Branch("f_charge_"+nameMCP->at(Ch_ref1),&f_charge_ref1,"f_charge_"+nameMCP->at(Ch_ref1)+"/F");
    outTree->Branch("f_charge_"+nameMCP->at(Ch_ref2),&f_charge_ref2,"f_charge_"+nameMCP->at(Ch_ref2)+"/F");
    outTree->Branch("f_prob_"+nameMCP->at(Ch_1),&f_prob_Ch1,"f_prob_"+nameMCP->at(Ch_1)+"/F");
    outTree->Branch("f_prob_"+nameMCP->at(Ch_2),&f_prob_Ch2,"f_prob_"+nameMCP->at(Ch_2)+"/F");
    outTree->Branch("f_prob_"+nameMCP->at(Ch_3),&f_prob_Ch3,"f_prob_"+nameMCP->at(Ch_3)+"/F");
    outTree->Branch("f_prob_"+nameMCP->at(Ch_ref1),&f_prob_ref1,"f_prob_"+nameMCP->at(Ch_ref1)+"/F");
    outTree->Branch("f_prob_"+nameMCP->at(Ch_ref2),&f_prob_ref2,"f_prob_"+nameMCP->at(Ch_ref2)+"/F");
    //---MCP thresholds
    outTree->Branch("th_3s_"+nameMCP->at(Ch_1),&th_3s_Ch1,"th_3s_"+nameMCP->at(Ch_1)+"/F");
    outTree->Branch("th_3s_"+nameMCP->at(Ch_2),&th_3s_Ch2,"th_3s_"+nameMCP->at(Ch_2)+"/F");
    outTree->Branch("th_3s_"+nameMCP->at(Ch_3),&th_3s_Ch3,"th_3s_"+nameMCP->at(Ch_3)+"/F");
    outTree->Branch("th_3s_"+nameMCP->at(Ch_ref1),&th_3s_ref1,"th_3s_"+nameMCP->at(Ch_ref1)+"/F");
    outTree->Branch("th_3s_"+nameMCP->at(Ch_ref2),&th_3s_ref2,"th_3s_"+nameMCP->at(Ch_ref2)+"/F");
    outTree->Branch("th_5s_"+nameMCP->at(Ch_1),&th_5s_Ch1,"th_5s_"+nameMCP->at(Ch_1)+"/F");
    outTree->Branch("th_5s_"+nameMCP->at(Ch_2),&th_5s_Ch2,"th_5s_"+nameMCP->at(Ch_2)+"/F");
    outTree->Branch("th_5s_"+nameMCP->at(Ch_3),&th_5s_Ch3,"th_5s_"+nameMCP->at(Ch_3)+"/F");
    outTree->Branch("th_5s_"+nameMCP->at(Ch_ref1),&th_5s_ref1,"th_5s_"+nameMCP->at(Ch_ref1)+"/F");
    outTree->Branch("th_5s_"+nameMCP->at(Ch_ref2),&th_5s_ref2,"th_5s_"+nameMCP->at(Ch_ref2)+"/F");
    outTree->Branch("th_SB_"+nameMCP->at(Ch_1),&th_SB_Ch1,"th_SB_"+nameMCP->at(Ch_1)+"/F");
    outTree->Branch("th_SB_"+nameMCP->at(Ch_2),&th_SB_Ch2,"th_SB_"+nameMCP->at(Ch_2)+"/F");
    outTree->Branch("th_SB_"+nameMCP->at(Ch_3),&th_SB_Ch3,"th_SB_"+nameMCP->at(Ch_3)+"/F");
    outTree->Branch("th_SB_"+nameMCP->at(Ch_ref1),&th_SB_ref1,"th_SB_"+nameMCP->at(Ch_ref1)+"/F");
    outTree->Branch("th_SB_"+nameMCP->at(Ch_ref2),&th_SB_ref2,"th_SB_"+nameMCP->at(Ch_ref2)+"/F");
    //---hodoscope branches
    outTree->Branch("fibreX",&fibreX,"fibreX[8]/I");
    outTree->Branch("fibreY",&fibreY,"fibreY[8]/I");
    //---global branches
    outTree->Branch("sci_front_adc",&sci_front_adc,"sci_front_adc/I");
    outTree->Branch("run_id",&run_id,"run_id/I");
}

//----------------------------------------------------------------------------------------
void DFT_lowCut(vector<float>* samples, float f_cut)
{
    float* a = (float*)malloc(sizeof(float)*samples->size());
    float* b = (float*)malloc(sizeof(float)*samples->size());
    for (int k=0; k<samples->size(); k++) 
    { 
        a[k] = 0;
        b[k] = 0; 
        for (int t=0; t<samples->size(); t++) 
        { 
            float angle = 2 * M_PI * t * k / samples->size(); 
            a[k] += samples->at(t) * cos(angle); 
            b[k] += samples->at(t) * sin(angle); 
        } 
    }
}

//---------------------------------------------------------------------------------------
//---estimate the baseline in a given range and then subtract it from the signal 
float SubtractBaseline(int tb1, int tb2, vector<float>* samples)
{
    float baseline=0;
    //---compute baseline
    for(int iSample=tb1; iSample<tb2; iSample++)
    {
        baseline += samples->at(iSample);
    }
    baseline = baseline/(float)(tb2-tb1);
    //---subtract baseline
    for(int iSample=0; iSample<samples->size(); iSample++)
    {
        samples->at(iSample) = samples->at(iSample) - baseline;
    }
    return baseline;
}

//---------------------------------------------------------------------------------------
//---estimate time (ns) with CFD, samples must be a negative signal and baseline subtracted
float TimeConstFrac(int t1, int t2, const vector<float>* samples, float AmpFraction, 
                    float step=DIGITIZER_SAMPLING_UNIT, int Nsamples = 5)
{
    float xx= 0.;
    float xy= 0.;
    float Sx = 0.;
    float Sy = 0.;
    float Sxx = 0.;
    float Sxy = 0.;
    float Chi2 = 0.;
    int minSample=t1;
    int cfSample=t1; // first sample over AmpMax*CF 
    float minValue=0;

    for(int iSample=t1; iSample<t2; iSample++)
    {
        if(samples->at(iSample) < samples->at(minSample)) minSample = iSample;
    }
    minValue = samples->at(minSample);
    if(AmpFraction == 1) 
        return minSample*step;
    for(int iSample=minSample; iSample>t1; iSample--)
    {
        if(samples->at(iSample) > minValue*AmpFraction) 
        {
            cfSample = iSample;
            break;
        }
    }
    for(int n=-(Nsamples-1)/2; n<=(Nsamples-1)/2; n++)
    {
        if(cfSample+n<0) continue;
        xx = (cfSample+n)*(cfSample+n)*step*step;
        xy = (cfSample+n)*step*(samples->at(cfSample+n));
        Sx = Sx + (cfSample+n)*step;
        Sy = Sy + samples->at(cfSample+n);
        Sxx = Sxx + xx;
        Sxy = Sxy + xy;
    }

    float Delta = Nsamples*Sxx - Sx*Sx;
    float A = (Sxx*Sy - Sx*Sxy) / Delta;
    float B = (Nsamples*Sxy - Sx*Sy) / Delta;

    float sigma2 = pow(step/sqrt(12)*B,2);
 
    for(int n=-(Nsamples-1)/2; n<=(Nsamples-1)/2; n++)
    {
        if(cfSample+n<0) continue;
        Chi2 = Chi2 + pow(samples->at(cfSample+n) - A - B*((cfSample+n)*step),2)/sigma2;
    } 
    // A+Bx = AmpFraction * amp
    float interpolation = (samples->at(minSample) * AmpFraction - A) / B;
    return interpolation;
}

//---------------------------------------------------------------------------------------
//---compute the maximum amplitude for negative signals (range selectable)

float AmpMax(int t1, int t2, const vector<float>* samples)
{
    int minSample=t1;
    for(int iSample=t1; iSample<t2; iSample++)
    {
        if(samples->at(iSample) < samples->at(minSample)) minSample = iSample;
    }
    return samples->at(minSample);
}

//---------------------------------------------------------------------------------------
//---compute charge for a given signal in a given samples interval 
float ComputeIntegral(int t1, int t2, const vector<float>* samples)
{
    float integral=0;
    for(int bin=t1; bin<t2; bin++)
        integral += samples->at(bin);

    return integral;
}

//---------------------------------------------------------------------------------------
//---build signal fitfunc
TF1* GetFitFunc(TString Ch_n, histoFunc* wave, float t1, float t2) 
{
    Ch_n = "fitFunc_"+Ch_n;
    TF1* fitFunc = new TF1(Ch_n, wave, t1, t2, 4, "histoFunc");
    fitFunc->SetParLimits(1, 0.5, 1.5);
    fitFunc->FixParameter(3, 0.);
    fitFunc->SetNpx(100000);
    
    return fitFunc;
}

#endif
