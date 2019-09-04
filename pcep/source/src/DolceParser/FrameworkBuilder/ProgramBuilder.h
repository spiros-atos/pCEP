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


#pragma once

#include "../../ComplexEventDetector/Engine/Types/EProgram.h"
#include "../../Types/TValue.h"
#include "../../Types/TAddress.h"
#include "../ScriptCompiler/ASTree.h"

/* 
    builds programs for the Expression Evaluator	
*/
class ProgramBuilder 
{

public:

	ProgramBuilder();
	~ProgramBuilder();

	/*
		Recursively builds a program starting from an expression node
	*/
	int build(ASTree* const _node, const bool _new = true);
	
	
	/*
		Finalizes the program and returns a pointer to the instructions.
		The caller is responsible for releasing the returned memory
	*/
	EProgram* finalize();
        
#ifdef CEP_DEBUG
	void print(const EProgram* _code, const int _indent = 0, const bool _raw = false);
	const char* indent(const int _indent);
	static const char* opcodeAsString(const unsigned char _opcode);
	char mIndent[64];
#endif


protected:

	int emitInstruction(const unsigned char _opcode,
						const unsigned char _operandLength = 0,
						const unsigned char* _operand = 0);
						
	int emitInstruction(const unsigned char _opcode,
						const TAddress& _addr);
						
	int emitInstruction(const unsigned char _opcode,
						const TValue* _val);
						
						
	void createProgram();

	TAddress getComplexEventFuncAddrFromNode(ASTree* const _node) const;
	
private:

	unsigned long mAllocProgramSize; // block-allocated size
	int mPC;
	
	EProgram* mProgram;
	
	static const unsigned long BLOCK_SIZE = 64UL;
	
	
	// returns oid
	long addConstant(const TValue* _val);

	long mNextOid;      
};
