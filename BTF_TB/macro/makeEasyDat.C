void makeEasyDat()
{
    TFile *_file0 = TFile::Open("../ntuples/outAnalysis_PbLow.root");
    TTree* nt = (TTree*)_file0->Get("analysis_tree");
    TH1F* h = new TH1F("h","h",400,-20000,1000);
    
    //---CuLow
//    int rid[11]={141,179,186,191,196,197,200,202,206,208,210};
//    float xo[11]={0,1*0.64,2*0.64,3*0.64,4*0.64,5*0.64,6*0.64,7*0.64,8*0.64,9*0.64,11*0.64};
    //---CuHigh
//    int rid[10]={261,189,193,195,199,201,203,207,209,211};
//    float xo[10]={0,2*0.64,3*0.64,4*0.64,5*0.64,6*0.64,7*0.64,8*0.64,9*0.64,11*0.64};    
    //---PbLow
    int rid[8]={213,215,218,226,228,230,235,238};
    int xo[8]={0,1,2,3,4,5,7,9};
    //---PbHigh
//    int rid[8]={214,216,219,227,229,231,236,239};
//    int xo[8]={0,1,2,3,4,5,7,9};

    for(int i=0; i<8; i++)
    {
	char cut[100]; 
	sprintf(cut,"sci_front_adc > 500 && sci_front_adc < 1500 && run_id == %d",rid[i]);
	nt->Draw("charge_MiB2>>h",cut);
	cout << xo[i] << " " << -1.*h->GetMean() << " " << "0 " << h->GetMeanError() << endl;
    }
}
