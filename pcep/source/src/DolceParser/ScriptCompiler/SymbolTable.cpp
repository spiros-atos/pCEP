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



#include "SymbolTable.h"
///#include <stdio.h>
///#include <string.h>
///#include <stdlib.h>

#include "../../Types/TValue.h"
#include "../../ComplexEventDetector/Engine/Types/EComplexEvent.h"
#include "../../ComplexEventDetector/Engine/Types/EEvent.h"
#include "../../ComplexEventDetector/Engine/Types/EExternal.h"

#include "ASTree.h"


SymbolTable::SymbolTable()
{
	mHead = 0;
	mCounter = 0;
}


SymbolTable::~SymbolTable()
{
	//FIXME cleanup
}



	
SymbolTable::Symbol* const SymbolTable::add(const char* _name,
											const int _lineNum,
											const int _colNum)
{
	Symbol* sym = new Symbol();
	
	sym->name = (char*)malloc(strlen(_name) + 1);
	strcpy(sym->name, _name);
	
	sym->lineNum = _lineNum;
	sym->colNum = _colNum;
	
	sym->prev = mHead;
	mHead = sym;

	return sym;
}	


SymbolTable::Symbol* SymbolTable::find(const char* _symbol, SType _symbolType)
{
	if(!_symbol)
		return 0;

	Symbol* iter = mHead;

	while(iter)
	{
		if(iter->name 
		&& !strcmp(_symbol, iter->name)
		&& (iter->symbolType == _symbolType))
		{
			return iter;
		}
		
		iter = iter->prev;
	}

	return 0;
}


SymbolTable::Symbol* SymbolTable::find(	const char* _attr, 
										const char* _scopeName,
										const bool _includeReferences)
{
#ifdef CEP_DEBUG
	printf("SymbolTable::find(%s.%s) [%s references]\n"
	, _scopeName, _attr, _includeReferences?"Including":"Excluding");
#endif
	if(!_attr || !_scopeName)
		return 0;

	SymbolTableItr itr(this);

	Symbol* sym = itr.first();
	while(sym)
	{
		if(!strcmp(_attr, sym->name)
		&& sym->owner
		&& !strcmp(_scopeName, sym->owner->name)
		&& (_includeReferences
			|| (!_includeReferences && (sym->symbolType != reference))))
		{
			return sym;
		}
		
		sym = itr.next();
	}

	return 0;

}


SymbolTable::Symbol* SymbolTable::find(	const char* _symbol, 
										SymbolTable::Symbol* _ref)
{
	if(!_symbol || !_ref)
		return 0;

	Symbol* iter = mHead;

	while(iter)
	{
		if(iter->name 
		&& !strcmp(_symbol, iter->name)
		&& (iter->owner == _ref))
		{
			return iter;
		}
		
		iter = iter->prev;
	}

	return 0;
}



SymbolTable::Symbol* SymbolTable::find(const char* _str,
										const bool _includeReferences)
{
	if(!_str)
		return 0;
		
	Symbol* iter = mHead;
	
	while(iter)
	{
		if(iter->name
		&& !strcmp(_str, iter->name)
		&& (_includeReferences
			|| (!_includeReferences && (iter->symbolType != reference))))
		{
			return iter;
		}
			
		iter = iter->prev;
	}
	return 0;
}



bool SymbolTable::isKnownType(ASTree* _node) const
{
	return dataTypeFromNode(_node) != t_invalid;
}



TType SymbolTable::dataTypeFromNode(ASTree* const _node) const
{
	int type = _node->getType();
	
	if(type == ASTree::PPos) return t_pos;
	if(type == ASTree::PArea) return t_area;
	if(type == ASTree::PChar) return t_char;
	if(type == ASTree::PInt) return t_int;
	if(type == ASTree::PFloat) return t_float;
	if(type == ASTree::PString) return t_string;
	if(type == ASTree::PDuration) return t_duration;
	if(type == ASTree::PTime) return t_time;

	return t_invalid;
}

	
//NOTE: this method must be present also in non-debug: used for error reporting.
const char* SymbolTable::dataTypeAsString(const TType _type) const
{
	if(_type == t_invalid)
		return "[invalid]";
	
	if(_type == t_pos) return "pos";
	if(_type == t_area) return "area";
	if(_type == t_char) return "char";
	if(_type == t_int) return "int";
	if(_type == t_float) return "float";
	if(_type == t_string) return "string";
	if(_type == t_duration) return "duration";
	if(_type == t_time) return "time";


	return "UNDEFINED";
}

	
#ifdef CEP_DEBUG
void SymbolTable::print()
{
	printf("\n++ Symbol Table ++\n");
	
	SymbolTableItr itr(this);
	
	SymbolTable::Symbol* sym = itr.first();
	
	
	while(sym)
	{
		if(sym->engineObj)
		{
			// assuming that OID always comes first in struct.
			// Just use EEvent to provide context
			//
			printf("%ld ", ((EEvent*)sym->engineObj)->oid);
		/*
			if(sym->symbolType == event_decl)
				printf("%3d ", ((EEvent*)sym->engineObj)->oid);
			else if(sym->symbolType == complex_decl)
				printf("%3d ", ((EComplexEvent*)sym->engineObj)->oid);
			else if(sym->symbolType == external_decl)
				printf("%3d ", ((EExternal*)sym->engineObj)->oid);
			else if(sym->symbolType == payload_attr)
				printf("%3d ", ((EExternal*)sym->engineObj)->oid);
			else
				printf("??? ");
		*/
		}
		else
			printf("    ");
			
		printf("%p: %15s %2d %8s %8s @%d", 
			sym,
			sym->name,
			sym->lineNum,
			symbolHasDataType(sym->symbolType) ? dataTypeAsString(sym->dataType) : "",
			symbolTypeAsString(sym->symbolType),
			sym->windowOffset);
			
		if(sym->decl)
		{
			printf(" ->%s %s (%p)", sym->decl->name, 
									symbolTypeAsString(sym->decl->symbolType),
									sym->decl);
		}

		if(sym->owner)
		{
			printf("  [%s %s (%p)]", sym->owner->name, 
									symbolTypeAsString(sym->owner->symbolType),
									sym->owner);
		}
		
		if(sym->value)
		{
			printf(" %s", sym->value->asString());
		}

		printf(" EngineObj: %p", sym->engineObj);
		

		printf("\n");
		sym = itr.next();
	}
	printf("\n");
}

const char* SymbolTable::symbolTypeAsString(const SType _symbolType) const
{
	if(_symbolType == event_decl) return "event";
	if(_symbolType == complex_decl) return "cmplx";
	if(_symbolType == event_attr_decl) return "eattr";
	if(_symbolType == payload_attr_decl) return "pattr";
	if(_symbolType == external_decl) return "ext";
	if(_symbolType == reference) return "ref";
	if(_symbolType == complex_event_function) return "ce-func";
	if(_symbolType == unknown) return "huh?";

	return "SymbolTable::typeAsString: unknown type";
}


bool SymbolTable::symbolHasDataType(const SType _symbolType) const
{
	return (_symbolType == event_attr_decl) 
		|| (_symbolType == payload_attr_decl) 
		|| (_symbolType == external_decl)
		;
}

#endif 



