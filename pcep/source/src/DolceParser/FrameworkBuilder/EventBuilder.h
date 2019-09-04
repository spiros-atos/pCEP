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
#ifndef _SOL_CEP_EVENT_BUILDER_H_
#define _SOL_CEP_EVENT_BUILDER_H_


///#include <stdio.h>

#include "../ScriptCompiler/ASTree.h"


class FrameworkBuilder;
struct EEvent;
struct EComplexEvent;
struct EEventAttribute;
struct ProgramBuilder;

class EventBuilder
{
public:
	EventBuilder(	FrameworkBuilder* const _eb,
					EEvent* _event_);
					
	EventBuilder(	FrameworkBuilder* const _eb);

	~EventBuilder();
	
							
	int build(ASTree* const _node);
	

	/*!
		\brief Relates a complex event to the event, only if it's 
			not already related.
		\param _complex The complex event to add
		\returns OK
	*/
	int addComplexEventRef(EComplexEvent* const _complex);
	
	/*!
		Returns a built event structure.
		Caller is responsible for releasing memory
	*/
	EEvent* finalize();
	
protected:
	
	EventBuilder();
	
	EventBuilder(const EventBuilder& rhs) : mEvent(0), mFrameworkBuilder(0)	{ }
	
	EventBuilder& operator= (const EventBuilder& rhs)	{ return *this; }


	void init();
	void deinit();

	int buildEventAttributes(ASTree* const _node);
	
private:

	EEvent* mEvent;
	FrameworkBuilder* const mFrameworkBuilder;
	
#ifdef CEP_DEBUG

	void printEventAttributes();

#endif

};



#endif


