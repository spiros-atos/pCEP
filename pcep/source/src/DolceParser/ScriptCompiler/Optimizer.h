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

/*
	FIXME Candidate optimizations:

		- pre-evaluate formulas not involving variables or functions
		- detect similar calculator instruction sets and reduce them
		
		For this, we need at least the generated code
		
*/

#ifndef _OPTIMIZER_H_
#define _OPTIMIZER_H_

#include <stdio.h>

class ASTree;
class SymbolTable;
class Program;

class Optimizer
{
public:

	Optimizer();
	~Optimizer();
	
	void optimize(	const ASTree* _tree, 
					const SymbolTable* _symTab,
					const Program* _program);
	
	
protected:

	typedef int (Optimizer::*TOptimizerFunc)(int x, int z);
	
	static const int KMaxStage = 10;
	
	int stage1(int y, int z) { printf("Optimizing stage 1\n"); return 0; }
	int stage2(int y, int z) { printf("Optimizing stage 2\n"); return 0; };
		
	TOptimizerFunc mStages[KMaxStage]; /* !! MAKE SURE this is big enough */


	Optimizer(const Optimizer& rhs)	{}
	Optimizer& operator= (const Optimizer &rhs)	{ return *this; }
	

private:

	void init();
};

#endif 

