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

#include "SocketUDPServer.h"
///#include <netinet/in.h>
///#include <arpa/inet.h>
#include "Tracer.h"
#include "Configuration/IniFileSection.h"
///#include <stdlib.h>
#include <string.h>

#ifndef _WIN32
#include <poll.h>
#else
#include <WinSock2.h>
#endif

namespace solcep {

    int mSocket;
    int mAddressLength, mAddrEmiterLenght;
    struct sockaddr_in mServerAddress;
    struct sockaddr_in clientAddr;
    int mEmitterSocket;
    struct sockaddr_in mEmitterServerAddress;
    
///    SocketUDPServer::SocketUDPServer() {
    SocketUDPServer::SocketUDPServer(IniFileSection _iniFileSec) {
    //init the socket;
                
        mListeningPort= _iniFileSec.portUDPCollect;
        mEmitterPort= _iniFileSec.portUDPPublish;
        
        mPubSocket = createSocket();
        if (mPubSocket == -1)
            solcep::Trace().Err() << "Unable to create the UDP Socket!!" << std::endl;
        else  {
            std::ostringstream sckt; 
            sckt << mListeningPort;
            INFO() << "Listening at UDP Socket port: "<< sckt.str() << std::endl;

        }
        
#ifndef _WIN32
        bzero(&(mServerAddress.sin_zero), sizeof(mServerAddress.sin_zero));
        mServerAddress.sin_family = AF_INET;
        mServerAddress.sin_port = htons(mListeningPort);
#else
		memset(&(mServerAddress.sin_zero), '\0', sizeof(mServerAddress.sin_zero));
		mServerAddress.sin_family = AF_INET;
        mServerAddress.sin_port = ::htons(mListeningPort);
#endif

        mServerAddress.sin_addr.s_addr = INADDR_ANY;
    
        mAddressLength = sizeof(mServerAddress);

        int resBind = bind(mPubSocket, (struct sockaddr *)&mServerAddress, mAddressLength);
        if (resBind == -1) {
            solcep::Trace().Err() << "Unable to bind the Collector UDP Socket!!" << std::endl;
        }


    };
    
    SocketUDPServer::~SocketUDPServer() { 
    
    mSocket = 0;
    mAddressLength = 0;
    mServerAddress = {};
    clientAddr = {};
    
    shutdown(mPubSocket, 2);
    shutdown(mEmitterSocket, 2);
    }

    
    int SocketUDPServer::createSocket() {
            
      if((mSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        {
            return -1;
        }

      return mSocket;
     
    }

    int SocketUDPServer::bindSocket(const char* connString) {
     
      	if(bind(mSocket, (struct sockaddr*)&mServerAddress, mAddressLength) == -1)
	{
		return -1;
	}

        return 0;
    }

/*
///    std::string SocketUDPServer::readEventFromSocketUDP() {
        char* SocketUDPServer::readEventFromSocketUDP() {
        
        int bytesRead = 0;
///        lineRead.clear();
        mPacket = (char*)malloc(mMaxPacketSize + 1);

      struct pollfd fds[1];             
///      struct pollfd* fds = (struct pollfd*)malloc(sizeof(struct pollfd));
      
      int timeout= 10;                   
      
      fds[0].fd = mPubSocket;
      fds[0].events = 0;
      fds[0].events |= POLLIN;
      bzero(mPacket, mMaxPacketSize);
          
      if ( poll(fds, 1, timeout) != 0 ) {   ///era == 0 
///         printf("Select has timed out...\n");
///       }

///          bzero(mPacket, mMaxPacketSize);
            
//            while (1) {
                bytesRead = recvfrom(mPubSocket, mPacket, mMaxPacketSize, 
                                    0,
                                    (struct sockaddr*)&mEmitterServerAddress,    //used in the client socket
                                    (socklen_t*)&mAddressLength);

///                        if(bytesRead > 0)
///                        {
///                            std::string msg(mPacket, mPacket + bytesRead);
///                            lineRead = msg;
///                        }
                
///                }
//            }
      }
///      delete fds;
///      free(mPacket);
      
///      return lineRead;
      return mPacket;

    }
*/    


///is socket client of external application
   
///     int SocketUDPServer::sendData(const char* data, int len) {
         int SocketUDPServer::sendData(char* data, int len) {
     
///     int mEmitterPort=50000;

     if (mEmitterSocket < 1)
        mEmitterSocket = socket(AF_INET, SOCK_DGRAM, 0);

    if(mEmitterSocket == -1)
    {
	return 0;
    }
    
#ifndef _WIN32
	bzero(&(mEmitterServerAddress.sin_zero), sizeof(mEmitterServerAddress.sin_zero));
#else
	memset(&(mEmitterServerAddress.sin_zero), '\0', sizeof(mEmitterServerAddress.sin_zero));
#endif

    mEmitterServerAddress.sin_family = AF_INET;

    mEmitterServerAddress.sin_port = ::htons(mEmitterPort);

    if (len < 1)
         return -1;

     int res = sendto(mEmitterSocket,
                        data, 
			len,
			0,
			(struct sockaddr *)&mEmitterServerAddress,
			sizeof(mEmitterServerAddress));
     
     	if(res <= 0)
		return -1;
     
        return 0;
        
 }
 
    
}
