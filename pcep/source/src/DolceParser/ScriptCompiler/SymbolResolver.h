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

#if 0
/*
	Candidate checks: FIXME 
	
		- type checking
		- duplicate identifiers
		- matching types in evaluations
		- matching types with functions
			e.g. count(e) e must be event, not attribute
		- warn for overflow when matching types 
		- warn for out-of-bounds pos / area
		- error for out-of-bounds abs time
		
*/

#ifndef _SOL_CEP_SYMBOL_RESOLVER_H_
#define _SOL_CEP_SYMBOL_RESOLVER_H_

#include <stdio.h>

#include "ASTree.h"
#include "SymbolTable.h"

class ErrorList;

class SymbolResolver
{
public:

	SymbolResolver();
	~SymbolResolver();
	
	int resolve(	ASTree* const _tree, 
					SymbolTable* _symTab,
					ErrorList* const _errList);
	
	
protected:

	typedef int (SymbolResolver::*TRuleFunc)(int x, int z);
	static const int KMaxRule = 10;
	TRuleFunc mRuleFunctions[KMaxRule]; /* !! MAKE SURE this is big enough */
	
	/*
		Does a sanity check before starting
	*/
	int preSanityCheck();

	/*
		Does a sanity check when finished
	*/
	int postSanityCheck();

	/*
		Establishes a relationship between the attributes declarations 
		and their owning event or complex event
	*/
	int resolveAttributeDeclarationScope(int y, int z);
	
	/*
		Checks for duplicate declarations - which are illegal
	*/
	int checkForDuplicateDeclarations(int y, int z);

	/*
		Establishes a relationship between the attributes declarations 
		and their types
	*/
	int resolveAttributeTypes(int y, int z); //TODO
	

	//SymbolResolver(const SymbolResolver& rhs)	{}
	SymbolResolver& operator= (const SymbolResolver &rhs)	{}
	
	
private:

	int resolveAttributeDeclarationScope(	SymbolTable::Symbol* _sym,
											ASTree::NodeType _nodeType,
											SymbolTable::Type _type);
											
	

	ASTree* mASTree;
	SymbolTable* mSymTab;
	ErrorList* mErrorList;

	void init();
};

#endif 

#endif
