/* 
 * File:   CThread.h
 * Author: frox
 *
 * Created on May 4, 2012, 9:03 PM
 */

#ifndef CTHREAD_H
#define	CTHREAD_H
#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <unistd.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <string>

#include "CVisitor.h"

using namespace std;
/**
 * \brief Simple thread class
 */
class CThread {    
public:

    CThread(int threadID);
    virtual ~CThread();
    int Run();
    int Join();
    int Stop();
    int ForceStop();    
/**
 * \brief Visitors requests que node
 */
    struct TQue {

        TQue(CVisitor* visitor, TQue * prev) {
            mNext = NULL;
            mPrev = prev;
            mVisitor = visitor;
        }
        CVisitor* mVisitor;
        TQue* mPrev, *mNext;
    };
    static void SetMQueSem_t(sem_t* queSem_t);
    static sem_t* mQueSem_t;
    static void InsertToQue(CVisitor*);
    static TQue* GetFromQue();
    static TQue* mQueHead, *mQueTail;    
protected:
    static void* threadLoop(void);
    pthread_t mThread_t;
    static pthread_mutex_t  mQue_mutex_t;
    int mThreadID;
    bool mRunning;

};


#endif	/* CTHREAD_H */


