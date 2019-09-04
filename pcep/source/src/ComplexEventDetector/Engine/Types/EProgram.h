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


/*!
	Types for the Execution Framework
	
*/
#ifndef _SOL_CEP_ENGINE_TYPES_EPROGRAM_H_
#define _SOL_CEP_ENGINE_TYPES_EPROGRAM_H_

/*!
	\brief P-Code for stack machine.
	
	See ProgramBuilder and InstructionEvaluator.
	
	// *******
	//WARNING: Used in memory operations! Keep this as a POC struct.
	// *******
	
		version - program format version
		size - total image length. Equals the size
			of code segment + data segment
		codeStart - start address of program code
		data - start of data segment + program data.
			Program starts at 'dataSegmentLength'
			
        +----------------+
	    | version        |
	    +----------------+ 
	    | imageSize      |
	    +----------------+ 
	 .--| codeStart      |
	 |  +----------------+ -------.
	 |  | <data segment> |        |
	 |  +----------------+         >--- image (imageSize)
	 '->| <code segment> |        |
	    +----------------+ -------'
*/
struct EProgram
{

	unsigned long version;
	unsigned long imageSize;
	unsigned long codeStart;
	unsigned char* image;	// Contains data segment + code segment
};



#endif
