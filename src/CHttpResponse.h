/* 
 * File:   CHttpResponse.h
 * Author: frox
 *
 * Created on May 4, 2012, 11:10 PM
 */

#ifndef CHTTPRESPONSE_H
#define	CHTTPRESPONSE_H
#include "constants.h"
#include "CFile.h"
#include "CHttpMessage.h"
#include "CBuffer.h"
/**
 * \brief Response to visitor
 */
class CHttpResponse : public CHttpMessage {
    int mBufferLen;

    //    string mServer=CFG_SERVER_NAME;
public:
    CHttpResponse(const string& visitorIP,const int socket);
    CHttpResponse(const CHttpMessage* source);

    ~CHttpResponse();
    void SetStatus(const int code);
    void SetDefaultHeader();
    void SetContentTypeHeader();
    void SetETag(const string& etag);
    void SetHeader(const string& headerItem, const string& headerItemContent);
    void SetHardHeader(const string& data);    
    void SetContent(const CBuffer* addBuffer);
    void SetContent(const string& content, const int len);
    bool SendResponse();
    void writeStringToHeader(const string& str);
    int getFileType();

    CBuffer* mBufferC;///< response buffer. All data are copied in here, then flushed
private:

};


#endif	/* CHTTPRESPONSE_H */

