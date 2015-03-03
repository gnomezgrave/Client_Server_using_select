/*
* Praneeth Nilanga Peiris : gnomez.grave@gmail.com
*/

#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <cstdio>
#include <list>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "DataCenter.h"
#include "Helper.h"

#define BUFFER_SIZE 1024
#define DEFAULT_PORT 5000

using namespace std;

class Server
{
public:
    Server();
    Server(int iPort);
    ~Server();

    void Start();

private:
    int i_Port;
    int i_SockFD, i_NewSockFD;
    socklen_t o_ClientLen;
    int i_MaxFD;
    fd_set s_FDAcceptSet;
    fd_set s_FDReadSet;
    fd_set s_FDWriteSet;
    timeval t_Delay;
    list<int> lst_AcceptedList;

    DataCenter o_DataCenter;

    char *pc_WelcomeMSG;

    void Initialize(int iPort);
    bool Bind();
    int Accept();
    int Read(int &iSockFD, char (*cBuffer)[BUFFER_SIZE]);
    bool Write(int &iSockFD, const string &sString);
    bool Trade(const char* pcItem, double dAmount);
};
#endif // SERVER_H_

