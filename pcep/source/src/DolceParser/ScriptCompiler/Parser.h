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

#ifndef _SOL_CEP_COMPILER_PARSER_H_
#define _SOL_CEP_COMPILER_PARSER_H_

///#include <stdio.h>
#include "SymbolTable.h"
#include "ParserContext.h"

class ASTree;
class ErrorList;


/* 
	External references to functions created
	by Flex/Bison, copy/pasted from the generated
	source code.
	
	Must be present to keep the compiler happy.
*/
extern int yylex_init(void** scanner);
extern int yylex_destroy(void* scanner);
extern int yyparse(ParserContext* _context);
extern void yyset_in(FILE * in_str, void* yyscanner );

/*!
	\brief Infrastructure for parsing a Dolce file
*/
class Parser
{
public:

	Parser(ErrorList* const _errList);
	~Parser();
	
	
	
	int parse(FILE* _in, ASTree* const _tree, SymbolTable* const _symTab);

	SymbolTable* getSymbolTable() { return mSymbolTable; }
	ASTree* getASTree() { return mTree; }

	void print();
	
	ErrorList* mErrorList;	// fixme: make protected/private.
	
protected:
	
	
private:
	ASTree* mTree;
	SymbolTable*  mSymbolTable;

};

#endif 

