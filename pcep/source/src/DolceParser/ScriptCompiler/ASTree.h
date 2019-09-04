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


#ifndef _SOL_CEP_ASTREE_H_
#define _SOL_CEP_ASTREE_H_


///#include <assert.h>

#include "SymbolTable.h"
#include "../../Types/TValue.h"


/*!
	\brief Represents an Abstract Syntax Tree, also known as an AST.
	The tree gets initially populated during the compilation phase,
	driven from dolce.y. 
*/
class ASTree
{
	friend class ASTreeItr;
	
public:

	/*!
		\brief The type of node. It corresponds to the Dolce syntax
	*/
	enum NodeType
	{ 
		PRoot = 0,
		PEvent,
		PEventUnits,
		PComplexEvent,
		PComplexEventUnits,
		PExternal,
		PMap,
		PEventType,
		PIdentifier,
		PConst,
		PPostfixExpression,
		PWindowOffsetPostfixExpression,
		PUse,
		PEventAttribute,
		PEventAttributeList,
		PDelegate,
		PAccept,
		PLasts,
		PEventIdentifierList,
		PPayload,
		PPayloadAttribute,
		PGroup,
		PDetect,
		PDetectEventExpression,
		PDetectWhere,
		PDetectWindow,
		PPos,
		PArea,
		PChar,
		PInt,
		PFloat,
		PString,
		PDuration,
		PTime,
		PFixedTimeWin,
		PSlidingTimeWin,
		PTupleWin,
		POpEq,
		POpNe,
		POpGe,
		POpGt,
		POpLe,
		POpLt,
		PLogicalOrOp,
		PLogicalAndOp,
		PEventOpOr,
		PEventOpAnd,
		PEventOpDuring,
		PEventOpAfter,
		PEventOpAbsence,
		PSumFunc,
		PAvgFunc,
		PCountFunc,
		PInAreaFuncX,	// always used with an eXpression, i.e. not auto-updated
		PDiffFunc,
		PDiffFuncX,	// always used with an eXpression, i.e. not auto-updated
		POpAdd,
		POpSub,
		POpMult,
		POpDiv,
		PAbsFunc,
		PFftFunc,
        PTrajFunc,
        PCollFunc,

		PValue = 900,
		PUnknown = 1000
	
	};
	
	
	/*!
		\brief ctor
		Creates a tree node of type with an optional const-value
		\param _type The node type
	*/
	ASTree(const NodeType _type);

	/*!
		\brief ctor
		Creates a tree node of type TValue with an optional const-value
		\param _val An instance of a node value.
		\remark The tree node is made the owner of _val. This means that it
			will take care of destroying it when the node is destroyed.
	*/
	ASTree(TValue* const _val);
	
	/*!
		\brief ctor
		Creates a tree node of type PIdentifier with a reference to 
		the symbol table
		\remark The tree node is \e not the owner of _sym. The caller
			is responsible for releasing the object referenced to by _sym.
	*/
	ASTree(SymbolTable::Symbol* const _sym);
		
	~ASTree();

	/*!
		\brief Adds a child to the node 
		\param _child The child to add
	*/
	ASTree* addChild(ASTree* _child);
        
        void replace(ASTree* _node);
	
	/*!
		\warning Not tested.
		\brief Removes a child from the node
	*/
	void removeChild(ASTree* _child);

	/*!
		\warning Not tested.
		\brief 
		Prune the node and all its descendants.
		Caller must hold a reference to the parent before
		doing this.
		\param _node The node to prune
	*/
	void prune(ASTree* _node);

	/*!
		\warning Not tested.
		\brief Destroys self
	*/
	void destroy();
	
	/*!
		\brief Gets the parent node
		\returns The parent node
	*/
	ASTree* const parent() const;
	
	/*!
		\brief Finds an ancestor node of a specified type
		\param _type The type of the node to apply
		\returns The ancestor or null if not found
	*/
	ASTree* const ancestor(const NodeType _type) const;
	
	/*!
		\brief  Finds a descendant node of a specified type
		\param _type The type of the node to apply
		\returns The descendant or null if not found
	*/
	ASTree* const descendant(const NodeType _type)  const;

	//NOT USED ASTree* const descendant(const char* _symbol)  const;

	/*!
		\brief Counts the children of a node of a specified type
		\param _type The type of the node to apply
		\returns The number of children
	*/
	int countChildren(const NodeType _type) const;
	
	/*!
		\brief Counts the children of a node
		\returns The number of children
	*/
	int countChildren() const;

	/*!
		\brief Counts the descendants of a node of a specified type
		\param _type The type of the node to apply
		\returns The number of descendants
		
	*/
	int countDescendants(const NodeType _type) const;

	/*!
		\brief Returns true if the node is a value-node
		\returns True if the node is a value node
	*/
	bool hasValue() const { return mType == PValue; }
	
	/*!
		\brief Returns true if the node is an identifier node
		\returns True if the node is an identifier node
	*/
	bool isIdentifier() const { return mType == PIdentifier; }

	/*!
		\brief Returns true if the node is a postfix identifier node
		\returns  True if the node is a postfix identifier node
	*/
	bool isPostfixIdentifier() const 
	{ return ((mType == PPostfixExpression) 
			|| (mType == PWindowOffsetPostfixExpression)); 
	}
	
	//! Getter for Symbol Name
	char* getSymbolName() const;
	
	//! Getter for Symbol table reference
	SymbolTable::Symbol* const getSymbol() const;

	//! Setter for Symbol
	void setSymbol(SymbolTable::Symbol* const _sym);

	//! Getter for Type
	NodeType getType() const { return mType; }

	//! Getter for Value
	TValue* const getValue() const { return mVal; }

        void startCompletDestroy();  ///RP:
        
#ifdef CEP_DEBUG	

	void print(int _level = 0) ;
	
	const char* asString() const;
	
#endif

protected:

	struct ChildNode
	{	
		ChildNode() : val(0), prev(0), next(0) {}
		
		ASTree* val;
		
		ChildNode* prev;
		ChildNode* next;
	};


	ASTree()
	{ 
		init();
	}

	void init();

        void completDestroy(ChildNode *_childTree);
	/*!
		\brief Gets the first child of the node. 
		\note With nested calls, use ASTreeItr.
	*/
	ASTree* const firstChild() const;
	
	/*!
		\brief Gets the next child of the node. 
		\note With nested calls, use ASTreeItr.
	*/
	ASTree* const nextChild() const;
	
private:
	
	NodeType mType;		// mandatory
	TValue* mVal;	// optional - only for constants TODO: rename to Value or ConstValue
	SymbolTable::Symbol* mSymbol;	
	
	ASTree* mParent;
	ChildNode* mParentChildNode;
	
	ChildNode* mLatestChild;
	mutable ChildNode* mChildIterator; // first/next
};



/*!
	\brief
	Iterator for ASTree.
	Decouples the ASTree from the logic that iterates
	through it. This permits thread safe operations and nested loops
	
	Usage:
		ASTreeItr itr(treeNode);
		
		ASTree* child = itr.firstChild();
		while(child)
			child = itr.nextChild();
*/

class ASTreeItr
{
public:

	ASTreeItr(ASTree* const _tree) :
		mTree(_tree)
	{
		assert(_tree);
	}
			
	ASTree* const firstChild() const
	{
		mCurrent = mTree->mLatestChild;
		
		if(mCurrent)
			return mCurrent->val;
			
		return 0;
	}
	
        ///RP:  this is the first read, but the last to access
	ASTree* const lastChild() const
	{
                mCurrent = mTree->mLatestChild;
		
		while(mCurrent->prev)
                    mCurrent = mCurrent->prev;
                
		return mCurrent->val;
	}

	
	ASTree* const nextChild() const
	{
		if(mCurrent)
			mCurrent = mCurrent->prev;
		
		if(mCurrent)
			return mCurrent->val;
			
		return 0;
	}

///RP:        
	ASTree* const prevChild() const
	{
		if(mCurrent)
			mCurrent = mCurrent->next;
		
		if(mCurrent)
			return mCurrent->val;
			
		return 0;
	}
        
	
protected:

private:
	
	ASTree* const mTree;
	mutable ASTree::ChildNode* mCurrent;

};

#endif

