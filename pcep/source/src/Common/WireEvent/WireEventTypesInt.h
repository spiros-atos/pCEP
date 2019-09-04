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
		\brief Atos Reserved (internal) Wire Format Event Types
		
		Not for publication to anyone.
		
*/

#ifndef _ATOS_SOLCEP_COMMON_WIRE_EVENT_TYPES_INTERNAL_H_
#define _ATOS_SOLCEP_COMMON_WIRE_EVENT_TYPES_INTERNAL_H_

struct WireEventTypesInt
{
	
	//@{
	//! \brief Reserved Event Types. The "public" event types are in public/PluginDev
	static const unsigned char EVT_TYPE_CS = 0x81;	// Atos Reserved
	static const unsigned char EVT_TYPE_TICK = 0x80; // Atos Reserved
	static const unsigned char EVT_TYPE_SHUTDOWN = 0x99; //Atos Reserved
//RP: 04022015 USED TO CHANGE THE VALUE OF A EXTERNAL VARIABLE
        static const unsigned char EVT_TYPE_CHANGE_EXTERNALVAR = 0x82; //Atos Reserved
	//@}
	
};

#endif 