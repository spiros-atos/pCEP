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
#ifndef _SOL_CEP_COMPLEX_EVENT_BUILDER_H_
#define _SOL_CEP_COMPLEX_EVENT_BUILDER_H_


///#include <stdio.h>

#include "../ScriptCompiler/ASTree.h"
#include "../../ComplexEventDetector/Engine/Types/EComplexEventFunction.h"

class ProgramBuilder;
class FrameworkBuilder;
struct EComplexEvent;
struct EPayloadAttribute;
struct EEventAttribute;
struct EDetectionTree;
struct EFramework;

class ComplexEventBuilder
{
public:
	ComplexEventBuilder(FrameworkBuilder* const _fb,
						EComplexEvent* _complex);

	ComplexEventBuilder(FrameworkBuilder* const _fb);

	~ComplexEventBuilder();
	
	int build(	EFramework*& _framework_,
				ASTree* const _node);
	
	/*
		Returns a built complex event structure.
		Caller is responsible for releasing memory
	*/
	EComplexEvent* finalize();
	

protected:
	
	ComplexEventBuilder();
	
	ComplexEventBuilder(const ComplexEventBuilder& rhs) : mFrameworkBuilder(0) {}
	
	ComplexEventBuilder& operator= (const ComplexEventBuilder& rhs)	{ return *this; }


	int buildPayloadAttributes(ASTree* const _node);
	
	int buildComplexEventFunctions(	EFramework*& _framework_,
									ASTree* const _node);
									
	int buildPrograms(ASTree* const _node);
	
	int resolveWindow(ASTree* const _node);

	int resolveGroupAttribute(ASTree* const _node);
	
	TValue* resolveIdValue(ASTree* const _node, const TType _expectedType);
	
	EDetectionTree* buildDetectionTree(ASTree* const _node);
	EDetectionTree* buildDetectionTreeNode(ASTree* const _node);

	int resolveDuration(ASTree* const _node);
	int resolveRecurringTime(ASTree* const _node);
	int resolveTupleCount(ASTree* const _node);
	
private:

	FrameworkBuilder* const mFrameworkBuilder;
	EComplexEvent* mComplex;
	
	ProgramBuilder* mProgramBuilder;
	
	EComplexEventFunction::Function nodeAsFunction(ASTree* const _node);


#ifdef CEP_DEBUG
	void dumpPayloadAttributes(EComplexEvent* const _complex);
#endif

};



#endif


