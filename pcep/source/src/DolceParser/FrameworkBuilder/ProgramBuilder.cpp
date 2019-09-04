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

#include "ProgramBuilder.h"

///#include <stdio.h>
///#include <string.h>
///#include <stdlib.h>
///#include <assert.h>

#include "../../Common/Err.h"

#include "../../ComplexEventDetector/InstructionEvaluator/InstructionEvaluator.h"
#include "../../ComplexEventDetector/Engine/Types/EProgram.h"
#include "../../ComplexEventDetector/Engine/Types/EExternal.h"
#include "../../ComplexEventDetector/Engine/Types/EEvent.h"
#include "../../ComplexEventDetector/Engine/Types/EEventAttribute.h"
#include "../../ComplexEventDetector/Engine/Types/EComplexEventFunction.h"
#include "../ScriptCompiler/ASTree.h"
#include "../../Common/Trace.h"

#include "../../ComplexEventDetector/Engine/Types/EPayloadAttribute.h"
#include "../../ComplexEventDetector/Engine/Types/EComplexEvent.h"

ProgramBuilder::ProgramBuilder()
{
	mNextOid = 2000L;
}


ProgramBuilder::~ProgramBuilder()
{

}


//TODO: Backlog U-0082 All values must be stored and handled as full, flattened in memory Type+TValue.
// This is of course waste of memory space, because TValue (apart from the Ctors) is mandated
// by the biggest member of its union (TAreaLiteral).
//
// In the future, we could reduce this waste by "flattening" even more based on the real
// value stored as TValue - this reduces the memory footprint in the Expression Evaluator (stack)
// and the mini-program size. But it defeats the "encapsulation" offered by the union in TValue.
// FIXME: **** THIS IS NOW SOLVED *** UPDATE  THE BACKLOG!!!

int ProgramBuilder::build(ASTree* const _node, const bool _new)
{
	ASTree* node;
	ASTreeItr itr(_node);

	if(_new) 
            createProgram();                
	
	node = itr.firstChild();        
	while(node)
	{
		build(node, false);
		
		node = itr.nextChild();
	}

	ASTree::NodeType nodeType = _node->getType();
        	
	if(nodeType == ASTree::PValue)
	{
		emitInstruction(InstructionEvaluator::PUSH, _node->getValue());
	}
	else if(nodeType == ASTree::PIdentifier)
	{
		// Attributes or external variables are pushed onto the stack by
		// address.
		// Events as such are never pushed onto the stack.
		//
		SymbolTable::Symbol* symRef = _node->getSymbol();
		assert(symRef);
		SymbolTable::Symbol* symDecl = symRef->decl;
		assert(symDecl);
		
		if(symDecl->symbolType == SymbolTable::external_decl)
		{
			EExternal* ex = (EExternal*)symDecl->engineObj;
			assert(ex);
			
			TAddress addr(ex->oid);
			emitInstruction(InstructionEvaluator::PUSHX, addr);
		}
		else if(symDecl->symbolType == SymbolTable::event_attr_decl)
		{
			EEventAttribute* ea = (EEventAttribute*)symDecl->engineObj;
			assert(ea);
			
			assert(symDecl->owner);
			
			EEvent* e = (EEvent*)symDecl->owner->engineObj;
			assert(e);
			
			TAddress addr(ea->oid, e->oid, symRef->windowOffset);

			emitInstruction(InstructionEvaluator::PUSHA, addr);
		} ///
                else if (symDecl->symbolType == SymbolTable::payload_attr_decl) {
                        EPayloadAttribute *epa = (EPayloadAttribute*)symDecl->engineObj;
                        assert(epa);
                        
                        assert(symDecl->owner);
                        EComplexEvent *ec = (EComplexEvent*)symDecl->owner->engineObj;
                        assert(ec);
                        
                        TAddress addr(epa->oid, ec->oid, symRef->windowOffset);
                        
                        emitInstruction(InstructionEvaluator::PUSHA, addr);
                }
                else if(symDecl->symbolType == SymbolTable::event_decl)

		{
			// do nothing
		}
		else
		{
#ifdef CEP_DEBUG
			printf("[%p] ProgramBuilder::build | UNEXPECTED symbol type on node: %p (type=%d)\n", 
					this,
					_node, symDecl->symbolType);
#endif
			assert(0);
                        ERROR() << "Internal error. Cannot build expression!" << std::endl;
		}
	}
	else if(nodeType == ASTree::PDiffFunc)
	{
		emitInstruction(InstructionEvaluator::DIFF, getComplexEventFuncAddrFromNode(_node));
	}
	else if(nodeType == ASTree::PSumFunc)
	{
		emitInstruction(InstructionEvaluator::SUM, getComplexEventFuncAddrFromNode(_node));
	}
	else if(nodeType == ASTree::PAvgFunc)
	{
		emitInstruction(InstructionEvaluator::AVG, getComplexEventFuncAddrFromNode(_node));
	}
    else if (nodeType == ASTree::PFftFunc)
    {
        emitInstruction(InstructionEvaluator::FFT, getComplexEventFuncAddrFromNode(_node));
    }
    else if (nodeType == ASTree::PTrajFunc)
    {
        emitInstruction(InstructionEvaluator::TRAJ, getComplexEventFuncAddrFromNode(_node));
    }
    else if (nodeType == ASTree::PCollFunc)
    {
        emitInstruction(InstructionEvaluator::COLL, getComplexEventFuncAddrFromNode(_node));
    }
    else if(nodeType == ASTree::PCountFunc)
	{
		emitInstruction(InstructionEvaluator::CNT, getComplexEventFuncAddrFromNode(_node));
	}
    else if(nodeType == ASTree::POpAdd)	emitInstruction(InstructionEvaluator::ADD);
    else if(nodeType == ASTree::POpSub)	emitInstruction(InstructionEvaluator::SUB);
    else if(nodeType == ASTree::POpDiv)	emitInstruction(InstructionEvaluator::DIV);
    else if(nodeType == ASTree::POpMult)    emitInstruction(InstructionEvaluator::MUL);
    else if(nodeType == ASTree::POpEq)  emitInstruction(InstructionEvaluator::CEQ);
    else if(nodeType == ASTree::POpNe)  emitInstruction(InstructionEvaluator::CNE);
    else if(nodeType == ASTree::POpGe)  emitInstruction(InstructionEvaluator::CGE);
    else if(nodeType == ASTree::POpGt)	emitInstruction(InstructionEvaluator::CGT);
    else if(nodeType == ASTree::POpLe)	emitInstruction(InstructionEvaluator::CLE);
    else if(nodeType == ASTree::POpLt)	emitInstruction(InstructionEvaluator::CLT);
    else if(nodeType == ASTree::PLogicalOrOp)   emitInstruction(InstructionEvaluator::ORL);
    else if(nodeType == ASTree::PLogicalAndOp)  emitInstruction(InstructionEvaluator::ANDL);
    else if(nodeType == ASTree::PInAreaFuncX)	emitInstruction(InstructionEvaluator::IAREA);
    else if(nodeType == ASTree::PDiffFuncX)     emitInstruction(InstructionEvaluator::DIFFX);
    else if(nodeType == ASTree::PAbsFunc)   emitInstruction(InstructionEvaluator::ABS);


#ifdef CEP_DEBUG
	printf("%s-\n", _node->asString());
#endif

	return OK;
}


EProgram* ProgramBuilder::finalize()
{
	return mProgram;
}

	
int ProgramBuilder::emitInstruction(const unsigned char _opcode,
									const TAddress& _addr)
{
	size_t binSize;
	_addr.serialize(0, &binSize); // calc size
	


	unsigned char* bin = (unsigned char*)malloc(binSize);

	int res = emitInstruction(_opcode, binSize, _addr.serialize(bin));
	
	free(bin);
	
	return res;
}



int ProgramBuilder::emitInstruction(const unsigned char _opcode,
									const TValue* _val)
{
	size_t binSize;
	_val->serialize(0, &binSize); // calc size
	
	unsigned char* bin = (unsigned char*)malloc(binSize);

	int res = emitInstruction(_opcode, binSize, _val->serialize(bin));
	
	free(bin);
	
	return res;
}

	
						
int ProgramBuilder::emitInstruction(const unsigned char _opcode,
						const unsigned char _operandLength,
						const unsigned char* _operand)
{
	int opSize = InstructionEvaluator::OPCODE_SIZE;
	
//	printf("EMIT %s %d\n", opcodeAsString(_opcode), _operandLength);

	if(_opcode & InstructionEvaluator::HAS_OPERAND_LENGTH)
	{
		//assert(_operandLength);
		opSize += InstructionEvaluator::OPERAND_LENGTH_SIZE + _operandLength;
	}
	
	mProgram->imageSize += opSize;
	
	// Alloc block.
	if(mProgram->imageSize > mAllocProgramSize)
	{
		if(!mProgram->image)
		{
			mProgram->image = (unsigned char*)malloc(BLOCK_SIZE);
		}
		else
		{
			mAllocProgramSize += BLOCK_SIZE;
			mProgram->image = (unsigned char*)realloc(mProgram->image, mAllocProgramSize);
		}
	}

	
	// OPCODE
	*(mProgram->image + mPC) = _opcode;
	mPC += InstructionEvaluator::OPCODE_SIZE;

	if(_opcode & InstructionEvaluator::HAS_OPERAND_LENGTH)
	{
		// operand length
		*(mProgram->image + mPC) = _operandLength;
		mPC += InstructionEvaluator::OPERAND_LENGTH_SIZE;
		
		// operand
		memcpy(mProgram->image + mPC, _operand, _operandLength);
		mPC += _operandLength;
	}
	
	return OK;
}


void ProgramBuilder::createProgram()
{
	mProgram = new EProgram();
	
	mProgram->version = InstructionEvaluator::SOLCEP_PROGRAM_VERSION;
	mProgram->imageSize = 0;
	mProgram->codeStart = 0;
	mProgram->image = 0;

	mPC = 0;
	mAllocProgramSize = 0L;
}


TAddress ProgramBuilder::getComplexEventFuncAddrFromNode(ASTree* const _node) const
{
#ifdef CEP_DEBUG
	printf("ProgramBuilder::getComplexEventFuncAddrFromNode | Node =(%p)\n", _node);
#endif
	SymbolTable::Symbol* sym = _node->getSymbol();
	assert(sym);
	
	EComplexEventFunction* cef = (EComplexEventFunction*)sym->engineObj;
	assert(cef);
	
	return TAddress(cef->oid);
}





#ifdef CEP_DEBUG

void ProgramBuilder::print(	const EProgram* _program, 
							const int _indent, 
							const bool _raw)
{
	int size = _program->imageSize;
	
	printf("ProgramBuilder::print | Program size = %d\n", size);

	indent(_indent);
	
	if(_raw)
	{
	//FIXME: Substitute with logger built-in hexdump

		for(int i = 0; i < size; i++)
		{
			printf("%02X ", *(_program->image + i));
		}
		
		printf("\n");
	}
	else
	{
		int pc = 0;
		unsigned char opcode;
		unsigned char operandLength;
		
		while(pc < size)
		{
			indent(_indent);

			printf("%4d ", pc);
			
			//FIXME: share code with the instruction-decoder in the Expression Evaluator engine.
			opcode = *(_program->image + pc);
			pc += InstructionEvaluator::OPCODE_SIZE;
			printf("%s ", opcodeAsString(opcode));

			if(opcode & InstructionEvaluator::HAS_OPERAND_LENGTH)
			{
				operandLength = *(_program->image + pc);
				pc += InstructionEvaluator::OPERAND_LENGTH_SIZE;
				
				if((opcode == InstructionEvaluator::PUSHA)
				|| (opcode == InstructionEvaluator::SUM)
				|| (opcode == InstructionEvaluator::AVG)
				|| (opcode == InstructionEvaluator::CNT)
				|| (opcode == InstructionEvaluator::DIFF))
				{
					TAddress addr;
					
					addr.deserialize(_program->image + pc);
					
//					printf("oid:%ld, ownerOid:%ld, ref:%d", 
//							addr.oid, addr.ownerOid, addr.ref);
				}
				else if(operandLength != sizeof(TValue))
				{
					for(int i = 0; i < operandLength; i++)
					{
						printf("%02X", *(_program->image + i + pc));
					}
				}
				else
				{
					TValue v;
					
					v.deserialize(_program->image + pc);
					printf("%s", v.asString());
				}
				pc += operandLength;
			}
			printf("\n");
		}
	}
}



const char* ProgramBuilder::indent(const int _indent)
{
	int i = 0;
	while(i < _indent)
		mIndent[i++] = ' ';

	mIndent[i] = '\0';
	
	return mIndent;
}


const char* ProgramBuilder::opcodeAsString(const unsigned char _opcode)
{
	static char fmt[16];
	
	if(_opcode == InstructionEvaluator::PUSHX) { return "PUSHX"; }
	if(_opcode == InstructionEvaluator::PUSHA) { return "PUSHA"; }
	if(_opcode == InstructionEvaluator::PUSH) { return "PUSH"; }
	if(_opcode == InstructionEvaluator::POP) { return "POP"; }
	if(_opcode == InstructionEvaluator::ADD) { return "ADD"; }
	if(_opcode == InstructionEvaluator::SUB) { return "SUB"; }
	if(_opcode == InstructionEvaluator::MUL) { return "MUL"; }
	if(_opcode == InstructionEvaluator::DIV) { return "DIV"; }
	if(_opcode == InstructionEvaluator::EVAL) { return "EVAL"; }
	if(_opcode == InstructionEvaluator::CEQ) { return "CEQ"; }
	if(_opcode == InstructionEvaluator::CNE) { return "CNE"; }
	if(_opcode == InstructionEvaluator::CGE) { return "CGE"; }
	if(_opcode == InstructionEvaluator::CGT) { return "CGT"; }
	if(_opcode == InstructionEvaluator::CLE) { return "CLE"; }
	if(_opcode == InstructionEvaluator::CLT) { return "CLT"; }
	if(_opcode == InstructionEvaluator::ANDL) { return "ANDL"; }
	if(_opcode == InstructionEvaluator::ORL) { return "ORL"; }
	if(_opcode == InstructionEvaluator::IAREA) { return "IAREA"; }
	if(_opcode == InstructionEvaluator::DIFF) { return "DIFF"; }
	if(_opcode == InstructionEvaluator::DIFFX) { return "DIFFX"; }
	if(_opcode == InstructionEvaluator::SUM) { return "SUM"; }
	if(_opcode == InstructionEvaluator::CNT) { return "CNT"; }
	if(_opcode == InstructionEvaluator::AVG) { return "AVG"; }
	if(_opcode == InstructionEvaluator::RESET ) { return "RESET"; }
	if(_opcode == InstructionEvaluator::TRAP ) { return "TRAP"; }
	if(_opcode == InstructionEvaluator::NOP) { return "NOP"; }
	if(_opcode == InstructionEvaluator::ABS) { return "ABS"; }
    if(_opcode == InstructionEvaluator::FFT) { return "FFT"; }

	sprintf(fmt,"invalid opcode (0x%x)", _opcode);
	
	return fmt;
}

#endif


