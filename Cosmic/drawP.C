{

  TFile *f = new TFile("WFRun006.root","r");
  TTree* ntu = (TTree*)f->Get("nt");
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
  ntu->SetBranchAddress("ampMax",ampMax);
  ntu->SetBranchAddress("tStamp",tStamp);
  ntu->SetBranchAddress("tTime",tTime);
  ntu->SetBranchAddress("tTime",tTime);
  ntu->SetBranchAddress("tTimeTreshold_50",tTimeTreshold_50);
  ntu->SetBranchAddress("tTimeTreshold_40",tTimeTreshold_40);
  ntu->SetBranchAddress("tTimeTreshold_30",tTimeTreshold_30);
  ntu->SetBranchAddress("tTimeTreshold_20",tTimeTreshold_20);
  ntu->SetBranchAddress("tTimeTreshold_15",tTimeTreshold_15);
  ntu->SetBranchAddress("tTimeTreshold_10",tTimeTreshold_10);
  ntu->SetBranchAddress("tTimeTreshold_5",tTimeTreshold_5);


  TH1F *hf = new TH1F("hf", "", 30000, 0., 30.);
  hf->GetXaxis()->SetRangeUser(5., 18.); 
  hf->SetXTitle("Time [ns]");
  hf->SetYTitle("Ampliture [mV]");

  TH1F *hf_50 = new TH1F("hf_50", "", 30000, 0., 30.);
  hf_50->GetXaxis()->SetRangeUser(5., 18.); 
  hf_50->SetXTitle("Time [ns]");
  hf_50->SetYTitle("Ampliture [mV]");

  TH1F *hf_40 = new TH1F("hf_40", "", 30000, 0., 30.);
  hf_40->GetXaxis()->SetRangeUser(5., 18.); 
  hf_40->SetXTitle("Time [ns]");
  hf_40->SetYTitle("Ampliture [mV]");

  TH1F *hf_30 = new TH1F("hf_30", "", 30000, 0., 30.);
  hf_30->GetXaxis()->SetRangeUser(5., 18.); 
  hf_30->SetXTitle("Time [ns]");
  hf_30->SetYTitle("Ampliture [mV]");

  TH1F *hf_20 = new TH1F("hf_20", "", 30000, 0., 30.);
  hf_20->GetXaxis()->SetRangeUser(5., 18.); 
  hf_20->SetXTitle("Time [ns]");
  hf_20->SetYTitle("Ampliture [mV]");

  TH1F *hf_15 = new TH1F("hf_15", "", 30000, 0., 30.);
  hf_15->GetXaxis()->SetRangeUser(5., 18.); 
  hf_15->SetXTitle("Time [ns]");
  hf_15->SetYTitle("Ampliture [mV]");

  TH1F *hf_10 = new TH1F("hf_10", "", 30000, 0., 30.);
  hf_10->GetXaxis()->SetRangeUser(5., 18.); 
  hf_10->SetXTitle("Time [ns]");
  hf_10->SetYTitle("Ampliture [mV]");

  TH1F *hf_5 = new TH1F("hf_5", "", 30000, 0., 30.);
  hf_5->GetXaxis()->SetRangeUser(5., 18.); 
  hf_5->SetXTitle("Time [ns]");
  hf_5->SetYTitle("Ampliture [mV]");

  std::cout << " >>> number of WF = " << ntu->GetEntries() << std::endl;
  for(int nWF=0; nWF<ntu->GetEntries(); ++nWF){
  //  for(int nWF=0; nWF<1; ++nWF){
    ntu->GetEntry(nWF);

     hf->Fill((tTime[1] - tTime[0]));
     hf_50->Fill((tTimeTreshold_50[1] - tTimeTreshold_50[0]));
     hf_40->Fill((tTimeTreshold_40[1] - tTimeTreshold_40[0]));
     hf_30->Fill((tTimeTreshold_30[1] - tTimeTreshold_30[0]));
     hf_20->Fill((tTimeTreshold_20[1] - tTimeTreshold_20[0]));
     hf_15->Fill((tTimeTreshold_15[1] - tTimeTreshold_15[0]));
     hf_10->Fill((tTimeTreshold_10[1] - tTimeTreshold_10[0]));
     hf_5->Fill((tTimeTreshold_5[1] - tTimeTreshold_5[0]));
     //     std::cout << " >>> t0 = " << tTime[0] << " t1 = " << tTime[1] << " diff = " << t0-t1  << std::endl;
  }

  hf->SetLineColor(kBlack);
  hf_50->SetLineColor(kRed);
  hf_40->SetLineColor(kOrange-3);
  hf_30->SetLineColor(kGreen+1);
  hf_20->SetLineColor(kCyan+1);
  hf_15->SetLineColor(kBlue);
  hf_10->SetLineColor(kViolet+1);
  hf_5->SetLineColor(kMagenta);

  hf->SetLineWidth(2);
  hf_50->SetLineWidth(2);
  hf_40->SetLineWidth(2);
  hf_30->SetLineWidth(2);
  hf_20->SetLineWidth(2);
  hf_15->SetLineWidth(2);
  hf_10->SetLineWidth(2);
  hf_5->SetLineWidth(2);


  TLegend* leg = new TLegend(0.88,0.65,0.98,0.85);
  leg->SetFillColor(0);
  leg->SetTextFont(41);
  leg->AddEntry(hf, "ampMax", "l");
  leg->AddEntry(hf_50, "50% ampMax", "l");
  leg->AddEntry(hf_40, "40% ampMax", "l");
  leg->AddEntry(hf_30, "30% ampMax", "l");
  leg->AddEntry(hf_20, "20% ampMax", "l");
  leg->AddEntry(hf_15, "15% ampMax", "l");
  leg->AddEntry(hf_10, "10% ampMax", "l");
  leg->AddEntry(hf_5, "5% ampMax", "l");

  int nReb = 50;
  hf->Rebin(nReb);
  hf_50->Rebin(nReb);
  hf_40->Rebin(nReb);
  hf_30->Rebin(nReb);
  hf_20->Rebin(nReb);
  hf_15->Rebin(nReb);
  hf_10->Rebin(nReb);
  hf_5->Rebin(nReb);

  hf->GetYaxis()->SetRangeUser(0., 150.);
  hf->GetXaxis()->SetRangeUser(6., 18.);
  hf_50->GetXaxis()->SetRangeUser(6., 18.);
  hf_40->GetXaxis()->SetRangeUser(6., 18.);
  hf_30->GetXaxis()->SetRangeUser(6., 18.);
  hf_20->GetXaxis()->SetRangeUser(6., 18.);
  hf_15->GetXaxis()->SetRangeUser(6., 18.);
  hf_10->GetXaxis()->SetRangeUser(6., 18.);
  hf_5->GetXaxis()->SetRangeUser(6., 18.);

  
  TF1* func = new TF1("func", "[0] * (exp(-0.5 * pow( (x-[1])/[2], 2) ) + [3])", 8., 16.);
  func->SetParName(0, "Amp");
  func->SetParName(1, "mean");
  func->SetParName(2, "sigma");
  func->SetParName(3, "ossfet");
  func->SetParameter(0, 800.);
  func->SetParameter(1, 11.);
  func->SetParameter(2, 0.04);
  func->SetParameter(3, 10.);

  func->SetParLimits(2, 0., 10.);
  
  TF1* func_50 = (TF1*)func->Clone("func_50");
  TF1* func_40 = (TF1*)func->Clone("func_40");
  TF1* func_30 = (TF1*)func->Clone("func_30");
  TF1* func_20 = (TF1*)func->Clone("func_20");
  TF1* func_15 = (TF1*)func->Clone("func_15");
  TF1* func_10 = (TF1*)func->Clone("func_10");
  TF1* func_5 = (TF1*)func->Clone("func_5");

  func->SetLineColor(kBlack);
  func_50->SetLineColor(kRed);
  func_40->SetLineColor(kOrange-3);
  func_30->SetLineColor(kGreen+1);
  func_20->SetLineColor(kCyan+1);
  func_15->SetLineColor(kBlue);
  func_10->SetLineColor(kViolet+1);
  func_5->SetLineColor(kMagenta);


  TCanvas *c = new TCanvas("c","c"); 
  c->cd();
  hf->Draw();
  hf_50->Draw("same");
  hf_40->Draw("same");
  hf_30->Draw("same");
  hf_20->Draw("same");
  hf_15->Draw("same");
  hf_10->Draw("same");
  hf_5->Draw("same");
  leg->Draw("same");

  hf->Fit("func", "RQ");
  hf_50->Fit("func_50", "RQ");
  hf_40->Fit("func_40", "RQ");
  hf_30->Fit("func_30", "RQ");
  hf_20->Fit("func_20", "RQ");
  hf_15->Fit("func_15", "RQ");
  hf_10->Fit("func_10", "RQ");
  hf_5->Fit("func_5", "RQ");


  TCanvas *cf = new TCanvas("cf","cf"); 
  cf->cd();
  hf->Draw();
  hf_50->Draw("same");
  hf_40->Draw("same");
  hf_30->Draw("same");
  hf_20->Draw("same");
  hf_15->Draw("same");
  hf_10->Draw("same");
  hf_5->Draw("same");
  leg->Draw("same");

  TGraph* sigmas = new TGraph();
  sigmas->SetPoint(0, 0.05, func_5->GetParameter(2));
  sigmas->SetPoint(1, 0.10, func_10->GetParameter(2));
  sigmas->SetPoint(2, 0.15, func_15->GetParameter(2));
  sigmas->SetPoint(3, 0.20, func_20->GetParameter(2));
  sigmas->SetPoint(4, 0.30, func_30->GetParameter(2));
  sigmas->SetPoint(5, 0.40, func_40->GetParameter(2));
  sigmas->SetPoint(6, 0.50, func_50->GetParameter(2));
  sigmas->SetPoint(7, 1., func->GetParameter(2));

  sigmas->SetMarkerStyle(20);
  TCanvas* c2 = new TCanvas();
  c2->cd();
  sigmas->GetXaxis()->SetTitle("ampMax (%)");
  sigmas->GetYaxis()->SetTitle("sigma [ns]");
  sigmas->Draw("ap");

  TGraph* Chi2 = new TGraph();
  Chi2->SetPoint(0, 0.05, func_5->GetChisquare()/func_5->GetNDF());
  Chi2->SetPoint(1, 0.10, func_10->GetChisquare()/func_10->GetNDF());
  Chi2->SetPoint(2, 0.15, func_15->GetChisquare()/func_15->GetNDF());
  Chi2->SetPoint(3, 0.20, func_20->GetChisquare()/func_20->GetNDF());
  Chi2->SetPoint(4, 0.30, func_30->GetChisquare()/func_30->GetNDF());
  Chi2->SetPoint(5, 0.40, func_40->GetChisquare()/func_40->GetNDF());
  Chi2->SetPoint(6, 0.50, func_50->GetChisquare()/func_50->GetNDF());
  Chi2->SetPoint(7, 1., func->GetChisquare()/func->GetNDF());

  Chi2->SetMarkerStyle(20);
  TCanvas* c2 = new TCanvas();
  c2->cd();
  Chi2->GetXaxis()->SetTitle("ampMax (%)");
  Chi2->GetYaxis()->SetTitle("chi2");
  Chi2->Draw("ap");

}
