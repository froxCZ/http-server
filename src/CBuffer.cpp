#include "CBuffer.h"

/**
 * New buffer instance
 * @param size
 */
CBuffer::CBuffer(const int size) {
    mBufferSize = size;
    mBuffer = new byte[mBufferSize];
    mDataLen = 0;
}
/**
 * Copy constructor
 * @param source
 */
CBuffer::CBuffer(const CBuffer& source) {
    mBufferSize = source.mBufferSize;
    mDataLen = source.mDataLen;
    mBuffer = new byte[mBufferSize];
    int iByte;
    for (iByte = 0; iByte < mDataLen; iByte++) {
        mBuffer[mDataLen] = source.mBuffer[iByte];
    }
}

CBuffer::~CBuffer() {    
    delete[] mBuffer;
}
/**
 * Copy
 * @param source
 * @return This reference
 */
CBuffer& CBuffer::operator =(const CBuffer& source) {      
    if(this==&source)return *this;
    delete[] mBuffer;
    mBufferSize = source.mBufferSize;
    mDataLen = source.mDataLen;
    mBuffer = new byte[mBufferSize];
    int iByte;
    for (iByte = 0; iByte < mDataLen; iByte++) {
        mBuffer[mDataLen] = source.mBuffer[iByte];
    }
    return *this;
}

/**
 * Appends addData to the end of buffer
 * @param addData
 * @param dataLen
 * @return Bytes count added
 */
int CBuffer::AddToBuffer(const char* addData,const int dataLen) {
    checkBufferSize(dataLen);
    int iByte;
    for (iByte = 0; iByte < dataLen; iByte++) {
        mBuffer[mDataLen++] = addData[iByte];
    }
    return iByte;
}

/**
 * Appends addBuffer's content to buffer
 * @param addBuffer
 * @return Bytes count added
 */
int CBuffer::AddToBuffer(const CBuffer* addBuffer) {
    return AddToBuffer(addBuffer->GetBufferData(), addBuffer->mDataLen);
}

/**
 * Appends string to buffer
 * @param addStr
 * @return Bytes count added
 */
int CBuffer::AddToBuffer(const string addStr) {
    return AddToBuffer(addStr.c_str(), addStr.length());
}

/**
 * 
 * @return Data lenght in buffer
 */
long int CBuffer::GetDataLen()const {
    return mDataLen;
}

/**
 * 
 * @return Char* to the bufferData
 */
byte* CBuffer::GetBufferData()const {
    return mBuffer;
}
/**
 * Extends buffer if its size is too small
 * @param plusLen we want to add to buffer
 */
void CBuffer::checkBufferSize(const int plusLen) {
    if (mDataLen + plusLen + 100 < mBufferSize)return;
    byte* tmp = new byte[mBufferSize = (mBufferSize + plusLen)*2];
    int iByte;
    for (iByte = 0; iByte < mDataLen; iByte++) {
        tmp[iByte] = mBuffer[iByte];
    }
    delete[] mBuffer;
    mBuffer = tmp;
}
