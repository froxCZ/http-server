
#include "CHttpRequest.h"

CHttpRequest::CHttpRequest(const string& visitorIP,const int socketID) : CHttpMessage(visitorIP, socketID) {
}

/**
 * Reads the visitor request headers
 * @return statusCode
 */
int CHttpRequest::ReadRequest() {
    int headerLen;
    string headerLine;
    string headerItem;
    string headerItemContent;
    while ((headerLen = readHeaderFromSocket(headerLine))) {
        if (parseHeaderLine(headerLine, headerItem, headerItemContent))continue;
        switch (knownHeader(headerItem)) {
            case hHOST:
                mHost = processHost(headerItemContent);
                mDomain = G_Config->GetDomainByName(mHost);
                if (mDomain == NULL)throw CErrorException(NOT_FOUND, mHost + " domain does not exist.");
                break;
            case hHEAD: processGet(headerItemContent, mHEAD);
                break;
            case hGET: processGet(headerItemContent, mGET);
                break;
            case hPOST: processGet(headerItemContent, mPOST);
                break;
            case hUSER_AGENT: mUserAgent = headerItemContent;
                break;
            case hCHARSET:mCharset = headerItemContent;
                break;
            case hACCEPT:mAccept = headerItemContent;
                break;
            case hENCODING:mEncoding = headerItemContent;
                break;
            case hETAG:mETag = headerItemContent;
                break;
            default:
                G_ServerLogger->append("Unknow header: " + headerItem + headerItemContent, WARNING_LOG);
        }
    }
    return STATUS_OK;
}

void CHttpRequest::printHeaders() {
    G_ServerLogger->append("Request from: " + mVisitorIP + "\nFile: " + mFileName + "\nHEAD: " + mHEAD + "\nGET: " + mGET + "\nPOST: " + mPOST + "\nHost: " + mHost + "\nUser-Agent: " + mUserAgent + "\nCharset: " + mCharset + "\nAccept: " + mAccept, NETWORK_LOG1);
}
/**
 * Parses GET,HEAD,POST request
 * @param getContent Whole header line
 * @param dataVar Parsed Header value
 * @return statusCode
 */
int CHttpRequest::processGet(const string& getContent, string& dataVar) {
    istringstream iss(getContent);
    string file;
    string getData;
    string httpVersion;
    if (getContent.find('?') != (size_t)-1) {// means it doesnt have any get data
        getline(iss, file, '?');
        getline(iss, getData, ' ');
    } else {
        getline(iss, file, ' ');
    }
    getline(iss, httpVersion, '\r');
    if (httpVersion != "HTTP/1.1" && httpVersion != "HTTP/1.0") {        
        throw CErrorException(NOT_IMPLEMENTED, mVisitorIP + " unsupported http version: " + httpVersion);
    }
    mFileName = file;    
    dataVar = getData;    
    return mCode = STATUS_OK;
}
/**
 * Parses host header
 * @param host Header line
 * @return Host header value
 */
string CHttpRequest::processHost(const string& host) {
    if (host.find_first_of(':') < 0)return host;
    return host.substr(0, host.find_first_of(':'));
}
/**
 * Identifies header type
 * @param header
 * @return 
 */
int CHttpRequest::knownHeader(const string& header) {
    if (header == "Domain:") {
        return hDomain;
    } else if (header == "HEAD") {
        return hHEAD;
    } else if (header == "GET") {
        return hGET;
    } else if (header == "POST") {
        return hPOST;
    } else if (header == "Host:") {
        return hHOST;
    } else if (header == "User-Agent:") {
        return hUSER_AGENT;
    } else if (header == "Accept-Charset:") {
        return hCHARSET;
    } else if (header == "Accept:") {
        return hACCEPT;
    } else if (header == "Content-Lenght:") {
        return hCONTENT_LEN;
    } else if (header == "Accept-Encoding:") {
        return hENCODING;
    } else if (header == "If-None-Match:") {
        return hETAG;
    } else {
        return -1;
    }
}
/**
 * Read header line from socket. Parses by \n.
 * @param headerLine
 * @return false when its last header line (\r\n only)
 */
int CHttpRequest::readHeaderFromSocket(string& headerLine) {
    headerLine.clear();
    char ch[1];
    int readBytes = 0;
    int len = 0;
    while ((readBytes = recv(mSocket, (void *) ch, 1, 0))) {
        len++;
        if (*ch == '\r') {
            readBytes = recv(mSocket, (void *) ch, 1, 0); //read the \n also
            len++;
            return (len == 2) ? false : true;            
        } else if (*ch == '\n') {
            return false;
        } else {
            headerLine += *ch;
        }
    }
    return false; // no more headers in socket
}
/**
 * Splits line on header type and head value
 * @param headerLine
 * @param headerItem header type
 * @param headerItemContent header value
 * @return 
 */
bool CHttpRequest::parseHeaderLine(const string& headerLine, string& headerItem, string& headerItemContent) {
    if (headerLine.length() == 0)return false;
    istringstream iss(headerLine);
    headerItemContent.clear();
    headerItem.clear();
    iss >> headerItem;
    headerItemContent = headerLine.substr(headerItem.length() + 1); //+1 to leave out the space.. GET: /file ..... GET:' '/file
    if (iss)return false; //maybe check for :?
    return true;
}

