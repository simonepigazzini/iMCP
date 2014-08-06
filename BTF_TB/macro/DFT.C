/*************************************************************
     FFT studies
*************************************************************/

void DFT()
{
    float f_cut=0;
    TFile *inFile = TFile::Open("tmp/Scan4_MS_func.root");
    TH1F* inWave = (TH1F*)inFile->Get("MS_fitfunc_Ch0");
//    TFile *inFile = TFile::Open("tmp/testWave.root");
//    TH1F* inWave = (TH1F*)inFile->Get("testWave");
    int nbins = inWave->GetNbinsX();
    float *a, *b, *mod;
    a = (float*)malloc(sizeof(float)*nbins);
    b = (float*)malloc(sizeof(float)*nbins);
    mod = (float*)malloc(sizeof(float)*nbins);
    TH1F* outFreq = new TH1F("outFreq", "outFreq", nbins, 0, nbins); 
    TH1F* outFreq1 = new TH1F("outFreq1", "outFreq1", nbins, 0, nbins); 
    TH1F* outWave = new TH1F("outWave", "outWave", nbins, 0, nbins); 
    for(int k=0; k<nbins/2; k++) 
    { 
        a[k]=0;
        b[k]=0; 
	mod[k]=0;
        for(int t=1; t<=2100; t++) 
        { 
            float angle = 2 * TMath::Pi() * t * k / (nbins); 
            a[k] += 2*inWave->GetBinContent(t) * cos(angle)/(nbins); 
            b[k] += -2*inWave->GetBinContent(t) * sin(angle)/(nbins);
        }	    
	if(k == 0 || k == nbins-1)
	{
	    a[k] = a[k]/2;
	    b[k] = b[k]/2;
	}
	mod[k] = TMath::Sqrt(TMath::Abs(a[k]**2 - b[k]**2));
	outFreq->SetBinContent(k+1, mod[k]);
/*	
	a[k]=0;
	b[k]=0; 
	mod[k]=0;
	for(int t=10/0.01; t<=20/0.01; t++) 
	{ 
            float angle = 2 * TMath::Pi() * t * k / nbins; 
            a[k] += inWave->GetBinContent(t) * cos(angle)/nbins; 
            b[k] += -inWave->GetBinContent(t) * sin(angle)/nbins; 
        } 
	mod[k] = TMath::Sqrt(TMath::Abs(a[k]**2 - b[k]**2));
	outFreq1->SetBinContent(k, mod[k]);*/
    }
    outFreq->Draw();
    outFreq1->SetLineColor(kRed);
    outFreq1->Draw("same");
//    return 0;
    for(int iBin=1; iBin<nbins; iBin++)
	outFreq1->SetBinContent(iBin, outFreq1->GetBinContent(iBin)-
				outFreq->GetBinContent(iBin));
    outFreq1->Draw();
//    return 0;
    for(int t=1; t<=nbins; t++) 
    { 
        float s=0;
        for(int k=0; k<nbins/2; k++) 
        { 
	    if(k > 50)
	    	continue;
            float angle = 2 * TMath::Pi() * t * k / nbins; 
	    s += a[k]*cos(angle) - b[k]*sin(angle);
	}  
	outWave->SetBinContent(t, s);
    }
    inWave->SetLineColor(kRed);
    outWave->Draw();
    inWave->Draw("hist sames");
    for(int k=0; k<nbins/2; k++) 
    { 
        a[k]=0;
        b[k]=0; 
	mod[k]=0;
        for(int t=1; t<=nbins; t++) 
        { 
            float angle = 2 * TMath::Pi() * t * k / (nbins); 
            a[k] += outWave->GetBinContent(t) * cos(angle)/nbins; 
            b[k] += -outWave->GetBinContent(t) * sin(angle)/nbins; 
        } 
	mod[k] = TMath::Sqrt(TMath::Abs(a[k]**2 - b[k]**2));
	outFreq1->SetBinContent(k+1, mod[k]);	    
    }
    TCanvas* c2 = new TCanvas();
    c2->cd();
    outFreq1->Draw("hist");    
}

