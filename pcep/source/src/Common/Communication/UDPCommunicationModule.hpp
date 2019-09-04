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


#ifndef UDPCOMMUNICATIONMODULE_HPP
#define	UDPCOMMUNICATIONMODULE_HPP


#include "../Configuration/ReadIniFile.h"

#ifndef _WIN32
#include <netinet/in.h>
#include <poll.h>
#else
#include <WinSock2.h>
#include <WS2tcpip.h>	//socklen_t
#include "Trace.h"
#endif

namespace solcep {

    class UDPCommunicationModule : public CommunicationModule {
    public:
        UDPCommunicationModule(IniFileSection iniFileSection);
        ~UDPCommunicationModule();
        virtual char * receiveData();
        virtual void sendData(const char * message);
    protected:
        static int createSocket();
        static int bindSocket(const char*);

        unsigned short int mListeningPort; //= 29654;
        unsigned short int mEmitterPort; //= 50000;
        static const int mMaxPacketSize = 1024;

        char* mPacket;
        int mPubSocket;
    };

    int tsocket;
    int taddressLength, taddrEmiterLenght;
    struct sockaddr_in tServerAddress;
    struct sockaddr_in tclientAddr;
    int tEmitterSocket;
    struct sockaddr_in tEmitterServerAddress;

    UDPCommunicationModule::UDPCommunicationModule(IniFileSection iniFileSection) {
        
        IniFileSection _iniFileSec = iniFileSection;

        mListeningPort = _iniFileSec.portUDPCollect;
        mEmitterPort = _iniFileSec.portUDPPublish;

        mPubSocket = createSocket();
        if (mPubSocket == -1) {
            solcep::Trace().Err() << "Unable to create the UDP Socket!!" << std::endl;
        }
        else {
            std::ostringstream sckt;
            sckt << mListeningPort;
            INFO() << "Listening at UDP Socket port: " << sckt.str() << std::endl;
        }

#ifndef _WIN32
        bzero(&(tServerAddress.sin_zero), sizeof(tServerAddress.sin_zero));
#else
		memset(&(tServerAddress.sin_zero), '\0', sizeof (tServerAddress.sin_zero));
#endif
        tServerAddress.sin_family = AF_INET;
        tServerAddress.sin_port = htons(mListeningPort);
        tServerAddress.sin_addr.s_addr = INADDR_ANY;

        taddressLength = sizeof (tServerAddress);

		int resBind = ::bind(mPubSocket, (struct sockaddr *) &tServerAddress, taddressLength);
        if (resBind == -1)
            solcep::Trace().Err() << "Unable to bind the Collector UDP Socket!!" << std::endl;
    }
    UDPCommunicationModule::~UDPCommunicationModule(){
        tsocket = 0;
        taddressLength = 0;
        tServerAddress = {};
        tclientAddr = {};

        shutdown(mPubSocket, 2);
        shutdown(tEmitterSocket, 2);

#ifdef _WIN32
        WSACleanup();
#endif
    }
    
#ifndef _WIN32

    int UDPCommunicationModule::createSocket() {

       if ((tsocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
       {
           return -1;
       }

       return tsocket;
    }

#else

    int UDPCommunicationModule::createSocket() {

        WSADATA wsaData = { 0 };
        int iResult = 0;

        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0) {
            wprintf(L"WSAStartup failed: %d\n", iResult);
            return 1;
        }

        return socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }

#endif

    //int bindSocket(const char* connString) {

    //    if (bind(tsocket, (struct sockaddr*)&tServerAddress, taddressLength) == -1)
    //    {
    //        return -1;
    //    }

    //    return 0;
    //}
    
    int UDPCommunicationModule::bindSocket(const char* connString) 
    {
        if (::bind(tsocket, (struct sockaddr*)&tServerAddress, taddressLength) == -1)
        {
            return -1;
        }

        return 0;
    }

    char* UDPCommunicationModule::receiveData() 
    {
        mPacket = (char*)malloc(mMaxPacketSize + 1);
        struct pollfd fds[1];             /* fd's used by poll */      
        fds[0].fd = mPubSocket;           /* Poll socket s */
        fds[0].events = 0;
        fds[0].events |= POLLIN;          /* Poll on read readiness */

#ifndef _WIN32
        bzero(mPacket, mMaxPacketSize);
#else
        memset(mPacket, '\0', mMaxPacketSize);
#endif

        recvfrom(
            mPubSocket, 
            mPacket, 
            mMaxPacketSize, 
            0,
            (struct sockaddr*)&tEmitterServerAddress,    //used in the client socket
            (socklen_t*)&taddressLength
        );

        return mPacket;
    }
    
    void UDPCommunicationModule::sendData(const char* data) 
    {
        int len = strlen(data);

        if (tEmitterSocket < 1)
            tEmitterSocket = socket(AF_INET, SOCK_DGRAM, 0);
             
#ifndef _WIN32
        bzero(&(tEmitterServerAddress.sin_zero), sizeof(tEmitterServerAddress.sin_zero));
#else
        memset(&(tEmitterServerAddress.sin_zero), '\0', sizeof(tEmitterServerAddress.sin_zero));
#endif

        tEmitterServerAddress.sin_family = AF_INET;
        tEmitterServerAddress.sin_port = htons(mEmitterPort);
    
        sendto(
            tEmitterSocket,
            data, 
            len,
            0,
            (struct sockaddr *)&tEmitterServerAddress,
            sizeof(tEmitterServerAddress)
        );
    }
    
#endif	/* UDPCOMMUNICATIONMODULE_HPP */
}



