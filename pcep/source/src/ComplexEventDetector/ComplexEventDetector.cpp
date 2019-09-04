/*
 * Copyright (C) 2019  Atos Spain SA. All rights reserved.
 *
 * This file is part of pCEP.
 *
 * pCEP is free software: you can redistribute it and/or modify it under the
 * terms of the Apache License, Version 2.0 (the License);
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * The software is provided "AS IS", without any warranty of any kind, express or implied,
 * including but not limited to the warranties of merchantability, fitness for a particular
 * purpose and noninfringement, in no event shall the authors or copyright holders be
 * liable for any claim, damages or other liability, whether in action of contract, tort or
 * otherwise, arising from, out of or in connection with the software or the use or other
 * dealings in the software.
 *
 * See README file for the full disclaimer information and LICENSE file for full license
 * information in the project root.
 *
 * Authors:  Atos Research and Innovation, Atos SPAIN SA
 */

#include "ComplexEventDetector.h"
///#include <cstdlib>
///#include <stdio.h>
///#include <unistd.h>
///#include <pthread.h>

#include "../Types/SEvent.h"
#include "../Common/Err.h"
///RP:#include "../Common/Common.h"
#include "../Common/WireEvent/WireEventDecoder.h"
#include "../Common/WireEvent/WireEventBuilder.h"

#include "../Common/WireEvent/WireEventTypesInt.h"
#include "../public/pluginDev/WireEventTypes.h"

#include "Engine/Engine.h"
///RP:  #include "../Common/Configuration/Configuration.h"
#include <cstring>
///#include <iostream>

#include "../Plugins/SimpleEncoder/SimpleEncoderPlugin.h"

///RP: #include "../Common/FileEventsOperations.h"
#include "../Common/Trace.h"


namespace solcep{

ComplexEventDetector::ComplexEventDetector(EFramework* const framework)
	
{
//    mLogger->log(Logger::INFO, "[%p] Creating Complex Event Detector", this);  
    mEngine = new Engine(framework, this);
///    wed = new WireEventDecoder();
}

ComplexEventDetector::~ComplexEventDetector()
{
    delete mEngine;
///    delete wed;
}

void ComplexEventDetector::processPublishedMessage(std::string message)
{
       WireEventDecoder wed; //FIXME: make this a member var for efficiency.
///        WireEventDecoder *mWed = new WireEventDecoder();

    // Decode the message
///    SEvent* myEvent = wed->create(reinterpret_cast<const unsigned char*>(message.c_str()), message.length());
       
    SEvent* myEvent = wed.create((unsigned char*)(message.c_str()), message.length());
    message.clear();

    if(!myEvent)
    {
        ERROR() << this << " ComplexEventDetector::doWork | cannot decode incoming event\n";
        return;
    }
       
///    int delEvent=0;
    if(myEvent->type == WireEventTypesInt::EVT_TYPE_SHUTDOWN) 
    {
        //todo exit.  ///RP: Now only reload the dolce file
        
        if (mEngine->restartBCEP(myEvent)) 
        {
            publishExceptionalData("7 Shutdown int Result 1 \n");
        }
        else 
        {
            publishExceptionalData("7 Shutdown int Result 0 \n");
        }
    }
    else if(myEvent->type == WireEventTypesInt::EVT_TYPE_CHANGE_EXTERNALVAR) 
    {
        ///maybe implemented in engine?¿¿!!
        if (mEngine->changeExternalVariable(myEvent)) 
        {
            publishExceptionalData("6 ChangeExternalVar int Result 1 \n");
        }
        else 
        {
            publishExceptionalData("6 ChangeExternalVar int Result 0 \n");
        }
    }
    else if(myEvent->type == WireEventTypesInt::EVT_TYPE_TICK) 
    {
        mEngine->step();
        wed.externalDestroy(1/*delEvent*/);
    }
    else 
    {
#ifdef PERFORMANCE
    	if(eventsCounter==0)
    	{
    		clock_gettime(CLOCK_MONOTONIC,&first_event_time);
    	}
#endif
///        if (mEngine->step(myEvent) == ERR_CED_DECODE)   ///ERR_CED_DECODE
///            delEvent=1;
        mEngine->step(myEvent);

#ifdef PERFORMANCE
    	struct timespec current_time={0,0};
		clock_gettime(CLOCK_MONOTONIC,&current_time);
		double seconds= (double)(current_time.tv_sec +1.0e-9*current_time.tv_nsec) - (double)(first_event_time.tv_sec +1.0e-9*first_event_time.tv_nsec);
        eventsCounter++;
        INFO() << "EVENTS:"<< eventsCounter <<",Time:"<<seconds <<"seconds,Xput:"<<(double)eventsCounter/seconds<<"events/second"<< endl;
#endif
        wed.externalDestroy(1/*delEvent*/);  ///       wed.externalDestroy(delEvent);
    }
}

// Implements the ComplexEventObserver method.
void ComplexEventDetector::raise(EComplexEvent* _complexEvent, ECapturedEvent* _origEvent)
{      
    // Pack it to wire event.
///RP    WireEventBuilder wb;	// FIXME: make this member for efficiency.
            
    if (_complexEvent->detectionTree->op == _complexEvent->detectionTree->opAbsence) {
        preparePublishToAbsent(_complexEvent, _origEvent);
    }
    else {
        WireEventBuilder *wb;
        wb = addAttributesToPublish(_complexEvent, _origEvent);  ///is different to publish and for create the injected event.

        std::string data = wb->finalize();

        delete wb;

    ///RP: check if this complex is published
        if (complexEventIsPrintable(_complexEvent)) {
            publishData(data);
        }

        createInjectedEvent(_complexEvent, _origEvent);
    }
}

//prepare data to absent event
    void ComplexEventDetector::preparePublishToAbsent(EComplexEvent* _complexEvent, ECapturedEvent* _origEvent) {

        WireEventBuilder *wb = new WireEventBuilder();

        wb->create(WireEventTypes::EVT_TYPE_COMPLEX, _complexEvent->name);
        if (_complexEvent->groupAttributeOid > 0) {
            EEventAttribute *eAttr = mEngine->findEventAttribute(_complexEvent->groupAttributeOid);

            if (eAttr->type == t_int) {
                wb->add(eAttr->name, _origEvent->values->val.v.integerVal);
            } else if (eAttr->type == t_char) {
                wb->add(eAttr->name, _origEvent->values->val.v.charVal);
            }  else if (eAttr->type == t_string) {
                char *tmpStr = (char*)malloc(strlen(_origEvent->values->val.v.stringVal)+5);
                putQuotationMarks(tmpStr, _origEvent->values->val);
                wb->add(eAttr->name, tmpStr);
                free(tmpStr);
            }
            wb->addNVPairCount(1);
        }
        std::string data = wb->finalize();
        delete wb;

        publishData(data);
    }

///RP:method to complete the complex event to publish
WireEventBuilder* ComplexEventDetector::addAttributesToPublish(EComplexEvent* _complexEvent, ECapturedEvent* _origEvent) {
    
    WireEventBuilder *wb = new WireEventBuilder();
    wb->create(WireEventTypes::EVT_TYPE_COMPLEX, _complexEvent->name);    
    
    TValue tv;
    EPayloadAttribute *pa;
    int countPairValue = 0, flagValType=0;
    
    if (_origEvent->type == WireEventTypes::EVT_TYPE_INJECTED_EVENT) {   //first add the the payload of the complex event
        
        pa = _complexEvent->payloadAttributes;
        
        if (pa) {
            while (pa->oid > 0) {
                if (strcmp(pa->name, "printable")) {
                    tv = mEngine->executeProgramTValue(pa->expr, _complexEvent->capturedEventPool);
                    if (tv.t == t_int) {
                        wb->add(pa->name, tv.v.integerVal);
                        flagValType++;
                    } else if (tv.t == t_char) {
                            wb->add(pa->name, tv.v.charVal);
                            flagValType++;
                    } else if (tv.t == t_float) {
                           wb->add(pa->name, tv.asFloat());
                           flagValType++;
                    } else if (tv.t == t_string) {
                            char *tmpStr = (char*)malloc(strlen(tv.v.stringVal)+5);
                            putQuotationMarks(tmpStr, tv);
                            wb->add(pa->name, tmpStr);
                            flagValType++;
                            free(tmpStr);
                    } else if (tv.t == t_pos) {
                           wb->add(pa->name, tv.v.posVal);
                           flagValType++;
                    } else if (tv.t == t_area) {
                           wb->add(pa->name, tv.v.areaVal);
                           flagValType++;
                    } else if (tv.t == t_time) { 
                           wb->add(pa->name, tv.v.timeVal);
                           flagValType++;
                    }
                    if (flagValType)
                        countPairValue++;
                    }
                    pa = pa->prev;
                    flagValType=0;                
            }
//                wb->addNVPairCount(countPairValue);
        }
        
///now aggregate the payload of orignal complex event
/*  ///{discard payload of injected event
        EEventAttributeValue *eeAV = _origEvent->values;
        char *nameTmp;
        
         while (eeAV && eeAV->oid > 0) {
///                if (strcmp(pa->name, "printable")) {
            nameTmp = mEngine->findPayLoadAttributeByOid(_origEvent->oid, eeAV->oid)->name;
            
            if (eeAV->val.t == t_int) {
                wb->add(nameTmp , eeAV->val.v.integerVal);
                flagValType++;
            } else if (eeAV->val.t == t_char) {
                wb->add(nameTmp, eeAV->val.v.charVal);
                flagValType++;
            } else if (eeAV->val.t == t_float) {
                wb->add(nameTmp, eeAV->val.asFloat());
                flagValType++;
            } else if (eeAV->val.t == t_string) {
                wb->add(nameTmp, eeAV->val.v.stringVal);
                flagValType++;
            } else if (eeAV->val.t == t_pos) {
                wb->add(nameTmp, eeAV->val.v.posVal);
                flagValType++;
            } else if (eeAV->val.t == t_area) {
                wb->add(nameTmp, eeAV->val.v.areaVal);
                flagValType++;
            } else if (tv.t == t_time) { 
                wb->add(pa->name, tv.v.timeVal);
                flagValType++;
            }
            
            if (flagValType)
                countPairValue++;
///                    }  //at printable
            eeAV = eeAV->prev;
            flagValType=0;                
        }
 * /// discard payload of injected event}*/
        
            wb->addNVPairCount(countPairValue);

    }
    else {
         pa = _complexEvent->payloadAttributes;

        if (pa) {
            while (pa->oid>0) {
                if (strcmp(pa->name, "printable")) {        ///0j0 asi hacerlo: un metodo que reciba el name y el tv
                    tv = mEngine->executeProgramTValue(pa->expr, _complexEvent->capturedEventPool);
                    if (tv.t == t_int) {
                        wb->add(pa->name, tv.v.integerVal);
                        flagValType++;
                    } else if (tv.t == t_char) {
                            wb->add(pa->name, tv.v.charVal);
                            flagValType++;
                    } else if (tv.t == t_float) {
                           wb->add(pa->name, tv.asFloat());
                           flagValType++;
                    } else if (tv.t == t_string) {
//                            wb->add(pa->name, tv.v.stringVal);
                            char *tmpStr = (char*)malloc(strlen(tv.v.stringVal)+5);
                            putQuotationMarks(tmpStr, tv);
                            wb->add(pa->name, tmpStr);
            ///                free(tv.v.stringVal);
                            flagValType++;
                            free(tmpStr);
                    } else if (tv.t == t_pos) {
                            wb->add(pa->name, tv.v.posVal);
                            flagValType++;
                    } else if (tv.t == t_area) {
                            wb->add(pa->name, tv.v.areaVal);
                            flagValType++;
                    }
                    else if (tv.t == t_time) { 
                           wb->add(pa->name, tv.v.timeVal);
                           flagValType++;
                    }
                    if (flagValType)
                        countPairValue++;
                }
                    pa = pa->prev;
                    flagValType=0;                
            }
                wb->addNVPairCount(countPairValue);
        }
    }
    
    return wb;
}


///RP: create a event based on complex event that has a complex event in the detect clause
    void ComplexEventDetector::createInjectedEvent(EComplexEvent* _complexEvent, ECapturedEvent* _origEvent) {
    
        EComplexEvent* refCE = appearInSomeDetectionTree(_complexEvent);
        
        if (refCE) {
            SEvent* myEvent = new SEvent();

            myEvent->name = (char*)malloc(strlen(_complexEvent->name) + 1);
            strcpy(myEvent->name, _complexEvent->name);
            
            myEvent->oid = _complexEvent->oid;
            myEvent->refComplex = refCE;
            myEvent->payloadAttrs =_complexEvent->payloadAttributes;
            myEvent->type= WireEventTypes::EVT_TYPE_INJECTED_EVENT;

            EPayloadAttribute *epa = myEvent->payloadAttrs;
            
            TValue tv;
            while(epa && epa->oid > 0) {  //include if to validate??
                if (strcmp(epa->name, "printable")) {  //exclude the printable attr
                    SNVPairs *snvP = new SNVPairs();
                    tv = mEngine->executeProgramTValue(epa->expr, _complexEvent->capturedEventPool);
                    
                    snvP->name = (char*)malloc(strlen(epa->name) + 1);
                    strcpy(snvP->name, epa->name);
                    
                    if (tv.t == t_string) {
                        TValue tvStr(tv.v.stringVal);
                        snvP->val = tvStr;
                    }
                    else
                        snvP->val = tv;
                    
                    snvP->prev = myEvent->nvPairs;
                    myEvent->nvPairs = snvP;
                }
                epa= epa->prev;
            }
            
            mEngine->step(myEvent);
            
            delete myEvent;
        }
    }

    
///RP: check if this complex Event is published
    bool ComplexEventDetector::complexEventIsPrintable(EComplexEvent* _complexEvent) {

        if (appearInSomeDetectionTree(_complexEvent)) {  ///intermediate?
            EPayloadAttribute *pa = _complexEvent->payloadAttributes;
            if (pa && !strcmp(pa->name, "printable") && 
                    (mEngine->executeProgramTValue(pa->expr, _complexEvent->capturedEventPool)).t == t_char &&
                    (mEngine->executeProgramTValue(pa->expr, _complexEvent->capturedEventPool)).v.charVal == 'N')                
                
                return false;            
        }
        else 
            return true;
        
        return true;
    }

    
///RP: check if this complex appears in some of detection tree
    EComplexEvent* ComplexEventDetector::appearInSomeDetectionTree(EComplexEvent* _complexEvent) {

        EComplexEvent *cE = mEngine->mFramework->complexEvents;
        
        while (cE) {
            if (cE->detectionTree->findOnly(_complexEvent->oid)) {
                return cE;
            }
            cE= cE->prev;            
        }
        
        return 0;
    }
    


    void ComplexEventDetector::publishData(std::string message) {
    
        SimpleEncoderPlugin* sep = new SimpleEncoderPlugin();
        sep->load();
        
        std::string msg = sep->encode(message);

        INFO() << "Publish: " << msg;
                
///        mFileEventOper->writeCEventToFile(msg);    oooh no! include a class factory
        
        char *data = new char[msg.length() + 1];
        std::strcpy(data, msg.c_str());
        
///        mSocketUDPServer->sendData(msg.c_str(), msg.size());
        //mSocketUDPServer->sendData(data, msg.size());                
        cm->sendData(data);
        sep->unload();
        delete sep;
        delete[] data;

//        delete mFeo;
        
    }

//To publish data different to a complex event detected
    void ComplexEventDetector::publishExceptionalData(std::string message) {

        char *data = new char[message.length() + 1];
        std::strcpy(data, message.c_str());

        INFO() << "Publish: " << message;

        cm->sendData(data);

        delete[] data;

    }

//To put QuotationMarks to attr with string data type
    void ComplexEventDetector::putQuotationMarks(char *_tmpStr, TValue _tv) {
        int i = 1;
        _tmpStr[0]= '"';
        for (unsigned j = 0; j < strlen(_tv.v.stringVal); i++, j++ ) {
            _tmpStr[i] = _tv.v.stringVal[j];
        }
        _tmpStr[i]= '"'; 
        _tmpStr[++i]='\0';
    }


}
