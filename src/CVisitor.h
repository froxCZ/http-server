/* 
 * File:   CVisitor.h
 * Author: frox
 *
 * Created on May 4, 2012, 10:50 PM
 */

#ifndef CVISITOR_H
#define	CVISITOR_H
#include "constants.h"
#include "CExceptions.h"
#include "CHttpMessage.h"
#include "CHttpRequest.h"
#include "CHttpResponse.h"
using namespace std;
/**
 * \brief Connected user
 */
class CVisitor {
public:
    int mSocket;
    int mPort;
    string mAddress;
    string mPortString;
    string mVisitorIP;
    CHttpRequest* mHttpRequest;///< visitors request
    CHttpResponse* mHttpResponse;///< visitors response
    struct sockaddr_in mSockAddr;///< mSockadd variable
public:
    CVisitor(const int inSocket,const sockaddr_in sockAddr);

    ~CVisitor();
    int ProcessRequest();
    void CloseConn();
private:
    void sendErrorCode(int code);
};


#endif	/* CVISITOR_H */

