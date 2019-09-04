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


#include "InstructionEvaluator.h"

#include "../../Types/TValue.h"
#include "../../Types/TAddress.h"
#include "../../Common/Err.h"

#include "Stack.h"
#include "../Engine/Types/EProgram.h"
#include "../Engine/Types/ECapturedEventPool.h"
#include "../Engine/Engine.h"
#include "../Engine/Types/ECapturedEvent.h"
#include "../Engine/Types/EFramework.h"
#include "../Engine/Types/EComplexEventFunction.h"

#include "PCEPFunctions.h"
#include <omp.h>

#ifdef CEP_DEBUG
#include "../../DolceParser/FrameworkBuilder/ProgramBuilder.h"
#endif


#include "../../CollisionDetection/constants.h"


#define TUPPLE_WINDOW_SIZE 1
#define FFT_SIZE	TUPPLE_WINDOW_SIZE


InstructionEvaluator::InstructionEvaluator(Engine* const _engine) : mEngine(_engine)
{
	mStack = new Stack();
	
	initInstructions();
	
#ifdef CEP_DEBUG
//	unittest();
//	exit(333);
#endif
}


InstructionEvaluator::~InstructionEvaluator()
{
	delete mStack;
}


void InstructionEvaluator::initInstructions()
{
	int i = 0;

	initInstruction(i++, PUSH,	&InstructionEvaluator::f_push);
	initInstruction(i++, PUSHX, &InstructionEvaluator::f_pushx);
	initInstruction(i++, PUSHA,	&InstructionEvaluator::f_pusha);
	initInstruction(i++, POP,	&InstructionEvaluator::f_pop);
	initInstruction(i++, ADD,	&InstructionEvaluator::f_add);
	initInstruction(i++, SUB,	&InstructionEvaluator::f_sub);
	initInstruction(i++, MUL,	&InstructionEvaluator::f_mul);
	initInstruction(i++, DIV,	&InstructionEvaluator::f_div);
	initInstruction(i++, EVAL,	&InstructionEvaluator::f_eval);
	initInstruction(i++, CEQ,	&InstructionEvaluator::f_ceq);
	initInstruction(i++, CNE,	&InstructionEvaluator::f_cne);
	initInstruction(i++, CGE,	&InstructionEvaluator::f_cge);
	initInstruction(i++, CGT,	&InstructionEvaluator::f_cgt);
	initInstruction(i++, CLE,	&InstructionEvaluator::f_cle);
	initInstruction(i++, CLT,	&InstructionEvaluator::f_clt);
	initInstruction(i++, ANDL,	&InstructionEvaluator::f_andl);
	initInstruction(i++, ORL,	&InstructionEvaluator::f_orl);
	initInstruction(i++, IAREA,	&InstructionEvaluator::f_iarea);
	initInstruction(i++, DIFF,	&InstructionEvaluator::f_diff);
	initInstruction(i++, DIFFX,	&InstructionEvaluator::f_diffx);
	initInstruction(i++, CNT,	&InstructionEvaluator::f_cnt);
	initInstruction(i++, SUM,	&InstructionEvaluator::f_sum);
	initInstruction(i++, AVG,	&InstructionEvaluator::f_avg);
	initInstruction(i++, RESET,	&InstructionEvaluator::f_reset);
	initInstruction(i++, TRAP,	&InstructionEvaluator::f_trap);
	initInstruction(i++, NOP,	&InstructionEvaluator::f_nop);
	initInstruction(i++, ABS,	&InstructionEvaluator::f_abs);
    initInstruction(i++, FFT,   &InstructionEvaluator::f_fft);
    initInstruction(i++, TRAJ,  &InstructionEvaluator::f_traj);
    initInstruction(i++, COLL,  &InstructionEvaluator::f_coll);
    initInstruction(i++, 0,	0);
	
	// just in case we haven't crashed already due to out-of-bounds init above
	assert(i <= KMaxInstruction); 
}

int InstructionEvaluator::run(EProgram* _program, ECapturedEventPool* _pool)
{
#ifdef CEP_DEBUG
    printf("InstructionEvaluator::run | programSize = %lu\n", _program->imageSize);
    //Common::dumpData(_program->image, _program->imageSize, "Program", this);
#endif

    assert(_program);
    assert(_pool);
	        
    mPool = _pool;
    if(_program->version != SOLCEP_PROGRAM_VERSION)
    {
#ifdef CEP_DEBUG
        printf("InstructionEvaluator::run | invalid program version\n");
#endif
        return ERR_IE_INV_PROG_VER;
    }
	
    unsigned char opcode;
    unsigned char operandLength;
    unsigned long size = _program->imageSize;
	
    int res = OK;

    reset();
	
    mPC = _program->codeStart;

#ifdef CEP_DEBUG
    printf("--- exec start ---\n");
#endif
    while((mPC < size) && (res == OK))
    {
        opcode = *(_program->image + mPC);
        mPC += OPCODE_SIZE;

        if(opcode & HAS_OPERAND_LENGTH)
        {
            operandLength = *(_program->image + mPC);
            mPC += OPERAND_LENGTH_SIZE;
        }
        else
        operandLength = 0;

        res |= doInstruction(opcode, operandLength, _program->image + mPC); 
        mPC += operandLength;
    }
#ifdef CEP_DEBUG
    printf("--- exec done. res = %d ---\n", res);
#endif
	
    return res;
}

///RP:20150211 
/*
int InstructionEvaluator::runWithoutComplexFunctions(EProgram* _program, ECapturedEventPool* _pool)
{
#ifdef CEP_DEBUG
	printf("InstructionEvaluator::run | programSize = %lu\n", _program->imageSize);
	Common::dumpData(_program->image, _program->imageSize, "Program", this);
#endif

	assert(_program);
	assert(_pool);
        
	printf("InstructionEvaluator::runWithoutComplexFunctions | programSize = %lu\n", _program->imageSize);
        
	mPool = _pool;
	if(_program->version != SOLCEP_PROGRAM_VERSION)
	{
#ifdef CEP_DEBUG
		printf("InstructionEvaluator::run | invalid program version\n");
#endif
		return ERR_IE_INV_PROG_VER;
	}
	
	unsigned char opcode;
	unsigned char operandLength;
	unsigned long size = _program->imageSize;
	
	int res = OK;

	reset();
	
	mPC = _program->codeStart;

#ifdef CEP_DEBUG
	printf("--- exec start ---\n");
#endif
	while((mPC < size) && (res == OK))
	{
		opcode = *(_program->image + mPC);
		mPC += OPCODE_SIZE;

		if(opcode & HAS_OPERAND_LENGTH)
		{
			operandLength = *(_program->image + mPC);
			mPC += OPERAND_LENGTH_SIZE;
		}
		else
			operandLength = 0;

///                if (!isComplexFunction(opcode))
///                    res |= doInstruction(opcode, operandLength, _program->image + mPC);
                
		mPC += operandLength;
	}
#ifdef CEP_DEBUG
	printf("--- exec done. res = %d ---\n", res);
#endif
	
	return res;
}
*/

TValue* const InstructionEvaluator::getTopOfStack() const
{
	return mStack->peek();
}

///RP:popStack()
bool InstructionEvaluator::popStack(TValue* val_) const
{
	return mStack->pop(val_);
}

void InstructionEvaluator::reset()
{
	mPC = 0;
}

//--------------------------------------------------------------
void InstructionEvaluator::initInstruction(	const int _index, 
											const unsigned char _opcode,
											TInstructionFunc func)
{
	mInstructions[_index].opcode = _opcode;
	mInstructions[_index].func = func;
}

//--------------------------------------------------------------
int InstructionEvaluator::doInstruction(	const unsigned char _opcode,
											const int _operandLen,
											const unsigned char* _operand)
{
    int i = 0;
    int res = OK;
	
    while(mInstructions[i].func != 0)
    {
        if(mInstructions[i].opcode == _opcode)
        {
#ifdef CEP_DEBUG
            //			printf("exec %s | operandLength: %d\n", ProgramBuilder::opcodeAsString(_opcode), _operandLen);
#endif
            res = (*this.*mInstructions[i].func)(_operandLen, (unsigned char*)_operand);
            fflush(stdout);
			
            return res;
        }
        i++;
    }
	
    // When we reach this, we have an invalid instruction.
    assert(0);
	
#ifdef CEP_DEBUG	
    printf("InstructionEvaluator::doInstruction | opcode = %s\n",
    ProgramBuilder::opcodeAsString(_opcode));
#endif 

    return ERR_IE_ABORT;
}

//--------------------------------------------------------------
int InstructionEvaluator::f_push(int _operandLen, unsigned char* _operand)
{
#ifdef CEP_DEBUG	
    printf("InstructionEvaluator::f_push\n");
#endif 

    TValue v;
	
    v.deserialize(_operand);
    mStack->push(v);
	
    return OK;
}

//--------------------------------------------------------------
int InstructionEvaluator::f_pushx(int _operandLen, unsigned char* _operand)
{
#ifdef CEP_DEBUG	
	printf("InstructionEvaluator::f_pushx\n");
#endif 

	// resolve the value
	TAddress addr;
	
	TValue* ext = mEngine->getExternalValue(addr.deserialize(_operand));
	
	if(!ext)
		return ERR_IE_ABORT;

	mStack->push(*ext);
	
	return OK;
}

//--------------------------------------------------------------
int InstructionEvaluator::f_pusha(int _operandLen, unsigned char* _operand)
{
#ifdef CEP_DEBUG	
	printf("InstructionEvaluator::f_pusha\n");
#endif 

	// resolve the value
	TAddress addr;
	
	TValue* attr = mEngine->getAttributeValue(addr.deserialize(_operand), mPool);
	
	if(!attr)
	{
		// Push Ersatz var
		mStack->push(TValue(0,0,0));
	}
	else
	{
		mStack->push(*attr);
	}
	
	return OK;
}

//--------------------------------------------------------------
int InstructionEvaluator::f_pop(int _operandLen, unsigned char* _operand)
{
#ifdef CEP_DEBUG	
	printf("InstructionEvaluator::f_pop\n");
#endif 
	mStack->pop(&mLatestStackValue);
	
	return OK;	
}

//--------------------------------------------------------------
int InstructionEvaluator::f_add(int _operandLen, unsigned char* _operand)
{
#ifdef CEP_DEBUG	
	printf("InstructionEvaluator::f_add\n");
#endif 

	TValue op1;
	if(!mStack->pop(&op1)) return ERR_IE_ABORT;

	TValue op2;
	if(!mStack->pop(&op2)) return ERR_IE_ABORT;
		
	TValue resVal = op1 + op2;
	
	mStack->push(resVal);
	
	return OK;
}

//--------------------------------------------------------------
int InstructionEvaluator::f_sub(int _operandLen, unsigned char* _operand)
{
#ifdef CEP_DEBUG	
	printf("InstructionEvaluator::f_sub\n");
#endif 

	TValue op1;
	if(!mStack->pop(&op1)) return ERR_IE_ABORT;

	TValue op2;
	if(!mStack->pop(&op2)) return ERR_IE_ABORT;
		
	TValue resVal = op1 - op2;
	
	mStack->push(resVal);
	
	return OK;

}

//--------------------------------------------------------------
int InstructionEvaluator::f_mul(int _operandLen, unsigned char* _operand)
{
#ifdef CEP_DEBUG	
	printf("InstructionEvaluator::f_mul\n");
#endif 

	TValue op1;
	if(!mStack->pop(&op1)) return ERR_IE_ABORT;

	TValue op2;
	if(!mStack->pop(&op2)) return ERR_IE_ABORT;
		
	TValue resVal = op1 * op2;
	
	mStack->push(resVal);
	
	return OK;
}

//--------------------------------------------------------------
int InstructionEvaluator::f_div(int _operandLen, unsigned char* _operand)
{
#ifdef CEP_DEBUG	
	printf("InstructionEvaluator::f_div\n");
#endif 

	TValue op1;
	if(!mStack->pop(&op1)) return ERR_IE_ABORT;

	TValue op2;
	if(!mStack->pop(&op2)) return ERR_IE_ABORT;


	// Division by 0
	if(op2 == 0)
	{
		f_trap(_operandLen, _operand);
		return ERR_IE_ABORT;
	}
		
	TValue resVal = op1 / op2;
	
	mStack->push(resVal);
	
	return OK;
}

//--------------------------------------------------------------
int InstructionEvaluator::f_eval(int _operandLen, unsigned char* _operand)
{
#ifdef CEP_DEBUG	
	printf("InstructionEvaluator::f_eval\n");
#endif 

	TValue op;
	if(!mStack->pop(&op)) return ERR_IE_ABORT;
	
	TValue resVal((int)(op != 0));
	mStack->push(resVal);
	
	return OK;
}

//--------------------------------------------------------------
int InstructionEvaluator::f_ceq(int _operandLen, unsigned char* _operand)
{
#ifdef CEP_DEBUG	
	printf("InstructionEvaluator::f_ceq\n");
#endif 

	TValue op1;
	if(!mStack->pop(&op1)) return ERR_IE_ABORT;

	TValue op2;
	if(!mStack->pop(&op2)) return ERR_IE_ABORT;
	
	TValue resVal((int)(op1 == op2));
	
	mStack->push(resVal);

	return OK;
}

//--------------------------------------------------------------
int InstructionEvaluator::f_cne(int _operandLen, unsigned char* _operand)
{
#ifdef CEP_DEBUG	
	printf("InstructionEvaluator::f_cne\n");
#endif 

	TValue op1;
	if(!mStack->pop(&op1)) return ERR_IE_ABORT;

	TValue op2;
	if(!mStack->pop(&op2)) return ERR_IE_ABORT;
	
	TValue resVal((int)(op1 != op2));
	
	mStack->push(resVal);

	return OK;
}

//--------------------------------------------------------------
int InstructionEvaluator::f_cge(int _operandLen, unsigned char* _operand)
{
#ifdef CEP_DEBUG	
	printf("InstructionEvaluator::f_cge\n");
#endif 

	TValue op1;
	if(!mStack->pop(&op1)) return ERR_IE_ABORT;

	TValue op2;
	if(!mStack->pop(&op2)) return ERR_IE_ABORT;
	
	TValue resVal((int)(op1 >= op2));
	
	mStack->push(resVal);

	return OK;
}

//--------------------------------------------------------------
int InstructionEvaluator::f_cgt(int _operandLen, unsigned char* _operand)
{
#ifdef CEP_DEBUG	
	printf("InstructionEvaluator::f_cgt\n");
#endif 

	TValue op1;
	if(!mStack->pop(&op1)) return ERR_IE_ABORT;

	TValue op2;
	if(!mStack->pop(&op2)) return ERR_IE_ABORT;
	
	TValue resVal((int)(op1 > op2));
	
	mStack->push(resVal);

	return OK;
}

//--------------------------------------------------------------
int InstructionEvaluator::f_cle(int _operandLen, unsigned char* _operand) 
{
#ifdef CEP_DEBUG	
	printf("InstructionEvaluator::f_cle\n");
#endif 

	TValue op1;
	if(!mStack->pop(&op1)) return ERR_IE_ABORT;

	TValue op2;
	if(!mStack->pop(&op2)) return ERR_IE_ABORT;
	
	TValue resVal((int)(op1 <= op2));
	
	mStack->push(resVal);

	return OK;
}

//--------------------------------------------------------------
int InstructionEvaluator::f_clt(int _operandLen, unsigned char* _operand) 
{
#ifdef CEP_DEBUG	
	printf("InstructionEvaluator::f_clt\n");
#endif 

	TValue op1;
	if(!mStack->pop(&op1)) return ERR_IE_ABORT;

	TValue op2;
	if(!mStack->pop(&op2)) return ERR_IE_ABORT;
	
	TValue resVal((int)(op1 < op2));
	
	mStack->push(resVal);

	return OK;
}

//--------------------------------------------------------------
int InstructionEvaluator::f_andl(int _operandLen, unsigned char* _operand)
{
#ifdef CEP_DEBUG	
	printf("InstructionEvaluator::f_andl\n");
#endif 

	TValue op1;
	if(!mStack->pop(&op1)) return ERR_IE_ABORT;
	
	f_eval(0,0);

	TValue op2;
	if(!mStack->pop(&op2)) return ERR_IE_ABORT;
	
	f_eval(0,0);

	TValue resVal((int)((op1 == op2) && (op1 == (int)1)));
	
	mStack->push(resVal);

	return OK;
}

//--------------------------------------------------------------
int InstructionEvaluator::f_orl(int _operandLen, unsigned char* _operand)
{
#ifdef CEP_DEBUG	
	printf("InstructionEvaluator::f_orl\n");
#endif 

	TValue op1;
	if(!mStack->pop(&op1)) return ERR_IE_ABORT;
	
	f_eval(0,0);

	TValue op2;
	if(!mStack->pop(&op2)) return ERR_IE_ABORT;
	
	f_eval(0,0);

	TValue resVal((int)((op1 == (int)1) || (op2 == (int)1)));
	
	mStack->push(resVal);

	return OK;
}

//--------------------------------------------------------------
int InstructionEvaluator::f_iarea(int _operandLen, unsigned char* _operand)
{
#ifdef CEP_DEBUG	
	printf("InstructionEvaluator::f_iarea\n");
#endif 

	//NOTE: variable types are expected to have been verified
	// in the parsing phase. They are assert()-ed in the debug version.

	TValue area;
	if(!mStack->pop(&area)) return ERR_IE_ABORT;
///	assert(area.t == t_area);
	
	TValue pos;
	if(!mStack->pop(&pos)) return ERR_IE_ABORT;
///	assert(pos.t == t_pos);
        
        if (area.t == t_area && pos.t == t_pos) {	
            TValue resVal((int)isInArea(area.v.areaVal, pos.v.posVal));
            mStack->push(resVal);

            return OK;            
        }

	return ERR_IE_ABORT;
} 

//--------------------------------------------------------------
int InstructionEvaluator::f_diff(int _operandLen, unsigned char* _operand)
{
    #ifdef CEP_DEBUG
            printf("InstructionEvaluator::f_diff\n");
    #endif

   TAddress addr;
   addr.deserialize(_operand);
        
    EComplexEventFunction* complexEventFunction = mEngine->findComplexEventFunction(addr.oid);
    
    ECapturedEvent* eCEvent = 0;
    if (mPool) 
        eCEvent = mPool->capturedEvents;
    assert(eCEvent);
    
    TValue *recentVal = mEngine->getAttributeValue(&complexEventFunction->address, mPool);   ///used to incoming value of complex function; 
    TValue groupVal = getAttributeToGroup(eCEvent->oid);
   
   short idxMax=1, idxMin= 1, idx=1;
  
    if (complexEventFunction) {
        
        short tend1= 0, tend2= 0;
        
        float max= recentVal->asFloat(), min=recentVal->asFloat();
        EEventAttributeValue *eEAttVal;
        while(eCEvent->prev) {
            
            eCEvent = eCEvent->prev;
            eEAttVal = mEngine->findEventAttributeValue(eCEvent->values, mPool->groupAttributeOid);
            if (mPool->groupAttributeOid == -1 || ((groupVal.t != t_ersatz && eEAttVal) && 
               (eEAttVal->val.operator ==(groupVal)))) {
                eEAttVal = mEngine->findEventAttributeValue(eCEvent->values, complexEventFunction->address.oid);

                if (eCEvent->oid == complexEventFunction->address.ownerOid) {
                    idx++;
                    if (eEAttVal->val.asFloat() > max) {
                        max = eEAttVal->val.asFloat();
                        idxMax = idx;
                        tend1++;
                    }
                    else if (eEAttVal->val.asFloat()< min) {
                        min = eEAttVal->val.asFloat();
                        idxMin = idx;
                        tend2++;
                    }
                }
            }
///            eCEvent = eCEvent->prev;
        }
        float rDiff=0.0;
/// ? ((max - min) / max) * (-100) : ((max - min) / min) * 100);
        if (tend1 == (idx-1) || tend2 == (idx-1)) {
            if (idxMax > idxMin)
                rDiff= (max == 0 ? 0 : ((max - min) / max) * -100);
            else 
                rDiff= (min == 0 ? 0 : ((max - min) / min) * 100);
        }
        else
            rDiff = 0;
        
        TValue resVal = rDiff;
        mStack->push(resVal);

        return OK;
    }
   
    return ERR_IE_ABORT;
}

//--------------------------------------------------------------
int InstructionEvaluator::f_diffx(int _operandLen, unsigned char* _operand)
{
	// This is a "real" complex event function. It takes into account the
	// time / tuple windows 
	return ERR_IE_ABORT;
    
///0j0 to improve; is a first draft only; copied of diff function
/*        
    TAddress addr;
    addr.deserialize(_operand);
        
    EComplexEventFunction* complexEventFunction = mEngine->findComplexEventFunction(addr.oid);
    
    ECapturedEvent* eCEvent;
    eCEvent = mPool->capturedEvents;
    assert(eCEvent);
        
   int cntE=0;

    if (complexEventFunction) {
        while(eCEvent) {
            if(eCEvent->oid == complexEventFunction->address.oid)
                cntE++;
            eCEvent = eCEvent->prev;
        }
    }
    TValue resVal = cntE;
    mStack->push(resVal);

    return OK;
*/
}

//--------------------------------------------------------------
//RP: 18122014 count an event if it appear in the detect statement of the complex event 
int InstructionEvaluator::f_cnt(int _operandLen, unsigned char* _operand)
{
///	return ERR_IE_ABORT;
        
    TAddress addr;
    addr.deserialize(_operand);
        
    EComplexEventFunction* complexEventFunction = mEngine->findComplexEventFunction(addr.oid);
    
    ECapturedEvent *eCEvent;
    eCEvent = mPool->capturedEvents;
    assert(eCEvent);
    
    TValue groupVal= getAttributeToGroup(eCEvent->oid);
    int cntE=0;
    EEventAttributeValue *eEAttVal;
    if (complexEventFunction) {
        while(eCEvent) {                           
            eEAttVal= mEngine->findEventAttributeValue(eCEvent->values, mPool->groupAttributeOid);            
            if (mPool->groupAttributeOid == -1 || ((groupVal.t != t_ersatz && eEAttVal) && 
               (eEAttVal->val.operator ==(groupVal)))) {
                if (eCEvent->oid == complexEventFunction->address.oid)
                    cntE++;
            }
            
            eCEvent = eCEvent->prev;
        }        
        TValue resVal = cntE;
        mStack->push(resVal);
        
///        delete eCEvent;

       return OK;
    }    
    return ERR_IE_ABORT;    
}

//--------------------------------------------------------------
int InstructionEvaluator::f_sum(int _operandLen, unsigned char* _operand)
{
    #ifdef CEP_DEBUG	
            printf("InstructionEvaluator::f_sum\n");
    #endif
    TAddress addr;
    addr.deserialize(_operand);
    
    EComplexEventFunction* complexEventFunction = mEngine->findComplexEventFunction(addr.oid);
    ECapturedEvent *eCEvent = mPool->capturedEvents;
    assert(eCEvent);
    
    TValue groupVal = getAttributeToGroup(eCEvent->oid);
    
    float temp = 0;
    EEventAttributeValue *eEAttVal;
    
    if (complexEventFunction) {
        while(eCEvent) {
            eEAttVal = eCEvent->values;
            eEAttVal = mEngine->findEventAttributeValue(eCEvent->values, mPool->groupAttributeOid);
            if (mPool->groupAttributeOid == -1 || ((groupVal.t != t_ersatz && eEAttVal) &&
                (eEAttVal->val.operator ==(groupVal)))) {
                eEAttVal = mEngine->findEventAttributeValue(eCEvent->values, complexEventFunction->address.oid);
                if(eCEvent->oid == complexEventFunction->address.ownerOid) {
                    temp = temp + eEAttVal->val.asFloat();
                }
          }
          eCEvent = eCEvent->prev;
        }
        TValue resVal = temp;
        mStack->push(resVal);
        return OK;               
    }
    
    return ERR_IE_ABORT;
}
    

//--------------------------------------------------------------
int InstructionEvaluator::f_avg(int _operandLen, unsigned char* _operand)
{
    #ifdef CEP_DEBUG	
            printf("InstructionEvaluator::f_avg\n");
    #endif
   TAddress addr;
    addr.deserialize(_operand);
    
    EComplexEventFunction* complexEventFunction = mEngine->findComplexEventFunction(addr.oid);
    ECapturedEvent *eCEvent = mPool->capturedEvents;
    assert(eCEvent);
    
    TValue groupVal = getAttributeToGroup(eCEvent->oid);
    
    float temp = 0;
    int count = 0;
    EEventAttributeValue *eEAttVal;
    
    if (complexEventFunction) {
        while(eCEvent) {
            eEAttVal = eCEvent->values;
            eEAttVal = mEngine->findEventAttributeValue(eCEvent->values, mPool->groupAttributeOid);                
            if (mPool->groupAttributeOid == -1 || ((groupVal.t != t_ersatz && eEAttVal) && 
               (eEAttVal->val.operator ==(groupVal)))) {
                eEAttVal = mEngine->findEventAttributeValue(eCEvent->values, complexEventFunction->address.oid);
                if(eCEvent->oid == complexEventFunction->address.ownerOid) {
                    temp = temp + eEAttVal->val.asFloat();
                    count++;
                }
          }
          eCEvent = eCEvent->prev;
        }
        
        TValue resVal;
        if (count == 0) {
            resVal= 0;
            mStack->push(resVal);
            return ERR_IE_ABORT;
        }
        else {
            resVal= temp/count;
            mStack->push(resVal);
        }                
        return OK;               
    }    
    return ERR_IE_ABORT;

}

//--------------------------------------------------------------
int InstructionEvaluator::f_abs(int _operandLen, unsigned char* _operand)
{
#ifdef CEP_DEBUG
	printf("InstructionEvaluator::f_abs\n");
#endif

	TValue op1;
	if(!mStack->pop(&op1)) return ERR_IE_ABORT;

	TValue resVal = op1.TV_abs();

	mStack->push(resVal);

	return OK;
}

//--------------------------------------------------------------
int InstructionEvaluator::f_fft(int _operandLen, unsigned char* _operand) 
{
#ifdef CEP_DEBUG	
    printf("InstructionEvaluator::f_fft\n");
#endif
    TAddress addr;
    addr.deserialize(_operand);

    EComplexEventFunction* complexEventFunction = mEngine->findComplexEventFunction(addr.oid);
    ECapturedEvent *eCEvent = mPool->capturedEvents;
    assert(eCEvent);

    if (!mPool || !complexEventFunction)
        return ERR_IE_ABORT;

    TValue groupVal = getAttributeToGroup(eCEvent->oid);

    int idx = 0;

    float temp = 0;
    int count = 0;

    EEventAttributeValue *eEAttVal;

    if (complexEventFunction) 
    {
        while (eCEvent) 
        {
            eEAttVal = eCEvent->values;
            eEAttVal = mEngine->findEventAttributeValue(eCEvent->values, mPool->groupAttributeOid);

            if (mPool->groupAttributeOid == -1 || ((groupVal.t != t_ersatz && eEAttVal) &&
                (eEAttVal->val.operator ==(groupVal)))) 
            {
                eEAttVal = mEngine->findEventAttributeValue(eCEvent->values, complexEventFunction->address.oid);
                if (eCEvent->oid == complexEventFunction->address.ownerOid) 
                {
                    temp = temp + eEAttVal->val.asFloat();
                    count++;
                }
            }
            idx++;

            eCEvent = eCEvent->prev;
        }
        if (idx == 0)
            return ERR_IE_ABORT;
        else 
        {
            TValue resVal = func_c_fft_cluster(idx);
            mStack->push(resVal);
        }
    }

    return OK;
}

int InstructionEvaluator::f_traj(int _operandLen, unsigned char* _operand)
{
#ifdef CEP_DEBUG	
    printf("InstructionEvaluator::f_traj\n");
#endif
    TAddress addr;
    addr.deserialize(_operand);

    EComplexEventFunction* complexEventFunction = mEngine->findComplexEventFunction(addr.oid);
    ECapturedEvent *eCEvent = mPool->capturedEvents;
    assert(eCEvent);

    if (!mPool || !complexEventFunction)
        return ERR_IE_ABORT;

    TValue groupVal = getAttributeToGroup(eCEvent->oid);

    float x, xPrev, xTemp = 0.0;
    float y, yPrev, yTemp = 0.0;
    float t, tPrev, tTemp = 0.0;  // time
    short timeThroughLoop = 0;

    EEventAttributeValue *eEAttVal;

    if (complexEventFunction)
    {
        while (eCEvent)
        {
            eEAttVal = eCEvent->values;
            eEAttVal = mEngine->findEventAttributeValue(eCEvent->values, mPool->groupAttributeOid);

            if (mPool->groupAttributeOid == -1 || 
                ((groupVal.t != t_ersatz && eEAttVal) && (eEAttVal->val.operator ==(groupVal))))
            {
                eEAttVal = mEngine->findEventAttributeValue(eCEvent->values, 
                        complexEventFunction->address.oid);
                if (eCEvent->oid == complexEventFunction->address.ownerOid)
                {
                    if (eEAttVal)
                    {
                        TValue val = eEAttVal->val;
                        if (val.isValid() && val.t == t_float)  // x coordinate
                        {
                            xTemp = eEAttVal->val.asFloat();

                            EEventAttributeValue* pPrev = eEAttVal->prev;
                            if (pPrev)
                            {
                                val = pPrev->val;
                                if (val.isValid() && val.t == t_float)  // y coordinate
                                {
                                    yTemp = eEAttVal->prev->val.asFloat();

                                    pPrev = pPrev->prev;
                                    if (pPrev)
                                    {
                                        val = pPrev->val;
                                        if (val.isValid() && val.t == t_float)  // time
                                        {
                                            tTemp = val.asFloat();
                                        }
                                    }
                                }
                            }
                        }

                        if (timeThroughLoop == 0)
                        {
                            x = xTemp;
                            y = yTemp;
                            t = tTemp;
                        }
                        else if (timeThroughLoop == 1)
                        {
                            xPrev = xTemp;
                            yPrev = yTemp;
                            tPrev = tTemp;
                        }
                    }
                }
            }

            eCEvent = eCEvent->prev;
            ++timeThroughLoop;
        }

        if (timeThroughLoop >= 2)
        {
            TValue resVal = func_calc_trajectory(x, y, t, xPrev, yPrev, tPrev);
            mStack->push(resVal);
        }

        return OK;
    }

    return ERR_IE_ABORT;
}

int InstructionEvaluator::f_coll(int _operandLen, unsigned char* _operand)
{
#ifdef CEP_DEBUG	
    printf("InstructionEvaluator::f_coll\n");
#endif
    TAddress addr;
    addr.deserialize(_operand);

    EComplexEventFunction* pEComplexEventFunction = mEngine->findComplexEventFunction(addr.oid);
    ECapturedEvent* pECapEvent = mPool->capturedEvents;
    assert(pECapEvent);

    if (!mPool || !pEComplexEventFunction)
        return ERR_IE_ABORT;

    //TValue groupVal = getAttributeToGroup(eCEvent->oid);
    EEventAttributeValue* pEEAttVal;

    if (pEComplexEventFunction)
    {
        while (pECapEvent)
        {
            TType ttype;
            TValue tval;
            int obj_id = -1;
            pEEAttVal = pECapEvent->values;

            if (pEEAttVal)
            {
                tval = pEEAttVal->val;
                ttype = tval.t;
                if (ttype == t_int)
                {
                    obj_id = tval.v.integerVal;

                    if (GetCarByID(obj_id) == nullptr)
                    {
                        // new car
                        Car* pCar = new Car();

                        if (obj_id == 1) // ego
                        {
                            //pCar->_width = 40.0;
                            //pCar->_length = 80.0;
                            pCar->_width = 4.0;
                            pCar->_length = 8.0;
                        }
                        else if (obj_id == 2 || obj_id == 3)
                        {
                            //pCar->_width = 80.0;
                            //pCar->_length = 40.0;
                            pCar->_width = 8.0;
                            pCar->_length = 4.0;
                        }

                        _cars[obj_id] = pCar;
                    }

                    pEEAttVal = pEEAttVal->prev;
                    tval = pEEAttVal->val;
                    ttype = tval.t;
                    if (ttype == t_string)
                    {
                        pEEAttVal = pEEAttVal->prev;

                        std::string s_type = tval.v.stringVal;

                        if (s_type == "L")
                        {
                            getLidarLine(pEEAttVal, obj_id);
                        }
                        else if (s_type == "R")
                        {
                            //GetRadarLine();
                            assert(false);
                        }
                        else
                        {
                            assert(false);
                        }

                        TValue resVal;
                        if (obj_id == 1)    // ego vehicle
                        {
                            bool coll = testForCollisionsWithOtherVehicles(_cars[obj_id]);
                            if (coll)
                            {
                                resVal = (float)_cars[obj_id]->GetCurrentPosition().second;
                                mStack->push(resVal);
                            }
                        }
                    }
                }
            }

            pECapEvent = pECapEvent->prev;
        }

        return OK;
    }

    return ERR_IE_ABORT;
}

//--------------------------------------------------------------
int InstructionEvaluator::f_reset(int _operandLen, unsigned char* _operand)
{ 
	return ERR_IE_ABORT;
}


int InstructionEvaluator::f_trap(int _operandLen, unsigned char* _operand)
{ 
	//FIXME: clear stack, push int 0 onto the stack and return.
	return ERR_IE_ABORT;
}


int InstructionEvaluator::f_nop(int _operandLen, unsigned char* _operand) 
{
	// do nothing
	return OK;
}


//---- Auxiliary functions

// FIXME: REFACTOR MOVE TO COMMON!!!
bool InstructionEvaluator::isInArea(const TAreaLiteral& _area, 
									const TPosLiteral& _pos)
{
	// handle rect
	if(_area.type == TAreaLiteral::KAreaRectangular)
	{
		return 	(_pos.lat > _area.lat)
			&&	(_pos.lat < _area.lat2)
			&&  (_pos.lon > _area.lon)
			&&	(_pos.lon < _area.lon2);
	}
	else	
	{
		// handle circle
		float squareDist = 	powf(_area.lat - _pos.lat, 2.0) 
							+ powf(_area.lon - _pos.lon, 2.0);
	
		return squareDist <= powf(_area.radius, 2.0);
	}

	assert(0);
	return false; // not reached.
}

#ifdef CEP_DEBUG


//FIXME: Move this to a unit test infrastructure
/*
void InstructionEvaluator::unittest()
{
	TAreaLiteral rectArea = makeArea(10.0,10.0, 20.0,20.0);
	TAreaLiteral rectArea2 = makeArea(-45.0, -45.0, +45.0, +45.0);
	TAreaLiteral circArea = makeArea(11.0,11.0, 20.0);
	TAreaLiteral circArea2 = makeArea(0.0, 0.0, 20.0);
	TPosLiteral pos = makePos(11.0, 11.0);
	TPosLiteral pos2 = makePos(9.0, 9.0);
	
	printf("TEST-1:");
	if(isInArea(rectArea, pos)) printf("OK\n"); else printf("FAIL\n");

	printf("TEST-2:");
	if(!isInArea(rectArea, pos2)) printf("OK\n"); else printf("FAIL\n");
	
	printf("TEST-3:");
	if(isInArea(circArea, pos)) printf("OK\n"); else printf("FAIL\n");
	
	printf("TEST-4:");
	if(!isInArea(circArea, makePos(30.0,30.0))) printf("OK\n"); else printf("FAIL\n");

	printf("TEST-5:");
	if(isInArea(circArea2, makePos(-3.0,-3.0))) printf("OK\n"); else printf("FAIL\n");

	printf("TEST-6:");
	if(isInArea(rectArea2, makePos(-31.0,-36.0))) printf("OK\n"); else printf("FAIL\n");

	printf("TEST-7:");
	if(!isInArea(rectArea2, makePos(-31.0,67.0))) printf("OK\n"); else printf("FAIL\n");
	
	
	
}


TAreaLiteral InstructionEvaluator::makeArea(
						const float _lat, 
						const float _lon, 
						const float _lat2, 
						const float _lon2)
{
	TAreaLiteral a;
	
	a.type = TAreaLiteral::KAreaRectangular;
	a.lat = _lat;
	a.lon = _lon;
	a.lat2 = _lat2;
	a.lon2 = _lon2;
	
	return a;
}


TAreaLiteral InstructionEvaluator::makeArea(
						const float _lat, 
						const float _lon,
						const float _radius)
{
	TAreaLiteral a;
	
	a.type = TAreaLiteral::KAreaCircular;
	a.lat = _lat;
	a.lon = _lon;
	a.radius = _radius;
	
	return a;
}


TPosLiteral InstructionEvaluator::makePos(
						const float _lat, 
						const float _lon)
{
	TPosLiteral p;
	
	p.lat = _lat;
	p.lon = _lon;

	return p;
}

*/
#endif

///RP: 
TValue InstructionEvaluator::getAttributeToGroup(long _oid) {
    
    TAddress addr;
    
    TValue *groupVal, groupoValTR;
    
    if (mPool->groupAttributeOid > 0) {
        addr.ownerOid = _oid;
        addr.oid = mPool->groupAttributeOid;        
        groupVal = mEngine->getAttributeValue(&addr, mPool); //used to get the group of the comming event.
        if ((groupVal->t == t_int || groupVal->t == t_char || groupVal->t == t_string)) {
            groupoValTR.t = groupVal->t; 
            groupoValTR.v = groupVal->v;
        }
        else
            groupoValTR.t = t_ersatz;
    }
    
    return groupoValTR;

}

float InstructionEvaluator::func_c_fft_cluster(int size)
{
    float retFloat = 0;

    if (size < FFT_SIZE)
        return 0;

#ifdef PARALLEL_MODE
    //
    //	#pragma omp target device(_idxTargetCluster) map(to:_captEventsArray[0:_size], _size, _oidGroupId, _valIntGroup) map(from:retFloat)
    //retFloat = func_c_fft(_captEventsArray, _size, _oidGroupId, _valIntGroup, 16);
    retFloat = func_c_fft();
#else
    // time it
    std::clock_t start = std::clock();

    //int number_cores = 4;
    //for (int i = 0; i < number_cores; ++i)
    //#pragma omp parallel num_threads(4)
    {
        //int num_threads = omp_get_num_threads();
        retFloat = PCEPFunctions::func_c_fft();
    }

    // print time
    std::cout << "Time: " << (std::clock() - start) / double(CLOCKS_PER_SEC / 1000) << " ms - retFloat = "; 
    std::cout << retFloat;

#endif

    return retFloat;
}

bool InstructionEvaluator::func_calc_trajectory(float x2, float y2, float t2, 
        float x1, float y1, float t1)
{
    bool res = 0;

    // time it
    std::clock_t start = std::clock();

    //res = PCEPFunctions::func_calc_trajectory(x2, y2, t2, x1, y1, t1);

    // print time
    std::cout << "Time: " << (std::clock() - start) / double(CLOCKS_PER_SEC / 1000) << std::endl;

    //if (res == true)
    //{
    //    std::cout << "{(x1, y1), (x2, y2)} = {(" << x1 << ", " << y1;
    //    std::cout << "), (" << x2 << ", " << y2 << ")" << std::endl;
    //    std::cout << "Passes through (0, 0)" << std::endl;
    //}

    return res;
}

bool InstructionEvaluator::func_obj_collision(float x2, float y2, float t2,
    float x1, float y1, float t1)
{
    bool res = 0;

    // time it
    std::clock_t start = std::clock();

    //res = PCEPFunctions::func_calc_trajectory(x2, y2, t2, x1, y1, t1);

    // print time
    std::cout << "Time: " << (std::clock() - start) / double(CLOCKS_PER_SEC / 1000) << std::endl;

    //if (res == true)
    //{
    //    std::cout << "{(x1, y1), (x2, y2)} = {(" << x1 << ", " << y1;
    //    std::cout << "), (" << x2 << ", " << y2 << ")" << std::endl;
    //    std::cout << "Passes through (0, 0)" << std::endl;
    //}

    return res;
}


Car* InstructionEvaluator::GetCarByID(const int& id)
{
    if (_cars.find(id) == _cars.end())
        return nullptr;
    else
        return _cars[id];
}

void InstructionEvaluator::getLidarLine(EEventAttributeValue* pEEAttVal, 
    const int& obj_id)
{
    int idx = 0;
    double loc_x, loc_y;
    long long lltime;

    while (pEEAttVal)
    {
        TValue tval = pEEAttVal->val;
        TType ttype = tval.t;
        if (ttype == t_float && idx == 0)   // loc_x
        {
            loc_x = static_cast<double>(tval.v.floatVal);
        }
        else if (ttype == t_float && idx == 1)   // loc_y
        {
            loc_y = static_cast<double>(tval.v.floatVal);
        }
        else if (ttype == t_float && idx == 2)  // time
        {
            _cars[obj_id]->SetCurrentPosition(loc_x, loc_y);
            DEBUG() << obj_id << "\t" << loc_x << "\t" << loc_y << std::endl;
        }

        pEEAttVal = pEEAttVal->prev;
        idx++;
    }
}

bool InstructionEvaluator::testForCollisionsWithOtherVehicles(Car* pEgo)
{
    bool bRes = false;

    std::pair<double, double> ac = pEgo->GetCurrentPosition();
    std::pair<double, double> ar = std::make_pair(pEgo->_width, pEgo->_length);

    ac.first += 13.5;
    ac.second += 2.5;

    //
    // from Christer:
    // Two AABBs only overlap if they overlap on all three axes, where their extent along..
    // ..each dimension is seen as an interval on the corresponding axis.
    //

    auto it = _cars.begin();
    while (it != _cars.end())
    {
        if (it->first != 1)
        {
            auto bc = it->second->GetCurrentPosition();
            std::pair<double, double> br = std::make_pair(_cars[2]->_width, _cars[2]->_length);

            if (it->first == 2)
            {
                bc.first += 2.5;
                bc.second += 21.0;
            }
            else if (it->first == 3)
            {
                bc.first += 2.5;
                bc.second += 13.5;
            }

            if ((std::abs<double>(ac.first - bc.first) <= (ar.first / 2 + br.first / 2)) &&
                (std::abs<double>(ac.second - bc.second) <= (ar.second / 2 + br.second / 2)))
                bRes = true;
        }
        ++it;
    }

    return bRes;
}
