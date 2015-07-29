#ifndef __TEK_FILES_MANAGER_H__
#define __TEK_FILES_MANAGER_H__

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <time.h>

using namespace std;

typedef unsigned long int uint32;

class TekFilesManager
{
public:
    //---ctors---
    TekFilesManager();

    //---dtor---
    ~TekFilesManager() {};

    //---setters---
    void PushBack(string file);
    
    //---getters---
    map<uint32, vector<string> >::iterator& end() {return fileEnd_=filesList_.end();};
    map<uint32, vector<string> >::iterator& NextEvt();

    //---utils---
    inline void Reset() {fileIt_ = filesList_.begin();};

private:
    map<uint32, vector<string> > filesList_;
    map<uint32, vector<string> >::iterator fileIt_;
    map<uint32, vector<string> >::iterator fileEnd_;
};

#endif
