/*
 * =========================================================================================
 * Name        : dsaLib.h
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
 * Description : Library for Assignment 2 - Data structures and Algorithms - Spring 2018
 * =========================================================================================
 */


#ifndef A02_DSALIB_H
#define A02_DSALIB_H


#include <string>
#include <math.h>

#include <vector>
#include <functional>
#include <iostream>

using namespace std;


class DSAException {
    int     _error;
    string  _text;
public:

    DSAException() : _error(0), _text("Success") {}
    DSAException(int err) : _error(err), _text("Unknown Error") {}
    DSAException(int err, const char* text) : _error(err), _text(text) {}

    int getError() { return _error; }
    string& getErrorText() { return _text; }
};

template <class T>
class List {
public:
    virtual void    clean() = 0;
    virtual void    clone(void*) = 0;
    virtual bool    isEmpty() = 0;
    virtual size_t  getSize() = 0;
    virtual T&      at(int i) = 0;
    virtual T&      operator[](int i) = 0;

    virtual bool    find(T& a, int& idx) = 0;
    virtual T*      find(T& a) = 0;
    virtual T*      find(T& a, std::function<bool (T&, T&)> eqCmp) = 0;
    virtual int     insert(int i, T& a) = 0;
    virtual int     insert(int i, T&& a) = 0;
    virtual int     remove(int i) = 0;
    virtual int     remove(T& a, std::function<bool (T&, T&)> eqCmp) = 0;

    virtual int     push_back(T& a) = 0;
    virtual int     push_back(T&& a) = 0;
    virtual int     insertHead(T& a) = 0;
    virtual int     insertHead(T&& a) = 0;
    virtual int     removeHead() = 0;
    virtual int     removeLast() = 0;

    virtual void    traverse(std::function<void (T&)> op) = 0;
    virtual void    traverse(std::function<void (T&, void*)> op, void* pParam) = 0;

    virtual void    reverse() = 0;
};

template <class T>
struct L1Item {
    T data;
    L1Item<T> *pNext;

    L1Item(T &a) : data(a), pNext(NULL) {}
    L1Item(T &a, void* next) : data(a), pNext(next) {}
    L1Item(T &&a) : data(std::move(a)), pNext(NULL) {}
    L1Item(T &&a, void* next) : data(std::move(a)), pNext(next) {}
};

template <class T>
class L1List : public List<T> {
    L1Item<T>   *_pHead, *pCur;
    size_t      _size;
public:
    L1List() : _pHead(NULL),pCur(NULL), _size(0) {}
    ~L1List(){
        clean();
    };

    void    clean();
    void    clone(void*);
    bool    isEmpty() { return _pHead == NULL; }
    size_t  getSize() { return _size; }
    T&      getData() { return _pHead->data;}
    T&      at(int i);
    T&      operator[](int i);

    //custom function
    bool      current( T& ret){
        if(this->pCur == NULL){
            return false;
        }else{
            T temp = this->pCur->data;
            memcpy(&ret,&temp,sizeof(T));
            this->pCur = this->pCur->pNext;
            return true;
        }
    }
    void initCur(){this->pCur = this->_pHead;}
    void resetCur(){ this->pCur = this->_pHead;}

    //
    bool    find(T& a, int& idx);
    T*      find(T& a);
    T*      find(T& a, std::function<bool (T&, T&)> eqCmp);//bool (*eqCmp)(T&, T&));
    int     insert(int i, T& a);
    int     insert(int i, T&& a);
    int     remove(int i);
    int     remove(T& a, std::function<bool (T&, T&)> eqCmp);

    int     push_back(T& a);
    int     push_back(T&& a);
    int     insertHead(T& a);
    int     insertHead(T&& a);
    int     removeHead();
    int     removeLast();

    void    reverse(){
        L1Item<T> * current = this->_pHead;
        L1Item<T> * next = current->pNext;
        while(next){
            L1Item<T> * pre = current;
            current = next;
            next = current->pNext;
            current->pNext = pre;
        }
        this->_pHead->pNext = NULL;
        this->_pHead = current;
    }

    void    traverse(std::function<void (T&)> op) {
        L1Item<T>   *p = _pHead;
        while (p) {
            op(p->data);
            p = p->pNext;
        }
    }
    //void    traverse(void (*op)(T&, void*), void* pParam) {
    void    traverse(std::function<void (T&, void*)> op, void* pParam) {
        L1Item<T>   *p = _pHead;
        while (p) {
            op(p->data, pParam);
            p = p->pNext;
        }
    }
};

template <class T>
void L1List<T>::clean(){
    L1Item<T> * current = this->_pHead;
    while(this->_pHead){
        this->_pHead = this->_pHead->pNext;
        delete current;
        current = this->_pHead;
        this->_size--;
    }
    delete this->pCur;
}

template <class T>
void L1List<T>::clone(void * pRet){
    L1List<T> * pClone = (L1List<T>*)pRet;
    L1Item<T> * current = this->_pHead;
    while(current){
        pClone->push_back(current->data);
        current = current->pNext;
    }

}

template <class T>
bool L1List<T>::find(T& a,int &idx){
    L1Item<T> * current = this->_pHead;
    while(current){
        if(a == current->data){
            return true;
        }
        idx++;
        current = current->pNext;
    }
    return false;
}



template <class T>
T* L1List<T>::find(T&a){
    L1Item<T> * current = this->_pHead;
    while(current){
        if(a == current->data){
            return &current->data;
        }
        current = current->pNext;
    }
    return NULL;
}

template <class T>
T* L1List<T>::find(T& a, std::function<bool (T&, T&)> eqCmp){
    L1Item<T> * current = this->_pHead;
    while(current){
        if(eqCmp(a,current->data)){
            return &current->data;
        }
        current = current->pNext;
    }
}

template <class T>
int L1List<T>::insert(int i,T&a){
    L1Item<T> * current = this->_pHead;
    int idx = 0;
    while(current){
        if(i == idx){
            break;
        }
        current = current->pNext;
        idx++;
    }
    L1Item<T> * pNew = new L1Item<T>(a);
    pNew->pNext = current->pNext;
    current->pNext = pNew;
    this->_size++;
    return idx;
}

template <class T>
int L1List<T>::insert(int i,T&&a){
    L1Item<T> * current = this->_pHead;
    int idx = 0;
    while(current){
        if(i == idx){
            break;
        }
        current = current->pNext;
        idx++;
    }
    L1Item<T> * pNew = new L1Item<T>(std::move(a));
    pNew->pNext = current->pNext;
    current->pNext = pNew;
    this->_size++;
    return idx;
}

template <class T>
int L1List<T>::remove(int i){
    if(i == 0){
        L1Item<T> * current = this->_pHead;
        this->_size--;
        this->_pHead = this->_pHead->pNext;
        delete current;
    }else{
        L1Item<T> * current = this->_pHead;
        L1Item<T> * preCurrent;
        this->_size--;
        int idx = 0;
        while(current){
            if(i == idx){
                break;
            }
            preCurrent = current;
            current = current->pNext;
            idx++;
        }
        if(current){
            preCurrent->pNext = current->pNext;
            current = NULL;
        }
    }
    return i;
}

template <class T>
int L1List<T>::remove(T& a, std::function<bool (T&, T&)> eqCmp){
    if(eqCmp(a, this->_pHead->data)){
        L1Item<T> * current = this->_pHead;
        this->_pHead = this->_pHead->pNext;
        this->_size--;
        delete current;
    }else{
        L1Item<T> * current = this->_pHead;
        L1Item<T> * preCurrent;
        this->_size--;
        while(current){
            if(eqCmp(a, current->data)){
                break;
            }
            preCurrent = current;
            current = current->pNext;
        }
        if(current){
            preCurrent->pNext = current->pNext;
            current = NULL;
        }
    }
        return 0;
}   

template <class T>
T& L1List<T>::at(int i){
    if(this->_pHead != NULL){
        L1Item<T> * pRet = this->_pHead;
        int idx = 0;
        while(pRet){
            if(i != idx){
                pRet = pRet->pNext;
                idx++;
            }else  return (pRet->data);
        }
    }
    T ret;
    return ret;
}


template <class T>
T& L1List<T>::operator[](int i){
    return at(i);
}


/// Insert item to the end of the list
/// Return 0 if success
template <class T>
int L1List<T>::push_back(T &a) {
    if (_pHead == NULL) {
        _pHead = new L1Item<T>(a);
    }
    else {
        L1Item<T>   *p = _pHead;
        while (p->pNext) p = p->pNext;
        p->pNext = new L1Item<T>(a);
    }

    _size++;
    return 0;
}

template <class T>
int L1List<T>::push_back(T &&a) {
    if (_pHead == NULL) {
        _pHead = new L1Item<T>(std::move(a));
    }
    else {
        L1Item<T>   *p = _pHead;
        while (p->pNext) p = p->pNext;
        p->pNext = new L1Item<T>(std::move(a));
    }

    _size++;
    return 0;
}

/// Insert item to the front of the list
/// Return 0 if success
template <class T>
int L1List<T>::insertHead(T &a) {
    L1Item<T>   *p = new L1Item<T>(a);
    p->pNext = _pHead;
    _pHead = p;
    _size++;
    return 0;
}

template <class T>
int L1List<T>::insertHead(T&& a) {
    L1Item<T>   *p = new L1Item<T>(std::move(a));
    p->pNext = _pHead;
    _pHead = p;
    _size++;
    return 0;
}

/// Remove the first item of the list
/// Return 0 if success
template <class T>
int L1List<T>::removeHead() {
    if(_pHead) {
        L1Item<T>* p = _pHead;
        _pHead = p->pNext;
        delete p;
        _size--;
        return 0;
    }
    return -1;
}

/// Remove the last item of the list
/// Return 0 if success
template <class T>
int L1List<T>::removeLast() {
    if(_pHead) {
        if(_pHead->pNext) {
            L1Item<T>* prev = _pHead;
            L1Item<T>* pcur = prev->pNext;
            while(pcur->pNext) {
                prev = pcur;
                pcur = pcur->pNext;
            }
            delete pcur;
            prev->pNext = NULL;
        }
        else {
            delete _pHead;
            _pHead = NULL;
        }
        _size--;
        return 0;
    }
    return -1;
}

/************************************************************************
 * This section is for AVL tree
 ************************************************************************/
#define cusCmp 

template <class T>
struct AVLNode {
    T           _data;
    AVLNode<T>   *_pLeft, *_pRight;
#ifdef AVL_USE_HEIGHT
    int         _height;
    AVLNode(T &a) : _data(a), _pLeft(NULL), _pRight(NULL), _height(1) {}
#else
    int         _bFactor;
    AVLNode(T &a) : _data(a), _pLeft(NULL), _pRight(NULL), _bFactor(0) {}
    //bFactor -1 = lefthigh, 0 = balance, 1 = righthight
#endif
};

template <class T>
class AVLTree {
    AVLNode<T> *_pRoot;
    int  (* customCompare)(T&,T&);
public:
    AVLTree() : _pRoot(NULL) {}
    ~AVLTree() { destroy(_pRoot); }


    bool find(T& key, T* &ret,int (* eqCmp)(T&, T&)) {
        this->customCompare = eqCmp; return find(_pRoot, key, ret); }
    bool insert(T& key,int (* eqCmp)(T&, T&)) {
        this->customCompare = eqCmp; return insert(_pRoot, key); }
    bool remove(T& key,int (* eqCmp)(T&, T&)) {
        this->customCompare = eqCmp; return remove(_pRoot, key); }

    bool find(T& key, T* &ret) { return find(_pRoot, key, ret); }
    bool insert(T& key) { return insert(_pRoot, key); }
    bool remove(T& key) { return remove(_pRoot, key); }
    void traverseNLR(void (*op)(T&)) { traverseNLR(_pRoot, op); }
    void traverseLNR(void (*op)(T&)) { traverseLNR(_pRoot, op); }
    void traverseLRN(void (*op)(T&)) { traverseLRN(_pRoot, op); }
    
    void traverseNLR(double para[6],void (*op)(double[6], T&)) { traverseNLR(_pRoot,para, op); }
protected:
    void destroy(AVLNode<T>* &pR);
    bool find(AVLNode<T> *pR, T& key, T* &ret);
    bool insert(AVLNode<T>* &pR, T& a);
    bool remove(AVLNode<T>* &pR, T& a);
    void traverseNLR(AVLNode<T> *pR, void (*op)(T&));
    void traverseLNR(AVLNode<T> *pR, void (*op)(T&));
    void traverseLRN(AVLNode<T> *pR, void (*op)(T&));
//
    void traverseNLR(AVLNode<T> *pR,double para[6], void (*op)(double[6],T&));

    void rotLeft(AVLNode<T>* &pR);
    void rotRight(AVLNode<T>* &pR);
    void rotLR(AVLNode<T>* &pR);
    void rotRL(AVLNode<T>* &pR);

    bool balanceLeft(AVLNode<T>* &pR);
    bool balanceRight(AVLNode<T>* &pR);

    //additional
    bool _replacingRootRightMost(AVLNode<T>* &pR, AVLNode<T>* &pRet);
    int eqCmp(T&a, T&b){
       return this->customCompare(a,b);
    }
};

template <class T>
void AVLTree<T>::rotLeft(AVLNode<T>* &pR){
    AVLNode<T> * pTemp = pR;
    pR = pR->_pRight;
    pTemp->_pRight = pR->_pLeft;
    pR->_pLeft = pTemp;
}

template <class T>
void AVLTree<T>::rotRight(AVLNode<T>* &pR){
    AVLNode<T> * pTemp = pR;
    pR = pR->_pLeft;
    pTemp->_pLeft = pR->_pRight;
    pR->_pRight = pTemp;
}

template <class T>
void AVLTree<T>::rotLR(AVLNode<T>* &pR){
    rotLeft(pR->_pLeft);
    rotRight(pR);
}

template <class T>
void AVLTree<T>::rotRL(AVLNode<T>* &pR){
    rotRight(pR->_pRight);
    rotLeft(pR);
}
template <class T>
bool AVLTree<T>::balanceLeft(AVLNode<T>* &pR){
    AVLNode<T> * leftTree = pR->_pLeft;
    bool taller;
    if(leftTree->_bFactor == -1){
        rotRight(pR);
        pR->_bFactor = 0;
        pR->_pRight->_bFactor = 0;
        taller = false;
    }else if(leftTree->_bFactor == 1){
        AVLNode<T> * rightTree = leftTree->_pRight;
        //?if(rightTree == NULL) return;
        if(rightTree->_bFactor == -1){
            pR->_bFactor = 1;
            leftTree->_bFactor = 0;
        }else if(rightTree->_bFactor == 0){
            pR->_bFactor = 0;
            leftTree->_bFactor = 0;
        }else{
            pR->_bFactor = 0;
            leftTree->_bFactor = -1;
        }
        rightTree->_bFactor = 0;
        /*rotateLeft(leftTree);
        pR->pLeft = leftTree;
        rotateRight(pR);*/
        rotLR(pR);
        taller = false;
    }else{
        int bFactor = 0;
        if(pR->_pLeft->_bFactor == -1)bFactor = 0;
        else bFactor = 1;
        rotRight(pR);
        pR->_bFactor = bFactor;
        taller = true; // for remove, so reuse balance of insert which is remove taller = !insert taller
    }
    return taller;
}

template <class T>
bool AVLTree<T>::balanceRight(AVLNode<T>* &pR){
    AVLNode<T> * rightTree = pR->_pRight;
    bool taller;
    if(rightTree->_bFactor == 1){
        rotLeft(pR);
        pR->_bFactor = 0;
        pR->_pLeft->_bFactor = 0;
        //Tree->balanceF = balanced;
        taller = false;
    }else if(rightTree->_bFactor == -1){
        AVLNode<T> * leftTree = rightTree->_pLeft;
        //?if(leftTree == NULL) return;
        if(leftTree->_bFactor == 1){
            pR->_bFactor = -1;
            rightTree->_bFactor = 0;
        }else if(leftTree->_bFactor == 0){
            rightTree->_bFactor = 0;
            pR->_bFactor = 0;
        }else{
            pR->_bFactor = 0;
            rightTree->_bFactor = 1;
        }
        leftTree->_bFactor = 0;
        /*rotateRight(rightTree);
        pR->pRight = rightTree;
        rotateLeft(pR);*/
        rotRL(pR);
        taller = false;
    }else{
        int bFactor = 0;
        if(pR->_pRight->_bFactor == 1)bFactor = 0;
        else bFactor = -1;
        rotLeft(pR);
        pR->_bFactor = bFactor;
        taller = true;
    }
    return taller;
}

template <class T>
void AVLTree<T>::destroy(AVLNode<T>* &pR){
    if(pR != NULL){
        destroy(pR->_pLeft);
        destroy(pR->_pRight);
        delete pR;
    }
    pR = NULL;
}

template <class T>
bool AVLTree<T>::find(AVLNode<T> *pR, T& key, T* &ret){
    if(pR == NULL){
        return false;
    }
    AVLNode<T> * ptr = pR;
    while(ptr){
        if(
            #ifdef cusCmp
                eqCmp(key,ptr->_data) < 0
            #else
                key < ptr->_data
            #endif
        ){
            ptr = ptr->_pLeft;
        }else if(
            #ifdef cusCmp
                eqCmp(key,ptr->_data) > 0
            #else
                key > ptr->_data
            #endif
        ){
            ptr = ptr->_pRight;
        }else{
            ret = &ptr->_data;
            return true;
        }
    }
    return false;
}

template <class T>
bool AVLTree<T>::insert(AVLNode<T>* &pR, T& key){
    if(pR == NULL){
        pR = new AVLNode<T>(key);
        return true;
    }
    bool taller;
    if(
        #ifdef cusCmp
            eqCmp(key,pR->_data) < 0
        #else
            key < pR->_data
        #endif
    ){
        taller = insert(pR->_pLeft, key);
        if(taller){
            if(pR->_bFactor == -1){
                taller = balanceLeft(pR);
            }else if(pR->_bFactor == 0){
                pR->_bFactor = -1;
            }else{
                pR->_bFactor = 0;
                taller = false;
            }

        }
    }else{
        taller = insert(pR->_pRight, key);
        if(taller){
            if(pR->_bFactor == -1){
                pR->_bFactor = 0;
                taller = false;
            }else if(pR->_bFactor == 0){
                pR->_bFactor = 1;
            }else{
                taller = balanceRight(pR);
            }
        }
    }
    return taller;
}

template <class T>
bool AVLTree<T>::_replacingRootRightMost(AVLNode<T> *& pR, AVLNode<T>* &pRet){
    if(pR->_pRight == NULL){
            pRet = pR;
            pR = pR->_pLeft;
            return true;
    }
    bool taller = _replacingRootRightMost(pR->_pRight, pRet);
    
    if(taller){
        if(pR->_bFactor== 0) {
            taller = false;
            pR->_bFactor = -1;
        }else if(pR->_bFactor == 1){
            pR->_bFactor = 0;
        }else pR->_bFactor = 0;
    }    
    return taller;
}

template <class T>
bool AVLTree<T>::remove(AVLNode<T>* &pR, T& key){
    if(pR == NULL){
        return false;
    }
    bool taller;
    if(
        #ifdef cusCmp
            eqCmp(key,pR->_data) < 0
        #else
            key < pR->_data
        #endif
    ){
        taller = remove(pR->_pLeft, key);
        if(taller){
            if(pR->_bFactor == 1){
                taller = !balanceRight(pR);
            }else if(pR->_bFactor == 0){
                pR->_bFactor = 1;
                taller = false;
            }else pR->_bFactor = 0;
        }
    }else if(
            #ifdef cusCmp
                eqCmp(key,pR->_data) > 0
            #else
                key > pR->_data
            #endif
    ){
        taller = remove(pR->_pRight, key);
        if(taller){
            if(pR->_bFactor == -1){
                taller = !balanceLeft(pR);
            }else if(pR->_bFactor == 0){
                pR->_bFactor = -1;
                taller = false;
            }else pR->_bFactor = 0;
        }
    }else{
        if(pR->_pLeft == NULL && pR->_pRight == NULL){
            pR = NULL;
            return true;
        }
        //
        AVLNode<T> * pRep = pR->_pRight;//preplacing
        AVLNode<T> * leftTree = pR->_pLeft;
        taller = true;
        if(leftTree){
            pRep = pR->_pLeft;
            if(leftTree->_pRight){
                taller = _replacingRootRightMost(leftTree, pRep);
                pRep->_pLeft = pR->_pLeft;
                pRep->_pRight = pR->_pRight;
                pRep->_bFactor = pR->_bFactor;
            }else{
                pRep->_pRight = pR->_pRight;                
                pRep->_bFactor = 1;
                if(pR->_bFactor == -1){
                    pRep->_bFactor = 0;
                }else if(pR->_bFactor == 0){
                    taller = false;
                }else{
                   taller = !balanceRight(pRep);                    
                }
            }
        }
        pR = pRep;
    }
    return taller;
}
//
template <class T>
void AVLTree<T>::traverseNLR(AVLNode<T> *pR,double para[6], void (*op)(double[6], T&)){
    if(pR != NULL){
        op(para, pR->_data);
        traverseNLR(pR->_pLeft,para, op);
        traverseNLR(pR->_pRight,para, op);
    }
}

template <class T>
void AVLTree<T>::traverseNLR(AVLNode<T> *pR, void (*op)(T&)){
    if(pR != NULL){
        op(pR->_data);
        traverseNLR(pR->_pLeft, op);
        traverseNLR(pR->_pRight, op);
    }
}

template <class T>
void AVLTree<T>::traverseLNR(AVLNode<T> *pR, void (*op)(T&)){
    if(pR != NULL){
        traverseLNR(pR->_pLeft, op);
        op(pR->_data);
        traverseLNR(pR->_pRight, op);
    }
}

template <class T>
void AVLTree<T>::traverseLRN(AVLNode<T> *pR, void (*op)(T&)){
    if(pR != NULL){
        traverseLRN(pR->_pLeft, op);
        traverseLRN(pR->_pRight, op);
        op(pR->_data);
    }
}
#endif //A02_DSALIB_H
