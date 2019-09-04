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


/************************

	NOTE: The codes descbibed should adhere to document SOL-CEP-0004,
		"SOL/CEP DOLCE COMPILER CODES".

*************************/

/* 
	Errors and warning messages related to Dolce.
	These errors are generated during compiling, parsing, and runtime
	engine construction.

*/
#ifndef _SOL_CEP_DOLCE_MSG_H_
#define _SOL_CEP_DOLCE_MSG_H_


//---- Compiler

// Generic compiler error. In the future, must extract more
// precise info from yacc.
static const int DMSG_SYNTAX_ERROR = 100;



// ----Analyzer

// "Variable not declared"
static const int DMSG_AL_UNDECLARED_VAR = 1000;

// "'detect' statement missing. Identifier x must be associated to
//  an event in the 'detect' statement.
static const int DMSG_AL_MISSING_DETECT_STMT = 1001;

// "Undeclared event"
static const int DMSG_AL_EVENT_NOT_DECLARED = 1002;

// "Tuple scope specifier must refer to an event or complex event"
static const int DMSG_AL_TUPLE_SCOPE_SPEC_NOT_E_OR_CE = 1003;

// "Variable is not defined for event x, and is not an external variable"
static const int DMSG_AL_VAR_NOT_FOR_EVT_NOR_EXT = 1004;

// "Variable is ambiguous for event or complex event"
static const int DMSG_AL_AMBIGUOUS_VAR_FOR_E_OR_CE = 1005;

// "Undeclared external variable"
static const int DMSG_AL_EXT_VAR_NOT_DECLARED = 1006;

// "Undeclared attribute for event"
static const int DMSG_AL_ATTR_NOT_DECLARED_FOR_EVT = 1007;


// Runtime engine construction

// "Time window identifier must be declared as an external variable"
static const int DMSG_RT_TIME_WIN_ID_NOT_EXT = 2000;

// "Time window is of invalid type. Must be 'integer' or 'duration'"
static const int DMSG_RT_TIME_WIN_INVALID_TYPE = 2001;

// "Identifier is of an unexpected type"
static const int DMSG_RT_ID_UNEXPECTED_TYPE = 2002;

#endif
