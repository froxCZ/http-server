#include "CTreap.h"
#include <iostream>
#include <iomanip> 
#include <cstdlib> 
#include <cstdio>  
#include <cstring> 
#include <string>  
#include <sstream>
using namespace std;
/**
 \brief Treap template. Balanced tree.
 */
template <typename I, class T>
class CTreap {
public:
/**
 * \brief Treap node
 */
    struct TNode {

        TNode(I idx, const T & val, int p = -1) : mIdx(idx), mVal(val) {
            mL = NULL;
            mR = NULL;
            mParent = NULL;            
            if (p == -1) {
                mPriority = rand();
            } else {
                mPriority = p;
            }
        }

        ~TNode() {            
            delete mL;
            delete mR;            
            if (mParent) {
                if (mParent->mR == this) {
                    mParent->mR = NULL;
                } else {
                    mParent->mL = NULL;
                }
            }
        }

        TNode * clone() {
            TNode*newNode = new TNode(mIdx, mVal, mPriority);
            if (mL)newNode->mL = mL->clone();
            if (mR)newNode->mR = mR->clone();
            return newNode;
        }

        TNode* mL, *mR, * mParent;
        I mIdx;
        int mPriority;
        T mVal;
    };

    CTreap() {
        srand(time(NULL));
        mNodes = 0;
        mRoot = NULL;
    }

    ~CTreap() {
        delete mRoot;
    }

    CTreap(const CTreap<I, T>& source) {
        mNodes = source.mNodes;
        if (source.mRoot) {
            mRoot = source.mRoot->clone();
        } else {
            mRoot = NULL;
        }
    }
    CTreap& operator =(const CTreap<I, T> & source);

    CTreap & Set(I idx, const T& val);
    CTreap & Unset(I idx);
    bool IsSet(I idx)const;

    int Size() const;
    T& Find(const I idx)const;
    T& operator[](const I idx) const;
    bool operator ==(const CTreap<I, T> & b)const;
    bool operator !=(const CTreap<I, T> & b)const;

    template <class J, class U>
    friend ostream& operator<<(ostream& os, const CTreap<J, U> & a);
    void printNodes(ostream& os, struct CTreap<I, T>::TNode * const& node, bool first)const;
    void checkList()const;
    void checkTreeNodes(struct CTreap<I, T>::TNode * node, int &nodesCounter, int depth)const;
private:
    TNode * findNode(I idx, TNode*& parent)const;
    bool compareTrees(TNode* a, TNode * b)const;
    TNode * rightRotation(TNode * lower);
    TNode * leftRotation(TNode * lower);
    int mNodes;
    TNode* mRoot;
};

template <typename I, class T>
CTreap<I, T>& CTreap<I, T>::operator =(const CTreap<I, T>& source) {
    if (this == &source)return *this;
    delete mRoot;
    mNodes = source.mNodes;
    if (source.mRoot) {
        mRoot = source.mRoot->clone();
    } else {
        mRoot = NULL;
    }
    return *this;
}

template <typename I, class T>
int CTreap<I, T>::Size() const {
    return mNodes;
}
/**
 * 
 * @param idx
 * @return T
 * @throw int(0) when idx not found
 */
template <typename I, class T>
T& CTreap<I, T>::Find(const I idx) const {
    return (*this)[idx];
}

template <typename I, class T>
CTreap<I, T>& CTreap<I, T>::Set(I idx, const T& val) {
    if (mRoot == NULL) {
        mRoot = new TNode(idx, val);
        mNodes++;
        return *this;
    }
    TNode* node = mRoot, *parent = NULL;
    bool leftSide;
    while (node) {
        if (node->mIdx > idx) {
            parent = node;
            node = node->mL;
            leftSide = true;
        } else if (node->mIdx < idx) {
            parent = node;
            node = node->mR;
            leftSide = false;
        } else {
            node->mVal = val;
            return *this;
        }
    }
    TNode* newNode = new TNode(idx, val);
    newNode->mParent = parent;
    mNodes++;
    if (leftSide)parent->mL = newNode;
    else parent->mR = newNode;
    while (true) {
        if (newNode->mParent) {
            if (newNode->mParent->mPriority < newNode->mPriority) {
                if (newNode->mParent->mL == newNode) {
                    rightRotation(newNode);
                } else {
                    leftRotation(newNode);
                }
            } else {
                break;
            }
        } else {
            break;
        }
    }

    return *this;
}

template <typename I, class T>
CTreap<I, T>& CTreap<I, T>::Unset(I idx) {
    if (mRoot == NULL)return *this;
    TNode*node;
    node = mRoot;
    while (node) {
        if (node->mIdx > idx) {
            node = node->mL;
        } else if (node->mIdx < idx) {
            node = node->mR;
        } else {
            //found = true;
            break;
        }
    }
    if (node == NULL)return *this;
    TNode* toDelete = node;
    while (true) {
        if (toDelete->mL == NULL && toDelete->mR == NULL) {//if its leaf
            break;
        } else if (toDelete->mL == NULL) {
            leftRotation(toDelete->mR);
        } else if (toDelete->mR == NULL) {
            rightRotation(toDelete->mL);
        } else {//if have both childs
            if (toDelete->mL->mPriority < toDelete->mR->mPriority) {//do left rotation
                leftRotation(toDelete->mR);
            } else {
                rightRotation(toDelete->mL);
            }
        }
    }
    delete toDelete;
    mNodes--;
    if (mNodes == 0)mRoot = NULL;
    return *this;
}

template <typename I, class T>
bool CTreap<I, T>::IsSet(I idx) const {
    TNode*node, *parent;
    node = findNode(idx, parent);
    if (node == NULL)return false;
    return true;
}

/**
 * Retuns value by idx from treap
 * @param idx
 * @return value
 * @throw int(0) when not found
 */
template <typename I, class T>
T& CTreap<I, T>::operator [](const I idx) const {
    if (mNodes == 0)throw 0;
    TNode*node = mRoot;
    while (node) {
        if (node->mIdx > idx) {
            node = node->mL;
        } else if (node->mIdx < idx) {
            node = node->mR;
        } else {//idx equal
            break;
        }
    }
    if (node)return node->mVal;
    throw 0;
}

template <typename I, class T>
typename CTreap<I, T>::TNode * CTreap<I, T>::findNode(I idx, TNode*& parent)const {
    TNode *node = mRoot;
    parent = NULL;
    while (node) {
        if (node->mIdx > idx) {
            parent = node;
            node = node->mL;
        } else if (node->mIdx < idx) {
            parent = node;
            node = node->mR;
        } else {//idx equal
            break;
        }
    }
    return node;
}

template <typename I, class T>
typename CTreap<I, T>::TNode* CTreap<I, T>::rightRotation(TNode* lower) {
    if (lower == NULL)return NULL;
    TNode* parent = lower->mParent;
    if (parent == NULL)return NULL;
    if (lower->mR)lower->mR->mParent = parent;
    parent->mL = lower->mR;
    lower->mR = parent;
    if (parent->mParent) {
        if (parent->mParent->mL == parent) {
            parent->mParent->mL = lower;
        } else {
            parent->mParent->mR = lower;
        }
    } else {
        mRoot = lower;
    }
    lower->mParent = parent->mParent;
    parent->mParent = lower;
    return NULL;
}

template <typename I, class T>
typename CTreap<I, T>::TNode* CTreap<I, T>::leftRotation(TNode* lower) {
    if (lower == NULL)return NULL;
    TNode* parent = lower->mParent;
    if (parent == NULL)return NULL;
    if (lower->mL)lower->mL->mParent = parent;
    parent->mR = lower->mL;
    lower->mL = parent;
    if (parent->mParent) {
        if (parent->mParent->mR == parent) {
            parent->mParent->mR = lower;
        } else {
            parent->mParent->mL = lower;
        }
    } else {
        mRoot = lower;
    }
    lower->mParent = parent->mParent;
    parent->mParent = lower;
    return NULL;
}

template <typename I, class T>
ostream& operator<<(ostream& os, const CTreap<I, T>& a) {
    if (a.mNodes == 0) {
        os << "{ }";
        return os;
    }
    os << "{ ";
    a.printNodes(os, a.mRoot, true);
    os << "}";
    return os;
}

template <typename I, class T>
void CTreap<I, T>::checkList() const {
    cout << "check list():" << endl;
}

template <typename I, class T>
void CTreap<I, T>::printNodes(ostream& os, struct CTreap<I, T>::TNode * const& node, bool last) const {
    if (node->mL) {
        printNodes(os, node->mL, false);
    }
    os << "[" << node->mIdx << "] => " << node->mVal;
    if (last == false || node->mR != NULL) {
        os << ",";
    }
    os << " ";

    if (node->mR) {
        printNodes(os, node->mR, last);
    }

}
