
#include "CVisitor.h"
#include "CFile.h"


/**
 * Creates new visitor.
 * @param inSocket socket
 * @param sockAddr visitors address info
 */

CVisitor::CVisitor(const int inSocket,const sockaddr_in sockAddr) {
    mSocket = inSocket;
    mSockAddr = sockAddr;
    mAddress = inet_ntoa(mSockAddr.sin_addr);
    mPort = mSockAddr.sin_port;
    mPortString = intToString(mPort);
    mVisitorIP = mAddress + ":" + mPortString;
    mHttpRequest = NULL;
    mHttpResponse = NULL;
}
/**
 * Deletes visitors request and response
 */
CVisitor::~CVisitor() {
    delete mHttpRequest;
    delete mHttpResponse;
}
/**
 * Manages server visitor. Reads request, prepares files, sets up response and flushs it.
 * @return status code
 */
int CVisitor::ProcessRequest() {
    //    cout << "New ProcessRequest():" << endl;
    //sleep(1);
    mHttpRequest = new CHttpRequest(mVisitorIP, mSocket);
    int statusCode = STATUS_OK;
    try {
        statusCode = mHttpRequest->ReadRequest();
        mHttpRequest->printHeaders();
    } catch (CErrorException e) {
        G_ServerLogger->append(e.GetErrorMessage(), WARNING_LOG);
        statusCode = e.GetErrorCode();
    }
    if (statusCode >= 300) {
        sendErrorCode(statusCode);
        return statusCode;
    }
    CFile* file;
    mHttpResponse = new CHttpResponse(mHttpRequest);
    if(mHttpResponse->mDomain==NULL)return NOT_FOUND;
    mHttpResponse->mDomain->OpenLogger();
    CBuffer* buffer;
    string filePath = mHttpRequest->mDomain->GetDomainPath() + mHttpRequest->mFileName.substr(1, mHttpRequest->mFileName.length());
    try {        
        file = CFile::FindFileInCache(filePath);        
        if (file == NULL) {
            file = new CFile(mHttpRequest->mDomain->GetDomainPath() + mHttpRequest->mFileName.substr(1, mHttpRequest->mFileName.length()), ios::binary);            
        }
        file->UseFile();
        mHttpResponse->mFileName = file->mFileName;
        buffer = file->GetFileBuffer();
    } catch (CErrorException e) {
        delete file;
        mHttpResponse->mDomain->mDomainLogger->append(e.GetErrorMessage(), INFO_LOG);
        statusCode = e.GetErrorCode();
    }
    if (statusCode >= 300) {
        sendErrorCode(statusCode);
        if(file) file->UnuseFile();
        return statusCode;
    }
    if (mHttpRequest->mETag == file->mETag) {
        statusCode = NOT_MODIFIED;
        sendErrorCode(statusCode);
        if(file) file->UnuseFile();
        return statusCode;
    }
    mHttpResponse->SetStatus(statusCode);
    mHttpResponse->SetDefaultHeader();
    mHttpResponse->SetETag(file->GetETag());
    mHttpResponse->SetContentTypeHeader();
    mHttpResponse->SetContent(buffer);
    mHttpResponse->SendResponse();
    file->UnuseFile();
    //delete file;
    return statusCode;
}
/**
 * Close communications socket with visitor.
 */
void CVisitor::CloseConn() {
    G_ServerLogger->append(mVisitorIP + " Closing socket", DEBUG_LOG);
    close(mSocket);
}
/**
 * Sets up and sends error message. (Ex. 404 file not found, etc..)
 * @param error code
 */
void CVisitor::sendErrorCode(const int code) {
    string errorResponse;
    string pageRequested = mHttpRequest->mHost + mHttpRequest->mFileName; //mHttpRequest->mHost + mpFileToSend->mFileName;
    errorResponse = "Error " + intToString(code) + "<br><b>noob</b>";
    mHttpResponse->SetStatus(code);
    mHttpResponse->SetDefaultHeader();
    mHttpResponse->SetContent((char *) errorResponse.c_str(), errorResponse.length());
    mHttpResponse->SendResponse();
}

