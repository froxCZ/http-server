/* 
 * File:   CBuffer.h
 * Author: frox
 *
 * Created on April 12, 2012, 10:59 PM
 */

#ifndef CBUFFER_H
#define	CBUFFER_H
typedef char byte;
#include <string>
#include <iostream>
#include <iomanip>
using namespace std;
/**
 * \brief Smart buffer
 */
class CBuffer {
public:
    CBuffer(const int size = 4000);
    CBuffer(const CBuffer& source);
    ~CBuffer();
    CBuffer& operator=(const CBuffer& source);
    int AddToBuffer(const byte* addData, const int dataLen);
    int AddToBuffer(const CBuffer* addBuffer);
    int AddToBuffer(const string addStr);
    long int GetDataLen()const;
    byte* GetBufferData()const;    
private:
    byte* mBuffer; ///< Pointer to buffer data
    long int mBufferSize;
    long int mDataLen;
    void checkBufferSize(const int plusLen = 1000);
};

#endif	/* CBUFFER_H */

