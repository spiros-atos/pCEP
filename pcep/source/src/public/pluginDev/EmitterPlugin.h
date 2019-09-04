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

#ifndef _ATOS_SOLCEP_PUB_EVENT_EMITTER_PLUGIN_H_
#define _ATOS_SOLCEP_PUB_EVENT_EMITTER_PLUGIN_H_

#include "CEPPlugin.h"

class EmitterPlugin : public CEPPlugin
{
public:

	EmitterPlugin() {}
	
	virtual ~EmitterPlugin() {}
	
															
	/*!
		\brief Emits an event.
			To be implemented by the plugin provider.
			
		\param data The event data
		\param size The event size
			
		\returns PLUGIN_OK if emitted succesfully.
	*/
	virtual int emit(	const unsigned char* _data, 
						const unsigned int _size) = 0;

protected:

	EmitterPlugin(const EmitterPlugin& rhs)	{ }
	
	EmitterPlugin& operator= (const EmitterPlugin& rhs)	{ return* this; }		

private:


};





#endif


