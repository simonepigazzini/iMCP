#include "../interface/WFClass.h"

//**********Constructors******************************************************************
WFClass::WFClass(int polarity, float tUnit):
    polarity_(polarity), tUnit_(tUnit), sWinMin_(-1), sWinMax_(-1), 
    bWinMin_(-1), bWinMax_(-1),  maxSample_(-1), baseline_(-1), nFitSamples_(5), 
    cfSample_(-1), cfFrac_(-1), cfTime_(-1), chi2_(-1)
{}

//**********Getters***********************************************************************

//----------Get the max/min amplitude wrt polarity----------------------------------------
float WFClass::GetAmpMax(int min, int max)
{
    //---check if signal window is valid
    if(min==max && max==-1 && sWinMin_==sWinMax_ && sWinMax_==-1)
        return -1;
    //---setup signal window
    if(min!=-1 && max!=-1)
        SetSignalWindow(min, max);
    //---return the max if already computed
    else if(maxSample_ != -1)
        return samples_.at(maxSample_);

    //---find the max
    maxSample_=sWinMin_;
    for(int iSample=sWinMin_; iSample<sWinMax_; iSample++)
    {
        if(samples_.at(iSample) > samples_.at(maxSample_)) 
	    maxSample_ = iSample;
    }    
    return samples_.at(maxSample_);
}

//----------Get CF time for a given fraction and in a given range-------------------------
float WFClass::GetTimeCF(float frac, int min, int max, int nFitSamples)
{
    if(frac == cfFrac_ && cfSample_ != -1)
        return cfTime_;

    //---definitions---
    float xx= 0.;
    float xy= 0.;
    float Sx = 0.;
    float Sy = 0.;
    float Sxx = 0.;
    float Sxy = 0.;
    //---setups---
    int tStart=min;
    if(tStart == -1)
        tStart=sWinMin_ == -1 ? 0 : sWinMin_;
    cfSample_ = tStart;
    cfFrac_ = frac;
    if(maxSample_ == -1)
        GetAmpMax(min, max);
    if(frac == 1) 
        return maxSample_;
    
    //---find first sample above Amax*frac
    for(int iSample=maxSample_; iSample>tStart; iSample--)
    {
        if(samples_.at(iSample) < samples_.at(maxSample_)*frac) 
        {
            cfSample_ = iSample;
            break;
        }
    }
    for(int n=-(nFitSamples_-1)/2; n<=(nFitSamples_-1)/2; n++)
    {
        if(cfSample_+n<0 || cfSample_+n>=samples_.size()) 
	    continue;
        xx = (cfSample_+n)*(cfSample_+n)*tUnit_*tUnit_;
        xy = (cfSample_+n)*tUnit_*(samples_.at(cfSample_+n));
        Sx = Sx + (cfSample_+n)*tUnit_;
        Sy = Sy + samples_.at(cfSample_+n);
        Sxx = Sxx + xx;
        Sxy = Sxy + xy;
    }

    float Delta = nFitSamples_*Sxx - Sx*Sx;
    float A = (Sxx*Sy - Sx*Sxy) / Delta;
    float B = (nFitSamples_*Sxy - Sx*Sy) / Delta;
    
    //---compute chi2---
    chi2_ = 0.;
    float sigma2 = pow(tUnit_/sqrt(12)*B,2);
 
    for(int n=-(nFitSamples_-1)/2; n<=(nFitSamples_-1)/2; n++)
    {
        if(cfSample_+n<0 || cfSample_+n>=samples_.size()) 
            continue;
        chi2_ = chi2_ + pow(samples_.at(cfSample_+n) - A - B*((cfSample_+n)*tUnit_),2)/sigma2;
    } 

    //---A+Bx = frac * amp
    cfTime_ = (samples_.at(maxSample_) * frac - A) / B;
    return cfTime_;
}

//----------Get the signal integral in the given range------------------------------------
float WFClass::GetIntegral(int min, int max)
{
    //---compute integral
    float integral=0;
    for(int bin=min; bin<max; bin++)
        integral += samples_.at(bin);

    return integral;
}

//----------Get the integral of Abs(WF) over the given range------------------------------
float WFClass::GetModIntegral(int min, int max)
{   
    float integral=0;
    for(int bin=min; bin<max; bin++)
    {
	if(samples_.at(bin) < 0)
	    integral -= samples_.at(bin);
	else
	    integral += samples_.at(bin);
    }
    return integral;
}

//**********Setters***********************************************************************

//----------Set the signal window---------------------------------------------------------
void WFClass::SetSignalWindow(int min, int max)
{
    sWinMin_=min;
    sWinMax_=max;
}

//----------Set the baseline window-------------------------------------------------------
void WFClass::SetBaselineWindow(int min, int max)
{
    bWinMin_=min;
    bWinMax_=max;
}

//**********Utils*************************************************************************

//---------estimate the baseline in a given range and then subtract it from the signal----
bool WFClass::SubtractBaseline(int min, int max)
{
    if(min!=-1 && max==-1)
    {
        bWinMin_=min;
        bWinMax_=max;
    }
    //---compute baseline
    float baseline_=0;
    for(int iSample=bWinMin_; iSample<bWinMax_; iSample++)
    {
        baseline_ += samples_.at(iSample);
    }
    baseline_ = baseline_/((float)(bWinMax_-bWinMin_));
    //---subtract baseline
    for(int iSample=0; iSample<samples_.size(); iSample++)
        samples_.at(iSample) = (samples_.at(iSample) - baseline_);
    
    return true;
}
