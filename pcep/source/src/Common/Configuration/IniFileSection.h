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


#ifndef INIFILESECTION_H
#define	INIFILESECTION_H

#include <string>


struct IniFileSection
{
#ifndef _WIN32
    std::string confFile = "./confFile.ini";
#else
    std::string confFile = "../source/confFile.ini";
#endif

    int portUDPCollect = 29201;
    int portUDPPublish = 50000;
    
    int portMQTT = 1883;
    std::string hostMQTT = "localhost";
    std::string topicMQTTCollect = "topicInputDefault";
    std::string topicMQTTPublish = "topicOutputDefault";
    int mqttCollectQoS= 1;
    int mqttPublishQoS= 1;
    int mqttKeepAlive= 60;
    std::string mqttClientId = "bcep";
    std::string dolceSpec = "detect.dolce";
    std::string userMQTT = "";
    std::string passwordMQTT = "";
    std::string logger="stdout";
    std::string loggerFile="";

    int logLevel=2;
};

#endif	/* INIFILESECTION_H */

