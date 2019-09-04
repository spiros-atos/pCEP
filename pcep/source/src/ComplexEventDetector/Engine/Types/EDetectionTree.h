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
	
	************************************************
	THIS IS THE CORE OF THE COMPLEX EVENT PROCESSOR.
	************************************************
	
*/
#ifndef _SOL_CEP_ENGINE_TYPES_DETECTION_TREE_H_
#define _SOL_CEP_ENGINE_TYPES_DETECTION_TREE_H_

#include "EComplexEvent.h"
#include "ECapturedEventPool.h"
#include "ECapturedEvent.h"


struct ECapturedEvent;

/*!
	Core CEP detection tree.
	
		IMPORTANT information when evaluating the 'during' / 'after' operations: as opposed
		to 'and'/'or' operations, the order of the operands DOES matter:
		In the expression 'a after b', the LEFT tree-node represents 'b' 
		(called the after-event-node in the technical doc) and the RIGHT tree-node 
		represents 'a' (called the before-event-node in the technical doc)
*/	
struct EDetectionTree
{	
	enum Operator
	{
		opOr,
		opAnd,
		opAfter,
		opDuring,
		opAbsence,
		opUnknown	// Not an operator, an Event (or Complex Event)
	};

	enum NodeType
	{
		valueNode,
		opNode
	};
	
	/*!
		\brief Result of iterative tree evaluation
	*/
	enum EvalResult
	{
		again,
		finished,
		detectedComplexEvent,
                false_detection                    // SKMMA - after and during fix
	};

	//! ctor
	EDetectionTree(const Operator _op)
					: type(opNode) 
					, op(_op)
					, resolved(false)
					, marked(false)
					, eventOid(-1L)
					, left(0)
					, right(0)
					, parent(0)
	{}
	
	//! ctor
	EDetectionTree(const long _oid)  
					: type(valueNode) 
					, op(opUnknown)	// not used
					, resolved(false)
					, marked(false)
					, eventOid(_oid)
					, left(0)
					, right(0)
					, parent(0)
					, found(false)

	{}

	NodeType type; 	//! One of value-node or operator-node
	

	//!@{
	//! Attributes used for nodes of type 'operator'
	Operator op;
	bool resolved;
	//!@}
	
	//!@{
	//! Attributes used for nodes of type 'value'
	bool marked;
	long eventOid;
	//!@}
	

	//!@{
	//! Attributes used for both Operator and Value nodes
	EDetectionTree* left;	//! represents the after-event-node in after/during ops
	EDetectionTree* right;	//! represents the before-event-node in after/during ops
	EDetectionTree* parent;
	//!@}
	
	//! Used for non-recursive, repeating find
	bool found;
	
	
#ifdef CEP_DEBUG
	char eventName[32+1];
	void setEventName(char* _name) { strncpy(eventName, _name, 32); }
#endif	
	
	/*!
		\brief Evaluates a captured event
		\param _capturedEvent The event to evaluate
		\returns 'again' if there's more to evaluate, otherwise 
			'finished' or 'detectedComplexEvent'. 
	*/
	EvalResult evaluate(ECapturedEvent* const _capturedEvent, EComplexEvent* const _complexEvent)
	{
		EvalResult res = finished;
		
		if((type == valueNode)
		&& !marked
		&& (eventOid == _capturedEvent->oid))
		{
			marked = true;
                        
#ifdef CEP_DEBUG
			if(parent)
			{
				if(parent->left == this)
					printf("I'm the left node\n");
				if(parent->right == this)
					printf("I'm the right node\n");
			}
#endif
			
			if(parent)
			{
				if(parent->resolve(_capturedEvent, this, _complexEvent))
					return detectedComplexEvent;
					
				return again;
			}
			else
			{
				return detectedComplexEvent;
			}
		}
                
///RP retrieve of solcep code:
		if ((parent->op == opDuring) || (parent->op == opAfter))		// SKMMA - during fix and after fix
		    if ((res == finished) && (parent->right == this) && (marked))				
		    {
                      marked= false;        ///RP: 
		      res = false_detection;
		    }
                               
		return res;
	}

	

	/*!
		\brief Resolves a captured event against a part of the detection tree
		\param _capturedEvent The event to resolve
		\param _child The part of the detection tree against which to evaluate
		\returns True if a complex event was detected
	*/
	bool resolve(ECapturedEvent* const _capturedEvent,
				 EDetectionTree* const _child, EComplexEvent* const _complexEvent)
	{
		if(type != opNode)
		{
			return false;
		}
			
		if(op == opOr)
		{
			setResolved(true);
		}
		else if(op == opAnd)
		{
			EDetectionTree* sib = _child->sibling();
#ifdef CEP_DEBUG
			printf("opAnd\n");
			printf("sib=%p\n", sib);
#endif 			
			if(sib && (sib->marked || sib->resolved))
			{
				resolved = true;
			}
		}
		else if(op == opAfter)
		{
			if(_child && (_child->type == valueNode))
			{

				if(_child == right)			// SKMMA - after fix -start
				{
					// after-event-node was detected
					if(left && left->marked)
					{
///RP: check if left & right are adjacent; first the left
                                            if (eventsAdjacentInPool(right->eventOid, left->eventOid, _complexEvent))
						resolved = true;
					}
				}
				else if(_child == left)		// SKMMA - after fix -end
				{
                                    _child->marked = true;
					// before-event-node was detected
					
				}
			}
		}
		else if(op == opDuring)
		{
			//FIXME
		// SKMMA - during fix -start
			if(_child && (_child->type == valueNode))
			  {
				  if(_child == right)
				  {
					  
					  if(left && left->marked)
					  {
                                              ///RP: check if left & right are adjacent; first the left
                                            if (findEventInPool(left->eventOid, _complexEvent))
						  resolved = true;
					  }
					  
				  }
				  else if(_child == left)
				  {
                                      _child->marked = true;
					// before-event-node was detected
					  
				  }
			  }
		  // SKMMA - during fix -ends                                        
		}
		
		
		// If we're still here and something was resolved,
		// also try to resolve the parent
		//
		if(resolved)
		{
			if(!parent)
			{
				root()->reset();
				return true;	// COMPLEX EVENT
			}
			else
			{
				return parent->resolve(_capturedEvent, this, _complexEvent);
			}
		}
		
		return false;
	}
	
	
	//--- helpers ---
	
	
	//! Set the tree as resolved.
	void setResolved(const bool _resolved)
	{
		resolved = _resolved;
		
		if(left)
			left->setResolved(_resolved);
		if(right)
			right->setResolved(_resolved);
	}
	

	//! Reset the find-result for iterative searches.
	void resetFind()
	{
		found = false;
		
		if(left)
			left->resetFind();
		if(right)
			right->resetFind();
	}


	//! Reset the complete tree, both find-status and resolution.
	void reset(const bool _recursive = true)
	{
		resolved = false;
		marked = false;
		
		if(_recursive)
		{
			if(left)
				left->reset();
			if(right)
				right->reset();
		}
	}
		
		
	void addLeft(EDetectionTree* const _addedNode) 
	{ 
		left = _addedNode; 
		_addedNode->parent = this;
	}
		
				
	void addRight(EDetectionTree* const _addedNode) 
	{ 	
		right = _addedNode; 
		_addedNode->parent = this;
	}
	
	
	/*!
		\brief Finds an event in the detection tree
		\param _oid The OID of the event
		\returns The node or NULL if not found
	*/
	EDetectionTree* find(const long _oid)
	{
		if((type == valueNode)
		&& (eventOid == _oid)
		&& !found)
		{
			found = true;
			return this;
		}
			
		EDetectionTree* res = NULL;
		
		if(left)
			res = left->find(_oid);
			
		if(!res && right)
			res = right->find(_oid);
	
		return res;
	}


///RP: find to detect with complex event
        EDetectionTree* findOnly(const long _oid) {
		if(type == valueNode && eventOid == _oid) {
			return this;
		}
			
		EDetectionTree* res = NULL;
		
		if(left)
			res = left->findOnly(_oid);			
		if(!res && right)
			res = right->findOnly(_oid);
	
		return res;
	}
///RP: find to detect with complex event;

	//! Gets root of the tree
	EDetectionTree* root()
	{
		if(!parent)
			return this;
			
		return root();
	}
	
	//! Get the sibling of a node
	EDetectionTree* sibling()
	{
		if(!parent)
			return NULL;
			
		if(parent->left && (parent->left != this))
			return parent->left;
			
		if(parent->right && (parent->right != this))
			return parent->right;
	
		return NULL;
	}

///RP: check if the node Right & left are adjacent;
bool eventsAdjacentInPool(long _oidR, long _oidL, EComplexEvent* const _complexEvent) {
    
    if (_complexEvent->capturedEventPool) {
        
        ECapturedEvent *eCapEvent = _complexEvent->capturedEventPool->capturedEvents;
        while (eCapEvent) {
            if (eCapEvent->oid == _oidR) {
                if (eCapEvent->prev && eCapEvent->prev->oid == _oidL)
                    return true;
            }
            eCapEvent = eCapEvent->prev;
        }
    }
               
    return false;
}

///RP: check if the node Right & left are adjacent;
bool findEventInPool(long _oid, EComplexEvent* const _complexEvent) {
    
    if (_complexEvent->capturedEventPool) {
        
        ECapturedEvent *eCapEvent = _complexEvent->capturedEventPool->capturedEvents;
        while (eCapEvent) {
            if (eCapEvent->oid == _oid) {
                    return true;
            }
            eCapEvent = eCapEvent->prev;
        }
    }
               
    return false;
}



///RP: mark a captured event when the where clause is false
void setAsMarked(long _oid) {
    
    EDetectionTree *toMark = findOnly(_oid);
    
    if (toMark->parent) {
        if (toMark->parent->op == opAnd) {
            toMark->marked = true;
            toMark->sibling()->marked = false;
        }
        else if (toMark->parent->op == opAfter && toMark->parent->left == toMark) {
            toMark->marked = true;
            toMark->sibling()->marked = false;
        }
        else if (toMark->parent->op == opDuring && toMark->parent->left == toMark) {
            toMark->marked = true;
            toMark->sibling()->marked = false;
        }
    }

}

#ifdef CEP_DEBUG
#include <stdio.h>

	const char* operatorAsString(const Operator _op)
	{
		if(_op == opOr) return "or";
		if(_op == opAnd) return "and";
		if(_op == opAfter) return "after";
		if(_op == opDuring) return "during";
		if(_op == opAbsence) return "absent";
		
		return "???";
	}

	void print(int _level = 0)
	{
		for(int i = 0; i < _level; i++)
			printf("  ");

		if(type == opNode)
			printf("%s [%sresolved]", operatorAsString(op)
							, resolved ? "" : "un");
		else
			printf("%ld [%smarked] '%s'", eventOid
									, marked ? "" : "un" 
									, eventName);
			
		printf("\n");
		
		if(left)
			left->print(_level+1);
		if(right)
			right->print(_level+1);
	}
#endif
};


#endif
