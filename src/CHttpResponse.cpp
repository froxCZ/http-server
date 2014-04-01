#include "CHttpResponse.h"


/**
 * Creates response buffer
 * @param visitorIP
 * @param socket
 */
CHttpResponse::CHttpResponse(const string& visitorIP,const int socket) : CHttpMessage(visitorIP, socket) {
    mBufferLen = 0;
    mBufferC = new CBuffer(5000);
}

/**
 * Copies visitors info from CHttpRequest to CHttpResponse and creates response buffer
 * @param source
 */
CHttpResponse::CHttpResponse(const CHttpMessage* source) : CHttpMessage(source) {
    mBufferC = new CBuffer(5000);
}
/**
 * Deletes response buffer
 */
CHttpResponse::~CHttpResponse() {
    delete mBufferC;
}
/**
 * Set response status header
 * @param code response code
 */
void CHttpResponse::SetStatus(const int code) {
    SetHeader("HTTP/1.1", intToString(code));
}
/**
 * Sets date, server name and connection type headers
 */
void CHttpResponse::SetDefaultHeader() {
    SetHeader("Date", getDate() + " " + G_Config->TIME_ZONE);
    SetHeader("Server", G_Config->SERVER_NAME);
    SetHeader("Connection", "close");
}
/**
 * Sets header by parameters
 * @param headerItem
 * @param headerItemContent
 */
void CHttpResponse::SetHeader(const string& headerItem, const string& headerItemContent) {
    writeStringToHeader(headerItem + ": " + headerItemContent + "\r\n");
}
/**
 * According to file suffix, sets header content type
 */
void CHttpResponse::SetContentTypeHeader() {
    string contentType;
    //SetHeader("Content-Type", "image/gif");
    switch (getFileType()) {
        case FILE_HTML:contentType = "text/html";
            break;
        case FILE_JPEG:
            contentType = "image/jpeg";
            break;
        case FILE_GIF:
            contentType = "image/gif";
            break;
        case FILE_PNG:
            contentType = "image/png";
            break;
        case FILE_CSS:
            contentType = "text/css";
            break;
        default: contentType = "text/*";
    }
    SetHeader("Content-Type", contentType);
}
/**
 * Sets ETag header
 * @param etag
 */
void CHttpResponse::SetETag(const string& etag){
    SetHeader("ETag",etag);
}
/**
 * Copies CBuffer to response
 * @param addBuffer
 */
void CHttpResponse::SetContent(const CBuffer* addBuffer) {
    SetHeader("Content-lenght", intToString(addBuffer->GetDataLen()));
    SetHardHeader("\r\n");
    mBufferC->AddToBuffer(addBuffer);
}
/**
 * Copies string to response content
 * @param content
 * @param len
 */
void CHttpResponse::SetContent(const string& content, const int len) {
    SetHeader("Content-lenght", intToString(len));
    SetHardHeader("\r\n");
    mBufferC->AddToBuffer(content);
}
/**
 * Set header exactly by data param.
 * @param data
 */
void CHttpResponse::SetHardHeader(const string& data) {
    writeStringToHeader(data);
}
/**
 * Flushes the response buffer to visitor
 * @return true on success
 */
bool CHttpResponse::SendResponse() {
    int bytes_sent;
    int msgLen = mBufferC->GetDataLen(); //mContent.length();
    char* mBuffer;
    mBuffer = mBufferC->GetBufferData();
    bytes_sent = send(mSocket, mBuffer, msgLen, 0);
    if (msgLen != bytes_sent) {
        mDomain->mDomainLogger->append(mVisitorIP + " Sent bytes does not match data lenght.", WARNING_LOG);        
        return false;
    }
    mDomain->mDomainLogger->append(mVisitorIP + " Sent " + intToString(bytes_sent) + " bytes.", NETWORK_LOG0);
    return true;
}
/**
 * Recognizes file type from file suffix
 * @return File type constant
 */
int CHttpResponse::getFileType() {
    string suffix = mFileName.substr(mFileName.find_last_of('.') + 1);
    //    cout << "mFileName: " << mFileName << "Suffix: " << suffix << endl;
    if (suffix == "html") {
        return FILE_HTML;
    } else if (suffix == "gif") {
        return FILE_GIF;
    } else if (suffix == "jpeg" || suffix == "jpg") {
        return FILE_JPEG;
    } else if (suffix == "png") {
        return FILE_PNG;
    } else if (suffix == "css") {
        return FILE_CSS;
    } else {
        return FILE_UNKNOWN;
    }
}
/**
 * Adds string to response buffer
 * @param str to add
 */
void CHttpResponse::writeStringToHeader(const string& str) {
    mBufferC->AddToBuffer(str);
}
