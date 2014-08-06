/*
  Ricordati che la media sara' circa 10ns, cosi' recuperi la scala
  sembra buono:emailData147_140327184601

*/
#include "TROOT.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TH1F.h"
#include "TString.h"
#include "TSystem.h"
#include "TTree.h"
#include "TFile.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void plottersimple(TString runFolder)
{
  // General reset
  gROOT->Reset();

  // Define histograms
  TH1F* histo[3];
  char h1[10];
  for (int iw=0;iw<3;iw++){
    sprintf (h1,"Ch%d",iw);
    histo[iw] = new TH1F(h1,h1,1000,0.,50.);
    histo[iw] -> SetXTitle ("Time (ns)");
    histo[iw] -> SetYTitle ("Amplitude (V)");
    histo[iw]->SetLineWidth(2);
    histo[iw]->SetMaximum(0.05);
    histo[iw]->SetMinimum(-0.05);
  }
  histo[0]->SetLineColor(kRed+1);   
  histo[1]->SetLineColor(kBlue+1);  
  histo[2]->SetLineColor(kGreen+1); 

  // Tree branches and tree strcuture
  float ampMax[3];
  float tStamp[3]; 
  float tTime[3]; 
  float tTimeTreshold_50[3]; 
  float tTimeTreshold_40[3]; 
  float tTimeTreshold_30[3]; 
  float tTimeTreshold_20[3]; 
  float tTimeTreshold_15[3]; 
  float tTimeTreshold_10[3]; 
  float tTimeTreshold_5[3]; 
  TTree *nt = new TTree("nt","nt");
  nt->Branch("ampMax",&ampMax,"ampMax[3]/F"); 
  nt->Branch("tStamp",&tStamp,"tStamp[3]/F"); 
  nt->Branch("tTime",&tTime,"tTime[3]/F"); 
  nt->Branch("tTimeTreshold_50",&tTimeTreshold_50,"tTimeTreshold_50[3]/F"); 
  nt->Branch("tTimeTreshold_40",&tTimeTreshold_40,"tTimeTreshold_40[3]/F"); 
  nt->Branch("tTimeTreshold_30",&tTimeTreshold_30,"tTimeTreshold_30[3]/F"); 
  nt->Branch("tTimeTreshold_20",&tTimeTreshold_20,"tTimeTreshold_20[3]/F"); 
  nt->Branch("tTimeTreshold_15",&tTimeTreshold_15,"tTimeTreshold_15[3]/F"); 
  nt->Branch("tTimeTreshold_10",&tTimeTreshold_10,"tTimeTreshold_10[3]/F"); 
  nt->Branch("tTimeTreshold_5",&tTimeTreshold_5,"tTimeTreshold_5[3]/F"); 

  // define graphics
  TCanvas *c = new TCanvas("c","c");
  c ->cd(); 
  c->SetGrid();
  histo[0] -> Draw("");

  TLegend* leg = new TLegend(0.88,0.65,0.98,0.85);
  leg->SetFillColor(0);
  leg->SetTextFont(41);
  for (int iw=0;iw<3;iw++) {
    leg->AddEntry(histo[iw],histo[iw]->GetTitle(),"l");
  }
  leg->Draw();

  // dump the waveform file list into a temporary file
  TString command = "ls " + runFolder + "/*Ch1* > input.tmp"; 
  gSystem -> Exec(command); 

  // loop over the file and read data
  ifstream inFile("input.tmp");
  string line;
  int ifile=0;
  int isave=0;
  int iwin =0;

  // define file for wf
  TString fileWF = runFolder + "_wf_save.root"; 
  TFile *fw = new TFile(fileWF,"RECREATE");

  std::vector<float> waveFormAmp;

  TString waveFile[400];
  while(getline(inFile,line) && ifile<18000){  //  evento 17 e' buono per Run4
  //  while(getline(inFile,line) && ifile<1){  //  evento 17 e' buono per Run4
    // define wafeforms input
    //    std::cout << " >>> ok ci sono " << std::endl;
    //    waveFile[0] = (TString)line;
    waveFile[0] = line;

    Ssiz_t s=waveFile[0].Sizeof();
    waveFile[2] = (TString)waveFile[0].Replace(s-6,s,"3.txt"); // planacon
    waveFile[1] = (TString)waveFile[0].Replace(s-6,s,"2.txt"); // B-MCP1 trigger
    waveFile[0] = (TString)waveFile[0].Replace(s-6,s,"1.txt"); // B-MCP2 trigger
    ifile++;
  
    // read waveforms
    for (int iw=0;iw<3;iw++) {
      //cout << "Reading... " << waveFile[iw] << endl;

      float counts=0;
      ampMax[iw] = 0;
      tStamp[iw] = 0;
      tTime[iw] = 0;
      tTimeTreshold_50[iw] = 0;
      tTimeTreshold_40[iw] = 0;
      tTimeTreshold_30[iw] = 0;
      tTimeTreshold_20[iw] = 0;
      tTimeTreshold_15[iw] = 0;
      tTimeTreshold_10[iw] = 0;
      tTimeTreshold_5[iw] = 0;
      waveFormAmp.clear();

      std::ifstream infile (waveFile[iw].Data(), std::ios::in);
      for (int bin=1;bin<1001;bin++){
	infile >> counts;
	if (iw==2) counts = -counts; // planacon readout at the MCP output 
	                             // plane and not at the anode 

	//	if(iw == 0)	std::cout << " bin = " << bin << " " << counts << std::endl;
	if (counts < ampMax[iw]) {
	  ampMax[iw] = counts;
	  tStamp[iw] = bin; 
	  tTime[iw] = bin * 0.050; 
	}
	waveFormAmp.push_back(counts);
	histo[iw] -> SetBinContent(bin, counts);
      }
      histo[iw] -> Draw("same");
    
      //      if(iw == 0 ) {
// 	std::cout << " bin = " << tStamp[iw] << std::endl;
// 	std::cout << " ampMax = " << ampMax[iw] << std::endl;
      // other timing
      for(int bin=tStamp[iw];bin>1;bin--){
	waveFormAmp.at(bin-1);
// 	std::cout << " waveFormAmp = " << waveFormAmp.at(bin-1) << std::endl;
// 	std::cout << " bin = " << bin+1 << std::endl;
// 	std::cout << " * 0.5 = " << waveFormAmp.at(bin-1) * 0.5 << std::endl;

	if(waveFormAmp.at(bin-1) >= (ampMax[iw]*0.5) && tTimeTreshold_50[iw] == 0.) tTimeTreshold_50[iw] = (bin+1) * 0.05; 	
	if(waveFormAmp.at(bin-1) >= (ampMax[iw]*0.4) && tTimeTreshold_40[iw] == 0.) tTimeTreshold_40[iw] = (bin+1) * 0.05;
	if(waveFormAmp.at(bin-1) >= (ampMax[iw]*0.3) && tTimeTreshold_30[iw] == 0.) tTimeTreshold_30[iw] = (bin+1) * 0.05;
	if(waveFormAmp.at(bin-1) >= (ampMax[iw]*0.2) && tTimeTreshold_20[iw] == 0.) tTimeTreshold_20[iw] = (bin+1) * 0.05;
	if(waveFormAmp.at(bin-1) >= (ampMax[iw]*0.15) && tTimeTreshold_15[iw] == 0.) tTimeTreshold_15[iw] = (bin+1) * 0.05;
	if(waveFormAmp.at(bin-1) >= (ampMax[iw]*0.1) && tTimeTreshold_10[iw] == 0.) tTimeTreshold_10[iw] = (bin+1) * 0.05;
	if(waveFormAmp.at(bin-1) >= (ampMax[iw]*0.05) && tTimeTreshold_5[iw] == 0.) tTimeTreshold_5[iw] = (bin+1) * 0.05;
	if(tTimeTreshold_5[iw] != 0) {
// 	  std::cout << " iw = " << iw << std::endl;
//  	  std::cout << " tTimeTreshold_50 = " << tTimeTreshold_50[iw] << std::endl;
//  	  std::cout << " tTimeTreshold_40 = " << tTimeTreshold_40[iw] << std::endl;   
//  	  std::cout << " tTimeTreshold_30 = " << tTimeTreshold_30[iw] << std::endl;   
//  	  std::cout << " tTimeTreshold_20 = " << tTimeTreshold_20[iw] << std::endl;   
//  	  std::cout << " tTimeTreshold_15 = " << tTimeTreshold_15[iw] << std::endl;   
//  	  std::cout << " tTimeTreshold_10 = " << tTimeTreshold_10[iw] << std::endl;   
//  	  std::cout << " tTimeTreshold_5 = " << tTimeTreshold_5[iw] << std::endl;     
//  	  std::cout << " tTime = " << tTime[iw] << std::endl;     
	  break;
	}
      }
      //      }
    }


    // save interesting wavefo (VEEEEERY ROUGH)
    if (fabs((tStamp[1]-tStamp[0])*0.05-10.85)<1.2 ) {
      iwin++;
      if (tStamp[2]>320 && tStamp[2]<400 && ampMax[2]<-0.0026) {
	for (int iw=0;iw<3;iw++) {  
	  sprintf (h1,"Ch%d_%03d",iw,isave);
	  histo[iw]->SetName(h1);
	  histo[iw]->SetTitle(h1);
	  histo[iw]->Write();
	}
	isave++;
      }
    }

    // fill ntuple
    nt->Fill();
  }
  fw->Close();
  cout << "saved / triggered: " << isave << " / " << iwin << endl;
  
  TString filOut = runFolder + ".root"; 
  TFile *f = new TFile(filOut,"recreate"); 
  nt->Write();
  f->Close();
 
}
