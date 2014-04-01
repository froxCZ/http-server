#include "CFile.h"
#include "CBuffer.h"
#include "CCacheQue.h"

/**
 Index suffixes that are checked when file not found. If none exists, FileErrorException(404) is thrown
 */
const string CFile::mIndexSuffix[INDEX_SUFFIXES] = {".php", ".html", ".xml", ".php5"};
/**
 On start, create Treap where cashed files pointers are stored (sorted by fileName)
 */
CTreap<string, CFile*> * CFile::mCacheFilesTreap = new CTreap<string, CFile*>();
/**
 On start, create CachedFileList, where oldest files are on mTail. When saving new file to cache, check the tail for files older than > config_cache limit and delete them
 */
CCacheQue* CFile::mCacheQue = new CCacheQue();

CFile::CFile() {
    ;
}

/**
 * Finds file from filePath. If filePath is dir(/dir1/dir2/), looks for following index in dir2 (.php,.html,.php5,.xml)
 * @param filePath
 * @param openmode
 * @throw CErrorException If file is not found or don't have needed permissions to open
 */
/**
 * If path is dir, looks for indexes (mIndexSuffix).
 * Tries to open file and gets its size
 * Generates ETag value for the file for cache
 * Sets file timeStamp for cache
 * @param filePath File path
 * @param openmode Bin or Ascii mode
 * @throw CErrorException 404 on file not found
 */
CFile::CFile(const string filePath, const _Ios_Openmode openmode) {
    mUsingCnt = 0;
    mOpenmode = openmode;
    mDataBuffer = NULL;
    if (pathIsDir(filePath)) {
        if (findIndexInDir(filePath) == false) {
            throw CErrorException(404, filePath);
        }
    } else {
        mFileName = filePath;
    }
    mFileStream = NULL;
    mFileStream = new ifstream(mFileName.c_str(), ios::in | openmode | ios::ate);
    mFileLen = ((long int) mFileStream->tellg() + 1);
    if (!*mFileStream || !mFileStream->is_open() || !mFileStream->good()) {
        delete mFileStream;
        throw CErrorException(404, "File not found: " + filePath);
    }
    mFileStream->seekg(ios::beg);
    mFileStream->close();
    delete mFileStream;
    mETag = generateETag();
    mTimeStamp = time(NULL);
}
/**
 * Deletes file buffer
 */
CFile::~CFile() {
    delete mDataBuffer;
    mDataBuffer = NULL;
}

/**
 * 
 * @return File lenght
 */
long int CFile::GetFileLen()const {
    return mFileLen;
}

/**
 * @return Pointer to file buffer
 * @throw FileErrorException(404)  if file not found
 */
CBuffer* CFile::GetFileBuffer() {
    if (mDataBuffer == NULL)loadFile();
    return mDataBuffer;
}

/**
 * First checks cache que and deletes expired files from mTail. Then look in treap for fileName
 * @param fileName
 * @return File pointer or NULL
 */
CFile* CFile::FindFileInCache(const string& fileName) {
    mCacheQue->DeleteExpiredFiles();
    try {
        return mCacheFilesTreap->Find(fileName);
    } catch (int e) {
        return NULL;
    }
}

/**
 * 
 * @return file ETag
 */
string CFile::GetETag()const {
    return mETag;
}

/**
 * 
 * @param filePath
 * @return true on filePathLastChar == '/'
 */
bool CFile::pathIsDir(const string filePath)const {
    if (filePath[filePath.length() - 1] == '/')return true;
    return false;
}

/**
 * Loops filePath and looks for index(.php,.html,.php5,.xml)
 * @param filePath Must end on '/' ("/dir1/dir2/")
 * @return True if some index found
 */
bool CFile::findIndexInDir(string filePath) {
    int iSuffix;
    //loop suffixes and try to open one index
    filePath += "index";
    for (iSuffix = 0; iSuffix < INDEX_SUFFIXES; iSuffix++) {
        mFileStream = new ifstream((filePath + mIndexSuffix[iSuffix]).c_str(), ios::in | mOpenmode);
        if (*mFileStream) {
            mFileName = filePath + mIndexSuffix[iSuffix];
            mFileStream->close();
            delete mFileStream;
            return true;
        }
        mFileStream->close(); //maybee????
        delete mFileStream;
    }
    return false;
}

/**
 * Sets mDataBuffer. Looks in cache first, if not in cash, reads file from disk 
 * @return true when file found
 * @throw FileErrorException(404) if file not found
 */
bool CFile::loadFile() {
    mFileStream = NULL;
    mFileStream = new ifstream(mFileName.c_str(), ios::in | mOpenmode);
    if (mFileStream == NULL) {
        throw CErrorException(404, mFileName);
    }
    if (!(*mFileStream) || !(mFileStream->is_open()) || !(mFileStream->good())) {
        mFileStream->close();
        delete mFileStream;
        throw CErrorException(404, mFileName);
    }
    mFileStream->seekg(ios::beg);
    char tmpBuff[1000];
    int iByte = 0;
    char ch;
    mDataBuffer = new CBuffer();
    while (!mFileStream->eof()) {
        mFileStream->get(ch);
        tmpBuff[iByte++] = ch;
        if (iByte >= 999) {
            mDataBuffer->AddToBuffer(tmpBuff, iByte);
            iByte = 0;
        }
    }
    mFileStream->close();
    delete mFileStream;
    mDataBuffer->AddToBuffer(tmpBuff, iByte);
    //cachedFile = new CCacheFile(mFileName, mDataBuffer);
    AddToCacheQue();
    AddToTreap();
    G_ServerLogger->append("Added file to cache: " + mFileName, DEBUG_LOG2);
    return true;
}

/**
 * Looks to cache for file
 * @param fileName    delete mDataBuffer;
    mDataBuffer = NULL;
 * @return NULL if not found, CCacheFile pointer if found
 */
CFile* CFile::findFileInCache()const {    
    try {
        return mCacheFilesTreap->Find(mFileName);
    } catch (int e) {
        return NULL;
    }
}

/**
 * Generates ETag. 0-9 a-z A-Z. 18 chars
 * @return 
 */
string CFile::generateETag() {
    int i;
    string etag;
    static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
    for (i = 0; i < 18; i++) {
        etag += alphanum[rand() % (sizeof (alphanum) - 1)];
    }
    return etag;
}

/**
 * Sets file new timestamp
 */
void CFile::NewTimeStamp() {
    mTimeStamp = time(NULL);
    //mCacheQue->MoveOnHead(mFileInCacheQuePtr);
}

int CFile::GetTimeStamp() const {
    return mTimeStamp;
}

/**
 * Adds file to que and saves the que node pointer(mFileCacheQuePtr). mFileCacheQuePtr used when moving requested cached files on mHead (not used atm)
 */
void CFile::AddToCacheQue() {
    mFileCacheQuePtr = mCacheQue->Add(this);
}

/**
 * Moves file on que head. Used when moving requested cached files on mHead (not used atm)
 */
void CFile::MoveFileOnQueHead() {
    mCacheQue->MoveOnHead(mFileCacheQuePtr);
}

/**
 * Adds the file pointer to treap sorted by filename
 */
void CFile::AddToTreap() {
    mCacheFilesTreap->Set(mFileName, this);
}

/**
 * Removes file pointer from treap. Called when deleting file from cache que.
 */
void CFile::RemoveFromTreap() {
    mCacheFilesTreap->Unset(mFileName);
}

/**
 * Prevents from deleting file while it's used (ex.  in CVisitor->ProcessRequest)
 */
void CFile::UseFile() {
    mUsingCnt++;
}

/**
 * Prevents from deleting file while it's used (ex.  in CVisitor->ProcessRequest)
 */
void CFile::UnuseFile() {
    mUsingCnt--;
}