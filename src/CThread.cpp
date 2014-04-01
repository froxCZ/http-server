#include "CThread.h"
CThread::TQue* CThread::mQueHead = NULL;
CThread::TQue* CThread::mQueTail = NULL;
sem_t* CThread::mQueSem_t = NULL;
pthread_mutex_t CThread::mQue_mutex_t;
/**
 * Creates thread instance but doesnt run it yet.
 * @param threadID
 */
CThread::CThread(int threadID) {
    mThreadID = threadID;
    mRunning = false;
}
/**
 * Calls Stop()
 */
CThread::~CThread() {
    Stop();
}
/**
 * Starts thread in threadLoop()
 * @return 
 */
int CThread::Run() {
    pthread_create(&mThread_t, NULL, (void*(*)(void*)) & CThread::threadLoop, (void*) NULL);
    mRunning = true;
    return 1;
}

int CThread::Join() {
    pthread_join(mThread_t, NULL);
    return 1;
}
/**
 * Stops the loop.
 * @return 
 */
int CThread::Stop() {  
    //sleep(1);
    ForceStop();
    return 1;
}
/**
 * Similar to Stop()
 * @return 1
 */
int CThread::ForceStop() {
    pthread_cancel(mThread_t);
    mRunning = false;
   // pthread_detach(mThread_t);/* still causes memory leak for each thread*/
    return 1;
}

/**
 * Infinite thread loop. Gets visitors from waiting que and serves them.
 */
void* CThread::threadLoop() {
    CVisitor* visitor;
    TQue* que;
    while (true) {
        sem_wait(mQueSem_t);
        pthread_mutex_lock(&mQue_mutex_t);
        que = GetFromQue();
        pthread_mutex_unlock(&mQue_mutex_t);        
        visitor = que->mVisitor;
        visitor->ProcessRequest();
        visitor->CloseConn();
        delete que;
        delete visitor;
        //if (mStop == true)return;
    }
    return NULL;
}
/**
 * Sets visitors's waiting que semaphore
 * @param queSem
 */
void CThread::SetMQueSem_t(sem_t* queSem) {
    mQueSem_t = queSem;
}
/**
 * Adds server visitor to waiting que
 * @param visitor
 */
void CThread::InsertToQue(CVisitor* visitor) {
    pthread_mutex_lock(&mQue_mutex_t);
    TQue* newQue = new TQue(visitor, mQueTail);
    if (mQueTail)mQueTail->mNext = newQue;
    mQueTail = newQue;
    if (mQueHead == NULL) {
        mQueHead = newQue;
        mQueTail = newQue;
    }    
    sem_post(mQueSem_t);
    pthread_mutex_unlock(&mQue_mutex_t);
}

/**
 * Gets visitor from waiting que
 * @return server visitor
 */
CThread::TQue* CThread::GetFromQue() {
    if (mQueHead == NULL)return NULL;
    TQue* retQue = mQueHead;
    if (mQueHead->mNext == NULL) {
        mQueTail = NULL;
    }
    mQueHead = mQueHead->mNext;
    return retQue;
}

