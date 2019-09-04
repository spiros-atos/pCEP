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
	See the "SOL/CEP Plugin Developers Guide" (SOL-CEP-0009) 
	for more information.
*/

#ifndef _ATOS_SOLCEP_PUB_PLUGIN_H_
#define _ATOS_SOLCEP_PUB_PLUGIN_H_



class CEPPlugin
{
public:

	CEPPlugin() {}
	
	virtual ~CEPPlugin() {}
	
	/*!
		\brief Called by clients in order to set parameters
		on the plugin.
		Optionally implemented by the plugin provider
		
		\note Called only once before load()
		
	*/
	virtual int setParam(	const char* _id, 
							const char* _value)
	{
		return PLUGIN_OK;
	}


	/*!
		\brief Called by SOL/CEP when the plugin is loaded
		Must be implemented by the plugin provider. 
		\returns 0 if the plugin has been initialized correctly. If not,
			a code defined by the plugin provider is returned.
	*/
	virtual int load() = 0;
	
	/*!
		\brief Called by SOL/CEP when the plugin is unloaded
		Must be implemented by the plugin provider. 
	*/
	virtual void unload	() = 0;

	
	
	/*!
		\brief No err.
	*/
	static const int PLUGIN_OK = 0; // remove PLUGIN_ prefix. Rename this class "CEPPlugin"
	
	
	/*!
		\brief An incorrect or non-existing parameter was 
			provided
	*/
	static const int PLUGIN_ERR_PARAM = 1;
	
	
	/*!
		\brief This error means that the plugin is 
			not completely configured, e.g. not 
			all paramters were provided or are wrong
	*/
	static const int PLUGIN_CONFIG_ERR = 2;
	

	//!@{
	//! Atos internal plugin error codes. Plugin provider error
	//! codes must start at 100.
	static const int LOG_FATAL = 1;
	static const int LOG_ERROR = 2;
	static const int LOG_WARNING = 3;
	static const int LOG_INFO = 4;
	//@}
	
protected:

	CEPPlugin(const CEPPlugin& rhs)	{ }
	
	CEPPlugin& operator= (const CEPPlugin& rhs)	{ return* this; }		

	
private:


};

// the types of the class factories
typedef CEPPlugin* create_t();
typedef void destroy_t(CEPPlugin* _plugin);



#endif


