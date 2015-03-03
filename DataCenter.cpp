/*
* Praneeth Nilanga Peiris : gnomez.grave@gmail.com
*/

#include "DataCenter.h"
#include "Helper.h"

typedef map<string, vector<double> >::iterator dIterator;

DataCenter::DataCenter()
{
    //ctor
}

DataCenter::~DataCenter()
{
    //dtor
}

bool DataCenter::InitializeData(const char* cFileName)
{
    this ->_FileName = cFileName;
    _mData.clear();
    ifstream inputStream(cFileName);
    string sLine;
    char cDelim = ',';
    while (getline(inputStream, sLine))
    {
        istringstream iss(sLine);
        vector<string> vTokens = Helper::Split(sLine,cDelim);
        if(vTokens.size() == 3)
        {
            vector<double> vValues;
            vValues.push_back(atof(vTokens[1].c_str()));
            vValues.push_back(atof(vTokens[2].c_str()));

            _mData.insert(std::pair<string,vector<double> >(vTokens[0],vValues));
        }

    }
    if(_mData.size() > 0)
    {
        printf("%i data points were loaded to the data center.\n",_mData.size());
        return true;
    }
    else
    {
        return false;
    }
}

double DataCenter::BuyItem(const string& sItem, const double& dAmount)
{
    if(_mData.count(sItem)>0)
    {
        vector<double>* vData = &_mData[sItem];

        if (vData == NULL)
        {
            return -1;
        }
        else if((*vData)[1] >= dAmount)
        {
            (*vData)[1] = (*vData)[1] - dAmount;
            return dAmount * (*vData)[0];
        }
        else if((*vData)[1] < dAmount)
        {
            return -2;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
}

void DataCenter::PrintStock()
{

    for (dIterator it = _mData.begin(); it!=_mData.end(); it++ )
    {
        vector<double>* vData = &_mData[it->first];
        printf("%s \t %f -> %f\n",it->first.c_str(),(*vData)[0],(*vData)[1]);
    }
}

string DataCenter::GetStock()
{
    stringstream sStream;
    for (dIterator it = _mData.begin(); it!=_mData.end(); it++ )
    {
        vector<double>* vData = &_mData[string(it->first.c_str())];
        sStream << it->first \
        << "\t" << (*vData)[0] \
        << "\t" << (*vData)[1] << "\n";
    }
    return sStream.str();
}
/*
int main(int argc, char *argv[])
{
    DataCenter ds;
    ds.InitializeData("data.csv");

    ds.BuyItem("Dhal",4);

    ds.PrintStock();

    return 0;
}
*/

