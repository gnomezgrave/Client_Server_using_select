/*
* Praneeth Nilanga Peiris : gnomez.grave@gmail.com
*/

#include "Server.h"
#include "Helper.h"
#include "DataCenter.h"

Server::Server(int iPortNo)
{
    if(iPortNo > 1024)
    {
        Initialize(iPortNo);
        printf("Server will be bound to port : %i \n",iPortNo);
    }
    else
    {
        Initialize(DEFAULT_PORT);
        printf("Invalid Port. Server will be bound to default port : %i \n",DEFAULT_PORT);
    }
}

Server::Server()
{
    Initialize(DEFAULT_PORT);
    printf("Server will be bound to default port : %i \n",i_Port);
}

void Server::Initialize(int iPort)
{
    i_Port = iPort;
    FD_ZERO(&s_FDAcceptSet);
    FD_ZERO(&s_FDReadSet);
    FD_ZERO(&s_FDWriteSet);
    printf("Server initialized.\n");

    o_DataCenter.InitializeData("data.csv");

    //*pc_WelcomeMSG = "Welcome..\r\n";
}

bool Server::Bind()
{
    struct sockaddr_in ServerAddr;
    int iResponse;
    int iOptionValue = 1;

    i_SockFD = socket(AF_INET, SOCK_STREAM, 0);
    if (i_SockFD < 0)
    {
        printf("ERROR opening socket.\n");
        return false;
    }

    if (setsockopt(i_SockFD, SOL_SOCKET, SO_REUSEADDR, &iOptionValue, sizeof(int)) == -1 )
    {
        printf("ERROR setting socket options.\n");
        return false;
    }

    bzero((char *) &ServerAddr, sizeof(ServerAddr));

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = INADDR_ANY;
    ServerAddr.sin_port = htons(i_Port);

    if (bind(i_SockFD, (struct sockaddr *) &ServerAddr, sizeof(ServerAddr)) < 0)
    {
        return false;
    }

    printf("Server is listening to %i.\n",i_Port);
    listen(i_SockFD, 5);

    // Add the Non-Blocking param to the socket.
    fcntl(i_SockFD, F_SETFL, fcntl(i_SockFD,F_GETFL) | O_NONBLOCK);

    i_MaxFD = i_SockFD;

    return true;
}

int Server::Accept()
{
    struct sockaddr_in ClientAddr;
    o_ClientLen = sizeof(ClientAddr);
    i_NewSockFD = accept(i_SockFD, (struct sockaddr *) &ClientAddr, &o_ClientLen);

    if (i_NewSockFD < 0)
    {
        printf("ERROR on accepting the connection.\n");
        return -1;
    }
    else
    {
        printf("New Client Connected , ip is : %s , port : %d \n" , inet_ntoa(ClientAddr.sin_addr) , ntohs(ClientAddr.sin_port));
        return i_NewSockFD;
    }
}

int Server::Read(int &iSockFD, char (*cBuffer)[BUFFER_SIZE])
{
    //char cBuffer[BUFFER_SIZE];
    bzero(cBuffer,BUFFER_SIZE);
    int iResponse = read(iSockFD, cBuffer, BUFFER_SIZE);

    if (iResponse < 0)
    {
        printf("Error occurred while reading from socket.\n");
        //return string("");
    }
    return iResponse;
}


bool Server::Write(int &iSockFD, const string &sString)
{
    int iResponse = write(iSockFD, sString.c_str(), BUFFER_SIZE);

    if (iResponse < 0)
    {
        printf("Error occurred while writing to socket.\n");
        return false;
    }
    return true;
}

void Server::Start()
{
    if (!Bind())
    {
        Helper::Error("Error on binding to port.\n");
    }

    while (1)
    {
        FD_ZERO(&s_FDReadSet);
        FD_SET(i_SockFD, &s_FDReadSet);

        t_Delay.tv_sec = 1;
		t_Delay.tv_usec = 0;

		if (i_MaxFD < i_SockFD)
        {
            i_MaxFD = i_SockFD;
        }

		int iSelect = select(i_MaxFD + 1, &s_FDReadSet, NULL, NULL, &t_Delay);

        if ( iSelect > 0 )
        {
            i_NewSockFD = Accept();
            //
            if (FD_ISSET(i_SockFD, &s_FDReadSet) )
            {
                char pc_WelcomeMSG[] = "Welcome to Millennium Super Market.\n\n";

                if( send(i_NewSockFD, pc_WelcomeMSG, strlen(pc_WelcomeMSG), 0) != strlen(pc_WelcomeMSG) )
                {
                    perror("Cannot send.");
                }

                fcntl(i_NewSockFD, F_SETFL, fcntl(i_NewSockFD,F_GETFL) | O_NONBLOCK);

                lst_AcceptedList.push_back(i_NewSockFD);
                FD_SET(i_NewSockFD, &s_FDReadSet);
            }
        }

        for (list<int>::iterator it = lst_AcceptedList.begin(); it != lst_AcceptedList.end(); it++)
        {
            FD_SET(*it, &s_FDReadSet);
            if (*it >0 && i_MaxFD < *it)
            {
                i_MaxFD = *it;
            }
        }

        t_Delay.tv_sec = 1;
		t_Delay.tv_usec = 0;

        iSelect = select(i_MaxFD + 1, &s_FDReadSet, NULL, NULL, &t_Delay);

        if (iSelect > 0)
        {
            for (list<int>::iterator it = lst_AcceptedList.begin(); it != lst_AcceptedList.end(); it++)
            {
                if (FD_ISSET(*it, &s_FDReadSet))
                {
                    char cBuffer[BUFFER_SIZE];
                    int iRead = Read(*it, &cBuffer);

                    if (iRead != 0)
                    {
                        vector<string> vTokens = Helper::Split(string(cBuffer),' ');

                        if (vTokens.size() == 2)
                        {
                            string sItemName = vTokens[0];
                            double dAmount = atof(vTokens[1].c_str());
                            dAmount = o_DataCenter.BuyItem(sItemName.c_str(), dAmount);

                            printf("\nClient said : %s", cBuffer);

                            if ( Write(*it, Helper::ToString(dAmount)) )
                            {
                                if (dAmount == -1)
                                {
                                    printf ("Invalid Order.\n");
                                }
                                else if (dAmount == -2)
                                {
                                    printf("Out of stock for %s\n", cBuffer);
                                }
                                else
                                {
                                    printf ("Calculated Amount : %f \n", dAmount);
                                }

                            }
                            else
                            {
                            }
                        }
                        else if (vTokens.size() == 1)
                        {
                            string sString = vTokens[0];

                            if (sString.compare("stock\n") == 0)
                            {
                                sString = o_DataCenter.GetStock();
                                Write(*it, sString);
                                printf("\n");
                                o_DataCenter.PrintStock();
                            }
                            else
                            {
                                Write(*it, Helper::ToString(-1));
                            }
                        }
                        else
                        {
                            printf("Invalid Input.\n");
                        }

                    }
                    else
                    {
                        printf("Client disconnected.\n");
                        close(*it);
                        lst_AcceptedList.remove(*it);
                    }
                    break;
                }

            }
        }
    }

}

bool Server::Trade(const char* pcItem, double dAmount)
{

}


Server::~Server()
{
    close(i_NewSockFD);
    close(i_SockFD);
}
