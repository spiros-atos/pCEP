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



//FIXME-REFACTOR: move to 'lang/Types' ?

/*
	TValue - This is the main type used throughout the CEP 
*/
#ifndef _SOL_CEP_TYPES_TTYPE_H_
#define _SOL_CEP_TYPES_TTYPE_H_



enum TType
{
	//IMPORTANT - THESE VALUES MUST CORRESPOND TO THE OFFICIAL WIRE EVENT PROTOCOL SPEC.
	t_char = 		0,
	t_string =		1,
	t_int =			2,
	t_float =		3,
	t_pos =			4,
	t_area =		5,
	t_duration =	6,
	t_time =		7,
	t_ersatz = 		99,	//! 'wild card' used in InstructionEvaluator
	t_invalid =		100
};


#endif

