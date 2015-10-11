void doPlots_abs()
{
    TCanvas* c = new TCanvas();
    //c->Divide(1,2);

    Double_t *pX, *pY;
    Double_t *pErrX, *pErrY;
    Double_t zeroY=0, zeroErrY=0;

    TMultiGraph* mg1 = new TMultiGraph("MiB3 & Roma1 absorber scan (charge)","MiB2 absorber scan (charge)");
    TMultiGraph* mg3 = new TMultiGraph("MiB3 & Roma1 absorber scan (efficency)","Ekron i-MCP efficiency vs absorber tickness");

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
    TGraphErrors* g13 = new TGraphErrors("analyzed_data/MiB2_Q_vs_X0_CuLow_hodo.dat");
    g13->SetMarkerStyle(4);
    g13->SetMarkerColor(kGreen);
    g13->SetMarkerSize(0.7);
    pX = g13->GetX();
    pY = g13->GetY();
    pErrX = g13->GetEX();
    pErrY = g13->GetEY();
    zeroY = pY[0];
    zeroErrY = pErrY[0];
    for(int ip=0; ip<g13->GetN(); ip++)
    {
	g13->SetPoint(ip, pX[ip], pY[ip]/zeroY);
	g13->SetPointError(ip, pErrX[ip], (TMath::Sqrt(pErrY[ip]**2+(zeroErrY*pY[ip]/zeroY)**2))/zeroY);
    }
    TGraphErrors* g14 = new TGraphErrors("analyzed_data/MiB2_Q_vs_X0_CuHigh_hodo.dat");
    g14->SetMarkerStyle(20);
    g14->SetMarkerColor(kGreen);
    g14->SetMarkerSize(0.7);
    pX = g14->GetX();
    pY = g14->GetY();
    pErrX = g14->GetEX();
    pErrY = g14->GetEY();
    zeroY = pY[0];
    zeroErrY = pErrY[0];
    for(int ip=0; ip<g14->GetN(); ip++)
    {
	g14->SetPoint(ip, pX[ip], pY[ip]/zeroY);
	g14->SetPointError(ip, pErrX[ip], (TMath::Sqrt(pErrY[ip]**2+(zeroErrY*pY[ip]/zeroY)**2))/zeroY);
    }
/*    TGraph* g15 = new TGraph("analyzed_data/Cu_X0_sim_3MeV.dat");
    g15->SetMarkerStyle(20);
    g15->SetMarkerColor(kBlack);
    g15->SetMarkerSize(1);   
*/
    TGraphErrors* g31 = new TGraphErrors("analyzed_data/MiB2_eff_vs_X0_PbLow_hodo.dat");
    g31->SetMarkerStyle(4);
    g31->SetMarkerColor(kBlue);
    g31->SetMarkerSize(0.9);
    TGraphErrors* g32 = new TGraphErrors("analyzed_data/MiB2_eff_vs_X0_PbHigh_hodo.dat");
    g32->SetMarkerStyle(20);
    g32->SetMarkerColor(kBlue);
    g32->SetMarkerSize(0.9);
    TGraphErrors* g33 = new TGraphErrors("analyzed_data/MiB2_eff_vs_X0_CuLow_hodo.dat");
    g33->SetMarkerStyle(4);
    g33->SetMarkerColor(kOrange+2);
    g33->SetMarkerSize(0.9);
    g33->SetPoint(0,0,0.48062); //MiB2 correction
    TGraphErrors* g34 = new TGraphErrors("analyzed_data/MiB2_eff_vs_X0_CuHigh_hodo.dat");
    g34->SetMarkerStyle(20);
    g34->SetMarkerColor(kOrange+2);
    g34->SetMarkerSize(0.9);
    TGraphErrors* g35 = new TGraphErrors();
    g35->SetMarkerStyle(20);
    g35->SetMarkerColor(kMagenta);
    g35->SetMarkerSize(0.7);
    g35->SetPoint(0,0,0.440162);
    g35->SetPointError(0,0,0.00999837);

//     c->cd(1);
//     mg1->Add(g11);
//     mg1->Add(g12);
//     mg1->Add(g13);
//     mg1->Add(g14);
// //    mg1->Add(g15);
//     mg1->Draw("AP");
//     mg1->GetXaxis()->SetTitle("Radiation Length [X0]");
//     mg1->GetXaxis()->SetTitleSize(0.05);
//     mg1->GetXaxis()->SetLabelSize(0.05);    
//     mg1->GetYaxis()->SetTitle("Charge [arbitrary units]");
//     mg1->GetYaxis()->SetTitleSize(0.05);        
//     mg1->GetYaxis()->SetLabelSize(0.05);
// //    TGaxis::SetMaxDigits(3);
// //    mg1->GetYaxis()->SetNdivisions(510,0);
//     TLegend* lg1 = new TLegend(0.75,0.5,0.95,0.95);
//     lg1->SetFillColor(kWhite);
//     lg1->AddEntry(g11,"Pb HV=2800","P");
//     lg1->AddEntry(g12,"Pb HV=2900","P");
//     lg1->AddEntry(g13,"Cu HV=2800","P");    
//     lg1->AddEntry(g14,"Cu HV=2900","P");
// //    lg1->AddEntry(g15,"Cu Sim","P");
//     lg1->Draw("same");

//     c->cd(2);
    mg3->Add(g31);
    mg3->Add(g32);
    mg3->Add(g33);
    mg3->Add(g34);
    mg3->Draw("AP");
    mg3->GetXaxis()->SetTitle("Radiation Length [X0]");
    mg3->GetXaxis()->SetTitleSize(0.05);
    mg3->GetXaxis()->SetLabelSize(0.05);
    mg3->GetYaxis()->SetTitle("Efficency");
    mg3->GetYaxis()->SetTitleSize(0.05);
    mg3->GetYaxis()->SetLabelSize(0.05);
    mg3->GetYaxis()->SetRangeUser(0,1.01);TLegend* lg1 = new TLegend(0.6,0.6,0.95,0.85);
    lg1->SetFillStyle(0);
    lg1->SetBorderSize(0);
    lg1->AddEntry(g31,"Pb 2660V","P");
    lg1->AddEntry(g32,"Pb 2775V","P");
    lg1->AddEntry(g33,"Cu 2660V","P");    
    lg1->AddEntry(g34,"Cu 2775V","P");
    lg1->Draw("same");

    c->Print("plots/MiB2__X0.pdf");
}
