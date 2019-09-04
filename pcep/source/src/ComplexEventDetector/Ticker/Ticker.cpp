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


#include "../../Common/WireEvent/WireEventBuilder.h"
#include "../../Common/WireEvent/WireEventTypesInt.h"
#include "../../Common/Trace.h"

#include "Ticker.h"

namespace solcep
{

using namespace std;

Ticker::Ticker(const int _interval)
	: mInterval(_interval)
{ }

Ticker::~Ticker() { }

void Ticker::work()
{
    WireEventBuilder wb;
    wb.create(WireEventTypesInt::EVT_TYPE_TICK);
    string msg = wb.finalize();
    
    int poll = 0;
    while(this->canExecute())
    {
        if(++poll > 100)
        {
            poll = 0;
            publishData(msg);
        }
        sleepMs(100);
    }
}

void Ticker::processPublishedMessage(string message) { }


bool Ticker::canExecute() 
{
    bool res;
///    mMutex.lock();
    res = mCanExecute;
///    mMutex.unlock();
    return res;
}

void Ticker::publishData(string message) 
{
    INFO() << "Publish: " << message << endl;
    //mPubSocket->send((void*) message.c_str(), message.length(), ZMQ_DONTWAIT);
}


}