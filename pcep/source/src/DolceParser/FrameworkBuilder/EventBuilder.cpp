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

#include "EventBuilder.h"

///#include <stdio.h>
///#include <string.h>
///#include <stdlib.h>
///#include <assert.h>

#include "../../Common/Err.h"


#include "../../ComplexEventDetector/Engine/Types/EEvent.h"
#include "../../ComplexEventDetector/Engine/Types/EEventAttribute.h"
#include "../../ComplexEventDetector/Engine/Types/EComplexEvent.h"

#include "FrameworkBuilder.h"
#include "ProgramBuilder.h"


EventBuilder::EventBuilder(FrameworkBuilder* const _fb,
							EEvent* _event_)
	: mEvent(_event_)
	, mFrameworkBuilder(_fb)

{
	assert(_event_);
}



EventBuilder::EventBuilder(FrameworkBuilder* const _fb)
	: mEvent(0)
	, mFrameworkBuilder(_fb)

{

}

EventBuilder::~EventBuilder()
{

}


void EventBuilder::init()
{
}


void EventBuilder::deinit()
{
}


int EventBuilder::build(ASTree* const _node)
{
#ifdef CEP_DEBUG
	printf("EventBuilder::build | node = [0x%p]\n", _node);
#endif

	ASTreeItr itr(_node);
	
	ASTree* node;
	ProgramBuilder* pb = mFrameworkBuilder->getProgramBuilder();

	//--- Create
	//
	node = itr.firstChild();
	assert(node);
	
	mEvent = new EEvent();
	node->getSymbol()->engineObj = (void*)mEvent;
	
	char* name = node->getSymbolName();
	char* c = (char*)malloc(strlen(name) + 1);
	strcpy(c, name);
	
	mEvent->name = c;
        
	mEvent->oid = mFrameworkBuilder->getNextOid();
	
	//--- Attributes ("use")
	//
	node = _node->descendant(ASTree::PUse);
	if(node)
	{
		buildEventAttributes(node);
		
	}
	
	//--- Accept-Condition ("accept")
	//
	node = _node->descendant(ASTree::PAccept);
	
	if(node)
	{
		pb->build(node);
		mEvent->accept = pb->finalize();
		
	}
	
	//--- Period ("lasts")
	//
	node = _node->descendant(ASTree::PLasts);
	if(node)
	{
		//FIXME
	}

	return OK;
}



int EventBuilder::addComplexEventRef(EComplexEvent* const _complex)
{
	EComplexEvent* ce = mEvent->refComplexEvents;
	
	// Do not add if it already exists
	while(ce)
	{
		if(ce->oid == _complex->oid)
			return OK;
			
		ce = ce->prev;
	}
	
	_complex->prev = mEvent->refComplexEvents;
	mEvent->refComplexEvents = _complex;
                
	return OK;
}



EEvent* EventBuilder::finalize()
{
	return mEvent;
}


//FIXME: string-constants --> data page + back-ref



int EventBuilder::buildEventAttributes(ASTree* const _node)
{
	ASTreeItr itr(_node->descendant(ASTree::PEventAttributeList));
	
	ASTree* attr = itr.firstChild();
	ASTree* id;
	mFrameworkBuilder->getSymbolTable();
	SymbolTable::Symbol* sym;
	
	while(attr)
	{
		id = attr->descendant(ASTree::PIdentifier);
		assert(id);
		sym = id->getSymbol();
		assert(sym);
		
		EEventAttribute* ea = new EEventAttribute();
///RP: to fix   (char*)malloc(strlen(name) + 1)
                ea->name = (char*)malloc(strlen(sym->name) + 1);
		strcpy(ea->name, sym->name); //FIXME dynamic alloc!
		
		ea->type = sym->dataType;
		ea->oid = mFrameworkBuilder->getNextOid();

#ifdef CEP_DEBUG
		SymbolTable* symTab = mFrameworkBuilder->getSymbolTable();
		printf("[EventBuilder::buildEventAttributes] setting type for attr '%s' to '%s'\n", ea->name, symTab->dataTypeAsString(ea->type));
#endif
		// make sure we can find the attribute reference from other parts of the engine
		sym->engineObj = (void*)ea;
		
		// add to list
		ea->prev = mEvent->attributes;
		mEvent->attributes = ea;
		
		attr = itr.nextChild();
	}

	return 0;
}




#ifdef CEP_DEBUG

void EventBuilder::printEventAttributes()
{
	//mFrameworkBuilder->printEventAttributes(mEvent);
}



#endif


