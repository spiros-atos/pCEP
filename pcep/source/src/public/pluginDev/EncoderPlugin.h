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
	See the "SOL/CEP Plugin Developers Guide" (SOL-CEP-0009) and
	the "SOL/CEP Event Wire Protocol" (SOL-CEP-0007) for more information.
*/

#ifndef _ATOS_SOLCEP_PUB_ENCODER_PLUGIN_H_
#define _ATOS_SOLCEP_PUB_ENCODER_PLUGIN_H_

#include "CEPPlugin.h"
#include <string>

class EncoderPlugin : public CEPPlugin
{
public:

	EncoderPlugin() {}
	
	virtual ~EncoderPlugin() {}
	
	/*!
            \brief Must be implemented by plug-in provider.
                   Called by SOL/CEP when it wants to encode something. 

            \param message	Provided by SOL/CEP when invoked. Memory.

            \return encoded message.
	*/
	virtual std::string encode(std::string message) = 0;

protected:

	EncoderPlugin(const EncoderPlugin& rhs)	{ }
	
	EncoderPlugin& operator= (const EncoderPlugin& rhs)	{ return* this; }		

private:


};





#endif


