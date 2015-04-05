#include "../interface/CfgManager.h"

//**********getters***********************************************************************

//----------get option by name------------------------------------------------------------
template<> double CfgManager::GetOpt(string key, int opt)
{
    double opt_val;
    istringstream buffer(opts_[key].at(opt));
    buffer >> opt_val;
    
    return opt_val;
}

template<> float CfgManager::GetOpt(string key, int opt)
{
    return (float)GetOpt<double>(key, opt);
}

template<> int CfgManager::GetOpt(string key, int opt)
{
    return (int)GetOpt<double>(key, opt);
}

template<> string CfgManager::GetOpt(string key, int opt)
{
    return opts_[key].at(opt);
}    

//**********utils*************************************************************************

void CfgManager::ParseConfigFile(const char* file)
{
    ifstream cfgFile(file, ios::in);
    string buffer;
    while(cfgFile >> buffer)
    {
        if(buffer.at(0) == '#')
            continue;
        istringstream splitter(buffer);
        vector<string> tokens = vector<string>(istream_iterator<string>(splitter), 
                                               istream_iterator<string>());
        string key=tokens.at(0);
        tokens.erase(tokens.begin());
        opts_[key] = tokens;
    }
    cfgFile.close();
}

