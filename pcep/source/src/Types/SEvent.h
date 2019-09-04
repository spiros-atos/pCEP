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


#ifndef _SOL_CEP_S_EVENT_H_
#define _SOL_CEP_S_EVENT_H_

#include "TValue.h"
#include "../ComplexEventDetector/Engine/Types/EComplexEvent.h"
#include "../ComplexEventDetector/Engine/Types/EPayloadAttribute.h"

struct SNVPairs
{
    SNVPairs() : name(0), prev(0) {}
    ~SNVPairs() 
    { 
        if(name)
            free(name);
    }

    unsigned long id;
    char* name; // if 0, id is used

    TValue val;	

    SNVPairs* prev;
};

/*!
    \brief Incoming event from the outside, to be presented to the Framework
*/
struct SEvent
{
    SEvent() : type(0),	id(0), name(0), nvPairs(0) {}
    ~SEvent() {	
        
        SNVPairs *toDlt, *snvP = nvPairs;      

        while (snvP) {
            toDlt= snvP;
            snvP = snvP->prev;
            if (toDlt->val.t == t_string && strlen(toDlt->val.v.stringVal))
                free(toDlt->val.v.stringVal);
                        
            toDlt->val=0;
            
            delete toDlt;
        }

        free(name);            
        
    } //FIXME-TODO cleanup any allocated memory  
	
    unsigned char type;
	
    unsigned long id;
    char* name;	// if 0, id is used.
	
    SNVPairs* nvPairs;
///RP: used to injected events
    EComplexEvent *refComplex;
    long oid;
    EPayloadAttribute *payloadAttrs;
    
};





#endif

