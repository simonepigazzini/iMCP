#include "../interface/CfgManager.h"

//**********getters***********************************************************************

//----------get option by name------------------------------------------------------------
template<> double CfgManager::GetOpt(string block, string key, int opt)
{
<<<<<<< HEAD
    return stod(opts_[key].at(opt));
=======
    double opt_val;
    istringstream buffer(opts_[block].at(key).at(opt));
    buffer >> opt_val;
    
    return opt_val;
>>>>>>> d66efd2ee935fbb294e116873866c968fc30c3a2
}

template<> float CfgManager::GetOpt(string block, string key, int opt)
{
<<<<<<< HEAD
    return stof(opts_[key].at(opt));
=======
    return (float)GetOpt<double>(block, key, opt);
>>>>>>> d66efd2ee935fbb294e116873866c968fc30c3a2
}

template<> int CfgManager::GetOpt(string block, string key, int opt)
{
<<<<<<< HEAD
    return stoi(opts_[key].at(opt));
=======
    return (int)GetOpt<double>(block, key, opt);
>>>>>>> d66efd2ee935fbb294e116873866c968fc30c3a2
}

template<> string CfgManager::GetOpt(string block, string key, int opt)
{
    return opts_[block].at(key).at(opt);
}    

//**********utils*************************************************************************

void CfgManager::ParseConfigFile(const char* file)
{
    ifstream cfgFile(file, ios::in);
    string buffer;
<<<<<<< HEAD
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
=======
    string current_block="global";
    map<string, vector<string> > block_opts;
    while(cfgFile >> buffer)
    {
        if(buffer.at(0) == '#')
            continue;        
        istringstream splitter(buffer);
        vector<string> tokens = vector<string>(istream_iterator<string>(splitter), 
                                               istream_iterator<string>());
        if(tokens.at(0).at(0) == '<')
        {
            if(tokens.at(0).at(1) == '\\')
            {
                tokens.at(0).erase(tokens.at(0).begin(), ++tokens.at(0).begin());
                tokens.at(0).erase(--tokens.at(0).end());
                if(tokens.at(0) == current_block)
                {
                    opts_[current_block] = block_opts;
                    block_opts.clear();
                    current_block = "global";
                }
                else
                    cout << "ERROR, cfg file: wrong closing block // " << tokens.at(0) << endl;
            }
            else
            {
                tokens.at(0).erase(tokens.at(0).begin());
                tokens.at(0).erase(--tokens.at(0).end());
                current_block = tokens.at(0);
            }
        }
        else
        {
            string key=tokens.at(0);
            tokens.erase(tokens.begin());
            block_opts[key] = tokens;
        }
>>>>>>> d66efd2ee935fbb294e116873866c968fc30c3a2
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
