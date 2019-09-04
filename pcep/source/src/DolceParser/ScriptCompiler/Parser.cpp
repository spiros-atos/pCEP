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

///#include <stdio.h>


#include "Parser.h"
#include "ParserContext.h"

#include "ASTree.h"
#include "SymbolTable.h"
#include "ErrorList.h"

#include "../DolceCompilerCodes.h"



Parser::Parser(ErrorList* _errList)
{
	mErrorList = _errList;
}


Parser::~Parser()
{
}


#ifdef CEP_DEBUG

void Parser::print()
{
	if(mTree)
		mTree->print();
	else
		printf("No tree\n");
}
#endif

void* sxxx;



int Parser::parse(FILE* _in, ASTree* const _tree, SymbolTable* const _symTab)
{
	mTree = _tree;
	mSymbolTable = _symTab;


	int res;
	ParserContext* pc = new ParserContext();
	
	yylex_init(&pc->scanner);
	yyset_in(_in, pc->scanner);
	pc->parser = this;
	
	res = yyparse(pc);
	
	
	yylex_destroy(pc->scanner);	
	
//	printf("RES=%d\n", res);
	
	return res;
}



