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


///#include <string>
#ifndef _WIN32
#include <netinet/in.h>
#endif

#include "Configuration/IniFileSection.h"

#ifndef SOCKETUDPSERVER_H
#define	SOCKETUDPSERVER_H

namespace solcep {
    
  class SocketUDPServer
    {
        public:
            
            SocketUDPServer();
            SocketUDPServer(IniFileSection);
            virtual ~SocketUDPServer();
            
///            std::string readEventFromSocketUDP();
            char* readEventFromSocketUDP();
///            int sendData(const char*, int);
              int sendData(char*, int);
            
        protected:
            static int createSocket();
            static int bindSocket(const char*);
            
            unsigned short int mListeningPort;//= 29654;
            unsigned short int mEmitterPort;//= 50000;
            static const int mMaxPacketSize=1024;

            char* mPacket;
            int mPubSocket;
            
///            std::string lineRead;
    
    };
  
  }
  
#endif	/* SOCKETUDPSERVER_H */

