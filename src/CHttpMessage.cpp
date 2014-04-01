
#include "CHttpMessage.h"
using namespace std;

CHttpMessage::CHttpMessage(const string& visitorIP,const int socket) {
    mVisitorIP = visitorIP;
    mSocket = socket;    
    mDomain=NULL;
}
/**
 * Copy constructor (used when copiing from Request to Response)
 * @param source
 */
CHttpMessage::CHttpMessage(const CHttpMessage* source) {
    mSocket = source->mSocket;
    mPort = source->mPort;
    mAddress = source->mAddress;
    mVisitorIP = source->mVisitorIP;
    mDomain = source->mDomain;
    mFileName = source->mFileName;
}