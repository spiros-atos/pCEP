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

#ifndef _SOL_CEP_COMPILER_H_
#define _SOL_CEP_COMPILER_H_
 
#include <stdio.h>


class Parser;
class SymbolTable;
class ASTree;
class SymbolResolver;
class SemanticAnalyzer;
class Optimizer;
class ErrorList;

/*!
	\brief Declares a Compiler environment, including lexer/parser,
		symbol table, error list etc.
	
	This allows multiple instances of the a Dolce compiler in the same
	program. Lex/Yacc related modules have been accomodated to suppor this
	(via ParserContext).
*/
class Compiler
{
public:

	Compiler(ErrorList* const _errList);
	~Compiler();
	
	int compile(FILE* _in);
#ifdef CEP_DEBUG
	void dump();
#endif

	SymbolTable* const getSymbolTable() { return mSymbolTable; }
	Parser* const getParser() { return mParser; } 
	ErrorList* const getErrorList() { return mErrorList; }
        
        SymbolTable* mSymbolTable;   ///RP: was private;  but for the memory leak has changed
        ASTree* mTree;
	
protected:
	
	Compiler(const Compiler& rhs)	{}
	
	Compiler& operator= (const Compiler &rhs)	{ return *this; }
	
       
private:

	Parser* mParser;
	SymbolResolver* mSymbolResolver;
	SemanticAnalyzer* mAnalyzer;
	Optimizer* mOptimizer;
	ErrorList* mErrorList;	
	
};

#endif 

