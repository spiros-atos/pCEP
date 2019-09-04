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


#include "SemanticAnalyzer.h"

///#include <stdio.h>
///#include <assert.h>
#include "../../Common/Err.h"
#include "../DolceCompilerCodes.h"


#include "ASTree.h"
#include "ErrorList.h"




SemanticAnalyzer::SemanticAnalyzer()
{
	init();
}

SemanticAnalyzer::~SemanticAnalyzer()
{
}


void SemanticAnalyzer::init()
{
	int i = 0;

	mRuleFunctions[i++] = &SemanticAnalyzer::collectDeclarations;
	mRuleFunctions[i++] = &SemanticAnalyzer::resolveReferences;
	mRuleFunctions[i++] = &SemanticAnalyzer::checkResolutionCompleteness;

	mRuleFunctions[i++] = 0;
	
	// just in case we haven't crashed already due to out-of-bounds init above
	assert(i <= KMaxRule); 

}

	
int SemanticAnalyzer::analyze(	ASTree* const _tree, 
								SymbolTable* _symTab,
								ErrorList* const _errList)
{
#ifdef CEP_DEBUG
	printf("++++++++++ SemanticAnalyzer::resolve\n");
#endif
	mASTree = _tree;
	mSymTab = _symTab;
	mErrorList = _errList;
	
	
	int res = OK;


	int i = 0;
	while(mRuleFunctions[i] != 0)
	{
		res |= (*this.*mRuleFunctions[i])(1,3);
		i++;
	}

	if(res == OK)
	{
		if(mErrorList->hasError())
			res = ERR_PA_PARSE_ERRORS;
	}
	
	return res;
}



//-------------------------------------------------------------------

int SemanticAnalyzer::removeDuplicateReferences(int x, int y)
{
	//FIXME Implement

	return OK;
}

//-------------------------------------------------------------------

int SemanticAnalyzer::collectDeclarations(int x, int y)
{
#ifdef CEP_DEBUG
	printf(">>>>> SemanticAnalyzer::collectDeclarations \n");
#endif

	collectDeclarations(mASTree);

#ifdef CEP_DEBUG
	mSymTab->print();
	printf("<<<<< SemanticAnalyzer::collectDeclarations \n");
#endif	
	return OK;
}

//-------------------------------------------------------------------

int SemanticAnalyzer::resolveReferences(int x, int y)
{
#ifdef CEP_DEBUG
	printf("++++++++++ SemanticAnalyzer::resolveReferences\n");
#endif	

	resolveReferences(mASTree);
	
	return OK;
}


int SemanticAnalyzer::checkResolutionCompleteness(int x, int y)
{
	int res = OK;
	
	SymbolTableItr itr(mSymTab);
	
	SymbolTable::Symbol* sym = itr.first();

	while(sym)
	{
		if(sym->symbolType == SymbolTable::reference)
		{
			if(!sym->decl)
			{
#ifdef CEP_DEBUG
				printf("Symbol %s in line %d (%p) is not resolved\n", sym->name, sym->lineNum, sym);
#endif
				res |= ERR_PA_UNRESOLVED_REFERENCE;
			
			}
		}
		else if(sym->symbolType == SymbolTable::unknown)
		{
#ifdef CEP_DEBUG
			printf("Symbol %s in line %d (%p) is still unknown\n", sym->name, sym->lineNum, sym);
#endif
				res |= ERR_PA_UNRESOLVED_REFERENCE;		
		}
		sym = itr.next();
	}
	
	return res;


}




//TODO: must check: multiple statemeants: stop compilation if any of such discovered - OR - merge them in order to continue analysis
//

//-------------------------------------------------------------------

int SemanticAnalyzer::collectDeclarations(ASTree* const _tree)
{
	ASTree* node;
	SymbolTable::Symbol* sym;
	int type = _tree->getType();
	assert(_tree);

	if(type == ASTree::PIdentifier)
	{
		assert(_tree->parent());
		assert(_tree->parent()->parent());

		ASTree* tmp;
		
		sym = _tree->getSymbol();
		
		if(_tree->parent()->getType() == ASTree::PEvent)
		{
			sym->symbolType = SymbolTable::event_decl;
		}
		else if(_tree->parent()->getType() == ASTree::PComplexEvent)
		{
			sym->symbolType = SymbolTable::complex_decl;
		}
		else if(_tree->parent()->parent()->getType() == ASTree::PExternal)
		{
			sym->symbolType = SymbolTable::external_decl;
			sym->dataType = mSymTab->dataTypeFromNode(_tree->parent());
		}
		else if(_tree->ancestor(ASTree::PUse))
		{
			tmp = _tree->ancestor(ASTree::PEvent);
			
			ASTreeItr tmpItr(tmp);
			
			sym->symbolType = SymbolTable::event_attr_decl;
			sym->owner = tmpItr.firstChild()->getSymbol();
			sym->dataType = mSymTab->dataTypeFromNode(_tree->parent());
			
		}
		else if(_tree->ancestor(ASTree::PPayload))
		{
#ifdef CEP_DEBUG
		printf("ZZZ TRYING %p\n", _tree->getSymbol());
#endif

			tmp = _tree->ancestor(ASTree::PComplexEvent);
			
			ASTreeItr tmpItr(tmp);

			// Only store the actual declaration attribute, not any references that may be part of 
			// the declaration.
			//
			if(mSymTab->isKnownType(_tree->parent()))
			{
				sym->symbolType = SymbolTable::payload_attr_decl;
				sym->owner = tmpItr.firstChild()->getSymbol();
				sym->dataType = mSymTab->dataTypeFromNode(_tree->parent());
			}
		}
	}

	ASTreeItr itr(_tree);
	
	node = itr.firstChild();
	while(node)
	{
		collectDeclarations(node);

		node = itr.nextChild();
	}

	return OK;
}

//-------------------------------------------------------------------

int SemanticAnalyzer::resolveReferences(ASTree* const _tree)
{
	ASTree* node;

	if(_tree->isPostfixIdentifier())
	{
		resolvePostfixReference(_tree);
	}
	else if(_tree->isIdentifier() 
		&& (_tree->getSymbol()->symbolType == SymbolTable::unknown)
		&& !_tree->parent()->isPostfixIdentifier())
	{
		resolveReferencesForEvent(_tree);

		resolveReferencesForComplexEvent(_tree);
	}
			
	
	ASTreeItr itr(_tree);
	node = itr.firstChild();
	while(node)
	{
		resolveReferences(node);
		
		node = itr.nextChild();
	}

	return OK;
}



//-------------------------------------------------------------------

int SemanticAnalyzer::resolvePostfixReference(ASTree* const _ref)
{
#ifdef CEP_DEBUG
	printf("SemanticAnalyzer::resolvePostfixReference(%p)\n", _ref);
#endif

	ASTreeItr pf(_ref);

	ASTree* eventNode = pf.firstChild();
	ASTree* attrNode = pf.nextChild();
	int windowOffset = 0;
	
	// must save window offset specifier before remove the postfix-node and its children.
	if(_ref->getType() == ASTree::PWindowOffsetPostfixExpression)
	{
		ASTreeItr index(eventNode);
		
		assert(index.firstChild());
		
		windowOffset = index.firstChild()->getValue()->v.integerVal;
	}
		
		
	SymbolTable::Symbol* attrDecl = mSymTab->find(attrNode->getSymbolName(), eventNode->getSymbolName());

	if(attrDecl)
	{
		
		
		// Resolve the references
		SymbolTable::Symbol* eventDecl = mSymTab->find(eventNode->getSymbolName(), SymbolTable::event_decl);

                if (!eventDecl)
                    eventDecl = mSymTab->find(eventNode->getSymbolName(), SymbolTable::complex_decl);
                
		assert(eventDecl);
		

		//FIXME: must remove this symbol from the symtab.
		eventNode->getSymbol()->decl = eventDecl;
		eventNode->getSymbol()->symbolType = SymbolTable::reference;
		
		attrNode->getSymbol()->decl = attrDecl;
		attrNode->getSymbol()->symbolType = SymbolTable::reference;

		// Save origial attr-REF
		SymbolTable::Symbol* attrRef = attrNode->getSymbol();
		
		// window offset spec must be set on original event decl.
//		printf("ZZZ-1: Setting WO %d on %p (%s)\n", windowOffset, attrRef, attrRef->name);
		
		attrRef->windowOffset = windowOffset;

		// Substitute postfix expression for the attribute
///		ASTree* parent = _ref->parent();
///		_ref->destroy();
///need use a different operation something like a substitution!!
                _ref->replace(new ASTree(attrRef));

	}
	else
	{
		mErrorList->addError(	eventNode->getSymbol()->lineNum, 
								eventNode->getSymbol()->colNum,
								"%d: Variable '%s.%s' is not declared", 
								DMSG_AL_UNDECLARED_VAR,
								eventNode->getSymbol()->name,
								attrNode->getSymbolName());
	}

	return OK;
}


//-------------------------------------------------------------------

//FIXME: furhter semantic analysis: all the scope must reflect the events/complex events being detected.
// e.g detect a where otherEVent.b = 3 should result in an error.

int SemanticAnalyzer::resolveReferencesForComplexEvent(ASTree* const _ref)
{
#ifdef CEP_DEBUG
	printf("SemanticAnalyzer::resolveReferencesForComplexEvent %p\n", _ref);
#endif
	int res = OK;
	
	SymbolTable::Symbol* currSymbol = _ref->getSymbol();
	SymbolTable::Symbol* declSymbol;
	
	// Attributes found in this context belong to the event(s) used in the 
	// detect statement. However, since they are unscoped, only one event/complex-event name
	// may be used in the detect-statement.
	//
	if(_ref->ancestor(ASTree::PGroup)
	|| _ref->ancestor(ASTree::PDetectWhere)
	|| _ref->ancestor(ASTree::PPayload))
	{
		// Navigate to the detect statement (if any)
		ASTree* complex = _ref->ancestor(ASTree::PComplexEvent);
		assert(complex);
		ASTree* eventExpr = complex->descendant(ASTree::PDetectEventExpression);
		
		if(eventExpr)
		{
			resolveReferencesSubordinateToDetectStatement(eventExpr, _ref);
		}
		else
		{
			// Unless the identifier is the name of the payload member, an identifier
			// MUST have an associated detect event expression.
			
			assert(_ref->getSymbol());
			
			SymbolTable::Symbol* id = _ref->getSymbol();
			
			if(id->symbolType != SymbolTable::payload_attr_decl)
			{
				mErrorList->addError(id->lineNum,
									id->colNum,
									"%d: Missing detect statement. Identifier '%s' must be associated to an event in the detect statement", 
									DMSG_AL_MISSING_DETECT_STMT,
									id->name);			
			
			}
		}
	}
	else if(_ref->ancestor(ASTree::PDetectEventExpression))
	{
		// Must be Event  ///RP: or Complex-Event SymbolTable::complex_decl
		declSymbol = mSymTab->find(currSymbol->name, SymbolTable::event_decl);
                if (!declSymbol)
                    declSymbol = mSymTab->find(currSymbol->name, SymbolTable::complex_decl);
                    
		if(declSymbol)
		{
			currSymbol->decl = declSymbol;
			currSymbol->symbolType = SymbolTable::reference;
		}
		else
		{
			mErrorList->addError(currSymbol->lineNum,
								currSymbol->colNum,
								"%d: Event '%s' is not declared.", 
								DMSG_AL_EVENT_NOT_DECLARED,
								currSymbol->name);
		}
	}
	else if(_ref->ancestor(ASTree::PDetectWindow))
	{
		ASTree* tuple = _ref->ancestor(ASTree::PTupleWin);
		
		
			
			// FIXME: In case of tuple-window, the tuple-scope MUST be present if there's more than
			// one event in the detect (separate check)
			//
		if(tuple)
		{
			// Special case - two children
			// First child is always ID, must be event/complex event.
			// Second child is expected to be external var.
			// 
			if(tuple->countChildren() >= 2)
			{
				ASTreeItr tupleComponents(tuple);
				
				// ID
				ASTree* comp = tupleComponents.firstChild();
				SymbolTable::Symbol* id = comp->getSymbol();

				// must be event / complex event
				declSymbol = mSymTab->find(id->name, SymbolTable::event_decl);
				if(!declSymbol)
					declSymbol = mSymTab->find(id->name, SymbolTable::complex_decl);

				if(declSymbol)
				{
					id->decl = declSymbol;
					id->symbolType = SymbolTable::reference;
				}
				else
				{
					mErrorList->addError(id->lineNum,
										id->colNum,
										"%d: The tuple scope specifier '%s' is not a valid event or complex event", 
										DMSG_AL_TUPLE_SCOPE_SPEC_NOT_E_OR_CE,
										id->name);
				}
				
				// External (but only if it's an identifier)
				comp = tupleComponents.nextChild();
				if(comp->getType() == ASTree::PIdentifier)
				{
					checkIfExternal(comp->getSymbol());
				}
			}
		}
		else
		{
			checkIfExternal(currSymbol);
		}
	}
	
	res = resolveLastsStatementReference(_ref);
	
	return res;
}

//-------------------------------------------------------------------


int SemanticAnalyzer::resolveReferencesSubordinateToDetectStatement(
													ASTree* const _detectNode,		
													ASTree* const _idNode)
{
	int res = OK;
	
#ifdef CEP_DEBUG
	printf("SemanticAnalyzer::resolveReferencesSubordinateToDetectStatement %p\n", _detectNode);
#endif
	SymbolTable::Symbol* declSymbol;
	SymbolTable::Symbol* idSym = _idNode->getSymbol();
	assert(_idNode);
	assert(idSym);
        
        bool doErr = true;
        
	if(!hasMoreThanOneEventInDetectStatement(_detectNode, _detectNode))
	{
		ASTreeItr itr(_detectNode);
                
		// Get the event identifier from the detect statement
		ASTree* scope = _detectNode->descendant(ASTree::PIdentifier);
		assert(scope);

		// Find the attr/scope combo in the symbol table.
		declSymbol = mSymTab->find(idSym->name, 
									scope->getSymbolName());
		if(!declSymbol)
		{
			// It can also be an external variable
			declSymbol = mSymTab->find(idSym->name, SymbolTable::external_decl);
		}
		
		
		if(declSymbol)
		{
			idSym->decl = declSymbol;
			idSym->symbolType = SymbolTable::reference;
		}
		else
		{
//RP:			bool doErr = true;
			
			// FIXME-TODO: Consider different overall analysis approach.
			//
			// If it's an event then it's ok if it is used in the count() function
			//
			// We cannot use the references here, as this is just what we're 
			// resolving :-(. Do a symtab lookup.
			SymbolTable::Symbol* eventDecl = mSymTab->find(idSym->name, SymbolTable::event_decl);
                        
///RP: just trying include a complex event
                        if (!eventDecl)
                            eventDecl = mSymTab->find(idSym->name, SymbolTable::complex_decl);
                        			
//			if(idSym->decl && (idSym->decl->symbolType == SymbolTable::event_decl))
			if(eventDecl)
			{
				idSym->decl = eventDecl;
				idSym->symbolType = SymbolTable::reference;
			
				ASTree* parent = _idNode->parent();
				if(parent && parent->getType() == ASTree::PCountFunc)
				{
					doErr = false;
				}
			}

			if(doErr)
			{
				mErrorList->addError(	idSym->lineNum,
										idSym->colNum,
					"%d: Variable '%s' is not defined for event '%'s, nor is it an external variable", 
										DMSG_AL_VAR_NOT_FOR_EVT_NOR_EXT,
										idSym->name,
										scope->getSymbolName());
										
				res = ERR_AN_DETECT_REF_ERR;
			}
		}
	}
	else	// more than one event
	{
//RP: start
            SymbolTable::Symbol* eventDecl = mSymTab->find(idSym->name, SymbolTable::event_decl);
			
            if(eventDecl) {
                idSym->decl = eventDecl;
		idSym->symbolType = SymbolTable::reference;
		
		ASTree* parent = _idNode->parent();
		if(parent && parent->getType() == ASTree::PCountFunc)
		{
                    doErr = false;
		}
            }
            else {
                eventDecl = mSymTab->find(idSym->name, SymbolTable::complex_decl);
                if (eventDecl) {
                        idSym->decl = eventDecl;
                        idSym->symbolType = SymbolTable::reference;

                        ASTree* parent = _idNode->parent();
                        if(parent && parent->getType() == ASTree::PCountFunc)
                        {
                            doErr = false;
                        }
                }
                else {
//RP: end
                        // It can also be an external variable
                        declSymbol = mSymTab->find(idSym->name, SymbolTable::external_decl);
        ///to accept ext var with a composite detect
                        if (declSymbol) {
                            idSym->decl = declSymbol;
                            idSym->symbolType = SymbolTable::reference;
                            idSym->dataType = declSymbol->dataType;
                        }

                        if(!declSymbol)
                        {
                                mErrorList->addError(idSym->lineNum,
                                                                        idSym->colNum,
                                "%d: Variable '%s' is ambiguous because there is more than one (complex) event being detected", 
                                                                DMSG_AL_AMBIGUOUS_VAR_FOR_E_OR_CE,
                                                                idSym->name);

                                res = ERR_AN_DETECT_REF_ERR;
                        }
                }
            }
	}
	
	return res;
}

//-------------------------------------------------------------------

bool SemanticAnalyzer::checkIfExternal(SymbolTable::Symbol* _sym)
{
	SymbolTable::Symbol* lookup;
	bool res = true;

	// Expected to be external variables
	lookup = mSymTab->find(_sym->name, SymbolTable::external_decl);
	if(lookup)
	{
		_sym->decl = lookup;
	}
	else
	{
		mErrorList->addError(_sym->lineNum,
							_sym->colNum,
							"%d: External variable '%s' is not declared", 
							DMSG_AL_EXT_VAR_NOT_DECLARED,
							_sym->name);
							
		res = false;
	}

	return res;
}

//-------------------------------------------------------------------


int SemanticAnalyzer::resolveReferencesForEvent(ASTree* const _ref)
{
	int res = OK;
	
	SymbolTable::Symbol* sym = _ref->getSymbol();
	SymbolTable::Symbol* declSymbol;
	ASTree* eventId;
	
	if(_ref->ancestor(ASTree::PAccept))
	{
		eventId = getScopingEventNode(_ref);

		// Must be event-attribute or external var
		declSymbol = mSymTab->find(sym->name, eventId->getSymbolName());

#ifdef CEP_DEBUG
		printf("--Looking for %s-%s\n", sym->name, eventId->getSymbolName());

		if(declSymbol)
			printf("FOUND\n");
		else
			printf("NOT-FOUNd\n");
#endif

		if(!declSymbol)
			declSymbol = mSymTab->find(sym->name, SymbolTable::external_decl);
			
		if(declSymbol)
		{
			_ref->getSymbol()->decl = declSymbol;
			_ref->getSymbol()->symbolType = SymbolTable::reference;
		}
		else
		{
			mErrorList->addError(	sym->lineNum,
									sym->colNum,
									"%d: Attribute '%s' is not declared for this event", 
									DMSG_AL_ATTR_NOT_DECLARED_FOR_EVT,
									sym->name);
		}
	}
	else if(_ref->ancestor(ASTree::PUse))
	{
		eventId = getScopingEventNode(_ref);

		declSymbol = mSymTab->find(sym->name, eventId->getSymbolName());
		if(declSymbol)
		{
			_ref->getSymbol()->decl = declSymbol;
			_ref->getSymbol()->symbolType = SymbolTable::reference;
		}
		else
		{
			mErrorList->addError(sym->lineNum,
								sym->colNum,
								"%d: Event '%s' is not declared.", 
								DMSG_AL_EVENT_NOT_DECLARED,
								sym->name);
		}
	}
		
	
	res = resolveLastsStatementReference(_ref);
	
	return res;
	
}

//-------------------------------------------------------------------

ASTree* SemanticAnalyzer::getScopingEventNode(ASTree* const _node)
{
	// Find the scoping event.
	ASTree* event = _node->ancestor(ASTree::PEvent);
	assert(event);
	ASTreeItr itr(event);
	ASTree* eventId = itr.firstChild();
	assert(eventId);

	return eventId;
}


//-------------------------------------------------------------------

int SemanticAnalyzer::resolveLastsStatementReference(ASTree* const _ref)
{
	SymbolTable::Symbol* sym = _ref->getSymbol();
	int res = OK;
	
	if(_ref->ancestor(ASTree::PLasts))
	{
		// Can be Event, Complex Event or External
		SymbolTable::Symbol* declSymbol = mSymTab->find(sym->name, SymbolTable::event_decl);
		if(!declSymbol)
			declSymbol = mSymTab->find(sym->name, SymbolTable::complex_decl);
		if(!declSymbol)
			declSymbol = mSymTab->find(sym->name, SymbolTable::external_decl);
		
		if(declSymbol)
		{
			_ref->getSymbol()->decl = declSymbol;
			_ref->getSymbol()->symbolType = SymbolTable::reference;
		}
		else
		{
			mErrorList->addError(sym->lineNum,
								sym->colNum,
								"%d: %s' is not declared as an event, complex event or external variable.", 
								DMSG_AL_UNDECLARED_VAR,
								sym->name);
								
			res = ERR_AN_LASTS_ERR;
		}
	}
	
	return res;
}


//-------------------------------------------------------------------

// Compares two trees
bool SemanticAnalyzer::hasMoreThanOneEventInDetectStatement(ASTree* const _root,
															ASTree* const _sub)
{
	bool res = false;
	ASTree* node;

	if(_root->isIdentifier()
		&& _sub
		&& _sub->isIdentifier()
		&& strcmp(_root->getSymbolName(), _sub->getSymbolName()))
	{
		return true;
	}

//	if(_detectNode->isIdentifier())
//		idNode = _detectNode;
		
	ASTreeItr itr(_root);
	node = itr.firstChild();

	while(node)
	{
		if(_sub)
		{
			if(!res)
				res = hasMoreThanOneEventInDetectStatement(_sub, node);
		}
			
		if(!res)
			res = hasMoreThanOneEventInDetectStatement(node, 0); 
		
		node = itr.nextChild();
	}
	
	return res;
}
