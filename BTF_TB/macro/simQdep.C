void simQdep()
{
    for(int x0=1; x0<12; x0++)
    {
	TCanvas* c1 = new TCanvas();
	c1->cd();
	char file_name[60];
	sprintf(file_name, "G4_sim/Pb_FascioDisperso_004/molteplice%d.root", x0);
	TFile* dataFile = TFile::Open("ntuples/reco_Scan4.root","r");
	TTree* nt = (TTree*)dataFile->Get("reco_tree");
	TH1F* hCharge = new TH1F("hCharge", "hCharge", 300, -25000, 2000);
	hCharge->SetLineColor(kRed);

	float thX[8]={113,95,127,118,94,134,133,160};
	float thY[8]={110,101,119,136,94,98,87,97};
	char cut_hodoX[300];
	char cut_hodoY[300];
	TCut cut_sig("-charge_MiB2 > -13.28*amp_max_MiB2 - 350");
        TCut cut_trig1("charge_MiB1 < th_SB_MiB1 && charge_Roma2 < th_SB_Roma2 && sci_front_adc > 500 && sci_front_adc < 1500");
	sprintf(cut_hodoX, "(fibreX[0] > %f || fibreX[1] > %f || fibreX[2] > %f || fibreX[3] > %f || fibreX[4] > %f || fibreX[5] > %f || fibreX[6\
] > %f || fibreX[7] > %f)", thX[0], thX[1], thX[2], thX[3], thX[4], thX[5], thX[6], thX[7]);
	sprintf(cut_hodoY, "(fibreY[0] > %f || fibreY[1] > %f || fibreY[2] > %f || fibreY[3] > %f || fibreY[4] > %f || fibreY[5] > %f || fibreY[6\
] > %f || fibreY[7] > %f)", thY[0], thY[1], thY[2], thY[3], thY[4], thY[5], thY[6], thY[7]);

	nt->Draw("charge_MiB2>>hCharge", cut_trig1 && cut_sig && cut_hodoX && cut_hodoY && "run_id == 261"); 

//	cout << hCharge->GetMean() << endl;

//----------------------------------------------------------------------------------------

	TFile* simFile = TFile::Open(file_name, "r");
	TTree* inTree = (TTree*)simFile->Get("tree");

	TH1I* hMulti = new TH1I("hMulti","hMulti", 10, 0, 10);
	TH1F* hCorr = new TH1F("hCorr","hCorr", 300, -25000, 2000);
//	TH1F* hCorr = new TH1F("hCorr","hCorr", 10, 0, 10);
	inTree->Draw("number_0>>hMulti");	

	std::vector<float>* energy=0;
	inTree->SetBranchAddress("Energies_0", &energy);
	
	for(int iEntry=0; iEntry<inTree->GetEntriesFast(); iEntry++)
	{
	    //int count=0;
	    float corrQ=0;
	    inTree->GetEntry(iEntry);
	    for(int iPart=0; iPart<energy->size(); iPart++)
	    {
		corrQ += hCharge->GetRandom();
	    }
	    hCorr->Fill(corrQ);
	}

/*
	for(int iBin=1; iBin<11; iBin++)
	{
	    for(int j=0; j<hMulti->GetBinContent(iBin); j++)
	    {
		int tmp=0;
		for(int i=1; i<iBin; i++)
		{
		    if(gRandom->Uniform(0,1)<0.45)
			tmp++;
		}
		hCorr->Fill(tmp);
	    }
	}
*/
	hCorr->Scale(hCharge->GetEntries()/hCorr->GetEntries());
	hCorr->Draw();
	hCharge->Draw("sames");
	cout << x0*0.71 << "  " << hCorr->GetMean()/hCharge->GetMean() << endl;
    }
}
