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


#include "BCEPManager.h"
#include "../DolceParser/ScriptCompiler/Compiler.h"
#include "../ComplexEventDetector/Engine/Types/EFramework.h"
#include "../DolceParser/ScriptCompiler/ErrorList.h"
#include "../DolceParser/FrameworkBuilder/FrameworkBuilder.h"
#include "../ComplexEventDetector/ComplexEventDetector.h"
#include "SocketUDPServer.h"
#include "../Plugins/SimpleDecoder/SimpleDecoderPlugin.h"
#include "Tracer.h"
#include "Configuration/ReadIniFile.h"

#ifdef MQTT
#include "Communication/MQTTCommunicationModule.hpp"
#endif

#include "Communication/UDPCommunicationModule.hpp"

#include <signal.h>
#include <time.h>

#ifndef _WIN32

#include <unistd.h>
#include <pthread.h>

#endif


using namespace solcep;
using namespace std;

int atom = 1;
EFramework* framework;
ComplexEventDetector *ced;

///FileEventsOperations* mFeo;
//SocketUDPServer* mSockUDPS;
CommunicationModule *cm;
SimpleDecoderPlugin* sdp;

/**
 * @brief Function to check if a configuration file name has been passed through the command line arguments
 * @param arg Number of command line arguments
 * @param argv Command line arguments
 * @return Configuration file if provided or empty if not
 */
string BCEPManager::checkConfFileInputParams(int argc, char *argv[]){
    string result="";
	for (int i=1; i<argc; i++){
        string argument = argv[i];

        if (argument.compare("-f") == 0){
            i++;
            result = argv[i];
            break;
        }
    }

	return result;
}


/**
 * @brief Function to update configuration parameters with command line arguments
 * @param Number of command line arguments
 * @param Command line arguments
 * @param IniFileSection Configuration parameters to be updated with command line arguments
 */
void BCEPManager::checkInputParams(int argc, char *argv[],IniFileSection *iniFileSection){
    
    for (int i=1; i<argc; i++){
        string argument = argv[i];
        if (argument.compare("-h") == 0){
            std::cout << "bcep usage:" << "\n";
            std::cout << "  -h   help" << "\n";
            std::cout << "  -mi  mptt input topic (10 characters max)" << "\n";
            std::cout << "  -mo  mqtt output topic (10 characters max)" << "\n";
            std::cout << "  -n   client name for mqtt subscription (10 characters max)" << "\n";
            std::cout << "  -d   dolce specification file (30 characters max)" << "\n";
            std::cout << "  -f   configuration file" << "\n";
            std::cout << "\n" << "example:" << "\n";
            std::cout << "            -n trafficCep_1 bcep -mi speedCollector -mo incidencePublisher -d incidentRules.dolce";
            
            exit (0);
        }
        else if (argument.compare("-mi") == 0){
            i++;
            iniFileSection->topicMQTTCollect = argv[i];
        }
        else if (argument.compare("-mo") == 0){
            i++;
            iniFileSection->topicMQTTPublish = argv[i];
        }
        else if (argument.compare("-d") == 0){
            i++;
            iniFileSection->dolceSpec = argv[i];
        }
        else if (argument.compare("-n") == 0){
            i++;
            iniFileSection->mqttClientId = argv[i];
        }
    }
}


#ifndef _WIN32

void *checkQueue(void *arg);

#else

DWORD WINAPI checkQueue(LPVOID arg);

#endif


struct argsToThread {
    ComplexEventDetector *mced;
    Queue *mqueue;
    SimpleDecoderPlugin *msdp;
} *mArgsToThread;

BCEPManager::BCEPManager() {
    
        mQueue = new Queue();

}

BCEPManager:: ~BCEPManager(){
    
    delete framework;
    delete mQueue;
}

void shutdown(int sig) {
    std::cout << "Unexpected ending!!";
    
    delete framework;
    
    sdp->unload();
    
    delete ced;
    //delete mSockUDPS;
    delete cm;
    delete sdp;
    
    free(mArgsToThread);
    
    atom = 0;
}

/**
 * @brief Main function of BCEP
 * @param argc Number of command line arguments
 * @param argv Command line arguments
 */
void BCEPManager::BCEPStart(int argc, char *argv[]) {
    
    signal(SIGINT, &shutdown);
    Trace::LogLevel= 4;
    Trace::SetTracer(solcep::make_unique<StdOutTracer>());

    // Default configuration parameters
    IniFileSection iniFileSec;
    // Configuration file is looked for in command line arguments
    string confFile=checkConfFileInputParams(argc, argv);
    if(!confFile.empty())
    {
    	iniFileSec.confFile=confFile;
    }
    // Configuration file is read to load parameters
    ReadIniFile* iniFile = new ReadIniFile(iniFileSec.confFile);
    iniFileSec = iniFile->readFile();
    // Configuration parameters are updated with command line arguments
    checkInputParams (argc, argv,&iniFileSec);
    
    delete iniFile;

    createTracer(&iniFileSec);

    string fileName = iniFileSec.dolceSpec;
    int res;
    
    framework = createFramework(fileName.c_str(), res);

    if(!framework)
        throw runtime_error("Unable to create EFramework");
    
    INFO() << "Main framework created " << std::endl;
    
    ced = new ComplexEventDetector(framework);
    
    INFO() << "Complex Event Detector created " << std::endl;    

    sdp = new SimpleDecoderPlugin();
    sdp->load();
    INFO() << "Decoder utility loaded " << std::endl;

#ifndef MQTT
    cm = new UDPCommunicationModule(iniFileSec);
#endif
    
#ifdef MQTT
    cm = new MQTTCommunicationModule(iniFileSec);
#endif
    
    //mSockUDPS = new SocketUDPServer(iniFileSecUDP);
    
    //ced->mSocketUDPServer = mSockUDPS;
    ced->cm = cm;
    
////RP: threads!!!    
    mArgsToThread = (argsToThread*)malloc(sizeof(argsToThread));
    mArgsToThread->mced = ced;
    mArgsToThread->mqueue = mQueue;
    mArgsToThread->msdp = sdp;

#ifndef _WIN32
    pthread_t threadDetector;
    pthread_create(&threadDetector, NULL, checkQueue, (void*) mArgsToThread);
#else
    DWORD dwThreadIdArray[3];
    HANDLE hThreadDetectors[3];
    hThreadDetectors[0] = CreateThread(
        NULL,
        0,
        checkQueue,
        mArgsToThread,
        0,
        &dwThreadIdArray[0]
    );
#endif

    std::string lineToRead;
    char *charsRead;

    while (atom  /* eventosPermitidos < 100000 */) {
        charsRead = cm->receiveData();
        lineToRead.assign(charsRead);
        free(charsRead);
                 
        if (lineToRead.size()>0) {
            mQueue->push(lineToRead);
            DEBUG() << "Entry: " << lineToRead << std::endl;
        }
    }
}

///RP: method that is executed when the thread threadDetector is pick up by some processor
#ifndef _WIN32
void *checkQueue(void *arg) 
{
#else
DWORD WINAPI checkQueue(LPVOID arg) 
{
#endif
    argsToThread *_mArgsToThread= (argsToThread*) arg;
    int sntnl=0;
    time_t bgn = time(NULL);
    while (1) 
    {
        if (_mArgsToThread->mqueue->front()) 
        {
            _mArgsToThread->mced->processPublishedMessage(_mArgsToThread->msdp->decode(_mArgsToThread->mqueue->pop()));
            sntnl+=2;
        }
        else 
        {
#ifndef _WIN32
            usleep(10); // 10 microseconds
#else
			std::this_thread::sleep_for(std::chrono::microseconds(10));
#endif

            sntnl++;
        }
        if (sntnl >= 10000) 
        {
            if ((time(NULL) - bgn) >= 10) 
            {
                INFO() << "Publish: " << std::endl;
                _mArgsToThread->mced->processPublishedMessage("\001\200");  //to renew the trees
                sntnl=0;
                bgn = time(NULL);
            }
        }
    }
    
    return 0;
}


//RP: To restart with a new Dolce file
bool BCEPManager::BCEPRestart(int flag) {
    
    if (flag == 1) {  ///Now only 1; 1-->reload
        string fileName = "./detect.dolce";
        int res;
        framework = createFramework(fileName.c_str(), res);

        if(!framework && !res)   ///RP: in this point maybe!! to achieve that not die when the dolce is not 0k
            throw runtime_error("Unable to update Framework");
    
        INFO() << "Main framework updated " << std::endl;
    
        delete ced;
        ced = new ComplexEventDetector(framework);
        
        return 1;
    }
    
    return 0;    
}


void BCEPManager::compileSource(const char* fileName, Compiler* const compiler)
{
    FILE* f = 0;

    if(fileName)
    {
        f = fopen(fileName,"r");
        if(f == NULL)
        {
            throw std::runtime_error("Unable to open file for compilation.");
        }
    }
    else
    {
        f = stdin;
    }

    compiler->compile(f);
    if(fileName)
        fclose(f);
}


EFramework* BCEPManager::createFramework(const char* sourceFile, int& res_)
{
    ErrorList* errList = new ErrorList();
    Compiler* compiler = new Compiler(errList);

    compileSource(sourceFile, compiler);
	
    if(!errList->hasError())
    {
	FrameworkBuilder fb(compiler, errList);
	framework = fb.build(res_);
///RP:
        framework->bcepMngr = this;
    }
    else
    {
        delete compiler->mSymbolTable;  ///RP:
        delete compiler;
        
        INFO()<<"FATAL: Errors found in Dolce script. Engine not built! : "<<std::endl;
        throw std::runtime_error(/*"FATAL: Errors found in Dolce script. Engine not built! : " + */ errList->mHead->s);
                
        delete errList->mHead;    
        delete errList;
        delete sourceFile;
    }
    
//RP:

//rolando    compiler->mTree->startCompletDestroy();    

    delete compiler->mTree;
    delete compiler->mSymbolTable;
    delete compiler;
    delete errList->mHead;
    delete errList;
///    delete sourceFile;
    
    return framework;
}

/**
 * \brief Method that creates a tracer considering the input parameters
 * \param iniFileSection Configuration parameters
 */
void BCEPManager::createTracer(IniFileSection* iniFileSection)
{
	Trace::LogLevel= 4;
    if(iniFileSection)
    {
    	if(!iniFileSection->logger.empty())
    	{
    		Trace::LogLevel=iniFileSection->logLevel;

			if(iniFileSection->logger.compare("stdout")==0)
			{
				Trace::SetTracer(solcep::make_unique<StdOutTracer>());
			}
			else if(iniFileSection->logger.compare("file")==0)
			{
				if(!iniFileSection->loggerFile.empty())
				{
					FileTracer::SetLogFileName(iniFileSection->loggerFile);
					Trace::SetTracer(solcep::make_unique<FileTracer>());
				}
				else
				{
					Trace::SetTracer(solcep::make_unique<StdOutTracer>());
					WARN() << "Logger file has not been defined. Stdout will be used" << std::endl;
				}
			}
			else
			{
				Trace::SetTracer(solcep::make_unique<StdOutTracer>());
				WARN() << "Unknown logger option " << iniFileSection->logger <<" . Stdout will be used" << std::endl;
			}
    	}
    	else
    	{
			Trace::SetTracer(solcep::make_unique<StdOutTracer>());
			WARN() << "Logger option has not been defined . Stdout will be used" << std::endl;

    	}
    }

}

