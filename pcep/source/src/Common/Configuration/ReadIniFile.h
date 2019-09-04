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


#ifndef READINIFILE_H
#define	READINIFILE_H

///#include <string>
#include "IniFileSection.h"
#include <fstream>

#define MQTT_MAXIMUM_CLIENTID_LENGTH 23

///using namespace std;

class ReadIniFile {

public:
    
    ReadIniFile(std::string logFile);
    
    ReadIniFile(const ReadIniFile& orig);
    
    virtual ~ReadIniFile();
    
    
    std::string logFileName="./confFile.ini";
    
    IniFileSection readIniFileUDPSection();
    IniFileSection readIniFileMQTTSection();
    void readIniFileLoggerSection(IniFileSection *structIniFile);
    IniFileSection readFile();

private:
    
    std::string trim(const std::string & s);
    std::string makelower(const std::string & s);
    std::string value_for_key(const std::string & s, const std::string & k);
    bool getini(const std::string & inifile, const std::string & section, const std::string & key, std::string & value);
};

#endif	/* READINIFILE_H */

