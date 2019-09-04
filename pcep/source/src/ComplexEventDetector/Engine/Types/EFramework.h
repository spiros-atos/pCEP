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
	Core Execution Framework
	
	Frameworks are built by FrameworkBuilder and 
	executed by the ComplexEventDetector Engine.
	
*/
#ifndef _SOL_CEP_ENGINE_TYPES_E_FRAMEWORK_H_
#define _SOL_CEP_ENGINE_TYPES_E_FRAMEWORK_H_

#include "EEvent.h"
#include "EEventAttribute.h"

#include "EEventAttributeValue.h"

#include "ECapturedEvent.h"
#include "ECapturedEventPool.h"
#include "EComplexEvent.h"
#include "EComplexEventFunction.h"
#include "EDetectionTree.h"
#include "EEventPeriod.h"
#include "EExternal.h"
//#include "EPayloadAttribute.h"
#include "EPayloadAttribute.h"
#include "EProgram.h"
#include "../../../DolceParser/ScriptCompiler/SymbolTable.h"
#include "../../../Common/BCEPManager.h"


class BCEPManager;

struct EFramework
{
	EFramework() : complexEvents(0)
			, complexEventFunctions(0)
			, events(0)
			, externals(0)
	{}


	EComplexEvent* complexEvents;
	EComplexEventFunction* complexEventFunctions;
	EEvent* events;
	EExternal* externals;
        SymbolTable* mSymTab;
///RP: to restart
        BCEPManager* bcepMngr;
        
};


#endif
