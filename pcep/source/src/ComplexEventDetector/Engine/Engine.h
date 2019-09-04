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

#ifndef _SOL_CEP_ENGINE_H_
#define _SOL_CEP_ENGINE_H_

///#include <time.h>
#include "Types/EComplexEventFunction.h"
#include "Types/EPayloadAttribute.h"
///#include "../ComplexEventDetector/Engine/Types/EPayloadAttribute.h"

struct EFramework;
struct EProgram;
struct EEvent;
struct EEventAttribute;
struct EComplexEvent;
struct SEvent;
struct ECapturedEvent;
struct ECapturedEventPool;
struct TValue;
struct TAddress;
struct EEventAttributeValue;
struct EDetectionTree;
class InstructionEvaluator;
class Logger;
struct SNVPairs;
class ComplexEventObserver;


/*! 
	\brief Engine responsible for executing a complex event detection framework
	
*/
class Engine
{
public:

	/*!
		\ctor
		\param _framework The in-memory "state machine" generated
			from Dolce, used to detect the complex events.
		\param _complexEventObserver Observes generated complex events.
	*/
	Engine(	EFramework* const _framework, 
			ComplexEventObserver* const _complexEventObserver);
			
	~Engine();
	
	/*!
		\brief Advances the engine one step based on an incoming event.
		\param _event The incoming event. If there is no event, the step
		performs housekeeping.
		\returns
	*/
	int step(SEvent* _event = 0);
	
	
	
	TValue* const getAttributeValue(TAddress* const _addr,
									ECapturedEventPool* const _pool) const;
									
									
	TValue* const getExternalValue(TAddress* const _addr) const;
        
    EFramework* const mFramework;
    ///RP: 18122014
    EComplexEventFunction* findComplexEventFunction(long oid) const;
    EEventAttribute* findEventAttribute(long _oid) const;
    ///RP: 22122014        
    EEvent* findDetectEvent(long _oid) const;
    //20152311
    EComplexEvent* findComplexEvent(long _oid) const;
    ///RP: 04022015
    TValue executeProgramTValue(EProgram* _program, ECapturedEventPool* _pool);
    ///RP: 04022015 
    bool changeExternalVariable(SEvent* _event);
    ///RP: 19022015 
    bool restartBCEP(SEvent* _event);
    ///RP: 09032015 
    void deleteListAttributes(EEventAttributeValue* _listAttrs);
    ///RP: 18052015
    EEvent* const completeInjectedEvent(SEvent* _incomingEvent) const;

    ///RP: this method was protected
    EEventAttributeValue* findEventAttributeValue(EEventAttributeValue* const _attributeValues, const long _oid) const;
    EPayloadAttribute* findPayLoadAttributeByOid(long _onwerOid, long _oid) const;


protected:

	void init();
	void deinit();
	
	void expireCapturedEventPools();
	
        void expireCapturedEventPool(EComplexEvent* const _complexEvent, time_t _now);
	
	EEventAttributeValue* findCapturedEventAttribute(
											TAddress* const _addr,
											ECapturedEventPool* const _pool) const;


	/*!
		\brief Finds the most recently captured event in a pool
		\param _pool The pool to look in
		\param _oid The event oid
	*/
///	ECapturedEvent* findCapturedEvent(	ECapturedEventPool* const _pool, const long _oid) const;
        ECapturedEvent* findCapturedEvent(	ECapturedEventPool* const _pool, TAddress* const _addr) const;  ///take into account the ref field.
	

	/*!
		\brief Finds an event attribute value in a list of event attribute values
		\param _attributeValues the list of event attribute values
		\param _oid The oid of the attribute value to find
	*/
																		
    bool resolveWindowDuration(EComplexEvent*, time_t, ECapturedEvent*, int);
	Logger* mLogger;
        
	
private:

	EEvent* const lookupEvent(SEvent* _event) const;
	
	EEventAttribute* const lookupEventAttribute(	
											EEvent* const _engineEvent,
											SNVPairs* const _incomingEventAttr) const;
										
	ECapturedEvent* newCapturedEvent(SEvent* const _event, EEvent* const _eEvent, const time_t _timeStamp, long _oid = 0L);
	
	void addCapturedEventToPool(ECapturedEvent* _capturedEvent,
								EComplexEvent* _complexEvent);
	
	int executeProgram(EProgram* _program,
						ECapturedEventPool* _pool);
        ///RP: 
        ECapturedEvent* changeExtremeEnd(ECapturedEvent* _capturedEvent);
        ///RP: 
        int countCapturedEvents(ECapturedEvent* _capturedEvent) const;
        ///RP: 
        void manageCapturedEventPoolForWinTuple(EComplexEvent* _complexEvent, bool _accept);
        ///RP: 
        void manageCountersForWinTuple(EComplexEvent* _complexEvent, bool _accept);
        ///RP
        bool isOnlyOneInPoolEvents(EComplexEvent* _complexEvent, long _oid);
        ///RP
        EPayloadAttribute* const lookupPayloadAttribute(EPayloadAttribute* const _payloadAttrs, SNVPairs* const _incomingEventAttr) const;

        void checkAndReportEventAbsence(EComplexEvent* _complexEvent, ECapturedEvent *toBeDeleted, time_t _now);

        void deleteEEventGenerated(EEvent *_ee);
        
	/*!
		\brief Evaluates the tree for a complex event, matching it
			against a captured event. This process may have to
			be repeated, since the captured event can exist in more
			places in the complex event detection tree and the
			algorithm is _not_ implemented as recursion
		\param _capturedEvent The event to be found and evaluated
		\param _complexEvent The complex event for which the event
			needs to be evaluated.
		\param detected_ True if the complex event was evaluated correctly
			and as such was detected.
		
		\returns The detection tree node on which the event was detected. 
			If null, the detection cycle is finished.
	*/
	EDetectionTree* evaluateComplexEventTree(	
									ECapturedEvent* const _capturedEvent,
									EComplexEvent* const _complexEvent,
									bool& detected_);
	

	InstructionEvaluator* mInstructionEvaluator;

	ComplexEventObserver* const mComplexEventObserver;
	
	
	/*!
		\brief
		Attribute References used in the Instruction Evaluator refer to 
		"live" attributes in the captured event pools of the complex events.
		This is why in the where-clause evaluation a reference to the pool
		is passed to the Instruction Evaluator.
		
		For convencience, we use the same mechanism in the Event accept statement.
		This is why we set up a fake captured event pool, which always contains
		a temporary captured event. The fake pool is then passed to the Instruction 
		Evaluator for evaluating the accept-statement.
	*/
	ECapturedEventPool* mAcceptCapturedEventPool;
	
};





#endif


