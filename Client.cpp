/*
* Praneeth Nilanga Peiris : gnomez.grave@gmail.com
*/

#include "Client.h"
#include "Helper.h"

Client::Client()
{
    c_Host = DEFAULT_SERVER;
    i_Port = 5000;
}

Client::Client(const char* cHost,int iPortNo)
{
    this->c_Host = cHost;
    i_Port = iPortNo;
}

Client::~Client()
{
    close(i_SockFD);
}

bool Client::Connect()
{
    i_SockFD = socket(AF_INET, SOCK_STREAM, 0);

    if (i_SockFD < 0)
    {
        printf("Error occurred while opening socket.\n");
        return false;
    }

    o_Server = gethostbyname(c_Host);
    if (o_Server == NULL)
    {
        fprintf(stderr,"Error, no such host\n");
        return false;
    }

    bzero((char *) &o_ServerAddr, sizeof(o_ServerAddr));

    o_ServerAddr.sin_family = AF_INET;

    bcopy((char *)o_Server->h_addr, (char *)&o_ServerAddr.sin_addr.s_addr, o_Server->h_length);
    o_ServerAddr.sin_port = htons(i_Port);

    if (connect(i_SockFD,(struct sockaddr *) &o_ServerAddr,sizeof(o_ServerAddr)) < 0)
    {
        printf("Error occurred while connecting to the server.\n");
        return false;
    }

    return true;

}

bool Client::Write(const char* cString)
{
    int iResponse = write(i_SockFD,cString,strlen(cString));

    if (iResponse < 0)
    {
        printf("Error occurred while writing to the socket.\n");
        return false;
    }
    return true;
}

string Client::Read()
{
    char cBuffer[BUFFER_SIZE];
    bzero(cBuffer,BUFFER_SIZE);
    int iResponse = read(i_SockFD, cBuffer, BUFFER_SIZE);

    if (iResponse < 0)
    {
        printf("Error occurred while reading from socket.\n");
        string sTemp = string("");
        return sTemp;
    }
    else
    {
        //cout << cBuffer <<endl;
        return string(cBuffer);
    }
}

int main(int argc, char *argv[])
{
    string host = DEFAULT_SERVER;
    int iPort = DEFAULT_PORT;
    system("clear");
    if (argc < 3)
    {
        printf("Usage : %s <host> <port>\n",argv[0]);
        printf("No host or/and port specified. Using default settings.\n");
    }
    else
    {
        host = string(argv[1]);
        iPort = atoi(argv[2]);
    }

    Client c(host.c_str(),iPort);

    if(!c.Connect())
        Helper::Error("Can't connect to the server.\n");
    else
        printf("Successfully connected to the server %s:%i.\n",host.c_str(),iPort);

    printf("Place your orders as <item name> <quantity>.\n");

    string sRead = c.Read();
    printf("%s\n",sRead.c_str());

    while (1)
    {
        char cBuffer[BUFFER_SIZE];
        printf("Order $ ");
        bzero(cBuffer,BUFFER_SIZE);
        fgets(cBuffer,BUFFER_SIZE,stdin);

        if (!c.Write(cBuffer))
        {
            printf("Can't send the message to the server.\n");
        }

        string sRead = c.Read();
        string sReq = sRead;

        if (sRead.empty())
            printf("Error occurred while reading from the socket.\n");

        else
        {
            double dResponse = atof(sRead.c_str());

            if(dResponse == -1)
            {
                printf("Invalid request.\n");
            }
            else if ( dResponse == -2)
            {
                printf("Out of stock for %s\n", cBuffer);
            }
            else if(dResponse > 0)
            {
                printf("Order %s Placed at %.2f \n\n", cBuffer, dResponse);
            }
            else
            {
                printf("Stock : \n%s \n", sRead.c_str());
            }
        }
    }
    return 0;
}
