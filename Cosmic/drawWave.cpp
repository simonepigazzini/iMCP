#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include <algorithm> 

#include"TH2F.h"
#include"TCanvas.h"
#include"TFile.h"

using namespace std;

int main(int argc, char* argv[])
{
    int iCh=0;
    float sample_tmp=0;
    vector<float> samples[3];
    char ls_command[50];
    char* dir=argv[1];
    string file;
  
    sprintf(ls_command, "ls %s > tmp.wave", dir);
    system(ls_command);

    TFile* outPlots = TFile::Open("plots.root","recreate");
    outPlots->cd();
    TH2F* histo[3];
    for(int i=0; i<3; i++)
    {
	char histo_name[5];
	sprintf(histo_name, "Ch%d", i+1);
	histo[i] = new TH2F(histo_name, histo_name, 4000, 0, 4000, 500, -100, 100);
    }

    ifstream waveList("tmp.wave", ios::in);
    while(waveList >> file)
    {
	iCh++;
	if(iCh > 3) 
	    iCh=1;
	char current[100];
	sprintf(current, "%s%s", dir, file.c_str());
	cout << current << endl;
	ifstream inputFile(current, ios::in);
	while(inputFile >> sample_tmp)
	{
	    samples[iCh-1].push_back(sample_tmp*1000);
	}
	if(iCh == 3 && 
	   *(max_element(samples[2].begin(), samples[2].end())) > 5.)
	{
	    for(int i=0; i<3; i++)
	    {
		for(int iSample=0; iSample<4000; iSample++)
		{
		    histo[i]->Fill(iSample, samples[i].at(iSample));
		}
		samples[i].clear();
	    }
	}
	inputFile.close();
    }
    waveList.close();

    for(int i=0; i<3; i++)
	histo[i]->Write();
    outPlots->Close();
}
