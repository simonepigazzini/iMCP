#ifndef __CFG_MANAGER__
#define __CFG_MANAGER__

#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <iterator>
#include <map>
#include <vector>
#include <string>
#include <algorithm> 

using namespace std;

class CfgManager
{
public:
    //---ctors---
    CfgManager() {};
    CfgManager(map<string, map<string, vector<string> > >* defaultCfg) {SetDefaultCfg(defaultCfg);};
    //---dtor---
    ~CfgManager() {};

    //---getters---
    template<typename T> T GetOpt(string block, string key, int opt=0);

    //---setters---
    inline void            SetDefaultCfg(map<string, map<string, vector<string> > >* defaultCfg)
        {opts_ = *defaultCfg;};

    //---utils---
    inline void            ParseConfigFile(string* file) {ParseConfigFile(file->c_str());};
    void                   ParseConfigFile(const char* file);

private:
    map<string, map<string, vector<string> > >  opts_;
    //map<string, vector<string> >  opts_;
};

#endif
