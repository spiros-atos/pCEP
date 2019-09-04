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

#ifndef _ATOS_SOLCEP_PUB_EVENT_LISTENER_PLUGIN_H_
#define _ATOS_SOLCEP_PUB_EVENT_LISTENER_PLUGIN_H_

#include "CEPPlugin.h"

class ListenerPlugin : public CEPPlugin
{
public:

	ListenerPlugin() {}
	
	virtual ~ListenerPlugin() {}
	
	/*!
		\brief
		Prototype for callback function. Implemented by SOL/CEP.
		Must be called to present data to SOL/CEP
		
		\param _data The data to send 
		\param _size The size of the data
		\param _reserved MUST be the parameter supplied in _reserved by start().
	*/
	typedef int (ListenerPlugin::*TDataCallbackFunc)(const unsigned char* _data,
														  const unsigned int _size,
														  void* _reserved);
															
	/*!
		\brief 
			Must be implemented by plugin provider.
			
			Called by SOL/CEP when it wants to start listening. 
			It is expected a continuous loop that calls
			send() each time data is received.
			
		\param _dataCallback	Provided by SOL/CEP when invoked.
		\param _reserved 		Provided by SOL/CEP when invoked. 
								Atos reserved.
			
		\return PLUGIN_OK when finished or other error code when
			it cannot be started.
	*/
	virtual int start(	TDataCallbackFunc _dataCallback, 
						void* _reserved) = 0;

	/*!
		\brief 
			Must be implemented by plugin provider
			
			Called by SOL/CEP when it wants to stop	listening. 
			
		\return PLUGIN_OK.
	*/
	virtual int stop() = 0;
	
	
	/*!
		\brief Provided by SOL/CEP
	*/
	int callback(const unsigned char*, unsigned int, void*);

protected:

	ListenerPlugin(const ListenerPlugin& rhs)	{ }
	
	ListenerPlugin& operator= (const ListenerPlugin& rhs)	{ return* this; }		

private:


};





#endif


