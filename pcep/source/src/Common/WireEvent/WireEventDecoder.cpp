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


/*
        Unwire wire-event.
	
        DOES NOT SUPPORT IDs yet.
 */

///#include <stdio.h>
///#include <string.h>
///#include <stdlib.h>
///#include <assert.h>
///RP: #include <cstring>


#include "WireEventBuilder.h"
#include "WireEventDecoder.h"

#include "../../Types/SEvent.h"

#include "WireEventTypesInt.h"
#include "../../public/pluginDev/WireEventTypes.h"

namespace solcep{

WireEventDecoder::WireEventDecoder() 
: mData(0), mDataLen(0), mOffset(0), mEvent(0) { }

WireEventDecoder::~WireEventDecoder() 
{
    
}

///RP: to manage the memory leak; must be all of the default ~destroy  
void WireEventDecoder::externalDestroy(int delEvent) 
{    
    if (mEvent && delEvent) 
    {
/*        SNVPairs* toDlt, *snvP = mEvent->nvPairs;
        
        while (snvP) {
            toDlt= snvP;
            snvP = snvP->prev;
            if (toDlt->val.t == t_string && delEvent)
                free(toDlt->val.v.stringVal);
            
            toDlt->val=0;
            delete toDlt;
        }
        
        free(mEvent->name);
 */ //RP: included in the SEvent.h 
        
        delete mEvent;        
    }
    
    mDataLen=0;
    mOffset=0;
}


SEvent* WireEventDecoder::create(const unsigned char* _data, const int _dataLen) 
{
    reset();

///    mData = (const unsigned char*)_data;
    mData = _data;
    mDataLen = _dataLen;

    mEvent = new SEvent();

    bool ok = true;

    ok &= getVersion();
    ok &= getType();

    if ((mEvent->type == WireEventTypes::EVT_TYPE_NORMAL)
            || (mEvent->type == WireEventTypes::EVT_TYPE_COMPLEX)
            || (mEvent->type == WireEventTypes::EVT_TYPE_EXT)
            || (mEvent->type == WireEventTypes::EVT_TYPE_INJECTED_EVENT)                        
            || (mEvent->type == WireEventTypesInt::EVT_TYPE_CS)
            || (mEvent->type == WireEventTypesInt::EVT_TYPE_CHANGE_EXTERNALVAR)
            || (mEvent->type == WireEventTypesInt::EVT_TYPE_SHUTDOWN)) {
        
        ok &= getByteOrder();
        ok &= getEventId();

        int nvPairs;

        // Get the NVPair count
        ok &= getNVPairCount(nvPairs);

        unsigned long id;
        char* s;
        
        for (int i = 0; i < nvPairs; i++) {
            getIdentifier(id, s);

            SNVPairs* p = new SNVPairs();
            
            p->id = id;
            p->name = s;

            TValue v;

            size_t count;
            
            p->val = *(v.deserialize(mData + mOffset, &count));            
            mOffset += count;

            p->prev = mEvent->nvPairs;
            mEvent->nvPairs = p;
        }
    }

    if (!ok) {
        delete mEvent;
        return 0;
    }

    return mEvent;
}

bool WireEventDecoder::getVersion() 
{
    //OK: DO NOTHING (event does not record version)
    mOffset++;
    return mOffset <= mDataLen;
}

bool WireEventDecoder::getType() 
{
    mEvent->type = mData[mOffset++];
    return mOffset <= mDataLen;
}

bool WireEventDecoder::getByteOrder() 
{
    //TODO: must take into account byte order!!
    //OK: DO NOTHING (event does not record byte order)
    mOffset++;
    return mOffset <= mDataLen;
}

bool WireEventDecoder::getEventId() 
{
    unsigned long id;
    char* s;

    getIdentifier(id, s);

    mEvent->id = id;
    mEvent->name = s;


    return mOffset <= mDataLen;
}

bool WireEventDecoder::getNVPairCount(int& nvPairCount_) 
{
    nvPairCount_ = *(mData + mOffset);
    mOffset += 2;

    return mOffset <= mDataLen;
}

bool WireEventDecoder::getIdentifier(unsigned long& id_, char*& s_) 
{
    id_ = 0UL;
    s_ = 0;

    unsigned long id = (unsigned long) (*(mData + mOffset));
    mOffset += 4;
    assert(mOffset <= mDataLen); // sanity check

    // Check if we're dealing with ID or string.
    if (id & WireEventBuilder::ID_MASK) 
    {
        id_ = id & ~WireEventBuilder::ID_MASK; // strip MSB
    } 
    else 
    {
        unsigned long len = id & ~WireEventBuilder::ID_MASK; // strip MSB

        assert(len <= mDataLen);

        char* s = (char*) malloc(len + 1);
        memset(s, 0, len + 1);

        strncpy(s, (char*) (mData + mOffset), len);

        mOffset += len;

        s_ = s;
    }

    return mOffset <= mDataLen;
}

void WireEventDecoder::reset() 
{    
    mData = 0;       
    
    mOffset = 0UL;
    mDataLen = 0UL;
}

}