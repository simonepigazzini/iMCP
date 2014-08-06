void doPlots_sim()
{
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
    TCanvas* c = new TCanvas("c","c",650,700);
    c->Divide(1,2);

    Double_t *pX, *pY;
    Double_t *pErrX, *pErrY;
    Double_t zeroY=0, zeroErrY=0;

    TMultiGraph* mg1 = new TMultiGraph("MiB2 Pb","MiB2 absorber scan (Pu)");
    TMultiGraph* mg2 = new TMultiGraph("MiB2 Cu","MiB2 absorber scan (Cu)");

    //---Pb
    TGraphErrors* g11 = new TGraphErrors("analyzed_data/MiB2_Q_vs_X0_PbLow_hodo.dat");
    g11->SetMarkerStyle(4);
    g11->SetMarkerColor(kBlue);
    g11->SetMarkerSize(0.7);
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
    g12->SetMarkerSize(0.7);
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

    //---Cu
    TGraphErrors* g21 = new TGraphErrors("analyzed_data/MiB2_Q_vs_X0_CuLow_hodo.dat");
    g21->SetMarkerStyle(4);
    g21->SetMarkerColor(kGreen);
    g21->SetMarkerSize(0.7);
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
    g22->SetMarkerColor(kGreen);
    g22->SetMarkerSize(0.7);
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

    c->cd(1);
    mg1->Add(g11);
    mg1->Add(g12);
    mg1->Add(g13);
    mg1->Add(g14);
    mg1->Draw("AP");
//    g13->Draw("Lsame");
//    g14->Draw("Lsame");
    mg1->GetXaxis()->SetTitle("Radiation Length [X0]");
    mg1->GetXaxis()->SetTitleSize(0.05);
    mg1->GetXaxis()->SetLabelSize(0.05);    
    mg1->GetYaxis()->SetTitle("Track Multiplicity");
    mg1->GetYaxis()->SetTitleSize(0.05);        
    mg1->GetYaxis()->SetLabelSize(0.05);
    mg1->GetYaxis()->SetRangeUser(0, 2.8);
    TLegend* lg1 = new TLegend(0.75,0.5,0.9,0.95);
    lg1->SetFillColor(kWhite);
    lg1->SetTextFont(42);
    lg1->SetTextSize(0.04);
    lg1->AddEntry(g11,"Pb HV=2800V","P");
    lg1->AddEntry(g12,"Pb HV=2900V","P");
    lg1->AddEntry(g13,"Pb Sim","P");
    lg1->Draw("same");

    c->cd(2);
    mg2->Add(g21);
    mg2->Add(g22);
    mg2->Add(g23);
    mg2->Add(g24);
    mg2->Draw("AP");
//    g23->Draw("Lsame");
//    g24->Draw("Lsame");
    mg2->GetXaxis()->SetTitle("Radiation Length [X0]");
    mg2->GetXaxis()->SetTitleSize(0.05);
    mg2->GetXaxis()->SetLabelSize(0.05);    
    mg2->GetYaxis()->SetTitle("Track Multiplicity");
    mg2->GetYaxis()->SetTitleSize(0.05);        
    mg2->GetYaxis()->SetLabelSize(0.05);
    mg2->GetYaxis()->SetRangeUser(0, 2.5);
    TLegend* lg2 = new TLegend(0.75,0.5,0.9,0.95);
    lg2->SetFillColor(kWhite);
    lg2->SetTextFont(42);
    lg2->SetTextSize(0.04);
    lg2->AddEntry(g21,"Cu HV=2800V","P");
    lg2->AddEntry(g22,"Cu HV=2900V","P");
    lg2->AddEntry(g23,"Cu Sim","P");
    lg2->Draw("same");

    c->Print("plots/MiB2_X0_sim.pdf");
}
