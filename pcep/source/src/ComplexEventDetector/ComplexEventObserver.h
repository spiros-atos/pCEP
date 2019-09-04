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
	Abstract interface that observes the generation of complex events.
	
*/
#ifndef _SOL_CEP_COMPLEX_EVENT_OBSERVER_H_
#define _SOL_CEP_COMPLEX_EVENT_OBSERVER_H_

struct EComplexEvent;
struct ECapturedEvent;

class ComplexEventObserver
{
public:
	
	/*!
		\brief Called by the observed object when there is a complex
			event
		\param _complexEvent The Complex Event that is raised.
		\param _origEvent The event that originated the complex event.
			If 0, the complex event was caused by time expiry
	*/
	virtual void raise(EComplexEvent* _complexEvent, ECapturedEvent* _origEvent = 0) = 0;

	
protected:
	
	ComplexEventObserver() {}
					
	virtual ~ComplexEventObserver() {}


private:

	ComplexEventObserver(const ComplexEventObserver& rhs) 
	{ }
	
	ComplexEventObserver& operator= (const ComplexEventObserver& rhs)	{ return *this; }


};



#endif


