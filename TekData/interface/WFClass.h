#ifndef __WFCLASS_H__
#define __WFCLASS_H__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

class WFClass 
{
public:
    //---ctors---
    WFClass() {};
    WFClass(int polarity, float tUnit);
    //---dtor---
    ~WFClass() {};

    //---getters---
    inline vector<float>* GetSamples() {return &samples_;};
    inline float          GetChi2() {return chi2_;};
    inline float          GetBaseline() {return baseline_;}
    float                 GetAmpMax(int min=-1, int max=-1);
    float                 GetTimeCF(float frac, int min=-1, int max=-1, int nFitSamples=5);
    float                 GetIntegral(int min=-1, int max=-1);
    float                 GetModIntegral(int min=-1, int max=-1);
    //---setters---
    void                  SetSignalWindow(int min, int max);
    void                  SetBaselineWindow(int min, int max);
    //---utils---
    void                  AddSample(float sample) {samples_.push_back(polarity_*sample);};
    bool                  SubtractBaseline(int min=-1, int max=-1);

private:
    vector<float> samples_;
    float         tUnit_;
    int           polarity_;
    int           sWinMin_;
    int           sWinMax_;
    int           bWinMin_;
    int           bWinMax_;
    int           maxSample_;
    float         baseline_;
    int           nFitSamples_;
    int           cfSample_;
    float         cfFrac_;
    float         cfTime_;
    float         chi2_;
};

#endif
