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

#include "ASTree.h"

///#include <stdio.h>
///#include <assert.h>

#include "../../Types/TValue.h"
#include "../../Common/Err.h"


ASTree::ASTree(const NodeType _nodeType)
{
#ifdef CEP_DEBUG
//	printf("[%p - ASTree:ctor; type=%d-%s\n", this, _nodeType, 
	//				getNodeType(_nodeType));
#endif
	init();
	
	mType = _nodeType;
}

ASTree::ASTree(TValue* const _val)
{
	init();
	
	mType = PValue;
	mVal = _val;
}


ASTree::ASTree(SymbolTable::Symbol* const _sym)
{
	init();
	
	mType = PIdentifier;
	mSymbol = _sym;
}


ASTree::~ASTree()
{
	//FIXME: recursively destroy all children
///    if (mLatestChild)        
///        completDestroy(mLatestChild);

    
}



void ASTree::startCompletDestroy() {
    
    completDestroy(this->mLatestChild);
    
}

///RP:
void ASTree::completDestroy(ChildNode *_childTree) {
    
    ChildNode *toDlt;
    
    while(_childTree) {
        toDlt = _childTree;
        
        if (toDlt->val->mLatestChild)   // es el subarbol
                completDestroy(toDlt->val->mLatestChild);
///        else {
            if (toDlt->val->mVal && toDlt->val->mVal->t == t_string)
                free(toDlt->val->mVal->v.stringVal);
            
            delete toDlt->val->mVal;
            delete toDlt->val->mSymbol;
            delete toDlt->val;
///            delete toDlt;
///        }
        delete toDlt;
        _childTree = _childTree->prev;        
    }
}

void ASTree::init()
{
	mType = PUnknown; 
	mVal = 0;
	mLatestChild = 0;
	mChildIterator = 0;
	mParent = 0;
	mParentChildNode = 0;
	mSymbol = 0;
}


ASTree* ASTree::addChild(ASTree* _child)
{
#ifdef CEP_DEBUG
//	printf("[Adding child %p to parent %p\n", _child, this);
#endif
	ChildNode* n = new ChildNode();
	
	n->val = _child;
	n->prev = mLatestChild;
	n->next = 0;

	if(n->prev)
		n->prev->next = n;
		
	mLatestChild = n;
	
	_child->mParent = this;
	_child->mParentChildNode = n;
	
	return _child;
}

///
void ASTree::replace(ASTree* _node) {
    
//    	ChildNode *n = new ChildNode();	
//	n->val = _node; 

	ASTree *par = parent();
	ChildNode *itr = par->mLatestChild;
        _node->mParent = par;
	
	while(itr)
	{
		if(itr->val == this)
		{
                    itr->val = _node;
                    
                    return;
		}
		itr = itr->prev;
	}
}


void ASTree::destroy()
{
	ASTree* par = parent();

	ChildNode* itr = par->mLatestChild;
	
	itr = par->mLatestChild;
	while(itr)
	{
		if(itr->val == this)
		{
			if(itr->prev)
				itr->prev->next = itr->next;
			if(itr->next)
				itr->next->prev = itr->prev;
				
			if(itr == par->mLatestChild)
			{
				par->mLatestChild = itr->prev;
			}
			
//			delete itr;
			//FIXME: destroy children. Rewrite this into "prune()", taking
			// into account the par->mLatestChild = itr->prev bugfix
                        

///dudaRP                        completDestroy(itr);  useful ?? to avoid memory leak?

			return;
		}
		itr = itr->prev;
	}
}

void ASTree::prune(ASTree* _leaf)
{
//	printf("pruning-node-%p\n", this);
	ASTreeItr itr((ASTree*)_leaf);
	ASTree* child = itr.firstChild();

	while(child)
	{
//		printf("pruning-%p\n", child);
		
		prune(child);
		
		child = itr.nextChild();
	}

	delete _leaf;

	if(mParentChildNode)
	{
//	printf("(%p)--------%p %p %p \n", this, mParentChildNode->prev, mParentChildNode, mParentChildNode->next);
		if(mParentChildNode->prev)
			mParentChildNode->prev->next = mParentChildNode->next;
		if(mParentChildNode->next)
			mParentChildNode->next->prev = mParentChildNode->prev;
		delete mParentChildNode;
	}

//	printf("DEL %p\n", this);
//	delete mVal;
}


ASTree* const ASTree::parent()  const
{
	return mParent;
}


ASTree* const ASTree::ancestor(const NodeType _type) const
{
	ASTree* res = 0;
	
	if(mParent)
	{
		if(mParent->mType == _type)
			return mParent;
		else
			res =  mParent->ancestor(_type);
	}
		
	return res;
}


ASTree* const ASTree::descendant(const NodeType _type) const
{
	ASTree* res = 0;
	
	ASTreeItr itr((ASTree*)this);
	ASTree* child = itr.firstChild();
	
	while(child)
	{
		if(child->mType == _type)
		{
			return child;
		}
			
		if(!res)
			res = child->descendant(_type);
			
		child = itr.nextChild();
	}
	
	return res;
}


/*
 NOT USED
ASTree* const ASTree::descendant(const char* _symbol) const
{
	ASTree* res = 0;
	
	if(isIdentifier()
	&& !strcmp(getSymbolName(), _symbol))
	{
		return (ASTree*)this;
	}
	
	ASTreeItr itr((ASTree*)this);
	ASTree* child = itr.firstChild();
	
	while(child)
	{
		res = child->descendant(_symbol);
			
		child = itr.nextChild();
	}
	
	return res;
}
*/



int ASTree::countDescendants(const NodeType _type) const
{
	int count = 0;
	
	ASTree* child = firstChild();
	
	while(child)
	{
		if(child->mType == _type)
			count++;
			
		count += child->countDescendants(_type);
			
		child = nextChild();
	}
	
	return count;
}

int ASTree::countChildren(const NodeType _type) const
{
	int count = 0;
	
	ASTree* child = firstChild();
	while(child)
	{
		if(child->mType == _type)
			count++;
		child = nextChild();
	}
	
	return count;
}


int ASTree::countChildren() const
{
	int count = 0;
	
	ASTreeItr itr((ASTree*)this);
	
	ASTree* child = itr.firstChild();
	while(child)
	{
		count++;
		child = itr.nextChild();
	}
	
	return count;
}


ASTree* const ASTree::firstChild() const
{
//printf("[%p] ASTree::firstChild . itr=%p, head=%p\n", this, mChildIterator, mLatestChild);

	mChildIterator = mLatestChild;
	
	if(mChildIterator)
		return mChildIterator->val;
		
	return 0;
}


ASTree* const ASTree::nextChild() const
{
	if(!mChildIterator)
		return 0;

	mChildIterator = mChildIterator->prev;
	
	if(mChildIterator)
		return mChildIterator->val;
		
	return 0;
}



SymbolTable::Symbol* const ASTree::getSymbol() const
{
	return mSymbol;
}


char* ASTree::getSymbolName() const
{
	assert(mType == PIdentifier);

	return mSymbol ? mSymbol->name : 0;
}


void ASTree::setSymbol(SymbolTable::Symbol* const _sym)
{
	mSymbol = _sym;
}
	

#ifdef CEP_DEBUG

void ASTree::print(int _level) 
{
	printf("Node: %p", this);
	
	for(int i = 0; i < _level; i++)
		printf("    ");
	
	printf("%s", asString());
	
	if(mSymbol)
		printf("[E:%p]", mSymbol->engineObj);
	
	if(mType == PIdentifier)
		printf("->%p", mSymbol);
	
	printf("\n");
	
	ASTree* child = firstChild();
	
	while(child)
	{
		child->print(_level+1);
		
		child = nextChild();
	}
}


const char* ASTree::asString() const
{
	static char s[64];

	if(mType == PRoot) { return("(root)");  }
	if(mType == PEvent) { return("event");  }
	if(mType == PEventUnits) { return("event-units");  }
	if(mType == PComplexEvent) { return("complex");  }
	if(mType == PComplexEventUnits) { return("complex-units");  }
	if(mType == PExternal) { return("external");  }
	if(mType == PMap) { return("map");  }
	if(mType == PEventType) { return("event-type");  }
	if(mType == PPostfixExpression) { return("postfix-exp");  }
	if(mType == PWindowOffsetPostfixExpression) { return("window-offset-postfix-exp");  }
	if(mType == PUse) { return("use");  }
	if(mType == PLasts) { return("lasts");  }
	if(mType == PEventAttribute) { return("event-attribute");  }
	if(mType == PDelegate) { return("delegate");  }
	if(mType == PAccept) { return("accept");  }
	if(mType == PPayload) { return("payload");  }
	if(mType == PPayloadAttribute) { return("payload-attribute");  }
	if(mType == PEventIdentifierList) { return("event-identifier-list");  }
	if(mType == PEventAttributeList) { return("event-attribute-list");  }
	if(mType == PGroup) { return("group");  }
	if(mType == PDetect) { return("detect");  }
	
	if(mType == PDetectEventExpression) { return("expr-clause");  }
	if(mType == PDetectWhere) { return("where-clause");  }
	if(mType == PDetectWindow) { return("in-clause");  }
	if(mType == PPos) { return("pos");  }
	if(mType == PArea) { return("area");  }
	if(mType == PChar) { return("char");  }
	if(mType == PInt) { return("int");  }
	if(mType == PFloat) { return("float");  }
	if(mType == PString) { return("string");  }
	if(mType == PFixedTimeWin) { return("time window (fixed)");  }
	if(mType == PSlidingTimeWin) { return("time window (sliding)");  }
	if(mType == PDuration) { return("duration");  }
	if(mType == PTime) { return("time");  }
	if(mType == PDuration) { return("duration");  }
	if(mType == POpEq) { return("==");  }
	if(mType == POpNe) { return("!=");  }
	if(mType == POpGe) { return(">=");  }
	if(mType == POpGt) { return(">");  }
	if(mType == POpLe) { return("<=");  }
	if(mType == POpLt) { return("<");  }
	if(mType == PLogicalOrOp) { return("||");  }
	if(mType == PLogicalAndOp) { return("&&");  }
	if(mType == PEventOpOr) { return("or");  }
	if(mType == PEventOpAnd) { return("and");  }
	if(mType == PEventOpDuring) { return("during");  }
	if(mType == PEventOpAfter) { return("after");  }
	if(mType == PEventOpAbsence) { return("absent");  }
	if(mType == PSumFunc) { return("sum(event-attr)");  }
	if(mType == PAvgFunc) { return("avg(event-attr)");  }
    if(mType == PFftFunc) { return("fft(event-attr)"); }
    if(mType == PTrajFunc) { return("traj(event-attr)"); }
    if(mType == PCollFunc) { return("coll(event-attr)"); }
    if(mType == PCountFunc) { return("count(event)");  }
	if(mType == PInAreaFuncX) { return("inarea(expr, expr)");  }
	if(mType == PDiffFunc) { return("diff(event-attr)");  }
	if(mType == PDiffFuncX) { return("diff(expr, expr)");  }
	if(mType == POpAdd) { return("+");  }
	if(mType == POpSub) { return("-");  }
	if(mType == POpMult) { return("*");  }
	if(mType == POpDiv) { return("/");  }
	if(mType == PAbsFunc) { return("abs(expr)");  }
	if(mType == PValue) { return mVal->asString(); }
	
	if(mType == PIdentifier)
	{
		sprintf(s, "'%s'", mSymbol->name);
		return s;
	}

	
	sprintf(s, "%p - [UNKNOWN NODE-TYPE:%d (%d)]", this, mType, mType);
	
	return s;
}

#endif 

