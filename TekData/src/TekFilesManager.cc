#include "../interface/TekFilesManager.h"

//**********Constructors******************************************************************
TekFilesManager::TekFilesManager()
{}

//**********Setters***********************************************************************
void TekFilesManager::PushBack(string file)
{
    string tokens=file.substr(file.find_first_of("_")+1, file.find_last_of("_")-file.find_first_of("_")-1);    
    //---check timestamp string size
    if(tokens.size() < 12)
    {
        cout << "> TekFilesManager --- ERROR: invalid timestamp" << endl;
        exit(-1);
    }

    //---convert date to seconds
    struct tm file_time={0};
    file_time.tm_year=stoi(string(tokens, 0, 2));
    file_time.tm_mon=stoi(string(tokens, 2, 2));
    file_time.tm_mday=stoi(string(tokens, 4, 2));
    file_time.tm_hour=stoi(string(tokens, 6, 2));
    file_time.tm_min=stoi(string(tokens, 8, 2));
    file_time.tm_sec=stoi(string(tokens, 10, 2));
    
    uint32 seconds = mktime(&file_time);

    filesList_[seconds].push_back(file);

    //---set the file iterator to the last element
    //---effectively this operation reset the NextEvt loop
    fileIt_=filesList_.end();
}

//**********Getters***********************************************************************
map<uint32, vector<string> >::iterator& TekFilesManager::NextEvt() 
{
    if(fileIt_ == filesList_.end())
        return fileIt_=filesList_.begin();

    return ++fileIt_;
}
