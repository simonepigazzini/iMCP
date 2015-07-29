void drawWF()
{
    int iCh=-1, stop=0, iEvent=0;
    float sample_tmp=0;
    vector<float> samples[3];
    char ls_command[100];
    //char* dir=argv[1];
    string file;
    sprintf(ls_command, "ls /gwpool/users/martelli/public/iMCP/RUN_Oscilloscopio/WFRun016/* > tmp/wave.list");
    system(ls_command);
    TH2F* histo[3];

    ifstream waveList("tmp/wave.list", ios::in);
    while(waveList >> file && stop != 1)
    {
	iCh++;
	if(iCh > 2)
	{
	    iEvent++;
	    iCh=0;
	}
	if(iEvent<0)
	    continue;
	cout << "Event:  " << iEvent << "  " << file.c_str() << endl;
	ifstream inputFile(file.c_str(), ios::in);
	while(inputFile >> sample_tmp)
	{
	    samples[iCh].push_back(sample_tmp);
	}
	inputFile.close();
	if(iCh == 2 /*&&
		     *(max_element(samples[2].begin(), samples[2].end())) > 0.02)*/
	{
	    for(int i=0; i<3; i++)
	    {
		char histo_name[5];
		sprintf(histo_name, "Ch%d", i+1);
		histo[i] = new TH2F(histo_name, histo_name, samples[i].size(), 0, samples[i].size(), 100, -0.1, 0.1);
		histo[i]->SetMarkerColor(i+2);
		for(int iSample=0; iSample<samples[i].size(); iSample++)
		{	
		    histo[i]->Fill(iSample, samples[i].at(iSample));
		}
		samples[i].clear();
	    }
	    histo[0]->Draw();
	    histo[1]->Draw("same");
	    histo[2]->Draw("same");
	    gPad->Update();
	    cin >> stop;
	    histo[0]->Delete();
	    histo[1]->Delete();
	    histo[2]->Delete();
	}
	else if(iCh == 2)
	{
	    for(int i=0; i<3; i++)
		samples[i].clear();
	}
	//---
    }
    waveList.close();
}
