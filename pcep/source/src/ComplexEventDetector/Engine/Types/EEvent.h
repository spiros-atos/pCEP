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


/*!
	Types for the Execution Framework
	
*/
#ifndef _SOL_CEP_ENGINE_TYPES_EEVENT_H_
#define _SOL_CEP_ENGINE_TYPES_EEVENT_H_

#include "EProgram.h"
#include "EEventAttribute.h"

#include <stdlib.h>
#include <stdio.h>

struct EEventPeriod;
struct EProgram;
struct EComplexEvent;
///struct EEventAttribute;

struct EEvent
{
	EEvent() : oid(-1L)
			, id(0)
			, name(0) 
			, accept(0)
			, attributes(0)
			, refComplexEvents(0)
			, period(0)
			, prev(0)
	{}

	long oid;			//! engine object id (internal)
	
	//!@{
	/*!
		\brief Events can be identified either by id or by name
		The initial Dolce spec included a 'map' keyword to map names
		to internal ids, for performance reasons.
		Currently, just the name is used. Id has been left in the structs,
		should the 'map' keyword or similar be implemented in the future.
	*/
	unsigned long id;	//! Not used.
	char* name;
	//!@}
	
	EProgram* accept; //! Stack-program for expression evaluation.
	EEventAttribute* attributes;
	EComplexEvent* refComplexEvents; //! Complex events that reference this event.
	EEventPeriod* period;
	
	EEvent* prev; //  Used for LL
        
        ~EEvent() {
            
            EEventAttribute *toDlt, *eEAttr = attributes;
        
            while (eEAttr) {
                toDlt= eEAttr;
                eEAttr = eEAttr->prev;
                free(toDlt->name);
                delete toDlt;
            }
            free(name);
        }

};


#endif
