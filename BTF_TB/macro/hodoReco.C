void hodoReco()
{
    gROOT->Reset();
    TFile* inFile = TFile::Open("../ntuples/outAnalysis_nc_Scan4.root","r");
    TTree* nt = (TTree*)inFile->Get("analysis_tree");

    int *fX, *fY;
    fX = (int*)malloc(sizeof(int)*8);
    fY = (int*)malloc(sizeof(int)*8);
    int sci_front_adc=0;
    float mean_tmp=0;

    float thX[8]={113,95,127,118,94,134,133,160};
    float thY[8]={110,101,119,136,94,98,87,97};

    TH1F* hodoX = new TH1F("hodoX","hodoX",8,0,8);
    TH1F* hodoY = new TH1F("hodoY","hodoY",8,0,8);
    nt->SetBranchAddress("fibreX",fX);
    nt->SetBranchAddress("fibreY",fY);
    nt->SetBranchAddress("sci_front_adc",&sci_front_adc);

    for(int iEntry=0; iEntry<nt->GetEntriesFast(); iEntry++)
    {
	nt->GetEntry(iEntry);
	if(sci_front_adc > 500 && sci_front_adc < 1500)
	{
	    for(int iFbr=0; iFbr<8; iFbr++)
	    {
		if(fX[iFbr] > thX[iFbr])
		    hodoX->SetBinContent(iFbr+1, hodoX->GetBinContent(iFbr+1)+1);
		if(fY[iFbr] > thY[iFbr])
		    hodoY->SetBinContent(iFbr+1, hodoY->GetBinContent(iFbr+1)+1);
	    }
	}
    }
    TFile* out = TFile::Open("profile_beam.root","recreate");
    hodoX->Write();
    hodoY->Write();
    out->Close();
    nt->Delete();
} 
    
		
