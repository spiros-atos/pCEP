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


///RP: not used: now  we use udp sockets
/*
#include "FileEventsOperations.h"
#include <sys/stat.h>
#include <fstream>
#include "Tracer.h"
#include <ctime>
#include <string.h>
//#include <chrono>
#include <unistd.h>
#include <iostream>

namespace solcep {

    //        sprintf(fNTemp, "/proc/%d/exe", pid);
    FileEventsOperations::FileEventsOperations() {
//        char* fNTemp = new char[LENMAXPATH];
        char* fNTemp = (char*)"/proc/self/cwd";
        if (readlink(fNTemp, fNameIn, LENMAXPATH) == -1) {
            perror("readlink");
            ERROR() << "Impossible build the path for file events." << std::endl;
        }
        else {
            strcpy(fNameOut, fNameIn);
            strcat(fNameIn, "/incoming/event.txt");
            strcat(fNameOut, "/outgoing/");
            
            INFO() << "Looking file of events at: " << fNameIn << std::endl;
            INFO() << "Folder to put triggered events: " << fNameOut << std::endl;

        }
        
//        delete fNTemp;
    }

    FileEventsOperations::~FileEventsOperations() { 
    
        delete fNameIn;
        delete fNameOut;
    }
    
//verify if an event has arrived    
    bool FileEventsOperations::existsEventInFile() {
        
        struct stat buffer;

        return (stat(fNameIn, &buffer) == 0);
        
    }
    
    
    std::string FileEventsOperations::readEventFromFile() {
        std::string aLine;
        
///        std::ifstream input("/home/adminuser/projects/bcep/incoming/events.txt");        0k
        std::ifstream input(fNameIn);
        
        
        std::getline( input, aLine );
           
        input.close();
        
        if( remove(fNameIn) != 0 )
            ERROR() << "Unable to work with the events file!!" << std::endl;

        
        return aLine;
        
    }
    
    
    void FileEventsOperations::writeCEventToFile(std::string msg) {
        
        const char* ffName = getFileName();

        std::ofstream output(ffName);
        
        if (output.good()) {
            output.write(msg.c_str(), msg.size());
            output.flush();
        }
        else INFO() << "Output folder path is missing: " << fNameOut << std::endl;
        
        output.close();
        delete ffName;
        
    }
    
///RP: generate the file name to cEvent; based on timestamp.
    const char* FileEventsOperations::getFileName() {
        
        char* buffer = new char[16];

        struct timespec timeNow;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timeNow);  //  CLOCK_REALTIME
///        long int nanoS = timeNow.tv_nsec;           0k
        
        std::sprintf(buffer, "%lu", timeNow.tv_nsec);
               
        char* fName=  (char*)malloc(128); //malloc(strlen(fNameOut) + strlen(buffer) + 1);

        strcpy(fName, fNameOut);
        strcat(fName, buffer);
        strcat(fName, ".txt");
        INFO() << "File name generated: " << fName << std::endl;
        
       const char* fullName = (const char *)fName;
        
        delete buffer;
 
        return fullName;
    }
    
}

*/