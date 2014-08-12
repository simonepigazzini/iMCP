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

#define SAMPLING_UNIT 0.05
#define TOT_SAMPLES 4000

using namespace std;

//------------------------------------------------------------------------------
//---initialize all the pointer addressed by the outTree branches
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
	outTree->Branch("amp_max_"+nameMCP[iCh],  &(amp_max[iCh]), "amp_max_"+nameMCP[iCh]+"/F");
	outTree->Branch("charge_tot_"+nameMCP[iCh],  &(charge_tot[iCh]), "charge_tot_"+nameMCP[iCh]+"/F");
	outTree->Branch("charge_sig_"+nameMCP[iCh],  &(charge_sig[iCh]), "charge_sig_"+nameMCP[iCh]+"/F");
	outTree->Branch("baseline_"+nameMCP[iCh], &(baseline[iCh]), "baseline_"+nameMCP[iCh]+"/F");
    }
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
    baseline = baseline/((float)(tb2-tb1));
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
                    float step=SAMPLING_UNIT, int Nsamples = 5)
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
        return minSample;
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
        if(cfSample+n<0) 
	    continue;
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
        if(cfSample+n<0) 
	    continue;
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
        if(samples->at(iSample) < samples->at(minSample)) 
	    minSample = iSample;
    }
    return -samples->at(minSample);
}

//---------------------------------------------------------------------------------------
//---compute charge for a given signal in a given samples interval 
float ComputeIntegral(int t1, int t2, const vector<float>* samples)
{
    float integral=0;
    for(int bin=t1; bin<t2; bin++)
        integral += samples->at(bin);

    return -integral;
}

//---------------------------------------------------------------------------------------
//---compute module for a given signal in a given samples interval 
float ComputeModIntegral(int t1, int t2, const vector<float>* samples)
{
    float integral=0;
    for(int bin=t1; bin<t2; bin++)
    {
	if(samples->at(bin) < 0)
	    integral -= samples->at(bin);
	else
	    integral += samples->at(bin);
    }
    return integral;
}

#endif
