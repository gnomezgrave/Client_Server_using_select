/*
* Praneeth Nilanga Peiris : gnomez.grave@gmail.com
*/

#ifndef DATACENTER_H
#define DATACENTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>

using namespace std;

class DataCenter
{
    public:
        /** Default constructor */
        DataCenter();
        /** Default destructor */
        virtual ~DataCenter();

        bool InitializeData(const char* cFileName);
        double BuyItem(const string& sItem, const double& dAmount);
        void PrintStock();
        string GetStock();

    private:
        map<string,vector<double> > _mData;
        const char* _FileName;
};

#endif // DATACENTER_H
