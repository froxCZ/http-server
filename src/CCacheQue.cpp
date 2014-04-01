#include "CCacheQue.h"
#include "CFile.h"


using namespace std;

CCacheQue::CCacheQue() {
    mHead = mTail = NULL;
    mListSize = 0;
}
/**
 * Deletes whole file que
 */
CCacheQue::~CCacheQue() {    
    delete mHead;
}
/**
 * Adds CFile to cache que and returns its node pointer
 * @param newVal
 * @return Que new node pointer
 */
CCacheQue::TNode* CCacheQue::Add(CFile* newVal) {
    TNode* newNode = new TNode(newVal, mHead);
    //    cout << "CCacheQue::TNode* CCacheQue::Add(): newNode mVal: " << newVal << endl;
    mHead = newNode;
    mListSize++;
    if (mListSize == 1)mTail = mHead;
    return newNode;
}
/**
 * Moves cache file node to cache que's head.. Called when file is requested by visitor. (Not used atm)
 * @param node that we want to move on head
 */
void CCacheQue::MoveOnHead(TNode* node) {
    if (node == mHead)return;
    if (node->prev)node->prev->next = node->next;
    if (node->next)node->next->prev = node->prev;
    if (node == mTail)mTail = node->prev;
    mHead->prev = node;
    node->next = mHead;
    mHead = node;
}
/**
 * Loops from mTail while deleting all expired files. TNode destructor removes it from treap
 */
void CCacheQue::DeleteExpiredFiles() {
    if (mListSize == 0)return;
    TNode* prev = mTail;
    while (prev) {
        if ((time(NULL) - prev->mVal->GetTimeStamp()) <= G_Config->FILE_CACHE_EXPIRATION_TIME) {/* MUST BE IMPLEMENTED*/
            break; //keep loop until the file shoudl stay in cache
        }        
        TNode* toDelete = prev;        
        if (toDelete->mVal->mUsingCnt == 0) {
            prev = prev->prev;
            G_ServerLogger->append("DeleteExpiredFiles(): deleted " + toDelete->mVal->mFileName, DEBUG_LOG2);            
            mListSize--;
            delete toDelete;            
        }else{
            G_ServerLogger->append("DeleteExpiredFiles(): cant delete used file " + toDelete->mVal->mFileName, DEBUG_LOG2);            
            break;
        }
    }
    mTail = prev;
    if (mListSize == 0)mHead = mTail = NULL;
}
/**
 * Debug - not used
 */
void CCacheQue::Print()const {
    TNode* next = mHead;
    while (next) {
        cout << next->mVal << " ->> ";
        next = next->next;
    }
}
/**
 * Removes file pointer from treap and deletes it. Then deletes next node.
 */
CCacheQue::TNode::~TNode() {    
    if (prev) prev->next = NULL;    
    mVal->RemoveFromTreap();
    delete mVal;
    mVal = NULL;
    delete next;
}