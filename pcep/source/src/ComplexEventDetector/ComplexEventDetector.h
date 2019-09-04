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


#pragma once

//#include <pthread.h>
#include "ComplexEventObserver.h"
#include "Engine/Types/EFramework.h"
#include "Engine/Engine.h"
///#include <string>
#include "../Common/Trace.h"
#include "../Common/Tracer.h"
//#include "../Common/FileEventsOperations.h"
#include "../Common/SocketUDPServer.h"
#include "../Common/WireEvent/WireEventDecoder.h"
#include "../Common/WireEvent/WireEventBuilder.h"
#include "../Common/Communication/CommunicationModule.hpp"

namespace solcep {

/*!
    \brief Detects complex events.
 */
class ComplexEventDetector : public ComplexEventObserver
{
public:
    ComplexEventDetector(EFramework * const framework);

    virtual ~ComplexEventDetector();

    //! Implemented from ComplexEventObserver base class.
    virtual void raise(EComplexEvent* _complexEvent, ECapturedEvent* _origEvent = 0);

    void publishData(std::string message);
    void publishExceptionalData(std::string message);
    void preparePublishToAbsent(EComplexEvent* _complexEvent, ECapturedEvent* _origEvent);
    
    Engine* mEngine;
///    FileEventsOperations* mFileEventOper;
    //SocketUDPServer* mSocketUDPServer;
///    WireEventDecoder *wed;
    
    CommunicationModule* cm;
    
    //! CEPWorker Callback
    void processPublishedMessage(std::string message);
    
private:
///    bool belongToPayload(std::string, EComplexEvent*);
    
///RP:    
    bool complexEventIsPrintable(EComplexEvent* _complexEvent);
///    long appearInSomeDetectionTree(EComplexEvent* _complexEvent);
    //RP:
    EComplexEvent* appearInSomeDetectionTree(EComplexEvent* _complexEvent);
    void createInjectedEvent(EComplexEvent* _complexEvent, ECapturedEvent* _origEvent);
    WireEventBuilder* addAttributesToPublish(EComplexEvent* _complexEvent, ECapturedEvent* _origEvent);
    void putQuotationMarks(char *_tmpStr, TValue _tv);

#ifdef PERFORMANCE
	struct timespec first_event_time={0,0};
	int eventsCounter=0;
#endif
    
};

}
