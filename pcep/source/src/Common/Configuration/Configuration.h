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
    Conventions for file<path | dir | name>:
            dir = directory, EXCLUDING trailing file separator. Example: /var/log
            name = name WITHOUT dir. Example: cep.log
            path = [ dir + file separator ] + name. Examples: /var/log/cep.log, cep.log


    solcep [rootpath] 

    Configurable items:
    These must be done at startup. This file contains the master config settings. Cannot be edited.

            ConfigFileLocation
            administration interface port
            location of the log file
            location of the dolce file
            name of the dolce file	
*/

#pragma once

///#include <string>
#include "ConfigTypes.h"

namespace solcep {

class Configuration
{
public:		
	//! Root path where SOL/CEP executes.
	std::string serverRoot;
	
	Config::Coordinator coordinator; 
	Config::ComplexEventDetector complexEventDetector;
	Config::EventCollector eventCollector;
        Config::ComplexEventPublisher complexEventPublisher;

	// App Helpers
	Config::ComplexEventDetector* findComplexEventDetector(std::string id) const;
	
protected:

	//@{
	//! Internal ports (ZMQ). 
	static const int INT_CEP_PORT = 5555;	// The Complex Event Processor listens here.
	//@}
};

}


