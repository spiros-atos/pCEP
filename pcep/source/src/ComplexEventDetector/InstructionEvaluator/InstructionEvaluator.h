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

	

#ifndef _SOL_CEP_INSTRUCTION_EVALUATOR_H_
#define _SOL_CEP_INSTRUCTION_EVALUATOR_H_


struct EProgram;
class ECapturedEvent;
struct ECapturedEventPool;

class Stack;
class Engine;

#include "../../Types/TValue.h"
#include "Stack.h"

#include <vector>
#include <map>
#include "../../CollisionDetection/Car.h"
struct EEventAttributeValue;

/*!
	\brief Executes code generated with ProgramBuilder.
	
	Implemented as a simple stack machine that grinds through the code
	and updates the stack accordingly
*/
class InstructionEvaluator
{
public:
	InstructionEvaluator(Engine* const _engine);
					
	~InstructionEvaluator();
	
	
	/*!
		\brief Executes an instruction
		\param _operandLen Length of operand
		\param _operand the operand data
		\returns ERR_IE_ABORT if the engine for some reason cannot continue, OK otherwise
	*/
	typedef int (InstructionEvaluator::*TInstructionFunc)(int _operandLen, unsigned char* _operand);
	
	//! Maps opcodes to implementations
	struct TInstructionMap
	{
		unsigned char opcode;
		TInstructionFunc func;
	};
	
	//!@{
	//! Must be sized according to the set of opcodes (see below)
	static const int KMaxInstruction = 31;
	TInstructionMap mInstructions[KMaxInstruction]; 
	//!@}
	
	
	/*!
		\brief Runs a program
		\param _program The program to run
		\param _pool The captured events against which to operate
		\return OK on success
	*/
	int run(EProgram* _program, ECapturedEventPool* _pool);
        
        ///RP:20150211
        int runWithoutComplexFunctions(	EProgram* _program,ECapturedEventPool* _pool);
	
	TValue* const getTopOfStack() const;
        
///RP:        
        bool popStack(TValue* val_) const;
	
	//! Change this when instruction set changes.
	static const unsigned long SOLCEP_PROGRAM_VERSION = 0x00010000;

	//!@{ 
	//! Instruction set constants
	static const unsigned char HAS_OPERAND_LENGTH = 0x80;
	static const int OPCODE_SIZE = sizeof(unsigned char);
	static const int OPERAND_LENGTH_SIZE = sizeof(unsigned char); //! instruction data size - size
	//!@}

	//!@{
	//! opcodes
	static const unsigned char PUSH = 0x80;
	static const unsigned char PUSHX = 0x81;
	static const unsigned char PUSHA = 0x82;
	static const unsigned char POP = 0x03;
	static const unsigned char ADD = 0x10;
	static const unsigned char SUB = 0x11;
	static const unsigned char MUL = 0x12;
	static const unsigned char DIV = 0x13;
	static const unsigned char EVAL = 0x14;
	static const unsigned char CEQ = 0x20;
	static const unsigned char CNE = 0x21;
	static const unsigned char CGE = 0x22;
	static const unsigned char CGT = 0x23;
	static const unsigned char CLE = 0x24;
	static const unsigned char CLT = 0x25;
	static const unsigned char ANDL = 0x30;
	static const unsigned char ORL = 0x31;
	static const unsigned char IAREA = 0x40;
	static const unsigned char DIFF = 0xC1;
	static const unsigned char DIFFX = 0x42;
	static const unsigned char CNT = 0xC3;
	static const unsigned char SUM = 0xC4;
	static const unsigned char AVG = 0xC5;
	static const unsigned char ABS = 0xC6;
    static const unsigned char FFT = 0xC7;
    static const unsigned char TRAJ = 0xC8;
    static const unsigned char COLL = 0xC2;
    static const unsigned char RESET = 0x70;
	static const unsigned char TRAP = 0x76;
	static const unsigned char NOP = 0x77;	
	//!@}
	
protected:

    float func_c_fft_cluster(int size);
    bool func_calc_trajectory(float x1, float y1, float t1, float x2, float y2, float t2);
    bool func_obj_collision(float x1, float y1, float t1, float x2, float y2, float t2);

    void reset();

	void initInstructions();
	void initInstruction(	const int _index, 
							const unsigned char _opcode,
							TInstructionFunc func);
							
							
	int doInstruction(	const unsigned char _opcode,
						const int _operandLen,
						const unsigned char* _operand);
	
	//!@{
	//! Instruction implementations
	int f_push	(int _operandLen, unsigned char* _operand);
	int f_pushx	(int _operandLen, unsigned char* _operand);
	int f_pusha	(int _operandLen, unsigned char* _operand);
	int f_pop	(int _operandLen, unsigned char* _operand);
	int f_add	(int _operandLen, unsigned char* _operand);
	int f_sub	(int _operandLen, unsigned char* _operand);
	int f_mul	(int _operandLen, unsigned char* _operand);
	int f_div	(int _operandLen, unsigned char* _operand);
	int f_eval	(int _operandLen, unsigned char* _operand);
	int f_ceq	(int _operandLen, unsigned char* _operand);
	int f_cne	(int _operandLen, unsigned char* _operand);
	int f_cge	(int _operandLen, unsigned char* _operand);
	int f_cgt	(int _operandLen, unsigned char* _operand);
	int f_cle	(int _operandLen, unsigned char* _operand);
	int f_clt	(int _operandLen, unsigned char* _operand);
	int f_andl	(int _operandLen, unsigned char* _operand);
	int f_orl	(int _operandLen, unsigned char* _operand);
	int f_iarea	(int _operandLen, unsigned char* _operand);
	int f_diff	(int _operandLen, unsigned char* _operand);
	int f_diffx	(int _operandLen, unsigned char* _operand);
	int f_cnt	(int _operandLen, unsigned char* _operand);
	int f_sum	(int _operandLen, unsigned char* _operand);
	int f_avg	(int _operandLen, unsigned char* _operand);
	int f_reset	(int _operandLen, unsigned char* _operand);
	int f_trap	(int _operandLen, unsigned char* _operand);
	int f_nop	(int _operandLen, unsigned char* _operand);
	int f_abs	(int _operandLen, unsigned char* _operand);
    int f_fft   (int _operandLen, unsigned char* _operand);
    int f_traj  (int _operandLen, unsigned char* _operand);
    int f_coll  (int _operandLen, unsigned char* _operand);

	//!@}
	
	
	unsigned long mPC;	//! Program counter
	
	Stack* mStack; //! Stack
	Engine* const mEngine; 
	ECapturedEventPool* mPool;
	
	TValue mLatestStackValue;

	bool isInArea(const TAreaLiteral& _area, const TPosLiteral& _pos);

#ifdef CEP_DEBUG
	TAreaLiteral makeArea(
						const float _lat, 
						const float _lon, 
						const float _lat2, 
						const float _lon2);

	TAreaLiteral makeArea(
						const float _lat, 
						const float _lon,
						const float _radius);
						
	TPosLiteral makePos(const float _lat, 
						const float _lon);						
#endif


private:

	void unittest();
        
    //RP: 20150211
    bool isComplexFunction (unsigned char _opcode);        
    TValue getAttributeToGroup(long _oid);
        
    std::vector<DataPoint> _vct_data;
    Car* GetCarByID(const int& id);
    std::map<int, Car*> _cars;

    void getLidarLine(EEventAttributeValue* pEEAttVal, const int& obj_id);

    bool testForCollisionsWithOtherVehicles(Car* pEgo);
};

#endif
