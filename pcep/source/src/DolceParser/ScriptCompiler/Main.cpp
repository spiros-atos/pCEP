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

#include <stdio.h>
#include "ASTree.h"

#include "ErrorList.h"
#include "Compiler.h"


Parser* _SL_PARSER_;

//extern int yydebug;
int yydebug;


int main( int argc, const char* argv[] )
{
	//doTest();

	yydebug = 0;

    ErrorList* errList = new ErrorList();
	Compiler* compiler = new Compiler(errList);
	
	_SL_PARSER_ = compiler->getParser();
	
	FILE* f = 0;
	
	if(argc > 1)
		f = fopen(argv[1],"r");
	else
		f = stdin;

	int res = compiler->compile(f);

	
	if(f != stdin)
		fclose(f);
		
#ifdef CEP_DEBUG
	compiler->dump();
#endif

//	delete compiler;
	
	if(res != 0)
	{
		printf("SOL/CEP: Compiler error\n");
	}
}


