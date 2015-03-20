void doPlots_sim()
{
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
    TCanvas* c = new TCanvas();
    TPad* p1 = new TPad("pad1", "Cu plot", 0.0, 0.3, 0.5, 1.0, 21);
    TPad* p2 = new TPad("pad2", "Cu pull", 0.5, 0.3, 1.0, 1.0, 21);
    TPad* p3 = new TPad("pad3", "Pb plot", 0.0, 0.0, 0.5, 0.3, 21);
    TPad* p4 = new TPad("pad4", "Pb pull", 0.5, 0.0, 1.0, 0.3, 21); 
    p1->SetFillColor(kWhite);
    p2->SetFillColor(kWhite);
    p3->SetFillColor(kWhite);
    p4->SetFillColor(kWhite);
    p1->Draw();
    p2->Draw();
    p3->Draw();
    p4->Draw();

    Double_t *pX, *pY;
    Double_t *pErrX, *pErrY;
    Double_t zeroY=0, zeroErrY=0;

    //---graph
    TMultiGraph* mg1 = new TMultiGraph("MiB2 Pb","Ekron response to shower (Pb)");
    TMultiGraph* mg2 = new TMultiGraph("MiB2 Cu","Ekron response to shower (Cu)");
    //---pull
    TMultiGraph* mgp1 = new TMultiGraph("MiB2 Pb pull","");
    TMultiGraph* mgp2 = new TMultiGraph("MiB2 Cu pull","");

    //---Pb
    TGraphErrors* g11 = new TGraphErrors("analyzed_data/MiB2_Q_vs_X0_PbLow_hodo.dat");
    g11->SetMarkerStyle(4);
    g11->SetMarkerColor(kBlue);
    g11->SetMarkerSize(0.9);
    pX = g11->GetX();
    pY = g11->GetY();
    pErrX = g11->GetEX();
    pErrY = g11->GetEY();
    zeroY = pY[0];
    zeroErrY = pErrY[0];
    for(int ip=0; ip<g11->GetN(); ip++)
    {
	g11->SetPoint(ip, pX[ip], pY[ip]/zeroY);
	g11->SetPointError(ip, pErrX[ip], (TMath::Sqrt(pErrY[ip]**2+(zeroErrY*pY[ip]/zeroY)**2))/zeroY);
    }
    TGraphErrors* g12 = new TGraphErrors("analyzed_data/MiB2_Q_vs_X0_PbHigh_hodo.dat");
    g12->SetMarkerStyle(20);
    g12->SetMarkerColor(kBlue);
    g12->SetMarkerSize(0.9);
    pX = g12->GetX();
    pY = g12->GetY();
    pErrX = g12->GetEX();
    pErrY = g12->GetEY();
    zeroY = pY[0];
    zeroErrY = pErrY[0];
    for(int ip=0; ip<g12->GetN(); ip++)
    {
	g12->SetPoint(ip, pX[ip], pY[ip]/zeroY);
	g12->SetPointError(ip, pErrX[ip], (TMath::Sqrt(pErrY[ip]**2+(zeroErrY*pY[ip]/zeroY)**2))/zeroY);
    }
    TGraph* g13 = new TGraph("analyzed_data/Pb_X0_sim_det_resp.dat");
    g13->SetMarkerStyle(20);
    g13->SetMarkerColor(kBlack);
    g13->SetMarkerSize(1);
    TGraph* g14 = new TGraph("analyzed_data/Pb_X0_sim.dat");
    g14->SetMarkerStyle(20);
    g14->SetMarkerColor(kMagenta);
    g14->SetMarkerSize(1);
    //---pull histos
    TGraphErrors* pull_11 = new TGraphErrors("analyzed_data/MiB2_Q_vs_X0_pull_Pb_High.dat");
    pull_11->SetMarkerStyle(20);
    pull_11->SetMarkerColor(kBlue);
    pull_11->SetMarkerSize(1);
    TGraphErrors* pull_12 = new TGraphErrors("analyzed_data/MiB2_Q_vs_X0_pull_Pb_Low.dat");
    pull_12->SetMarkerStyle(4);
    pull_12->SetMarkerColor(kBlue);
    pull_12->SetMarkerSize(1);

    //---Cu
    TGraphErrors* g21 = new TGraphErrors("analyzed_data/MiB2_Q_vs_X0_CuLow_hodo.dat");
    g21->SetMarkerStyle(4);
    g21->SetMarkerColor(kOrange+2);
    g21->SetMarkerSize(0.9);
    pX = g21->GetX();
    pY = g21->GetY();
    pErrX = g21->GetEX();
    pErrY = g21->GetEY();
    zeroY = pY[0];
    zeroErrY = pErrY[0];
    for(int ip=0; ip<g21->GetN(); ip++)
    {
	g21->SetPoint(ip, pX[ip], pY[ip]/zeroY);
	g21->SetPointError(ip, pErrX[ip], (TMath::Sqrt(pErrY[ip]**2+(zeroErrY*pY[ip]/zeroY)**2))/zeroY);
    }
    TGraphErrors* g22 = new TGraphErrors("analyzed_data/MiB2_Q_vs_X0_CuHigh_hodo.dat");
    g22->SetMarkerStyle(20);
    g22->SetMarkerColor(kOrange+2);
    g22->SetMarkerSize(0.9);
    pX = g22->GetX();
    pY = g22->GetY();
    pErrX = g22->GetEX();
    pErrY = g22->GetEY();
    zeroY = pY[0];
    zeroErrY = pErrY[0];
    for(int ip=0; ip<g22->GetN(); ip++)
    {
	g22->SetPoint(ip, pX[ip], pY[ip]/zeroY);
	g22->SetPointError(ip, pErrX[ip], (TMath::Sqrt(pErrY[ip]**2+(zeroErrY*pY[ip]/zeroY)**2))/zeroY);
    }
    TGraph* g23 = new TGraphErrors("analyzed_data/Cu_X0_sim_len.dat");
    g23->SetMarkerStyle(20);
    g23->SetMarkerColor(kBlack);
    g23->SetMarkerSize(1);   
    TGraph* g24 = new TGraph("analyzed_data/Cu_X0_sim_corr.dat");
    g24->SetMarkerStyle(20);
    g24->SetMarkerColor(kMagenta);
    g24->SetMarkerSize(1);   
    //---pull histos
    TGraphErrors* pull_21 = new TGraphErrors("analyzed_data/MiB2_Q_vs_X0_pull_Cu_High.dat");
    pull_21->SetMarkerStyle(20);
    pull_21->SetMarkerColor(kGreen);
    pull_21->SetMarkerSize(1);
    TGraphErrors* pull_22 = new TGraphErrors("analyzed_data/MiB2_Q_vs_X0_pull_Cu_Low.dat");
    pull_22->SetMarkerStyle(4);
    pull_22->SetMarkerColor(kGreen);
    pull_22->SetMarkerSize(1);

    //---refernce pull function f=1;
    TF1* pull_fit = new TF1("pull_fit", "pol0", 0, 10);
    pull_fit->SetParameter(0, 1);
    pull_fit->SetNpx(10000);

    //-----DRAW-----
    //---graph Pb
    p1->cd();
    mg1->Add(g11);
    mg1->Add(g12);
    mg1->Add(g13);
    mg1->Draw("AP");
    mg1->GetXaxis()->SetTitle("Radiation Length [X0]");
    mg1->GetXaxis()->SetTitleSize(0.05);
    mg1->GetXaxis()->SetLabelSize(0.05);    
    mg1->GetYaxis()->SetTitle("Track Multiplicity");
    mg1->GetYaxis()->SetTitleSize(0.05);        
    mg1->GetYaxis()->SetLabelSize(0.05);
    mg1->GetYaxis()->SetRangeUser(0, 2.8);
    TLegend* lg1 = new TLegend(0.6,0.6,0.95,0.85);
    lg1->SetFillStyle(0);
    lg1->SetBorderSize(0);
    lg1->AddEntry(g11,"Pb 2660V","P");
    lg1->AddEntry(g12,"Pb 2775V","P");
    lg1->AddEntry(g13,"Pb Sim","P");
    lg1->Draw("same");
    //---pull Pb
    p3->cd();
    mgp1->Add(pull_11);
    mgp1->Add(pull_12);
    mgp1->Draw("AP");
    pull_fit->Draw("same");

    //---graph Cu
    p2->cd();
    mg2->Add(g21);
    mg2->Add(g22);
    mg2->Add(g23);
    mg2->Draw("AP");
    mg2->GetXaxis()->SetTitle("Radiation Length [X0]");
    mg2->GetXaxis()->SetTitleSize(0.05);
    mg2->GetXaxis()->SetLabelSize(0.05);    
    mg2->GetYaxis()->SetTitle("Track Multiplicity");
    mg2->GetYaxis()->SetTitleSize(0.05);        
    mg2->GetYaxis()->SetLabelSize(0.05);
    mg2->GetYaxis()->SetRangeUser(0, 2.5);
    TLegend* lg2 = new TLegend(0.6,0.6,0.95,0.85);
    lg2->SetFillStyle(0);
    lg2->SetBorderSize(0);
    lg2->AddEntry(g21,"Cu 2660V","P");
    lg2->AddEntry(g22,"Cu 2775V","P");
    lg2->AddEntry(g23,"Cu Sim","P");
    lg2->Draw("same");
    //---pull Cu
    p4->cd();
    mgp2->Add(pull_21);
    mgp2->Add(pull_22);
    mgp2->Draw("AP");
    pull_fit->Draw("same");

    c->Print("plots/MiB2_X0_sim.pdf");
}
