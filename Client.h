/*
* Praneeth Nilanga Peiris : gnomez.grave@gmail.com
*/

#ifndef CLIENT_H_
#define CLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFFER_SIZE 1024
#define DEFAULT_SERVER "127.0.0.1"
#define DEFAULT_PORT 5000

using namespace std;

class Client
{
public:
    Client();
    Client(const char* cHost, int iPort);
    ~Client();

    bool Connect();
    string Read();
    bool Write(const string &sString);
    bool Write(const char* cString);

private:
    const char* c_Host;
    int i_Port;
    int i_SockFD;
    struct sockaddr_in o_ServerAddr;
    struct hostent *o_Server;

};
#endif // SERVER_H_

