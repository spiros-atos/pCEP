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


///#include <sys/time.h>   ///REMOVE used only for PSocrates Analysis

#include "Engine.h"

///#include <string.h>
///#include <assert.h>
///#include <time.h>

#include "../ComplexEventObserver.h"

#include "../../Types/TValue.h"
#include "../../Types/TAddress.h"
#include "../../Types/SEvent.h"

#include "Types/EEvent.h"
#include "Types/EFramework.h"
#include "Types/ECapturedEvent.h"

#include "../InstructionEvaluator/InstructionEvaluator.h"

#include "../../Common/Err.h" 
#include "../../Common/Trace.h"

#include "../../DolceParser/ScriptCompiler/SymbolTable.h"
#include "../../public/pluginDev/WireEventTypes.h"
#include "./Types/EComplexEventFunction.h"
///#include "Types/EPayloadAttribute.h"


Engine::Engine(	EFramework* const _framework,
				ComplexEventObserver* const _complexEventObserver) 
	: mFramework(_framework)
	, mComplexEventObserver(_complexEventObserver)
{
	assert(_framework);
	assert(_complexEventObserver);
	
	init();
}

Engine::~Engine()
{
	deinit();
}


void Engine::init()
{
	mInstructionEvaluator = new InstructionEvaluator(this);
	mAcceptCapturedEventPool = new ECapturedEventPool();
}


void Engine::deinit()
{
    delete mAcceptCapturedEventPool;
    delete mInstructionEvaluator;
}


int Engine::step(SEvent* _event)
{
    if(!_event)
    {
        expireCapturedEventPools();
        return OK;
    }
#ifdef CEP_DEBUG
    printf("Engine::step()\n");
#endif

    int res = OK;
    int resToCleanEvent = ERR_CED_DECODE; ///RP: maybe must change
	
    // Look up the event.
    EEvent* e = lookupEvent(_event);
	
    if(!e)
        return res;
		
#ifdef CEP_DEBUG
    printf("[%p] Engine::step() | Found Event %ld\n", this, e->oid);
#endif

    ///INFO()<<"TIME ELAPSED TO START: ";
    ///current_timestamp(1);

    // Filter it if an accept-statement is present.        
    if(e->accept)
    {
        // See comment in header file related to the fake captured event pool.
        ECapturedEvent* evalCapturedEvent = newCapturedEvent(_event, e, 0);
        mAcceptCapturedEventPool->capturedEvents = evalCapturedEvent;
		
        res = executeProgram(e->accept, mAcceptCapturedEventPool);
		
        ///RP:                
        deleteListAttributes(evalCapturedEvent->values);
        delete evalCapturedEvent;

        if(res != OK) 
        {
            ///RP:             
            EComplexEvent* rejectCE = e->refComplexEvents;
            while (rejectCE) 
            {
                manageCountersForWinTuple(rejectCE, false);
                manageCapturedEventPoolForWinTuple(rejectCE, false);
                rejectCE = rejectCE->prev;
            }
                   
            delete rejectCE;
            return res;
        }
    }
	
#ifdef CEP_DEBUG
    printf("[%p] Engine::step() | Event %ld Accepted\n", this, e->oid);
#endif

    // If Event is not associated to a Complex Event, there's nothing left to do.
    if(!e->refComplexEvents)
        return res;		
		
    // Consider all Complex Events to which Event is associated.
    // For consistency, we use the same time stamp for all captured
    // events.
    EComplexEvent* complexEvent = e->refComplexEvents;
    time_t timeStamp = time(0);

    ECapturedEvent *capturedEvent;
    while(complexEvent) 
    {
#ifdef CEP_DEBUG
        printf("[%p] Engine::step() | evaluating complex event '%s' (%ld)\n", this,
        complexEvent->name, complexEvent->oid);
#endif
        if (complexEvent->detectionTree->findOnly(e->oid)) 
        {
            if (complexEvent->detectionTree->op == complexEvent->detectionTree->opAbsence) 
            {
                capturedEvent = newCapturedEvent(_event, e, timeStamp, complexEvent->groupAttributeOid);
                addCapturedEventToPool(capturedEvent, complexEvent);
            }
            else 
            {
                if (complexEvent->windowType == EComplexEvent::tuple) 
                {
                    manageCountersForWinTuple(complexEvent, true);
                    manageCapturedEventPoolForWinTuple(complexEvent, true);
                }
                else 
                    expireCapturedEventPool(complexEvent, timeStamp);

                capturedEvent = newCapturedEvent(_event, e, timeStamp);
                addCapturedEventToPool(capturedEvent, complexEvent);

                // Note that the IN-clause (time window, tuple window) is managed
                // through the expireCapturedEventPool() function, which removes
                // any out-of-scope events from the detection tree. We do not bother
                // with them here.

                // First do the where-clause to see if we need to consider evaluating

                bool whereOK = true;
                complexEvent->capturedEventPool->groupAttributeOid = complexEvent->groupAttributeOid;

                if(complexEvent->whereClause)
                {
                    res = executeProgram(complexEvent->whereClause, 
                            complexEvent->capturedEventPool);

                    if(res != OK) 
                    {
                        whereOK = false;
                        complexEvent->detectionTree->setAsMarked(capturedEvent->oid);
                    }
                    else        ///RP:
                        resToCleanEvent = OK;
                }
                else 
                    resToCleanEvent = OK;

                if(complexEvent->detectionTree && whereOK)
                {
                    // Make sure we can redo the "recursive" find.
                    complexEvent->detectionTree->resetFind();

                    // The captured event may be present on multiple nodes in the 
                    // detection tree. We must handle all occurences and reset
                    // those that have been detected (so they can re-evaluated)
                    //
                    bool detected;
                    EDetectionTree* evalNode;

                    while((evalNode = evaluateComplexEventTree(capturedEvent, complexEvent, detected)))
                    {
                        if(detected)
                        {
                            evalNode->reset();
                        }
                    }
                }

                ///RP: only with generated EEvent; this event not belong to the framework.
                if (_event->type == WireEventTypes::EVT_TYPE_INJECTED_EVENT) 
                {
                    deleteEEventGenerated(e);  // lo borra???
                    //                        complexEvent = 0;
                }
            }
        }
        complexEvent = complexEvent->prev;
    }
        
    ///INFO()<<"TIME ELAPSED TO END: ";
    ///current_timestamp(1);
        
    ///RP:	return res;
    return resToCleanEvent;
}


int Engine::executeProgram(EProgram* _program,
							ECapturedEventPool* _pool)
{
    int res = OK;
    res = mInstructionEvaluator->run(_program, _pool);
	
    if(res != OK)
        return res;
		
    // Get top of stack
    ///	TValue* tos = mInstructionEvaluator->getTopOfStack();   0k
    TValue tos;
    res = mInstructionEvaluator->popStack(&tos);
        
#ifdef CEP_DEBUG
    if(tos.isValid())
        printf("[%p] Engine::step(). Top of Stack=%s\n", this, tos.asString());
    else
        printf("[%p] Engine::step(). Nothing on top of stack\n", this);
#endif

    ///	if(!tos || tos->v.integerVal != 1)
    if(tos.v.integerVal != 1)
        return ERR_EVAL_FALSE;

    ///RP:                
    if (tos.t==t_string)
        free(tos.v.stringVal);
                
    return OK;
}


TValue Engine::executeProgramTValue(EProgram* _program, ECapturedEventPool* _pool)
{
	int res = OK;
        TValue tos;
	res = mInstructionEvaluator->run(_program,_pool);
	if(res != OK)
		return tos;
///	TValue* tos = mInstructionEvaluator->getTopOfStack();
        
        res= mInstructionEvaluator->popStack(&tos);
        
#ifdef CEP_DEBUG
	if(tos.isValid())
		printf("[%p] Engine::step(). Top of Stack=%s\n", this, tos.asString());
	else
		printf("[%p] Engine::step(). Nothing on top of stack\n", this);
#endif
        return tos;
}


EEvent* const Engine::lookupEvent(SEvent* _incomingEvent) const
{
    if (_incomingEvent->type == WireEventTypes::EVT_TYPE_INJECTED_EVENT)
        return completeInjectedEvent(_incomingEvent);
    else {
	EEvent* e = mFramework->events;

	while(e)
	{
		if(_incomingEvent->name)
		{
			if(!strcmp(e->name, _incomingEvent->name))
				return e;
		}
		else
		{
			if(e->id && (_incomingEvent->id == e->id))
				return e;
		}

			
		e = e->prev;
	}

	return 0;
    }
}

///RP: complete the event created based on complex event defined in the detect clause
EEvent* const Engine::completeInjectedEvent(SEvent* _incomingEvent) const {
    
    EEvent *ee = new EEvent();
    ee->oid = _incomingEvent->oid;
    ee->accept = 0;
    ee->refComplexEvents = _incomingEvent->refComplex;
    SNVPairs* nvPairs = _incomingEvent->nvPairs;
    
    while (nvPairs) {
        if (nvPairs->name) {
            EEventAttribute *eeAttr = new EEventAttribute();
            
            eeAttr->name = (char*)malloc(strlen(nvPairs->name) + 1);
            strcpy(eeAttr->name, nvPairs->name);
            EPayloadAttribute *payloadAttr = lookupPayloadAttribute(_incomingEvent->payloadAttrs, nvPairs);  ///EPayloadAttribute *payloadAttrs;
            eeAttr->oid = payloadAttr->oid;
            
            eeAttr->prev = ee->attributes;
            ee->attributes= eeAttr;

        }
        nvPairs = nvPairs->prev;
    }
    
    return ee;
}

///RP:
EPayloadAttribute* const Engine::lookupPayloadAttribute(EPayloadAttribute* const _payloadAttrs, SNVPairs* const _incomingEventAttr) const
{
    EPayloadAttribute *pla = _payloadAttrs;
    while(pla) {      	
        if(_incomingEventAttr->name) {
            if(!strcmp(_incomingEventAttr->name, pla->name))
		return pla;
        }
        
        pla = pla->prev;
    }
    
    return 0;
}


EEventAttribute* const Engine::lookupEventAttribute(	
										EEvent* const _frameworkEvent,
										SNVPairs* const _incomingEventAttr) const
{
	EEventAttribute* ea = _frameworkEvent->attributes;
	
	while(ea)
	{
#ifdef CEP_DEBUG
//		printf("[%p] Engine::lookupEventAttribute | Comparing %s %ld with %s %ld\n", 
//				this, ea->name, ea->id,_incomingEventAttr->name, _incomingEventAttr->id);
#endif

		if(_incomingEventAttr->name)
		{
			if(!strcmp(_incomingEventAttr->name, 
						ea->name))
				return ea;
		}
		else
		{
		//TODO: either in spec, engine or semantic analyzer: MAP ID > 0, that is, ID may never be 0 .
		
			if(ea->id 
				&& (_incomingEventAttr->id == ea->id))
				return ea;
		}
	
		ea = ea->prev;
	}
	
	return 0;
}


ECapturedEvent* Engine::newCapturedEvent(SEvent* const _incomingEvent, 
        EEvent* const _frameworkEvent, const time_t _timeStamp, long _oid)
{
	ECapturedEvent* capturedEvent = new ECapturedEvent();
	
#ifdef CEP_DEBUG
	strncpy(capturedEvent->name, _frameworkEvent->name, 32);
#endif

	capturedEvent->oid = _frameworkEvent->oid;
	capturedEvent->timeStamp = _timeStamp;
    ///RP: to complex event in detect clause        
    capturedEvent->type = _incomingEvent->type; //WireEventTypes::EVT_TYPE_INJECTED_EVENT;
	
	// Get the attributes from the incoming event and add them to the captured event.
	
	SNVPairs* nvPairs = _incomingEvent->nvPairs;
        
	EEventAttribute* ea;
	
    while(nvPairs)
    {
        // Only add to captured event if it matches
        ea = lookupEventAttribute(_frameworkEvent, nvPairs);
        if(ea && (_oid == 0 || _oid == ea->oid))  //_oid used to decide if does filtering; absent operator
        {
            EEventAttributeValue* eaVal = new EEventAttributeValue();
                        
            eaVal->oid = ea->oid;
                        
            if (nvPairs->val.t == t_string) {
                TValue tvStr(nvPairs->val.v.stringVal);
                eaVal->val = tvStr;
            }
            else
                eaVal->val = nvPairs->val;

            eaVal->prev = capturedEvent->values;
            capturedEvent->values = eaVal;
        }
                
        nvPairs = nvPairs->prev;
    }
        
	return capturedEvent;
}


void Engine::addCapturedEventToPool(ECapturedEvent* _capturedEvent,
									EComplexEvent* _complexEvent)
{
	
///RP:
///    if (_complexEvent->windowType != EComplexEvent::noWindow) {

        ECapturedEventPool* pool = _complexEvent->capturedEventPool;
	
                
            _capturedEvent->prev = pool->capturedEvents;
            _capturedEvent->next = 0x00;

            if(pool->capturedEvents)
            	pool->capturedEvents->next = _capturedEvent;
		
            pool->capturedEvents = _capturedEvent;

///    }
}

	
EDetectionTree* Engine::evaluateComplexEventTree(
									ECapturedEvent* const _capturedEvent,
									EComplexEvent* const _complexEvent,
									bool& detected_)
{
	detected_ = false;
	
#ifdef CEP_DEBUG
	_complexEvent->detectionTree->print();
	
	assert(_capturedEvent);
	
	printf("[%p] Engine::evaluateComplexEventTree | Looking up captured event '%s' %ld\n", 
					this,
					_capturedEvent->name,
					_capturedEvent->oid);
#endif

	EDetectionTree* valNode = _complexEvent->detectionTree->find(_capturedEvent->oid);
	
	
	if(valNode)
	{
		EDetectionTree::EvalResult res = valNode->evaluate(_capturedEvent, _complexEvent);
		

#ifdef CEP_DEBUG 
		printf("[%p] Engine::evaluateComplexEventTree | Found valNode (%ld):\n", 
				this, valNode->eventOid);
		printf("\n");
		_complexEvent->detectionTree->print();
		printf("\n");
#endif
		
		while(res == EDetectionTree::again)
		{

#ifdef CEP_DEBUG 
			printf("[%p] Engine::evaluateComplexEventTree | Looking up another one\n", this);
			printf("\n");
			_complexEvent->detectionTree->print();
			printf("\n");
#endif
		
			res = valNode->evaluate(_capturedEvent, _complexEvent);
		}
		
		if(res == EDetectionTree::detectedComplexEvent)
		{
			// bingo.
#ifdef CEP_DEBUG
			printf("[%p] \n***\n***\n***", this);
			printf("[%p] [Engine::evaluateComplexEventTree] *** Detected Complex Event '%s' (%ld)\n",
					this, _complexEvent->name, _complexEvent->oid);
#endif
			detected_ = true;
			
			mComplexEventObserver->raise(_complexEvent, _capturedEvent);
					
			INFO() << "Complex Event Detected " << this << std::endl;
		}
	}
	
	return valNode;
}

///This method works with all complex events in the framework.	
void Engine::expireCapturedEventPools()
{
	if(!mFramework->complexEvents)
		return;
	
	time_t now = time(0); // use same time for all iterations.

	EComplexEvent* ce = mFramework->complexEvents;
	while(ce)
	{
            if (ce->windowType != EComplexEvent::tuple)
		expireCapturedEventPool(ce, now);
		
            ce = ce->prev;
	}

}


void Engine::expireCapturedEventPool(EComplexEvent* const _complexEvent, time_t _now) {
    
#ifdef CEP_DEBUGx
	printf("[%p] [Engine::expireCapturedEventPool] Complex Event '%s' (%ld)\n", 
					this,
					_complexEvent->name,
					_complexEvent->oid);
#endif

	// FIXME-ROADMAP: it is on the roadmap to complete the Dolce implementation
	//
	// Events expire on
	//	- TupleCount window size reached FIXME-TODO: not implemented
	//  - Time Window expired
	//	FIXME-TODO: suggested pool management by means of some internal vars (not having
	//		to do with the Dolce spec
	//	- MaxTimeToLive. Events are removed after this time.
	//	- MaxPoolSize - Do not removed, but remove  oldest event. Different algorithm
	                       
	ECapturedEventPool* pool = _complexEvent->capturedEventPool;

	if(!pool || !pool->capturedEvents)
	{
#ifdef CEP_DEBUGx
		if(!pool)
			printf("[%p] [Engine::expireCapturedEventPool] No pool.\n", this);
		else
			printf("[%p] [Engine::expireCapturedEventPool] No captured events in pool.\n", this);
#endif		
///RP:  null to capturedEvents
///                if (!pool->capturedEvents)
///                    pool = 0x00;
                
                return;
	}

	EDetectionTree* dt = _complexEvent->detectionTree;
	EDetectionTree* dtNode;
	
	ECapturedEvent* capturedEvent = pool->capturedEvents;
	bool wasDeleted;
	
#ifdef CEP_DEBUGx
	printf("[%p] expireCapturedEventPool for complex event oid %ld\n", this, _complexEvent->oid);
#endif
	
        int idxTuple=0;     ///RP: 18122014 is planned to use in window tuple
//IT'S 0K        if (_now == 0) {
            capturedEvent = changeExtremeEnd(capturedEvent);
//IT'S 0K        }
        ///to walk the list from the first incoming
	while(capturedEvent && idxTuple >= 0)
	{
		wasDeleted = false;
                (_now==0) ? idxTuple=-1 : idxTuple++;
		
///RP: 18122014		timeInPool = _now - capturedEvent->timeStamp;              moved to resolveWindowDuration method
#ifdef CEP_DEBUG		
		printf("[%p] Engine::expireCapturedEventPool | Checking captured event %p, time-left-in-pool = %ld\n", this, capturedEvent, _complexEvent->duration - _now - capturedEvent->timeStamp);
#endif
                
 //RP: 18122014		if(timeInPool > _complexEvent->duration)        // is replaced for:

                   if (_now==0 || resolveWindowDuration(_complexEvent, _now, capturedEvent, idxTuple))   //RP: this method must return true to delete the event
                   {
#ifdef CEP_DEBUG
			printf("[%p] Engine::expireCapturedEventPool | Event %p deleted from pool\n",
				this, capturedEvent);
#endif
			// Reset detection tree for the event(s)
			//                        
			dt = _complexEvent->detectionTree;
			
			if(dt)
			{
///RP: unlink the "and" in "detect" of the "windows"
                            if (dt->op != EDetectionTree::opAnd) {
///RP: if exist more than one dont make the reset.
                                if (isOnlyOneInPoolEvents(_complexEvent, capturedEvent->oid)) {                                
                                    dt->resetFind();

                                    while((dtNode = dt->find(capturedEvent->oid)))
                                    {
                                            // Reset 
                                            dtNode->reset(false); //non-recursive
                                    }
                                }
                            }
			}

                        if (_complexEvent->detectionTree->op == _complexEvent->detectionTree->opAbsence)
                            checkAndReportEventAbsence(_complexEvent, capturedEvent, _now);
                        
			ECapturedEvent* toBeDeleted = capturedEvent;
		// Delete from pool
			if(toBeDeleted->prev)
				toBeDeleted->prev->next = toBeDeleted->next;
			if(toBeDeleted->next)
				toBeDeleted->next->prev = toBeDeleted->prev;
                       
                        if(toBeDeleted == pool->capturedEvents)
                            pool->capturedEvents = 0x00;
                                                
                        deleteListAttributes(toBeDeleted->values);
                        toBeDeleted = NULL;
                        delete toBeDeleted;
                        
			// Advance to next before we delete it and prevent a double-advance.
//                        capturedEvent = capturedEvent->prev;  //USAR EL NEXT
                        capturedEvent = capturedEvent->next;
			wasDeleted = true;
		}
		
		if(!wasDeleted) // prevent double-advance in case we deleted an capturedEvents
//			capturedEvent = capturedEvent->prev;  //usar el NEXT
                    capturedEvent = capturedEvent->next;
			
		//FIXME: WE COULD HAVE TIME BASED COMPLEX EVENT HERE!!!!
		// mComplexEventObserver->raise(complexEvent);
	}

}

TValue* const Engine::getAttributeValue(TAddress* const _addr, 
            ECapturedEventPool* const _pool) const
{
#ifdef CEP_DEBUG
	printf("[%p] Engine::getAttributeValue | Getting Attribute Value for oid=%ld, owned by oid=%ld\n", 
                this, _addr->oid, _addr->ownerOid);
#endif

	EEventAttributeValue* attr = findCapturedEventAttribute(_addr, _pool);
	if(attr)
		return &attr->val;

#ifdef CEP_DEBUG
	printf("not found\n");
#endif

	return 0;
}

EEventAttributeValue* Engine::findCapturedEventAttribute(
											TAddress* const _addr,
											ECapturedEventPool* const _pool) const
{
///	ECapturedEvent* ce = findCapturedEvent(_pool, _addr->ownerOid);
	ECapturedEvent* ce = findCapturedEvent(_pool, _addr);   ///to consider the addr.ref; to offset operator
        
	if(ce)
		return findEventAttributeValue(ce->values, _addr->oid);
	
	return 0;
}


///ECapturedEvent* Engine::findCapturedEvent(	ECapturedEventPool* const _pool, const long _oid) const
    ECapturedEvent* Engine::findCapturedEvent(	ECapturedEventPool* const _pool, TAddress* const _addr) const
{
	ECapturedEvent* ce = _pool->capturedEvents;        
	int offset=0;
        
	while(ce)
	{
            if (_addr->ref == 0) {
        	if (ce->oid == _addr->ownerOid)
			return ce;
	    }
            else  {
                if (ce->oid == _addr->ownerOid) {
                    if (offset == _addr->ref)
                        return ce;
                    
                    offset++;
                }

            }
            ce = ce->prev;
	}

	return 0;
}


EEventAttributeValue* Engine::findEventAttributeValue(
									EEventAttributeValue* const _attributeValues, 
									const long _oid) const
{
	// Find the event.
	EEventAttributeValue* attr = _attributeValues;

	while(attr)
	{
		if(attr->oid == _oid)
			return attr;
		
		attr = attr->prev;
	}

	return 0;
}

EEvent* Engine::findDetectEvent(long _oid) const {
    
    EEvent* de = mFramework->events;
    
    while(de) 	{
        if(de->oid == _oid)
            return de;
        de = de->prev;
    }
    
    return 0;
}

EComplexEvent* Engine::findComplexEvent(long _oid) const {
    
    EComplexEvent* ce = mFramework->complexEvents;
    
    while(ce) {
        if(ce->oid == _oid)
            return ce;
        ce = ce->prev;
    }
    
    return 0;
}

///RP: 20052015
EPayloadAttribute* Engine::findPayLoadAttributeByOid(long _onwerOid, long _oid) const {
    
    EComplexEvent *eCE = mFramework->complexEvents;
    EPayloadAttribute *pa = 0;
            
    while (eCE) {
        if (eCE->oid == _onwerOid) {
            pa = eCE->payloadAttributes;
            if (pa) {
                while (pa->oid > 0) {
                    if (pa->oid == _oid)
                        return pa
                                ;
                    pa= pa->prev;
                }
            }
        }
        eCE = eCE->prev;
    }
    
    return pa;
}

///RP: 18122014
EComplexEventFunction* Engine::findComplexEventFunction(long _oid) const {
        
    EComplexEventFunction* cef = mFramework->complexEventFunctions;
	
    while(cef) 	{
        if(cef->oid == _oid)
            return cef;
        cef = cef->prev;
    }
    return 0;
    
}

///RP: 18122014
EEventAttribute* Engine::findEventAttribute(long _oid) const {
        
    EEvent *e = mFramework->events;
    EEventAttribute *eAttr;
    
    while(e) {
        eAttr = e->attributes;
        while (eAttr) {
            if(eAttr->oid == _oid)
                return eAttr;
            
            eAttr = eAttr->prev;
        }
        e = e->prev;
    }
    return 0;   
}

TValue* const Engine::getExternalValue(TAddress* const _addr) const
{
	// Find the external value.
	EExternal* ext = mFramework->externals;

	while(ext)
	{
		if(ext->oid == _addr->oid)
		{
			return &ext->value;
		}
		
		ext = ext->prev;
	}
	
	return NULL;
}

///RP: 18122014 created to manage the different time window types; return true if this event must be removed.
    bool Engine::resolveWindowDuration(EComplexEvent* _complexEvent, time_t _now, ECapturedEvent* capturedEvent, int idxTuple) {
        
        bool res=0;

        if (_complexEvent->windowType == EComplexEvent::noWindow)

            res=1;      // if don't defined window; delete;

        else if (_complexEvent->windowType == EComplexEvent::sliding) {
            
            time_t* timeInPool =  (time_t*)malloc(sizeof(time_t));

            *timeInPool = _now - capturedEvent->timeStamp;

            if (_complexEvent->durationExt)
                res = *timeInPool >= _complexEvent->durationExt->value.v.durationVal;
            else
                if (_complexEvent->duration)
                        res = *timeInPool >= _complexEvent->duration;
                else
                    res = 1;

            delete timeInPool;
        }
//This code is ready to integrate the window tuple
/*RP: is used other strategy         
        else if (_complexEvent->windowType == EComplexEvent::tuple && countCapEvents > 0) {

            res = idxTuple <= (countCapEvents - (_complexEvent->tupleCount-1));
            
        }
 */
        return res;
    }


///RP change the value of an External Var
    bool Engine::changeExternalVariable(SEvent* _event) {
 ///130 ChangeExternalVar int umbral01 22
        SNVPairs* pairs = _event->nvPairs;
        EExternal* extVars = mFramework->externals;
        while (extVars) {
                if (!strcmp(extVars->nameEV, pairs->name) && pairs->val.t == extVars->value.t) {
                    extVars->value = pairs->val;
                    return 1;
                }
            extVars = extVars->prev;
        }
        return 0;
    }
    
 
///RP: 19022015 reload the dolce file
    bool Engine::restartBCEP(SEvent* _event) {
 ///153 Shutdown int scope 1       ///1-> reload config 2,3,    9->shutdown.
        SNVPairs* pairs = _event->nvPairs;
//        EExternal* extVars = mFramework->externals;
        while (pairs && !strcmp(_event->name,"Shutdown")) {
                if (!strcmp(pairs->name, "scope") && pairs->val.t == t_int && pairs->val.v.integerVal == 1) {
                    this->mFramework->bcepMngr->BCEPRestart(1);
                    return 1;
                }
            pairs = pairs->prev;
        }
        return 0;
    }
    
  
///RP: 09022015 find the opposite end
    ECapturedEvent* Engine::changeExtremeEnd(ECapturedEvent* _capturedEvent) {

        while (_capturedEvent->prev)
            _capturedEvent = _capturedEvent->prev;
        
        return _capturedEvent;
    }
        
        
///RP: 09022015 count the events in the pool of a complex event
    int Engine::countCapturedEvents(ECapturedEvent* _capturedEvent) const {
        
        int countCE=0;
        while (_capturedEvent) {
            _capturedEvent = _capturedEvent->prev;
            countCE++;            
        }
        
        return countCE;
    }

///RP: 09022015 To control the event pool of complex event with tuple window; and unstack from tape
    void Engine::manageCapturedEventPoolForWinTuple(EComplexEvent* _complexEvent, bool _accept) {
                
        if (_complexEvent && _complexEvent->windowType == EComplexEvent::tuple && 
            _complexEvent->capturedEventPool && _complexEvent->capturedEventPool->capturedEvents) {
            
            int sizeTuple = (_complexEvent->tupleCountExt == 0 ? _complexEvent->tupleCount : _complexEvent->tupleCountExt->value.v.integerVal);
            int countCE = countCapturedEvents(_complexEvent->capturedEventPool->capturedEvents);
            
            if (_accept) {
                if (countCE == 0) { //empty 
                    //the tape is started;                    
                }
///             else if (_complexEvent->tupleCount <= countCE) {
                else if (sizeTuple <= countCE) {
                    //delete
                    expireCapturedEventPool(_complexEvent, 0);
//                    _complexEvent->tape = _complexEvent->tape>>1;
                    _complexEvent->tape = pow(2, sizeTuple)-1;
                }
                else if (countCE != 0) {  //empty: nada!!
///                    if ((_complexEvent->rejectedInStack + countCE) >= _complexEvent->tupleCount) {  sizeTuple
                       if ((_complexEvent->rejectedInStack + countCE) >= sizeTuple) {  
                        //delete 
                        if  ((_complexEvent->tape % 2) != 0) { //(_complexEvent->tape & 1)
                            expireCapturedEventPool(_complexEvent, 0);
                        }
                        else {
                            _complexEvent->rejectedInStack--;
                        }
                        
                        _complexEvent->tape = _complexEvent->tape>>1;
                    }                        
                }
                
            }
            else {
                if (countCE == 0) { //empty 
                    
                }
///                else if (_complexEvent->tupleCount <= countCE) {
                else if (sizeTuple <= countCE) {   
                    //delete & add?
                    expireCapturedEventPool(_complexEvent, 0);
                    _complexEvent->tape = _complexEvent->tape>>1;
                    }
///                    else if ((_complexEvent->rejectedInStack + countCE) >= _complexEvent->tupleCount) {
                    else if ((_complexEvent->rejectedInStack + countCE) >= sizeTuple) {
                        if ((_complexEvent->tape % 2) != 0) { //(_complexEvent->tape & 1)  {
                            expireCapturedEventPool(_complexEvent, 0);
                        }
                        else {
                            _complexEvent->rejectedInStack--;
                        }
                        
                        _complexEvent->tape = _complexEvent->tape>>1;
                        
                    }  
            }
                       
        }
        
    }
    
///RP: 16022015 To control counters and stacked of tape of complex event with tuple window;
    void Engine::manageCountersForWinTuple(EComplexEvent* _complexEvent, bool _accept) {
                    
        if (_complexEvent && _complexEvent->windowType == EComplexEvent::tuple) {
            
            int sizeTuple = (_complexEvent->tupleCountExt == 0 ? _complexEvent->tupleCount : _complexEvent->tupleCountExt->value.v.integerVal);
            int countCE = 0;
        
            if (_complexEvent->capturedEventPool && _complexEvent->capturedEventPool->capturedEvents) 
            {
                countCE = countCapturedEvents(_complexEvent->capturedEventPool->capturedEvents);
            }
            
            if (_accept) {
                    if (countCE == 0) {
///                       _complexEvent->rejectedInStack= (_complexEvent->rejectedToStack >= _complexEvent->tupleCount) ? _complexEvent->tupleCount-1 : _complexEvent->rejectedToStack;  //or positive subtraction?? related to cuantos se apilan
                       _complexEvent->tape = pow(2, _complexEvent->rejectedToStack);
                       
                       (_complexEvent->rejectedInStack == 0) ? 0 : _complexEvent->rejectedInStack--;                       
                       _complexEvent->rejectedToStack= 0;  //is possible stack all
///                       _complexEvent->tape=1;
                    }
///                    else if (_complexEvent->tupleCount == countCE) {
                    else if (sizeTuple == countCE) {
///                             _complexEvent->tape = pow(2, _complexEvent->tupleCount)-1;
                             _complexEvent->tape = pow(2, sizeTuple)-1;  
                             _complexEvent->rejectedInStack=0;
                             _complexEvent->rejectedToStack=0;

//                             _complexEvent->tape = _complexEvent->tape
                        }
                        else {   //not empty, not full
                            _complexEvent->tape = _complexEvent->tape + (pow(2, _complexEvent->rejectedInStack + countCE));
                            _complexEvent->rejectedToStack=0;   //o nada!!
                        }
        //                expireCapturedEventPool(_complexEvent, 0);                
            }
            else {  //event no accepted (zeros)
                 if (countCE == 0) {  //empty pool
//                    _complexEvent->rejectedInStack++;
///                    _complexEvent->rejectedInStack = ((_complexEvent->rejectedInStack >= _complexEvent->tupleCount) ? _complexEvent->tupleCount-1 : _complexEvent->rejectedInStack+1);
                    _complexEvent->rejectedInStack = ((_complexEvent->rejectedInStack >= sizeTuple) ? sizeTuple-1 : _complexEvent->rejectedInStack+1);
///                    _complexEvent->rejectedToStack = ((_complexEvent->rejectedToStack >= _complexEvent->tupleCount-1) ? _complexEvent->tupleCount-1 : _complexEvent->rejectedToStack+1);
                    _complexEvent->rejectedToStack = ((_complexEvent->rejectedToStack >= sizeTuple-1) ? sizeTuple-1 : _complexEvent->rejectedToStack+1);  
                  }
///                  else if (_complexEvent->tupleCount == countCE) { //full pool
                  else if (sizeTuple == countCE) { //full pool
                       _complexEvent->rejectedInStack=1;
                       _complexEvent->rejectedToStack = 1; // (_complexEvent->rejectedToStack >= _complexEvent->tupleCount) ? _complexEvent->tupleCount-1 :_complexEvent->rejectedToStack+1;
                       }
                        else { //not empty, not full
                           _complexEvent->rejectedInStack++;
///                           _complexEvent->rejectedToStack = ((_complexEvent->rejectedToStack >= _complexEvent->tupleCount) ? _complexEvent->tupleCount-1 :_complexEvent->rejectedToStack+1);
                              _complexEvent->rejectedToStack = ((_complexEvent->rejectedToStack >= sizeTuple) ? sizeTuple-1 :_complexEvent->rejectedToStack+1);
                        }
        //                expireCapturedEventPool(_complexEvent, 0);                
            }
        }
        
    }

///RP: 16022015 To delete the list of attributes;
    void Engine::deleteListAttributes(EEventAttributeValue* _listAttrs) {
        
        EEventAttributeValue *toDlt, *eEAttr = _listAttrs;
        
        while (eEAttr) {
            toDlt= eEAttr;
            eEAttr = eEAttr->prev;

            if (toDlt->val.t == t_string && toDlt->val.v.stringVal)
                free(toDlt->val.v.stringVal);

            delete toDlt;
        }
    }

///RP: 20052015 To delete EEvent generated based on complex in detected clause;
    void Engine::deleteEEventGenerated(EEvent *_ee) {
/*        
        EEventAttribute *toDlt, *eEAttr = _ee->attributes;
        
        while (eEAttr) {
            toDlt= eEAttr;
            eEAttr = eEAttr->prev;
            
            free(toDlt->name);

            delete toDlt;
        }
*/       

        delete _ee;
    }
    
///RP: 16042015 To check if an event is the only  one in the pool events (of its kind);
    bool Engine::isOnlyOneInPoolEvents(EComplexEvent* _complexEvent, long _oid) {

        int cnts=0;
        if (_complexEvent && _complexEvent->capturedEventPool) {
            ECapturedEvent *eCapEvent = _complexEvent->capturedEventPool->capturedEvents;
            while (eCapEvent) {
                if (eCapEvent->oid == _oid)
                    cnts++;
                
                eCapEvent = eCapEvent->prev;
            }
            if (cnts>1)
                return false;
        }       
        return true;
    }


/// to analysis, get milliseconds or microseconds    
/*
void Engine::current_timestamp(int _m) {
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    long long xseconds = 0;
    xseconds = (_m == 1 ? (te.tv_sec*1000LL + te.tv_usec/1000) : (te.tv_sec*1000000LL + te.tv_usec)); // caculate milliseconds
    
    printf("milliseconds: %lld\n", xseconds);
    
//    return xseconds;
} 
 * */   
    
//RP: method to check and report the absence of some event
void Engine::checkAndReportEventAbsence(EComplexEvent *_complexEvent, ECapturedEvent *toBeDeleted, time_t _now) {

        EEvent *detectedEventTBDel = findDetectEvent(toBeDeleted->oid);
        TValue groupValOut; bool woGroupId=false;
        EEventAttributeValue *eAttValGrp = 0;

        if (_complexEvent->windowType != EComplexEvent::noWindow) {
            int cnt=0;
            ECapturedEvent *eventAtPool = _complexEvent->capturedEventPool->capturedEvents;
            if (_complexEvent->groupAttributeOid > 0) {
                eAttValGrp = findEventAttributeValue(toBeDeleted->values, _complexEvent->groupAttributeOid);
                woGroupId = true;
                groupValOut.t = eAttValGrp->val.t;
                groupValOut.v = eAttValGrp->val.v;
            }
            while (eventAtPool && cnt < 2) {//please check me!!
                if (eventAtPool->oid == detectedEventTBDel->oid) {
                    if (woGroupId)
                        eAttValGrp = findEventAttributeValue(eventAtPool->values, _complexEvent->groupAttributeOid);
                    
                    if (woGroupId && eAttValGrp->val.operator ==(groupValOut))
                        cnt++;
                    else if (!woGroupId)
                        cnt++;
                }
                eventAtPool = eventAtPool->prev;
            }
            if (cnt <= 1)
                mComplexEventObserver->raise(_complexEvent, toBeDeleted);
            
            cnt=0;
        }
}
