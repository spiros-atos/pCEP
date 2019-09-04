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
		\brief Public Wire Format Event Types
		
*/

#ifndef _ATOS_SOLCEP_PUB_WIRE_EVENT_TYPES_H_
#define _ATOS_SOLCEP_PUB_WIRE_EVENT_TYPES_H_

struct WireEventTypes
{

	//@{
	//! \brief Public Event Types
	static const unsigned char EVT_TYPE_NORMAL = 0x00;
	static const unsigned char EVT_TYPE_EXT = 0x01;
	static const unsigned char EVT_TYPE_COMPLEX = 0x02;
        static const unsigned char EVT_TYPE_INJECTED_EVENT = 0x03;
	//@}
	
	//! Maximum id of public event.
	static const unsigned char EVT_TYPE_MAX = 0x7F;
};

#endif 