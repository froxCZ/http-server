/* 
 * File:   CListenSocket.h
 * Author: frox
 *
 * Created on May 4, 2012, 7:40 PM
 */

#ifndef CLISTENSOCKET_H
#define	CLISTENSOCKET_H
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>//addrinfo

#include "constants.h"

#include "CExceptions.h"
#include "CLogger.h"

using namespace std;
/**
 * \brief Listens on server port for incomming connections
 */
class CListenSocket {
    string m_Port;
    string m_Address;
    int m_SocketID;
    int m_AcceptQue;///< how many visitors can wait in accept que
    addrinfo * m_AddrInfo;
public:

    CListenSocket(const string,const string,int=100);
    ~CListenSocket(){
       ;
    }
    bool Launch();  
    bool Close();
    int AcceptConn(sockaddr_in& inAddr);    
private:    
    bool setSocketAddr();
    bool initSocket();
    bool setSocketOpt();    
    bool bindSocket();
    bool listenSocket();
};

#endif	/* CLISTENSOCKET_H */

