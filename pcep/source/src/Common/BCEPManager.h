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


//using namespace solcep;
using namespace std;

#include "../DolceParser/ScriptCompiler/Compiler.h"
#include "../ComplexEventDetector/Engine/Types/EFramework.h"
#include "../ComplexEventDetector/ComplexEventDetector.h"
#include "SocketUDPServer.h"
#include "Queue.h"
#include "../ComplexEventDetector/ComplexEventDetector.h"
#include "./Configuration/IniFileSection.h"
        
#ifndef BCEPMANAGER_H
#define	BCEPMANAGER_H

class EFramework;

class BCEPManager
{
    
    protected:
        Queue *mQueue;
        void checkInputParams(int argc, char *argv[],IniFileSection *iniFileSection);
        string checkConfFileInputParams(int argc, char *argv[]);

    public:
        
        BCEPManager();
        virtual ~BCEPManager();
        void BCEPStart(int argc, char *argv[]);
        bool BCEPRestart(int flag);
        
        
        void compileSource(const char* fileName, Compiler* const compiler);
        EFramework* createFramework(const char* sourceFile, int& res_);
        void createTracer(IniFileSection* iniFileSection);
};

#endif	/* BCEPMANAGER_H */
