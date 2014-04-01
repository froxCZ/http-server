/* 
 * File:   CCacheList.h
 * Author: frox
 *
 * Created on May 9, 2012, 5:20 PM
 */

#ifndef CCACHEQUE_H
#define	CCACHEQUE_H
#include <iostream>
//      #include "CFile.h"
#include "CConfig.h"
class CFile;
using namespace std;
/**
 * \brief Que where files on mTail are checked for expiration time
 */
class CCacheQue {
public:
/**
 * \brief Files cache que node
 */
    struct TNode {

        TNode(CFile* val, TNode * pNext) {
            mVal = val;
            next = pNext;
            if(next)next->prev=this;
            prev = NULL;
        }

        ~TNode();
        TNode* next;
        TNode* prev;
        CFile* mVal;
    };
    CCacheQue();
    ~CCacheQue();
    TNode* Add(CFile* add);
    void MoveOnHead(TNode* node);
    void DeleteExpiredFiles(); /* time cmp not implemented yet*/
    void Print()const;

protected:
    TNode* mHead, *mTail;
    int mListSize;

};


#endif	/* CCACHEQUE_H */

