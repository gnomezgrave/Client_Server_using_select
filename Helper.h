/*
* Praneeth Nilanga Peiris : gnomez.grave@gmail.com
*/

#ifndef HELPER_H_
#define HELPER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstdio>
#include <iostream>
#include <vector>
#include <sstream>


using namespace std;

class Helper
{
public:
    static vector<string> Split(const string &sString, char cDelim);
    static void Error(const char *cMsg);
    static string ToString(double dValue);
    //static void Error(const string &sMsg);

private:


};

#endif // HELPER_H_

