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
	Engine
	
*/
#ifndef _SOL_CEP_ENGINE_BUILDER_H_
#define _SOL_CEP_ENGINE_BUILDER_H_


///#include <stdio.h>


#include "../../ComplexEventDetector/Engine/Types/EComplexEvent.h"
#include "../../ComplexEventDetector/Engine/Types/EComplexEventFunction.h"
#include "../ScriptCompiler/ASTree.h"


struct Engine;
struct EEvent;
struct EFramework;
struct EComplexEvent;
struct EEventAttribute;
struct EDetectionTree;
struct EEventPeriod;
struct EPayloadAttribute;
struct EExternal;




class Compiler;
class SymbolTable;
class ProgramBuilder;
class ErrorList;


class FrameworkBuilder
{
public:
	FrameworkBuilder(	Compiler* const _c, 
					ErrorList* const _el);
					
	~FrameworkBuilder();
	
	/*!
		\brief Builds a new Execution Framework
		\param res_ The result, which is OK on success or another
			code if there's an error.
		\returns An instance of the execution framework or 0 in case
			of an error. Caller must delete the instance when done.
	*/
	EFramework* build(int& res_);

	EEvent* newEvent(const char* _name);
	EComplexEvent* newComplexEvent(const char* _name);
	
	int newAttributeForEvent(const char* _eventName,
							 EEventAttribute* const _attr);
	
							
	int build(ASTree* const _node, const int _pass);
	

	ProgramBuilder* const getProgramBuilder() const { return mProgramBuilder; }
	SymbolTable* const getSymbolTable() const { return mSymTab; }
	ErrorList* const getErrorList() const { return mErrorList; }
	EExternal* getExternalIdReference(ASTree* const _node, const TType _expectedType);
	
	long getOidFromIdNode(ASTree* const _idNode);
	
	long getNextOid() { return ++mOid; }
	
//FIXME: move to Engine 
long findEvent(const char* _name);
void resolveWindowValues(EComplexEvent* const _complex);

/*FIXmE: REMOVE
time_t durationLiteralAsTime(const TDurationLiteral& _dl) const;
time_t durationUnitAsSeconds(const TDurationUnit& _durationUnit) const;
*/


protected:
	
	FrameworkBuilder();
	
	FrameworkBuilder(const FrameworkBuilder& rhs) : mCompiler(0), mSymTab(0), mErrorList(0)	{}
	
	FrameworkBuilder& operator= (const FrameworkBuilder& rhs)	{ return *this; }

// Structs that end with "s" are a collection
//
	void init();
	void deinit();

	
	int buildExternals(EFramework*& _framework_);
	int buildEvents(EFramework*& _framework_);
	int buildComplexEvents(EFramework*& _framework_);

	EExternal* createExternal(ASTree* const _node);
	
private:

	Compiler* const mCompiler;
	SymbolTable* mSymTab;
	ErrorList* const mErrorList;
	
	ProgramBuilder* mProgramBuilder;
	
	long mOid;

	

#ifdef CEP_DEBUG

		
	void print(EFramework*& _framework_);
	void printExternals(EFramework*& _framework_);
	void printEvents(EFramework*& _framework_);
	void printComplexEvents(EFramework*& _framework_);
	void printComplexEventFunctions(EFramework*& _framework_);
	void printEventAttributes(EEvent* _e, const int _indent = 0);
	void printPayload(EComplexEvent* _ce, const int _indent = 0);
	const char* indent(const int _indent);

	char* windowInfo(EComplexEvent* const _complex);
	const char* windowTypeAsString(const EComplexEvent::TWindowType _winType);
	const char* complexEventFunctionAsString(const EComplexEventFunction::Function _func);
	
	char mIndent[64];

#endif

};



#endif


