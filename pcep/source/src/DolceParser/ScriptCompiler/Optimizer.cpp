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

#include "Optimizer.h"


///#include <stdio.h>
#include <assert.h>



Optimizer::Optimizer()
{
	init();
}

Optimizer::~Optimizer()
{
}


void Optimizer::init()
{
	int i = 0;
	mStages[i++] = &Optimizer::stage1;
	mStages[i++] = &Optimizer::stage2;
	
	mStages[i++] = 0;
	
	// just in case we haven't crashed already due to out-of-bounds init above
	assert(i <= KMaxStage); 

}


void Optimizer::optimize(	const ASTree* _tree, 
							const SymbolTable* _symTab,
							const Program* _program)
{
	int i = 0;
	
	while(mStages[i] != 0)
	{
		(*this.*mStages[i])(1,3);
		i++;
	}
}



