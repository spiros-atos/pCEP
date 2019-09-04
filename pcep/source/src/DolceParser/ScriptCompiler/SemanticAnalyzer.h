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




#ifndef _SOL_CEP_SEMANTIC_ANALYZER_H_
#define _SOL_CEP_SEMANTIC_ANALYZER_H_

///#include <stdio.h>

#include "SymbolTable.h"

class ASTree;
class Program;
class ErrorList;

/*!
	\brief Semantic analyzer.
	
	Goes through the ASTree to make sure everything is correct.
	(are all symbols resolved, do the referenced events exist in the events, etc.
	
	
*/
class SemanticAnalyzer
{
public:

	SemanticAnalyzer();
	~SemanticAnalyzer();
	
	typedef int (SemanticAnalyzer::*TRuleFunc)(int x, int z);
	static const int KMaxRule = 10;
	TRuleFunc mRuleFunctions[KMaxRule]; /* !! MAKE SURE this is big enough */

	int analyze(	ASTree* const _tree, 
					SymbolTable* const _symTab,
					ErrorList* const _errList);
	
	
protected:


private:

	void init();

	/*!
		\brief Collects event, complex event and external declarations from the AST
		\param x default function ptr parameter
		\param y default function ptr parameter
	*/
	int collectDeclarations(int x, int y);
	
	/*
	- postfix
		- is the scope-part correctly defined as a event/complex-event in relation to the context 
			- where-clause, group, payload: only those events that are used in the detect-statement
			- accept: scope-part must refer to the event
			
			
	- non-postfix
		- is the scope unambiguous?
			- where-clause, group, payload: if there's more than one event, there's ambiguity
		- 
	*/
	int resolveReferences(int x, int y);
	
	/* 
		Checks if all references have been resolved
	*/
	int checkResolutionCompleteness(int x, int y);
	
	//
	int removeDuplicateReferences(int x, int y);
	
	int verifyDataTypes(int x, int y);
	
	
	int collectDeclarations(ASTree* const _tree);
	
	int resolveReferences(ASTree* const _tree);
	int resolvePostfixReference(ASTree* const _ref);
	int resolveReferencesForComplexEvent(ASTree* const _ref);
	int resolveReferencesForEvent(ASTree* const _ref);
	
	/*
		Resolves any reference that depends on the events or complex events
		declared in the DETECT statement (payload, where-clause, group)
	*/
	int resolveReferencesSubordinateToDetectStatement(	ASTree* const _detectNode,
														ASTree* const _idNode);
	
	ASTree* getScopingEventNode(ASTree* const _node);
	
	bool checkIfExternal(SymbolTable::Symbol* _sym);
	
	// The 'lasts' statement is used in both events and complex events
	int resolveLastsStatementReference(ASTree* const _ref);
	
	bool isTreeResolved(ASTree* const _tree);
	
	/*
		Checks if more than one different event name is used 
		in a detect-statement
	*/
	bool hasMoreThanOneEventInDetectStatement(ASTree* const _detectNode, ASTree* const _otherNode = 0);
	
	ASTree* mASTree;
	SymbolTable* mSymTab;
	ErrorList* mErrorList;
	
};

#endif 

