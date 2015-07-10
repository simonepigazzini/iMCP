#include "../interface/CfgManager.h"

//**********getters***********************************************************************

//----------get option by name------------------------------------------------------------
template<> double CfgManager::GetOpt(string key, int opt)
{
    return stod(opts_[key].at(opt));
}

template<> float CfgManager::GetOpt(string key, int opt)
{
    return stof(opts_[key].at(opt));
}

template<> int CfgManager::GetOpt(string key, int opt)
{
    return stoi(opts_[key].at(opt));
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
    while(getline(cfgFile, buffer))
    {
        if(buffer.size() == 0 || buffer.at(0) == '#')
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

//**********operators*********************************************************************

ostream& operator<<(ostream& out, const CfgManager& obj)
{
    map<string, vector<string> >::const_iterator  itOpt;
    //---banner
    out << "current configuration:" << endl;
    //---options
    for(itOpt=obj.opts_.begin(); itOpt!=obj.opts_.end(); ++itOpt)
    {
        out << setw(20) << itOpt->first;
        for(int iOpt=0; iOpt<itOpt->second.size(); ++iOpt)
            out << setw(itOpt->second.at(iOpt).size()+3) << itOpt->second.at(iOpt);
        out << endl;
    }
    return out;
}
