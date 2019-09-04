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

==========================

FIXME: total refactoring. review this.
new and better approach:

* Parser: only stores declarations, not references.

* Semantic Analyzer: 
	- type checking between declarations
	- NO symbol resolution as we have now.
	- check for duplicate declarations.
	
	
Any errors: STOP.
	

* Builder: builds the engine. 
	- Tries to resolve any variables on the fly and add them to symboltable.
		USING THE SAME CODE AS THE Resolve-Engine step explained later!
		
	- Recursively walk the tree and intercept "key-nodes" that need to be built,
	 the build those key nodes providing context from the parent.
		Exmaple
			PComplex. Create EComplexEvent
				Recusrively build whatever is underneath, with EComplexEvent instance
				as context.
				
				Open itme: what if we hit a Payload-node, without the Detect-node (and hte detection tree) being built yet? Is this part of a second resolution pass??
					- yes, will detect e.g. count(evt) and add the reference.
					The Consistency Check on the engine will detect if evt is actually
					used in the detect statememnt.

* Resolve variables that have not been resolved with the help of the engine and the 
symboltable.

* Consistency check on the engine
	- For example, is Payload using attributes that have are used in the Detect-statement?
	


Some open items:

	* The E-structures are to be non-flat?? (so we can walk quickly through them, e.g. useful for "Programs")
	
* "Flatten" the E-Strcutures into Runtime-ready-structure (Runtime)
	- to save space (linked lists of address refereceneces consume 2*sizeof(addr_t)
	- to process quicker (no linked-lists)
	- "compile" the programs, possibly truncating (de-unionizing) the TValues to save space (to oprevent storing a char as a 24-byte TArea)
	
	* must also consider saving the engine to disk and restoring it 
	
	* Separate build from runtime structure?
	
*/









/* 
	SOL/CEP 
	Copyright (c) Atos S.A.
	Research & Innovation - Smart Objects Lab
	All Rights Reserved.
	
	ATOS PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
*/
#include "ComplexEventBuilder.h"

///#include <stdio.h>
///#include <string.h>
///#include <stdlib.h>
///#include <assert.h>

#include "../../Types/TValue.h"
#include "../../Types/TAddress.h"

#include "../../Common/Err.h"
#include "../DolceCompilerCodes.h"

#include "FrameworkBuilder.h"
#include "EventBuilder.h"
#include "ProgramBuilder.h"


#include "../../ComplexEventDetector/Engine/Types/EFramework.h"
#include "../../ComplexEventDetector/Engine/Types/EEvent.h"
#include "../../ComplexEventDetector/Engine/Types/EEventAttribute.h"
#include "../../ComplexEventDetector/Engine/Types/EComplexEvent.h"
#include "../../ComplexEventDetector/Engine/Types/EComplexEventFunction.h"
#include "../../ComplexEventDetector/Engine/Types/EPayloadAttribute.h"
#include "../../ComplexEventDetector/Engine/Types/EDetectionTree.h"
#include "../../ComplexEventDetector/Engine/Types/ECapturedEventPool.h"
#include "../../ComplexEventDetector/Engine/Types/EExternal.h"

#include "../ScriptCompiler/ErrorList.h"

ComplexEventBuilder::ComplexEventBuilder(FrameworkBuilder* const _fb,
										EComplexEvent* _complex_)
	: mFrameworkBuilder(_fb)
	, mComplex(_complex_)
{
	assert(_complex_);
	
}



ComplexEventBuilder::ComplexEventBuilder(FrameworkBuilder* const _fb)
	: mFrameworkBuilder(_fb)
	, mComplex(0)
{
}


ComplexEventBuilder::~ComplexEventBuilder()
{

}



int ComplexEventBuilder::build(	EFramework*& _framework_,
								ASTree* const _node)
{
	int res = OK;
	
#ifdef CEP_DEBUG
	printf("ComplexEventBuilder::build | node = [0x%p]\n", _node);
#endif

	ASTreeItr itr(_node);
	
	ASTree* node;
	ASTree* payloadNode = 0;
	
	//--- Name (creation of the complex event)
	//
	node = itr.firstChild();
	assert(node);

	mComplex = new EComplexEvent();
	node->getSymbol()->engineObj = (void*)mComplex;

	// We need a local copy of the symbol name in the Complex Event
	char* name = node->getSymbolName();
	mComplex->name = (char*)malloc(strlen(name) + 1);
	
	strcpy(mComplex->name, name);
	mComplex->oid = mFrameworkBuilder->getNextOid();

	// set up pool. TODO: possibly only needs to be done in Engine
	mComplex->capturedEventPool = new ECapturedEventPool();
	
	//--- Payload
	//
	payloadNode = _node->descendant(ASTree::PPayload);
	if(payloadNode)
	{
		mComplex->payloadAttributes = new EPayloadAttribute();
		
		res |= buildPayloadAttributes(payloadNode);
                

#ifdef CEP_DEBUG
		dumpPayloadAttributes(mComplex);
#endif
	}
	
	
	// Detect
	node = _node->descendant(ASTree::PDetect);
	if(node)
	{
		mComplex->detectionTree = buildDetectionTree(node); // mandatory
	
		// NOTICE: Program for Expression Evaluator is built in later stage!
			
		node = _node->descendant(ASTree::PDetectWindow);
		if(node)
			resolveWindow(node);	
	}
	
	// Group
	node = _node->descendant(ASTree::PGroup);
	if(node)
	{
		res |= resolveGroupAttribute(node);
	}

	// Period
	node = _node->descendant(ASTree::PLasts);
	if(node)
	{
		// FIXME-not yet implemented 
	}
	
	
	//--- Complex Event Functions Attributes / Refs
	res |= buildComplexEventFunctions(_framework_, _node);
	
	res |= buildPrograms(_node);
	
	if(mFrameworkBuilder->getErrorList()->hasError())
		res = ERR_EB_COMPLEX_EVENT_BUILD_ERR; 
		
	return res;
}


EComplexEvent* ComplexEventBuilder::finalize()
{
	return mComplex;
}


int ComplexEventBuilder::buildComplexEventFunctions(EFramework*& _framework_,
													ASTree* const _node)
{
#ifdef CEP_DEBUG
	printf("ComplexEventBuilder::buildComplexEventFunctions | node (%p)\n", 
				_node);
#endif

	ASTree* tmp;
	EComplexEventFunction* cf = 0;
	SymbolTable* symTab = mFrameworkBuilder->getSymbolTable();
	SymbolTable::Symbol* sym;
	
	
	if(_node->getType() == ASTree::PCountFunc)
	{
		ASTreeItr members(_node);
		
		tmp = members.firstChild();
		assert(tmp);

		SymbolTable::Symbol* event = tmp->getSymbol();
		assert(event);
		assert(event->decl);
		
		EEvent* e = (EEvent*)event->decl->engineObj;
		assert(e);
		
		cf = new EComplexEventFunction();
		cf->oid = mFrameworkBuilder->getNextOid();
		cf->func = nodeAsFunction(_node);
		cf->address = TAddress(e->oid);
		
		// Add back-reference to ourselves
		sym = symTab->add("{complex-event-func}", event->lineNum, event->colNum);
		sym->engineObj = (void*)cf;
		
		// We destroy the variable as we do not need it anymore to be picked
		// up by the codegen for generating a PUSH. The TAddress is part of the 
		// operand.
		//
		tmp->destroy();
		
		_node->setSymbol(sym);
	}
	else if((_node->getType() == ASTree::PSumFunc)
	|| (_node->getType() == ASTree::PDiffFunc)
	|| (_node->getType() == ASTree::PAvgFunc)
    || (_node->getType() == ASTree::PFftFunc)
    || (_node->getType() == ASTree::PTrajFunc)
    || (_node->getType() == ASTree::PCollFunc))
    {
		ASTreeItr members(_node);
			
		tmp = members.firstChild();
		assert(tmp);

		//FIXME: assuming that types have been checked earlier. Add type checking in an earlier stage
		// and don't build engine if there's any discrepancy
		//
		SymbolTable::Symbol* attr = tmp->getSymbol();
		assert(attr);
		
		attr = attr->decl;
		assert(attr);
		
		EEventAttribute* ea = (EEventAttribute*)attr->engineObj;
		assert(ea);
		
		assert(attr->owner);
		EEvent* e = (EEvent*)attr->owner->engineObj; //FIXME: get rid of EngineObj and store it on the tre.
		assert(e);
		
		cf = new EComplexEventFunction();
		cf->oid = mFrameworkBuilder->getNextOid();
		cf->func = nodeAsFunction(_node);
		cf->address = TAddress(ea->oid, e->oid, attr->windowOffset);
		
		// Add back-reference to ourselves
		sym = symTab->add("{complex-event-func}", attr->lineNum, attr->colNum);
		sym->engineObj = (void*)cf;
		
		_node->setSymbol(sym);

		// We destroy the variable as we do not need it anymore to be picked
		// up by the codegen for generating a PUSH. The TAddress is part of the 
		// operand.
		//
		tmp->destroy();
	}

	if(cf)
	{
		// add to framework
		cf->prev = _framework_->complexEventFunctions;
		_framework_->complexEventFunctions = cf;
	}
	
	// do the rest recursively.
	ASTreeItr itr(_node);

	ASTree* node = itr.firstChild();
	
	while(node)
	{
		buildComplexEventFunctions(_framework_, node);
		
		node = itr.nextChild();
	}

	return OK;
}




int ComplexEventBuilder::buildPrograms(ASTree* const _node)
{
#ifdef CEP_DEBUG
	printf("ComplexEventBuilder::buildPrograms | node (%p)\n", _node);
#endif
 

	if(_node->getType() == ASTree::PPayloadAttribute)
	{
		ASTreeItr children(_node);
		ProgramBuilder* pBuilder = mFrameworkBuilder->getProgramBuilder();

		// First child is the expression.
		
		//FIXME: an expression consisting of a single attr / constant,
		// coould be optimized to attr/const (not having to pass through expr. eval)
		// This is to be solved in the optimizer, not here.
		//
		//
		ASTree* expressionNode = children.firstChild();
		assert(expressionNode);
		
		ASTree* attributeNode = children.nextChild();
		assert(attributeNode);

		ASTreeItr itr(attributeNode);
		attributeNode = itr.firstChild();
		assert(attributeNode);
		
		
		pBuilder->build(expressionNode);
		
		// Get the payload attr.
		SymbolTable::Symbol* sym = attributeNode->getSymbol();
		assert(sym);
		
		EPayloadAttribute* pa = (EPayloadAttribute*)sym->engineObj;
		assert(pa);
		
		pa->expr = pBuilder->finalize();
	}
	else if(_node->getType() == ASTree::PDetectWhere)
	{
		ProgramBuilder* pBuilder = mFrameworkBuilder->getProgramBuilder();

		pBuilder->build(_node);
                ///RP:
                ///SE CREARA UN ATRIBUTO EN .H QUE NOS DIGA SI HAY COMPLEX FUNCTIONS;
                ///SI HAY COMPLEX FUNCTIONS DENTRO DEL PROGRAMA, SE DEBE LLENAR EL NUEVO ATRIBUTO LLAMADO subWhereClause
                ///IR LLEVANDO UNA PILA CON LOS OPERANDOS QUE SE VAYAN ENCONTRANDO HASTA DECIDIR SI SE AGREGA AL subWhereClause
                ///
                
                
			
		mComplex->whereClause = pBuilder->finalize();
	}

	
	// do the rest recursively.
	ASTreeItr itr(_node);

	ASTree* node = itr.firstChild();
	
	while(node)
	{
		buildPrograms(node);
		
		node = itr.nextChild();
	}

	return OK;
}



EDetectionTree* ComplexEventBuilder::buildDetectionTree(ASTree* const _node)
{
#ifdef CEP_DEBUG
	printf("ComplexEventBuilder::buildDetectionTree | node [0x%p]\n", _node);
#endif
	ASTreeItr itr(_node->descendant(ASTree::PDetectEventExpression));

	EDetectionTree* tree = buildDetectionTreeNode(itr.firstChild());
	
#if CEP_DEBUG
	printf("ComplexEventBuilder::buildDetectionTree | Detection tree:\n");
	tree->print(0);
#endif

	return tree;
}


EDetectionTree* ComplexEventBuilder::buildDetectionTreeNode(ASTree* const _node)
{	
#ifdef CEP_DEBUG
	printf("ComplexEventBuilder::buildDetectionTreeNode | node [0x%p]\n", _node);
#endif
	assert(_node);

	EDetectionTree* dt = 0;

	EDetectionTree::Operator op = EDetectionTree::opUnknown;
	
	ASTree::NodeType type = _node->getType();

	if(type == ASTree::PEventOpOr)
		op = EDetectionTree::opOr;
	else if(type == ASTree::PEventOpAnd)
		op = EDetectionTree::opAnd;
	else if(type == ASTree::PEventOpDuring)
		op = EDetectionTree::opDuring;
	else if(type == ASTree::PEventOpAfter)
		op = EDetectionTree::opAfter;
	else if(type == ASTree::PEventOpAbsence)
		op = EDetectionTree::opAbsence;
		
		
	if(op != EDetectionTree::opUnknown)
	{
		// It's an operator node
		dt = new EDetectionTree(op);
		
		ASTreeItr citr(_node);
		ASTree* leftNode = citr.firstChild();
		
		if(leftNode)
			dt->addLeft(buildDetectionTreeNode(leftNode));
			
		ASTree* rightNode = citr.nextChild();
		
		if(rightNode)
			dt->addRight(buildDetectionTreeNode(rightNode));
			
	}
	else
	{
		// Not an operator.
		// It's a Event or another Expression
		//
		if(type == ASTree::PDetectEventExpression)
		{
			// It's an expression
			ASTreeItr itr(_node);
			
			assert(itr.firstChild());

			dt = buildDetectionTreeNode(itr.firstChild());
		} else {
                            // It's an event
                            assert(type == ASTree::PIdentifier);

                            SymbolTable::Symbol* sym = _node->getSymbol();
                            assert(sym);
                            assert(sym->decl);

                            EEvent* e = (EEvent*)sym->decl->engineObj;
                            assert(e);

                            dt = new EDetectionTree(e->oid);

    #ifdef CEP_DEBUG
                            dt->setEventName(e->name);
    #endif

                            // Add ourselves to the event.
                            EventBuilder eventBuilder(mFrameworkBuilder, e);

                            eventBuilder.addComplexEventRef(mComplex);			
                    }
	}

	return dt;
}





int ComplexEventBuilder::buildPayloadAttributes(ASTree* const _node)
{
#ifdef CEP_DEBUG
	printf("ComplexEventBuilder::buildPayloadAttributes [0x%p]\n", _node);
#endif
	ASTree* tmp;
	SymbolTable::Symbol* sym;


	ASTreeItr itr(_node);
	ASTree* attrNode = itr.firstChild();
	
	
	while(attrNode)
	{
		ASTreeItr children(attrNode);

		EPayloadAttribute* pa = new EPayloadAttribute();
		pa->oid = mFrameworkBuilder->getNextOid();

		// First child is an expression.
		
		//FIXME: an expression consisting of a single attr / constant,
		// coould be optimized to attr/const (not having to pass through expr. eval)
		// This is to be solved in the optimizer, not here.
		//
		//
		
		//--- Expression
		//NOTICE: Program for Expression Evaluator will be built at later stage!
		tmp = children.firstChild();
		assert(tmp);
		

		//--- Attribute Name
		tmp = children.nextChild();
		tmp = tmp->descendant(ASTree::PIdentifier);
		assert(tmp);
		
		sym = tmp->getSymbol();
		assert(sym);
		
//RP:0k con char[]		strcpy(pa->name, sym->name);                
		pa->name= sym->name;
                
		// make sure we can find the attribute reference from other parts of the engine
		sym->engineObj = (void*)pa;
		
		
		// Add to the list
		pa->prev = mComplex->payloadAttributes;
		mComplex->payloadAttributes = pa;
					
		attrNode = itr.nextChild();
	}

	return 0;
}


	
int ComplexEventBuilder::resolveWindow(ASTree* const _node)
{
	ASTree* timeWinNode;
	ASTree* tmpNode;
	int res = OK;

	// Try all possible windows.
	ASTreeItr itr(_node);
	
	//-- Just an Identifier: can be tuple or sliding, depending on type.
	timeWinNode = itr.firstChild();
	if(timeWinNode->getType() == ASTree::PIdentifier)
	{
		// In both cases, symbol is expected to be an external decl.
		SymbolTable::Symbol* sym = timeWinNode->getSymbol();
		assert(sym);
		
		if(sym->decl->symbolType != SymbolTable::external_decl)
		{
			mFrameworkBuilder->getErrorList()->addError(	
								sym->lineNum, 
								sym->colNum,
								"%d: Time window identifier '%s' must be declared an external variable",
								DMSG_RT_TIME_WIN_ID_NOT_EXT,
								sym->name);

			return ERR_EB_EXPECTED_EXT_DECL;
		}

		if(sym->decl->dataType == t_int)
		{
			mComplex->windowType = EComplexEvent::tuple;
			mComplex->tupleCount = ((EExternal*)(sym->decl->engineObj))->value.v.integerVal;
		}
		else if(sym->decl->dataType == t_duration)
		{
			mComplex->windowType = EComplexEvent::sliding;
			res |= resolveDuration(timeWinNode);
		}
		else 
		{
			mFrameworkBuilder->getErrorList()->addError(	
								sym->lineNum, 
								sym->colNum,
								"%d: Time window '%s' is expected to be of type 'int' or type 'duration'",
								DMSG_RT_TIME_WIN_INVALID_TYPE,
								sym->name);
								
			return ERR_EB_UNEXPECTED_TYPE;
		}
		
		return res;
	}
	
	//--fixed
	timeWinNode = _node->descendant(ASTree::PFixedTimeWin);
	if(timeWinNode)
	{
		mComplex->windowType = EComplexEvent::fixed;

		ASTreeItr itr(timeWinNode);
		
		// duration
		tmpNode = itr.firstChild();
		assert(tmpNode);
		res |= resolveDuration(tmpNode);
		
		// time
		tmpNode = itr.nextChild();
		assert(tmpNode);
		res |= resolveRecurringTime(tmpNode);
	
		return res;
	}

	//--sliding
	timeWinNode = _node->descendant(ASTree::PSlidingTimeWin);
	if(timeWinNode)
	{
		mComplex->windowType = EComplexEvent::sliding;
		
		ASTreeItr itr(timeWinNode);
		
		// duration
		tmpNode = itr.firstChild();
		assert(tmpNode);
		res |= resolveDuration(tmpNode);
		
		return res;
	}
	
	
	///--tuple
	timeWinNode = _node->descendant(ASTree::PTupleWin);
	if(timeWinNode)
	{
		mComplex->windowType = EComplexEvent::tuple;
		
		ASTreeItr itr(timeWinNode);

#ifdef CEP_DEBUG
		timeWinNode->print();
#endif

		if(timeWinNode->countChildren() >= 2)
		{
			tmpNode = itr.firstChild();
			
			mComplex->tupleEvent = (EEvent*)tmpNode->getSymbol()->decl->engineObj;
			
			tmpNode = itr.nextChild();
			res |= resolveTupleCount(tmpNode);
		}
		else
		{
			tmpNode = itr.firstChild();
			res |= resolveTupleCount(tmpNode);
		}
		
		return res;
	}
	
	return res;
}


int ComplexEventBuilder::resolveDuration(ASTree* const _node) 
{
	if(_node->getType() == ASTree::PIdentifier)
	{
		mComplex->durationExt 
			= mFrameworkBuilder->getExternalIdReference(_node, t_duration);
	}
	else
	{
		assert(_node->getType() == ASTree::PValue);
		mComplex->duration = _node->getValue()->v.durationVal;
	}

	return OK;
}

//FIXME: semantic analyzer must check types, before even getting ot the engine generator
// e.g. external int bla = 5 minutes --> ERR (int != duration)
//


int ComplexEventBuilder::resolveRecurringTime(ASTree* const _node)
{
	TTimeLiteral res;
	
	res.time={0};
	mComplex->recurringTime = res;
	
	if(_node->getType() == ASTree::PIdentifier)
	{
		mComplex->recurringTimeExt 
				= mFrameworkBuilder->getExternalIdReference(_node, t_time);
	}
	else
	{
		assert(_node->getType() == ASTree::PValue);
		mComplex->recurringTime = _node->getValue()->v.timeVal;
	}
	
	return OK;
}


int ComplexEventBuilder::resolveTupleCount(ASTree* const _node)
{
	if(_node->getType() == ASTree::PIdentifier)
	{
		mComplex->tupleCountExt 
			= mFrameworkBuilder->getExternalIdReference(_node, t_int);
	}
	else
	{
		assert(_node->getType() == ASTree::PValue);
		mComplex->tupleCount = _node->getValue()->v.integerVal;
	}

	return OK;
}


int ComplexEventBuilder::resolveGroupAttribute(ASTree* const _node)
{
	ASTree* id = _node->descendant(ASTree::PIdentifier);
	assert(id);
	
	SymbolTable::Symbol* sym = id->getSymbol();
	assert(sym);
	
	assert(sym->decl);
	assert(sym->decl->engineObj);
	
	EEventAttribute* ea = (EEventAttribute*)sym->decl->engineObj;
	
#ifdef CEP_DEBUG
	printf("ComplexEventBuilder::resolveGroupAttribute | Group Attribute = %ld\n", ea->oid);
#endif
	
	mComplex->groupAttributeOid = ea->oid;
	
	return OK;
}


EComplexEventFunction::Function ComplexEventBuilder::nodeAsFunction(ASTree* const _node)
{
	ASTree::NodeType type = _node->getType();

	if(type == ASTree::PCountFunc) { return EComplexEventFunction::count; }
	if(type == ASTree::PSumFunc) { return EComplexEventFunction::sum; }
	if(type == ASTree::PDiffFunc) { return EComplexEventFunction::diff; }
	if(type == ASTree::PAvgFunc) { return EComplexEventFunction::avg; }
    if(type == ASTree::PFftFunc) { return EComplexEventFunction::fft; }
    if(type == ASTree::PTrajFunc) { return EComplexEventFunction::traj; }
    if(type == ASTree::PCollFunc) { return EComplexEventFunction::coll; }

	return EComplexEventFunction::unknown;
}


#ifdef CEP_DEBUG


void ComplexEventBuilder::dumpPayloadAttributes(EComplexEvent* const _complex)
{
	printf("ComplexEventBuilder::dumpPayloadAttributes\n");
	
	EPayloadAttribute* pa = _complex->payloadAttributes;
	
	SymbolTable* symTab = mFrameworkBuilder->getSymbolTable();
	
	while(pa)
	{
		if(pa->oid!=-1L)
		{
			printf("%p: %s (%s)\n",
					pa, pa->name, symTab->dataTypeAsString(pa->type));
		}
		
		pa = pa->prev;
	}
}

#endif


