/*
* Praneeth Nilanga Peiris : gnomez.grave@gmail.com
*/

#include "Helper.h"

vector<string> Helper::Split(const string &sString, char cDelim) {
    stringstream sStream(sString);
    string sItem;
    vector<string> vElems;

    while (getline(sStream, sItem, cDelim)) {
        vElems.push_back(sItem);
    }
    return vElems;
}

void Helper::Error(const char* cMsg)
{
    perror(cMsg);
    exit(-1);
}

string Helper::ToString(double dValue)
{
    ostringstream osStream;
    if (!(osStream << dValue))
        return "";
    return osStream.str();
}

