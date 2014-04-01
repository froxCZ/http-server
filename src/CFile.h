/* 
 * File:   CFile.h
 * Author: frox
 *
 * Created on April 12, 2012, 10:55 PM
 */

#ifndef CFILE_H
#define	CFILE_H
#include <string>
#include <fstream>
#include <iostream>
#include <time.h>
#include "CExceptions.h"
#include "CCacheQue.h"
#include "CTreap.cpp"

using namespace std;
typedef char byte;
#define INDEX_SUFFIXES 5
#define NOT_FOUND               404

//typedef CCacheQue::TNode;
class CBuffer;
class CCacheQue;
/**
 * \brief File data including name, ETag etc.
 */
class CFile {
public:
    CFile();
    CFile(string filePath, const _Ios_Openmode openmode = ios::ate);    
    virtual ~CFile();
    long int GetFileLen()const;
    virtual class CBuffer * GetFileBuffer();

    string GetETag()const;
    void UseFile();
    void UnuseFile();
    void NewTimeStamp();
    int GetTimeStamp()const;
    void AddToCacheQue();
    void MoveFileOnQueHead();
    void AddToTreap();
    void RemoveFromTreap();

    static CFile* FindFileInCache(const string& fileName);
    static class CCacheQue* mCacheQue;
    static class CTreap<string, CFile*> * mCacheFilesTreap;

    int mUsingCnt;///< When working with file, inc mUsingCnt, so other thread wont delete the file and buffer in the meantime
    string mFileName;
    string mETag;
protected:
    long int mFileLen;
    class CBuffer* mDataBuffer;///< file data
private:
    bool pathIsDir(const string filePath)const;
    bool findIndexInDir(const string filePath);
    bool loadFile();
    CFile* findFileInCache()const;
    string generateETag();
    static const string mIndexSuffix[INDEX_SUFFIXES];///< suffixes to try with 'index.' when folder requested
    ifstream* mFileStream;
    _Ios_Openmode mOpenmode;
    time_t mTimeStamp;
    CCacheQue::TNode* mFileCacheQuePtr;///< pointer to the file cache que
};

//class CCacheFile : public CFile {
//public:
//    CCacheFile(string fileName, class CBuffer* fileData);
//    CCacheFile(const CFile& source);
//
//    virtual ~CCacheFile();
//    virtual class CBuffer * GetFileBuffer()const;
//    void NewTimeStamp();
//    int GetTimeStamp()const;
//    void AddToCacheQue();
//    void MoveFileOnQueHead();
//    void AddToTreap();
//    void RemoveFromTreap();
//protected:
//    time_t mTimeStamp;
//    CCacheQue::TNode* mFileCacheQuePtr;
//
//};
#endif	/* CFILE_H */

