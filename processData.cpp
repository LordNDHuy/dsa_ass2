/*
 * =========================================================================================
 * Name        : processData.cpp
 * Description : Student code for Assignment 2 - Data structures and Algorithms - Spring 2018
 * =========================================================================================
 */
#include "requestLib.h"
#include "dbLib.h"

/// Initialize and Finalize any global data that you use in the program
bool avlCreated;
AVLTree<L1List<VRecord>> * recordData;

bool doubleCmp(double x, double y){
    unsigned long long int a = (unsigned long long int)(x*(10e13));
    unsigned long long int b = (unsigned long long int)(y*(10e13));
    if(a <= b) return true;
    else return false;
}
//request shortened compare method 

int eqCmp(L1List<VRecord> & a, L1List<VRecord> & b){
    if(strcmp((a)[0].id,(b)[0].id) > 0) return 1;
    else if(strcmp((a)[0].id,(b)[0].id) < 0) return -1;
    else return 0;
}

//request processing function
enum requestCode{
    _CYR, _CXR, _NYR, _NXR, _NPR, _NVR, _NRR, _CVP, _NRP
};

requestCode switchCase(std::string input){
    if(strcmp(input.c_str(), "CYR") == 0){
        //RY DY at params[0] an params[1]
        return _CYR;
    }else if(strcmp(input.c_str(), "CXR") == 0 ){
        //RX DX at params[0] an params[1]
        return _CXR;
    }else if(strcmp(input.c_str(), "NYR") == 0){
        //RY DY at params[0] an params[1]  
        return _NYR;
    }else if(strcmp(input.c_str(), "NXR") == 0){
        //RX DX at params[0] an params[1]  
        return _NXR;
    }else if(strcmp(input.c_str(), "NPR") == 0){
        return _NPR;
    }else if(strcmp(input.c_str(), "NVR") == 0){
        return _NVR;
    }else if(strcmp(input.c_str(), "NRR") == 0){
        return _NRR;
    }else if(strcmp(input.c_str(), "CVP") == 0){
        return _CVP;
    }else if(strcmp(input.c_str(), "NRP") == 0){
        return _NRP;
    }
}


bool reqCmp(double para[REQUEST_CODE_SIZE], VRecord &currentRec){
    //return doubleCmp(distanceVR(currentRec.x,currentRec.y,para[1],para[0]), distanceVR(currentRec.x,currentRec.y,currentRec.x + para[3], currentRec.y + para[2]));
    bool x = doubleCmp(para[1] - para[3], currentRec.x) && doubleCmp(currentRec.x, para[1] + para[3]);
    bool y = doubleCmp(para[0] - para[2], currentRec.y) && doubleCmp(currentRec.y, para[0] + para[2]);
    return x && y;
}

void CYR(double para[MAX_PARAM_SIZE],L1List<VRecord> * &recList);
void CXR(double para[MAX_PARAM_SIZE],L1List<VRecord> * &recList);
void NYR(double para[MAX_PARAM_SIZE],L1List<VRecord> * &recList);
void NXR(double para[MAX_PARAM_SIZE],L1List<VRecord> * &recList);
void NPR(double para[MAX_PARAM_SIZE],L1List<VRecord> * &recList);
void NVR(double para[MAX_PARAM_SIZE],L1List<VRecord> * recList);
void NRR(double para[MAX_PARAM_SIZE],L1List<VRecord> * recList);
void CVP(double para[MAX_PARAM_SIZE],L1List<VRecord> * &recList);
void NRP(double para[MAX_PARAM_SIZE],L1List<VRecord> * &recList);

bool initVGlobalData(void** pGData) {
    // TODO: allocate global data if you think it is necessary.
    /// pGData contains the address of a pointer. You should allocate data for the global data
    /// and then assign its address to *pGData
    ::avlCreated = false;
    return true;
}
void releaseVGlobalData(void* pGData) {
    // TODO: release the data if you finish using it
}

bool processRequest(VRequest& request, L1List<VRecord>& recList, void* pGData) {
    // TODO: Your code comes here

    /// NOTE: The output of the request will be printed on one line
    /// end by the end-line '\n' character.
    if(!avlCreated){
        recordData = new AVLTree<L1List<VRecord>>();
        
        //using cur method to get record include init current and reset current
        VRecord curRec;
        recList.initCur();
        while(recList.current(curRec)){
            std::cout<< curRec.x << "   "<< curRec.y<< "\n";
            L1List<VRecord> * id, *currentRec = new L1List<VRecord>();
            currentRec->insertHead(curRec);
            if(recordData->find(*currentRec,id,&eqCmp)){
                id->insertHead(curRec);
            }else{
                recordData->insert(*currentRec,&eqCmp);
            }
        }
        recList.resetCur();
        ::avlCreated = true;
        std::cout<<"\n\n\n";
    }
    
    char requestStr[REQUEST_CODE_SIZE+1];
    strcpy(requestStr,request.code);
    for(int i = 0; i < REQUEST_CODE_SIZE; i++){
        if(requestStr[i] == '_') requestStr[i] = ' ';
    }
    //get parameters and id and request
    std::stringstream ss(requestStr);
    char id[ID_MAX_LENGTH];
    char token[5];
    L1List<VRecord> * avlFind = new L1List<VRecord>(),*findRet;
    ss >> token;
    if(strcmp(token, "NVR") == 0 || strcmp(token,"NRR") == 0){
        ss >> request.params[0] >> request.params[1] >>request.params[2] >> request.params[3] ;
    }{
        ss >> id >> request.params[0] >> request.params[1] >>request.params[2] >> request.params[3] ;
        //get the linkedlist of id
        VRecord find;
        strcpy(find.id,id);

        avlFind->insertHead(find);
        recordData->find(*avlFind,findRet,&eqCmp);
    }
    switch(switchCase(token)){
        case _CYR:{
            //RY DY at params[0] an params[1]
            CYR(request.params,findRet);
            break;
        }
        case _CXR:{
            //RX DX at params[0] an params[1]
            CXR(request.params,findRet);
            break;
        }
        case _NYR:{
            //RY DY at params[0] an params[1]  
            NYR(request.params,findRet);
            break;
        }
        case _NXR:{
            //RX DX at params[0] an params[1]  
            NXR(request.params,findRet); 
            break;
        }
        case _NPR:{
            NPR(request.params,findRet);
            break;
        }
        case _NVR:{
            NVR(request.params, (&recList));
            break;
        }
        case _NRR:{
            NRR(request.params,(&recList));
            break;
        }
        case _NRP:{
            NRP(request.params,findRet);
            break;
        }
        case _CVP:{
            CVP(request.params,findRet);
            break;
        }
        default: return false; break;
    }
    
    return true;
}   
void CYR(double para[MAX_PARAM_SIZE],L1List<VRecord> * &recordList){
    bool found  = false;
    recordList->initCur();
    VRecord currentRec;
    while(recordList->current(currentRec)){
        if(doubleCmp(para[0] - para[1], currentRec.y) && 
                doubleCmp(currentRec.y, para[0] + para[1])) {//R - Delta_y <= P_y <= R + Delta_y
            found  = true;
            break;
        }
    }
    recordList->resetCur();
    std::cout<< "CYR " << (*recordList)[0].id << " "<<para[0]<<" "<<para[1]<<":";
    if(found){
        std::cout<< " Success\n";
    }else std::cout<< " Failed\n";
} 
void CXR(double para[MAX_PARAM_SIZE],L1List<VRecord> * &recordList){
    bool found  = false;
    recordList->initCur();
    VRecord currentRec;
    while(recordList->current(currentRec)){
        if(doubleCmp(para[0] - para[1], currentRec.x) && 
                doubleCmp(currentRec.x, para[0] + para[1])) {//R - Delta_y <= P_y <= R + Delta_y
            found  = true;
            break;
        }
    }
    recordList->resetCur();
    std::cout<< "CXR " << (*recordList)[0].id << " "<<para[0]<<" "<<para[1]<<":";
    if(found){
        std::cout<< " Success\n";
    }else std::cout<< " Failed\n";
}

 
void NYR(double para[MAX_PARAM_SIZE],L1List<VRecord> * &recordList){
    bool found  = false;
    recordList->initCur();
    VRecord currentRec;
    int number = 0;
    while(recordList->current(currentRec)){
        if(doubleCmp(para[0] - para[1], currentRec.y) &&
                doubleCmp(currentRec.y, para[0] + para[1])) {//R - Delta_y <= P_y <= R + Delta_y
            number++;
        }
    }
    recordList->resetCur();
    std::cout<< "NYR " << (*recordList)[0].id << " "<<para[0]<<" "<<para[1]<<":" << number <<"\n";
}

void NXR(double para[MAX_PARAM_SIZE],L1List<VRecord> * &recordList){
    bool found  = false;
    recordList->initCur();
    VRecord currentRec;
    int number = 0;
    while(recordList->current(currentRec)){
        if(doubleCmp(para[0] - para[1], currentRec.x) &&
                doubleCmp(currentRec.x, para[0] + para[1])) {//R - Delta_y <= P_y <= R + Delta_y
            number++;
        }
    }
    recordList->resetCur();
    std::cout<< "NXR " << (*recordList)[0].id << " "<<para[0]<<" "<<para[1]<<":" << number <<"\n";
}

void NPR(double para[MAX_PARAM_SIZE],L1List<VRecord> * &recordList){
    recordList->initCur();
    VRecord currentRec;
    int number = 0, pre = 0, cur = 0;
    AVLTree<std::string> * idTree = new  AVLTree<std::string>();
    while(recordList->current(currentRec)){
        std::string curId(currentRec.id), * ret;
        if(reqCmp(para, currentRec)){
            if(cur == 0){
                pre = cur;
                cur = 1;
            }
        }else{
            if(cur == 1 && pre ==0) number++;
            pre = cur;
            cur = 0;
        }
    }
    std::cout<< "NPR " << (*recordList)[0].id << " "<<para[0]<<" "<<para[1]<<" "<<para[2]<<" "<<para[3]<<": " << number <<"\n";
}

// this cmp funtion is for NVR only

int NVRCmp(std::string & a, std::string &b){
    if(strcmp(a.c_str(),b.c_str())> 0) return 1;
    else if(strcmp(a.c_str(),b.c_str())<0) return -1;
    else return 0;
}

/*void NVR(double para[MAX_PARAM_SIZE],L1List<VRecord> * recordList){
    recordList->initCur();
    VRecord currentRec;
    int number = 0;
    AVLTree<std::string> * idTree = new  AVLTree<std::string>();
    while(recordList->current(currentRec)){
        std::string curId(currentRec.id), * ret;
        if(reqCmp(para, currentRec)){
            if(!(idTree->find(curId, ret, &NVRCmp))){
                idTree->insert(curId, &NVRCmp);
                number++;
            }
        }
    }
    recordList->resetCur();
    std::cout<< "NVR "<<para[0]<<" "<<para[1]<<" "<<para[2]<<" "<<para[3]<<": " << number<<"\n";
}*/

bool NVR_(double para[MAX_PARAM_SIZE],L1List<VRecord> * &recordList){
    recordList->initCur();
    VRecord currentRec;
    int number = 0;
    bool prepre = true, pre = true, cur = true;
    if(recordList->getSize() < 3) return false;
    while(recordList->current(currentRec)){
        std::cout << " x: "<<para[1] - para[3]<<" < "<< currentRec.x <<" < "<<para[1] + para[3]<< "   y:  "<<para[0] - para[2]<< " < "<< currentRec.y<<" < "<< para[0] + para[2]<<std::endl;
        if(reqCmp(para, currentRec)){
            if(cur == false){
                prepre = pre;
                pre = cur;
                cur = true;
            }
        }else{
            prepre = pre;
            pre = cur;
            cur = false;
            if(!prepre && pre && !cur) return true;//number++;
        }
    }
    return false;
}
void NVR(double para[MAX_PARAM_SIZE],L1List<VRecord> * recordList){
    recordList->initCur();
    VRecord currentRec;
    int number = 0;
    AVLTree<std::string> * idTree = new  AVLTree<std::string>();
    while(recordList->current(currentRec)){
        std::string curId(currentRec.id), * ret;
        if(!(idTree->find(curId, ret, &NVRCmp))){
            idTree->insert(curId, &NVRCmp);
            L1List<VRecord> * find = new L1List<VRecord>(), *id;
            VRecord rec;
            strcpy(rec.id, curId.c_str());
            find->insertHead(rec);
            recordData->find(*find, id, &eqCmp);
            std::cout <<number<<": "<< curId<< "\n";
            if(!NVR_(para, id)){ 
                number++;
            }
        }
    }
    recordList->resetCur();
    std::cout<< "NVR "<<para[0]<<" "<<para[1]<<" "<<para[2]<<" "<<para[3]<<": " << number<<"\n";
}

void NRR(double para[MAX_PARAM_SIZE],L1List<VRecord> * recordList){
    recordList->initCur();
    VRecord currentRec;
    int number = 0;
    while(recordList->current(currentRec)){
        if(reqCmp(para, currentRec)){//R - Delta_y <= P_y <= R + Delta_y
            number++;
        }
    }
    recordList->resetCur();

    std::cout<< "NRR "<<para[0]<<" "<<para[1]<<" "<<para[2]<<" "<<para[3]<<": " << number<<"\n";
}

void CVP(double para[MAX_PARAM_SIZE],L1List<VRecord> * &recordList){
    bool found  = false;
    recordList->initCur();
    VRecord currentRec;
    while(recordList->current(currentRec)){
        if(reqCmp(para, currentRec)){//R - Delta_y <= P_y <= R + Delta_y
            found = true;
            break;
        }
    }
    recordList->resetCur();

    std::cout<< "CVP " << (*recordList)[0].id << " "<<para[0]<<" "<<para[1]<<" "<<para[2]<<" "<<para[3]<<":";
    if(found){
        std::cout<< " Success\n";
    }else std::cout << " Failed\n";
}

void NRP(double para[MAX_PARAM_SIZE],L1List<VRecord> * &recordList){
    L1List<VRecord> *curRec = new L1List<VRecord>();
    recordList->initCur();
    VRecord currentRec;
    int number = 0;
    while(recordList->current(currentRec)){
        if(reqCmp(para, currentRec)){
            number++;
        }
    }
    recordList->resetCur();

    std::cout<< "NRP " << (*recordList)[0].id << " "<<para[0]<<" "<<para[1]<<" "<<para[2]<<" "<<para[3]<<":" << number <<"\n";
}