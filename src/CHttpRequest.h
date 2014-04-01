/* 
 * File:   CHttpRequest.h
 * Author: frox
 *
 * Created on May 4, 2012, 11:03 PM
 */

#ifndef CHTTPREQUEST_H
#define	CHTTPREQUEST_H
#include "constants.h"
#include "CHttpMessage.h"
#include "CConfig.h"

#include <sstream>
/**
 * \brief Visitors request
 */
class CHttpRequest : public CHttpMessage {
public:
    string mHEAD;
    string mGET;
    string mPOST;
    string mHost;
    string mAccept;
    string mETag; ///< ETag for filename visitor requested in If-no-match: header
    int mCode;

public:
    CHttpRequest(const string& visitorIP,const int socketID);
    int ReadRequest();
    void printHeaders();

     ~CHttpRequest() {
        
    }
private:
    int readHeaderFromSocket(string& header);
    bool parseHeaderLine(const string& line, string& headerItem, string& headerItemContent);
    char* readContent();
    int knownHeader(const string& header);
    int processGet(const string& getContent, string& dataVar);
    string processHost(const string& host);
    inline bool statusCodeOK();
};


#endif	/* CHTTPREQUEST_H */

