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

#include "Compiler.h"

///#include <stdio.h>

#include "../../Common/Err.h"


#include "Parser.h"
#include "SymbolResolver.h"
#include "SemanticAnalyzer.h"
#include "Optimizer.h"
#include "ASTree.h"
#include "SymbolTable.h"
#include "ErrorList.h"


Compiler::Compiler(ErrorList* const _errList) : mErrorList(_errList)
{
	mParser = new Parser(_errList);
	mAnalyzer = 0;
}


Compiler::~Compiler()
{        
	delete mParser;        
}


int Compiler::compile(FILE* _in)
{
	int res = OK;
	
	mTree = new ASTree(ASTree::PRoot);
	mSymbolTable = new SymbolTable();

//	mOptimizer = new Optimizer();
	
	if(mParser->parse(_in, mTree, mSymbolTable))
	{
		return 1;
	}
	
//	mSymbolResolver = new SymbolResolver();
//	res |= mSymbolResolver->resolve(mTree, mSymbolTable, mErrorList);
	
#ifdef CEP_DEBUG
	mParser->print();
	mSymbolTable->print();
#endif
	
	mAnalyzer = new SemanticAnalyzer();
	res |= mAnalyzer->analyze(mTree, mSymbolTable, mErrorList);
	
#ifdef CEP_DEBUG
	mParser->print();
	mSymbolTable->print();
#endif

	//	delete mOptimizer;
//	delete mGenerator;

#ifdef CEP_DEBUG
	mErrorList->print();
#endif
	delete mAnalyzer;
		
	return res;
}

#ifdef CEP_DEBUG
void Compiler::dump()
{
	mParser->print();
}
#endif

