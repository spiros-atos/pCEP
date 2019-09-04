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


#include "SimpleDecoderPlugin.h"
#include <iostream>
#include <stdexcept>
#include <sys/types.h>
#include "../../Types/TPosLiteral.h"
///#include <sys/socket.h>
///#include <arpa/inet.h>
///#include <stdio.h>
///#include <unistd.h>
///#include <string.h>
///#include <stdlib.h>


using namespace std;

const int SimpleDecoderPlugin::MAX_BUF_SIZE = 1024;

int SimpleDecoderPlugin::load() 
{
	mWB = new solcep::WireEventBuilder();
	mBuf = (char*)malloc(MAX_BUF_SIZE);

	return 0; 
}

void SimpleDecoderPlugin::unload() 
{
	free(mBuf);
	delete mWB;
        
        delete mSz;
///        delete mSzSave;
        
}	

string SimpleDecoderPlugin::decode(string message)
{
    initTokenizer(reinterpret_cast<const unsigned char*>(message.c_str()), message.length());

///    char tok1[256];
      char tok1[128];
///    char tok2[256];
      char tok2[128];
///    char tok2[256];
      char tok3[128];

    // type
    if(!nextToken(tok1)) 
        throw runtime_error("Decoder: nextToken failed!");

    // name
    if(!nextToken(tok2)) 
        throw runtime_error("Decoder: nextToken failed!");

    // Setup the Wire Event
    mWB->create(atoi(tok1), tok2);

    // nv-pairs
    int nvPairCount = 0;
    while(nextToken(tok1))
    {
        // --name
        if(!nextToken(tok2))
            throw runtime_error("Decoder: nextToken failed!");

        // --value
        if(!nextToken(tok3))
            throw runtime_error("Decoder: nextToken failed!");
            
        if(!strcmp(tok1, "int")) 
                mWB->add(tok2, atoi(tok3));
        else if(!strcmp(tok1, "float"))
                mWB->add(tok2, (float)atof(tok3));
        else if(!strcmp(tok1, "string")) {
                completeStrToken(tok3);
                mWB->add(tok2, tok3);
        }
        else if(!strcmp(tok1, "char"))
                mWB->add(tok2, (char)tok3[0]);
        else if(!strcmp(tok1, "pos"))
                mWB->addTPL(tok2, tok3);
        else if(!strcmp(tok1, "area"))
                mWB->addArea(tok2, tok3);
        else if(!strcmp(tok1, "time"))
                mWB->addTime(tok2, tok3);

        //throw runtime_error("Decoder: unsupported type");
        
        nvPairCount++;
    }

    mWB->addNVPairCount(nvPairCount);	
///RP 16122014
    std::cout << "\n";
    return mWB->finalize();
}

void SimpleDecoderPlugin::initTokenizer(const unsigned char* _data, const int _size)
{
    mSz = mBuf;
    mSzSave = 0;
    int sz = (_size > MAX_BUF_SIZE) ? MAX_BUF_SIZE : _size;

#ifndef _WIN32
    bzero(mSz,MAX_BUF_SIZE);
#else
    memset(mSz, '\0', MAX_BUF_SIZE);
#endif

    strncpy(mSz, (char*)_data, sz);
	///RP: 15122014 
	///    for(int i = 0; i < sz + 10; i++)
	///	std::cout << " " << static_cast<unsigned>(mSz[i]) << "-"<< mSz[i];
    
//    delete _data;
}

bool SimpleDecoderPlugin::nextToken(char* token_) {
//    int cnt;	
///    bzero(token_, 256);

#ifndef _WIN32
    bzero(token_, 128);
#else
    memset(token_, '\0', 128);
#endif

#ifndef _WIN32
    char* t = strtok_r(mSz, " \n\r\t", &mSzSave);
#else
    char* t = strtok_s(mSz, " \n\r\t", &mSzSave);
#endif

    mSz = 0;
    if(t && strlen(t))
    {
    ///        strncpy(token_, t, 256);
        strncpy(token_, t, 128);
        //std::cout << "TOKEN='" << token_ << "' len=" << strlen(token_) << "\t";
        return true;
    }

    return false;
}

///RP:
bool SimpleDecoderPlugin::completeStrToken(char* token_) {
        
    if (token_[0] == '"') {
        if (strlen(token_) > 1 && token_[strlen(token_)-1] == '"') {
            int c=1, cc=0;
            char *tt = (char*)malloc(strlen(token_)+1);
            while (token_[c] != '"') {
                    tt[cc] = token_[c];
                    cc++; c++;
            }
            tt[cc]='\0';
            strcpy(token_, tt);
            free(tt);
        }
        else {
            char *t;
            if (mSzSave[0] == '"') {
                t=(char*)"";
                mSzSave++;
            }
            else
#ifndef _WIN32
                t = strtok_r(mSz, "\\\"", &mSzSave);
#else
                t = strtok_s(mSz, "\\\"", &mSzSave);
#endif
            
            strcat(token_, " ");
            strcat(token_, t);
            int cc=0;
            char *tt = (char*)malloc(strlen(token_)+1);
            for (unsigned c = 0; c < strlen(token_); c++) {
                if (token_[c] != '"') {
                    tt[cc] = token_[c];
                    cc++;
                }
            }
            tt[cc]='\0';
            strcpy(token_, tt);
            free(tt);
        }
    }
    else {
        char *t;
        char *aBackStep= (char*)malloc(strlen(mSzSave)+1);
        strcpy(aBackStep, mSzSave);

#ifndef _WIN32
        t = strtok_r(mSz, " \n\r\t", &mSzSave);    
#else
        t = strtok_s(mSz, " \n\r\t", &mSzSave);    
#endif
        mSz = 0;

        while (t && strlen(t) && !isTypeAccepted(t)) {
            strcat(token_, " ");
            strcat(token_, t);
            strcpy(aBackStep, mSzSave);

#ifndef _WIN32
            t = strtok_r(mSz, " \n\r\t", &mSzSave);
#else
            t = strtok_s(mSz, " \n\r\t", &mSzSave);
#endif
        }
        
        strcpy(mSzSave, aBackStep);
        free(aBackStep);
    }
    
    return true;
}

///RP: plesae make consistent with the dolce specification
bool SimpleDecoderPlugin::isTypeAccepted(char* token_) {
    
    if (strcmp(token_, "int") && strcmp(token_, "float") && strcmp(token_, "string") && strcmp(token_, "char") && strcmp(token_, "pos") &&
        strcmp(token_, "area") && strcmp(token_, "time"))
            return false;
    
    return true;
    
}
    

///RP: bool SimpleDecoderPlugin::nextToken(char* token_)
/*bool SimpleDecoderPlugin::nextTokenString(char* token_)
{
//    int cnt;	
///    bzero(token_, 256);
    bzero(token_, 128);
    
    char *ft = mSz, *ftSave;
    char* t = strtok_r(ft, " \n\r\t", &ftSave);
    
    if (t && strlen(t)) {
        while (t && strlen(t) && !(strcmp(t, "int") || strcmp(t, "float") || strcmp(t, "string"))) {
            t = strtok_r(mSz, " \n\r\t", &mSzSave);
            mSz = 0;
            if (t && strlen(t)) {
    ///                strncpy(token_, t, strlen(t));
                strcat(token_,t);

            }           
            t = strtok_r(ft, " \n\r\t", &ftSave);                    
        }
        std::cout << "TOKEN='" << token_ << "' len=" << strlen(token_) << "\t";        
        return true;
    }

    return false;
}
*/

extern "C" CEPPlugin* cep_plugin_create()
{
	return new SimpleDecoderPlugin();
}

extern "C" void cep_plugin_destroy(CEPPlugin* _plugin)
{
	delete _plugin;
}



