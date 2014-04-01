/* 
 * File:   CHttpMessage.h
 * Author: frox
 *
 * Created on May 4, 2012, 11:02 PM
 */

#ifndef CHTTPMESSAGE_H
#define	CHTTPMESSAGE_H

#include "constants.h"
#include "CDomain.h"
#include "CExceptions.h"
#include "CBuffer.h"

#include <arpa/inet.h>//ntoa
#include <netdb.h>//addrinfo
using namespace std;

class CHttpMessage {
public:
    CHttpMessage(const string& visitorIP,const int socket);
    CHttpMessage(const CHttpMessage* source);

    ~CHttpMessage() {
        ;
    }
    //IP
    int mSocket;
    string mPort;
    string mAddress;
    string mVisitorIP;

    //HTTP
    int statusCode; ///< response status code
    string mUserAgent;
    string mContentLen;
    string mContentType;
    string mCharset;
    string mEncoding;
    string mFileName; ///< file name requested
    CDomain* mDomain; ///< CDomain pointer the visitor specified in Host: header

};


#endif	/* CHTTPMESSAGE_H */

