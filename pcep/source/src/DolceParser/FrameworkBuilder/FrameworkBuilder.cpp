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

#include "FrameworkBuilder.h"

///#include <stdio.h>
///#include <string.h>
///#include <stdlib.h>
///#include <assert.h>

#include "../../Common/Err.h"
#include "../DolceCompilerCodes.h"

#include "../../Types/TValue.h"
#include "../../ComplexEventDetector/Engine/Types/EFramework.h"

#include "../ScriptCompiler/Compiler.h"
#include "../ScriptCompiler/SymbolTable.h"
#include "../ScriptCompiler/Parser.h"
#include "../ScriptCompiler/ErrorList.h"

#include "ProgramBuilder.h"
#include "ComplexEventBuilder.h"
#include "EventBuilder.h"


FrameworkBuilder::FrameworkBuilder(Compiler* const _c,
									ErrorList* const _el)
	: mCompiler(_c)
	, mSymTab(0)
	, mErrorList(_el)
	, mProgramBuilder(0)
	, mOid(0L)
{
	
	mSymTab = _c->getSymbolTable();
	init();
}


FrameworkBuilder::~FrameworkBuilder()
{
	deinit();
}

EFramework* FrameworkBuilder::build(int& res_)
{
	res_ = OK;
	
	EFramework* framework = new EFramework();
        
        framework->mSymTab = mSymTab;
	
	res_ |= buildExternals(framework);
	res_ |= buildEvents(framework);
	res_ |= buildComplexEvents(framework);
	
#ifdef CEP_DEBUG
	mCompiler->getParser()->getASTree()->print();
	
	mSymTab->print();
	
	print(framework);
#endif

	if(res_ != OK)
	{
		delete framework;
		framework = 0;
	}

	return framework;
}

//FIXME: get rid of member var. have buildEngine return an new instance

int FrameworkBuilder::buildExternals(EFramework*& _framework_)
{
#ifdef CEP_DEBUG
	printf("FrameworkBuilder::buildExternals\n");
#endif	

	ASTree* topNode = mCompiler->getParser()->getASTree();
	
	ASTreeItr itr(topNode);

	ASTree* curr = itr.firstChild();
	EExternal* external;
	
	while(curr)
	{
		if(curr->getType() == ASTree::PExternal)
		{
			external = createExternal(curr);
			
			// add to list
			external->prev = _framework_->externals;
			_framework_->externals = external;

		}	

		curr = itr.nextChild();
	}

	return OK;
}


int FrameworkBuilder::buildEvents(EFramework*& _framework_)
{
#ifdef CEP_DEBUG
	printf("FrameworkBuilder::buildEvents\n");
#endif		

	ASTree* topNode = mCompiler->getParser()->getASTree();
	
	ASTreeItr itr(topNode);

	ASTree* currNode = itr.firstChild();

	while(currNode)
	{
		if(currNode->getType() == ASTree::PEvent)
		{
			EventBuilder builder(this);

			builder.build(currNode);
			
			// add to list
			EEvent* e = builder.finalize();
			e->prev = _framework_->events;
			_framework_->events = e;
		}
		currNode = itr.nextChild();
	}

	return OK;
}


int FrameworkBuilder::buildComplexEvents(EFramework*& _framework_)
{
#ifdef CEP_DEBUG
	printf("FrameworkBuilder::buildComplexEvents\n");
#endif	
	int res = OK;
	
	
	ASTree* topNode = mCompiler->getParser()->getASTree();
	
	ASTreeItr itr(topNode);

///RP: Change the order to walk the tree
	ASTree* currNode = itr.lastChild();
	
	while(currNode)
	{
		if(currNode->getType() == ASTree::PComplexEvent)
		{
			ComplexEventBuilder builder(this);

			res |= builder.build(_framework_, currNode);
			
			// Add to the list.
			EComplexEvent* newComplexEvent = builder.finalize();
			
			newComplexEvent->prev = _framework_->complexEvents;
			_framework_->complexEvents = newComplexEvent;
		}

///RP:		currNode = itr.nextChild();
                currNode = itr.prevChild();   //reverse order
	}

	return res;
}


EExternal* FrameworkBuilder::createExternal(ASTree* const _node)
{
#ifdef CEP_DEBUG
	printf("FrameworkBuilder::createExternal [0x%p]\n", _node);
#endif
///        char* tmpName =	_node->getSymbolName();
        
	ASTreeItr itr(_node);
	ASTree* tmp;
	ASTree* val;
	
	// value
	val = itr.firstChild();
	assert(val);

	// ID
	tmp = itr.nextChild();
	tmp = tmp->descendant(ASTree::PIdentifier);
	assert(tmp);

	EExternal* e = new EExternal();
	
	e->oid = getNextOid();
	e->value = *(val->getValue());
///RP: name is neccesary        
        e->nameEV = tmp->getSymbolName();
	
	
	tmp->getSymbol()->engineObj = (void*)e;
	
	return e;
}



//FIXME-S2: string-constants --> data page + back-ref





EExternal* FrameworkBuilder::getExternalIdReference(ASTree* const _node, 
												const TType _expectedType)
{
#ifdef CEP_DEBUG
	printf("FrameworkBuilder::getExternalIdReference for %s\n", _node->asString());
#endif

	assert(_node->getType() == ASTree::PIdentifier);
	
	SymbolTable::Symbol* sym = _node->getSymbol();
	
	assert(sym);
	
	
	if(!sym->decl)
	{
		mErrorList->addError(	sym->lineNum, 
								sym->colNum,
								"%d: Variable '%s' is not declared",
								DMSG_AL_UNDECLARED_VAR,
								sym->name);
	
		return 0;
	}
	
	if(sym->decl->symbolType != SymbolTable::external_decl)
	{
		mErrorList->addError(	sym->lineNum, 
								sym->colNum,
								"%d: Identifier '%s' is not declared as an external variable", 
								DMSG_AL_EXT_VAR_NOT_DECLARED,
								sym->name);
		return 0;
	}
	
	assert(sym->decl->engineObj);

	EExternal* e = (EExternal*)sym->decl->engineObj;
	
	if(e->value.t != _expectedType)
	{
		mErrorList->addError(	sym->lineNum, 
								sym->colNum,
								"%d: Variable '%s' is expected to be of type '%s'.", 
								DMSG_RT_ID_UNEXPECTED_TYPE,
								sym->name,
								mSymTab->dataTypeAsString(_expectedType));
		return 0;
	}

	return e;
}




void FrameworkBuilder::init()
{
	mProgramBuilder = new ProgramBuilder();
}


void FrameworkBuilder::deinit()
{
	delete mProgramBuilder;
	//FIXME: clean up  more?
}



//FIXME: move to Engine Runtime framework
void FrameworkBuilder::resolveWindowValues(EComplexEvent* const _complex)
{
	if(_complex->recurringTimeExt)
		_complex->recurringTime = _complex->recurringTimeExt->value.v.timeVal;
	if(_complex->durationExt)
		_complex->duration = _complex->durationExt->value.v.durationVal;
	if(_complex->tupleCountExt)
		_complex->tupleCount = _complex->tupleCountExt->value.v.integerVal;
}


#if 0 //FIXME:REMOVE CODE
//FIXME: Move to some shared Tools / Engine Runtime framework
time_t FrameworkBuilder::durationLiteralAsTime(const TDurationLiteral& _dl) const
{
	return _dl.duration * durationUnitAsSeconds(_dl.units);

}

//FIXME: Move to some shared Tools / Engine Runtime framework
time_t FrameworkBuilder::durationUnitAsSeconds(const TDurationUnit& _durationUnit) const
{
	if(_durationUnit == d_years) return (time_t)31536000;
	if(_durationUnit == d_months) return (time_t)2628000; //based on month == 365/12 days.
	if(_durationUnit == d_weeks) return (time_t)604800;
	if(_durationUnit == d_days) return (time_t)86400;
	if(_durationUnit == d_hours) return (time_t)3600;
	if(_durationUnit == d_minutes) return (time_t)60;
	if(_durationUnit == d_seconds) return (time_t)1;
	
	return (time_t)0;
}
#endif 

#ifdef CEP_DEBUG

void FrameworkBuilder::print(EFramework*& _framework_)
{
	printExternals(_framework_);
	printEvents(_framework_);
	printComplexEvents(_framework_);
	printComplexEventFunctions(_framework_);
}

void FrameworkBuilder::printExternals(EFramework*& _framework_)
{
	EExternal* x = _framework_->externals;

	if(x)
	{
		printf("FrameworkBuilder::printExternals. \n");

		printf("oid address   value\n");
		printf("--- -------   -----\n");

		while(x)
		{
			printf("%3ld %p %s\n",
							x->oid, 
							x,
							x->value.asString());
				
			x = x->prev;
		}
	}
	else
	{
		printf("FrameworkBuilder::printExternals. (No Externals)\n");
	}
}


void FrameworkBuilder::printEvents(EFramework*& _framework_)
{
	EEvent* e = _framework_->events;

	if(e)
	{
		printf("FrameworkBuilder::printEvents.\n");

		printf("oid address   name      \n");
		printf("--- -------   ----------\n");

		while(e)
		{
			printf("%3ld %p %-10s\n",
				e->oid,
				e,
				e->name);
				
			printEventAttributes(e, 6);
				
			if(e->accept)
			{
				mProgramBuilder->print(e->accept);
			}

			e = e->prev;
		}
	}
	else
	{
		printf("FrameworkBuilder::printEvents. (No events)\n");
	}
}


void FrameworkBuilder::printEventAttributes(EEvent* _e, const int _indent)
{
	EEventAttribute* a = _e->attributes;

	if(a)
	{
		printf("FrameworkBuilder::printEventAttributes. \n");
		
		while(a)
		{
			printf("%s%3ld %p %-10s %s\n", 
										indent(_indent),
										a->oid,
										a,
										a->name,
										mSymTab->dataTypeAsString(a->type));
				
			a = a->prev;
		}
	}
	else
	{
		printf("FrameworkBuilder::printEventAttributes. (No Attributes)\n");
	}
}



void FrameworkBuilder::printPayload(EComplexEvent* _ce, const int _indent)
{
	EPayloadAttribute* pa = _ce->payloadAttributes;

	if(pa)
	{
		printf("FrameworkBuilder::printPayload.\n");
		while(pa)
		{
			if(pa->oid!=-1L)
			{
				printf("%s%3ld %p %-10s\n", indent(_indent),
										pa->oid,
										pa,
										pa->name);
			}
				
			if(pa->expr)
			{
				mProgramBuilder->print(pa->expr, 20);
			}
			
			pa = pa->prev;
		}
	}
	else
	{
		printf("FrameworkBuilder::printPayload. (No payload)\n");
	}
}


void FrameworkBuilder::printComplexEvents(EFramework*& _framework_)
{
	EComplexEvent* c = _framework_->complexEvents;
	
	if(c)
	{
		printf("FrameworkBuilder::printComplexEvents. \n");

		printf("oid  address   name       wintype value\n");
		printf("---  -------   ------- -----\n");

		while(c)
		{
			printf("%03ld %p %-10s %-7s %s\n",
				c->oid, 
				c,
				c->name, 
				windowTypeAsString(c->windowType),
				windowInfo(c));
				
			if(c->whereClause)
			{
				mProgramBuilder->print(c->whereClause, 20);
			}
			
			printPayload(c, 6);
			
			c = c->prev;
		}
	}
	else
	{
		printf("FrameworkBuilder::printComplexEvents. (None).\n");
	}

}


void FrameworkBuilder::printComplexEventFunctions(EFramework*& _framework_)
{
	EComplexEventFunction* ce = _framework_->complexEventFunctions;

	if(ce)
	{
		printf("FrameworkBuilder::printComplexEventFunctions.\n");

		printf("oid func\n");
		printf("--- ----\n");

		while(ce)
		{
			printf("%3ld %p %s [%ld %ld @%d]\n",
				ce->oid, 
				ce,
				complexEventFunctionAsString(ce->func),
				ce->address.ownerOid,
				ce->address.oid,
				ce->address.ref);

			ce = ce->prev;
		}
	}
	else
	{
		printf("FrameworkBuilder::printComplexEventFunctions. (None)\n");
	}
}


const char* FrameworkBuilder::complexEventFunctionAsString(const EComplexEventFunction::Function _func)
{
	if(_func == EComplexEventFunction::count) { return "count"; }
	if(_func == EComplexEventFunction::sum) { return "sum"; }
	if(_func == EComplexEventFunction::avg) { return "avg"; }
	if(_func == EComplexEventFunction::diff) { return "diff"; }
    if(_func == EComplexEventFunction::fft) { return "fft"; }

	return "??";
}


char* FrameworkBuilder::windowInfo(EComplexEvent* const _complex)
{
	static char fmt[256];
	const EComplexEvent::TWindowType winType = _complex->windowType;
	
	resolveWindowValues(_complex);
	
	if(winType == EComplexEvent::fixed)
		sprintf(fmt, "%02d:%02d:%02d %ld", 
			_complex->recurringTime.time.tm_hour,
			_complex->recurringTime.time.tm_min,
			_complex->recurringTime.time.tm_sec,
			_complex->duration);
	else if(winType == EComplexEvent::sliding)
		sprintf(fmt, "%ld", _complex->duration);
	else
		sprintf(fmt, "%d", _complex->tupleCount);
		
	return fmt;
}



const char* FrameworkBuilder::windowTypeAsString(const EComplexEvent::TWindowType _winType)
{
	if(_winType == EComplexEvent::fixed) { return "fixed"; }
	if(_winType == EComplexEvent::sliding) { return "sliding"; }
	if(_winType == EComplexEvent::tuple) { return "tuple"; }
	if(_winType == EComplexEvent::noWindow) { return "(none)"; }
	
	return "??";
}


const char* FrameworkBuilder::indent(const int _indent)
{
	int i = 0;
	while(i < _indent)
		mIndent[i++] = ' ';

	mIndent[i] = '\0';
	
	return mIndent;
}


#endif

