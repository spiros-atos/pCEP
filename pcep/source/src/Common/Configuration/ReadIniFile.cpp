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


#include <string.h>

#include "ReadIniFile.h"
#include "../Tracer.h"

#ifndef _WIN32
#include <unistd.h>
#else
#include <process.h>
#endif


ReadIniFile::ReadIniFile(std::string logFile) {
    
    if (logFile.length()>4){
        logFileName=logFile;
    }
    
    IniFileSection readIniFileUDPSection();
    IniFileSection readIniFileMQTTSection();
    IniFileSection readIniFileLoggerSection();
    IniFileSection readFile();

}
/*
ReadIniFile::ReadIniFile(const ReadIniFile& orig) {
}
*/ 
ReadIniFile::~ReadIniFile() {
    
}

std::string ReadIniFile::trim(const std::string & s)
{
    size_t l = s.length();
    if (l == 0)
        return s;
    size_t b = s.find_first_not_of(" \t\r\n\0");
    if (b == std::string::npos)
        b = 0;
    size_t e = s.find_last_not_of(" \t\r\n\0");
    if (e == std::string::npos)
        return s.substr(b);
    return s.substr(b, e-b+1);
}


std::string ReadIniFile::makelower(const std::string & s)
{
    std::string str = s;
    for( std::string::iterator i = str.begin(); i != str.end(); ++i)
        if (tolower(*i) != (int)*i)
            *i = (char)tolower(*i);
    return str;
}

std::string ReadIniFile::value_for_key(const std::string & s, const std::string & k)
{
    size_t p = s.find('=');
    if (p == std::string::npos || trim(makelower(s.substr(0,p))) != k)
        return "";
    p = s.find_first_not_of(" \t\n\r\0", p+1);
    // check for empty value and return space char
    if (p == std::string::npos)
        return " ";

    return s.substr(p);
}


bool ReadIniFile::getini(const std::string & inifile, const std::string & section, const std::string & key, std::string & value)
{
    std::string s, k;
    std::string sec = std::string("[") + makelower(section) + "]";
    std::ifstream file(inifile.c_str());
    if (!file) {
       INFO()<<"Configuration file could not be opened; the default sockets will be used!"<<std::endl;
       return 0;
    }
    k = trim(makelower(key));
    bool found_section  = 0;
    while (std::getline(file, s))
    {
       if (! found_section)
       {
          if (trim(makelower(s)) == sec)
              found_section = 1;
          continue;
       }
       // check for next section
       s = trim(s);
       int l;
       if ((l = s.length()) > 1 && s[0] == '[' && s[l-1] == ']')
           // break loop if next section begins
           break;
       value = value_for_key(trim(s), k);
       if (value.length())
       {
           // close file before return
           file.close();
           return 1;
       }
    }
    
    file.close();
    
    if (found_section) {
        INFO()<<"Key not found: "<<key;
        return 0;
    }
    else {
        INFO()<<"Section not found: "<<section;
        return 0;
    }
    return 1;
                
}



IniFileSection ReadIniFile::readIniFileUDPSection() {
    
    IniFileSection structIniFile;
    std::string updSection= "UPDPorts", aKey= "portUDPCollect", aValue;
    std::string dolceSection= "DOLCE";
    
    if (getini(logFileName, updSection, aKey, aValue)) {
        std::stringstream(aValue) >> structIniFile.portUDPCollect;
        aKey = "portUDPPublish";
        if (getini(logFileName, updSection, aKey, aValue))
            std::stringstream(aValue) >> structIniFile.portUDPPublish;
        
        aKey = "dolceSpec";
        if (getini(logFileName, dolceSection, aKey, aValue))
            std::stringstream(aValue) >> structIniFile.dolceSpec;
    }
    
    return structIniFile;
}

IniFileSection ReadIniFile::readIniFileMQTTSection() {
    
    IniFileSection structIniFile;
    std::string section= "MQTT", aKey= "portMQTT", aValue;
    std::string dolceSection= "DOLCE";
    
    if (getini(logFileName, section, aKey, aValue)) {
        std::stringstream(aValue) >> structIniFile.portMQTT;
        
        aKey = "hostMQTT";
        if (getini(logFileName, section, aKey, aValue))
            std::stringstream(aValue) >> structIniFile.hostMQTT;
        
        aKey = "topicMQTTCollect";
        if (getini(logFileName, section, aKey, aValue))
            std::stringstream(aValue) >> structIniFile.topicMQTTCollect; 
        
        aKey = "mqttCollectQoS";
        if (getini(logFileName, section, aKey, aValue))
        {
            std::stringstream(aValue) >> structIniFile.mqttCollectQoS;
            if(structIniFile.mqttCollectQoS<0 || structIniFile.mqttCollectQoS >2)
			{
            	std::cerr <<"ERROR: Invalid mqttCollectQoS. Mqtt only accepts 0,1 or 2 for the QoS"<<std::endl;
				exit(-1);
			}
        }


        aKey = "topicMQTTPublish";
        if (getini(logFileName, section, aKey, aValue))
            std::stringstream(aValue) >> structIniFile.topicMQTTPublish;

        aKey = "mqttPublishQoS";
        if (getini(logFileName, section, aKey, aValue))
        {
            std::stringstream(aValue) >> structIniFile.mqttPublishQoS;
            if(structIniFile.mqttPublishQoS<0 || structIniFile.mqttPublishQoS >2)
			{
            	std::cerr <<"ERROR: Invalid mqttPublishQoS. Mqtt only accepts 0,1 or 2 for the QoS"<<std::endl;
				exit(-1);
			}
        }

        aKey = "keepAlive";
        if (getini(logFileName, section, aKey, aValue))
            std::stringstream(aValue) >> structIniFile.mqttKeepAlive;

        aKey = "mqttClientId";
        if (getini(logFileName, section, aKey, aValue))
            std::stringstream(aValue) >> structIniFile.mqttClientId;
        else
        {
        	// If the mqtt client id is not provided in the configuration file.
        	// The default value is bcep-<uui>

        	srand(time(NULL));
        	int rand_number= rand();
        	char uuid[MQTT_MAXIMUM_CLIENTID_LENGTH];

			snprintf(uuid, MQTT_MAXIMUM_CLIENTID_LENGTH - 1, "%s-%d-%d",
				structIniFile.mqttClientId.c_str(),
#ifndef _WIN32
                getpid(), 
#else
                _getpid(), 
#endif
                rand_number);
        	structIniFile.mqttClientId=uuid;
        }
        
        aKey = "dolceSpec";
        if (getini(logFileName, dolceSection, aKey, aValue))
            std::stringstream(aValue) >> structIniFile.dolceSpec;        

        aKey = "userMQTT";
        if (getini(logFileName, section, aKey, aValue))
            std::stringstream(aValue) >> structIniFile.userMQTT;

        aKey = "passwordMQTT";
        if (getini(logFileName, section, aKey, aValue))
            std::stringstream(aValue) >> structIniFile.passwordMQTT;

    }
    return structIniFile;
}

 void ReadIniFile::readIniFileLoggerSection(IniFileSection *structIniFile) {

    std::string loggerSection= "LOGGER", aKey= "rootLogger", aValue;

    if (getini(logFileName, loggerSection, aKey, aValue)) {
        std::stringstream(aValue) >> structIniFile->logger;
        aKey = "logger.file";
        if (getini(logFileName, loggerSection, aKey, aValue))
            std::stringstream(aValue) >> structIniFile->loggerFile;

        aKey = "logger.level";
        if (getini(logFileName, loggerSection, aKey, aValue))
        {
        	if(aValue.compare("ERR")==0)
        	{
        		structIniFile->logLevel=0;
        	}
        	else if(aValue.compare("WARN")==0)
        	{
        		structIniFile->logLevel=1;
        	}
            else if (aValue.compare("INFO") == 0)
            {
                structIniFile->logLevel = 2;
            }
            else if (aValue.compare("DEBUG") == 0)
            {
        		structIniFile->logLevel=3;
        	}
            else
            {
                structIniFile->logLevel = 1;
            }
        }
    }

}

IniFileSection ReadIniFile::readFile(){
    IniFileSection structIniFile;
#ifndef MQTT
    structIniFile = readIniFileUDPSection();
#endif

#ifdef MQTT
    structIniFile = readIniFileMQTTSection();
#endif
    readIniFileLoggerSection(&structIniFile);
    return structIniFile;
}
