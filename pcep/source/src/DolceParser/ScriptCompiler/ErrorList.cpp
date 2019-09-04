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
#include "ErrorList.h"

#include <stdio.h>
#include <string.h>
///#include <stdlib.h>
#include <stdarg.h>


ErrorList::ErrorList()
{
	mHead = 0;
	mCount = 0;
}


ErrorList::~ErrorList()
{
	Error* e = mHead;
	Error* prev = 0;

	while(e)
	{
		prev = e->prev;
		
		free(e->s);
		delete e;
		
		e = prev;
	}
}

void ErrorList::addError(const int _lineNum,
							const int _colNum,
							const char* _err,
							...)
{
	va_list args;

	static char msg[MAX_MSG_SIZE + 1];
	
	va_start(args, _err);
	vsnprintf(msg, MAX_MSG_SIZE, _err, args);
	va_end(args);

	// add it
	Error* e = new Error();
	
	e->l = _lineNum;
	e->c = _colNum;
	e->s = (char*)malloc(strlen(msg) + 1);
	strcpy(e->s, msg);
	
	e->prev = mHead;
	mHead = e;
	
	mCount++;

}

	
void ErrorList::print()
{
	ErrorListItr itr(this);
	
	Error* e = itr.first();

	while(e)
	{
#ifdef CEP_DEBUG
		fprintf(stdout, "Line %d: %s (CEP_DEBUG-MODE)\n", e->l, e->s);
#endif
		fprintf(stderr, "Line %d: %s\n", e->l, e->s);

		e = itr.next();
	}
	
	/*
	for(int i = 0; i < count; i++)
	{
#ifdef CEP_DEBUG
		fprintf(stdout, "Line %d: %s (CEP_DEBUG-MODE)\n", list[i].err->l, list[i].err->s);
#endif
		fprintf(stderr, "Line %d: %s\n", list[i].err->l, list[i].err->s);
	}
	*/
}


ErrorList::ErrList* ErrorList::createSortedList(int& count_)
{
	count_ = mCount;
	
	ErrList* list = (ErrList*)malloc(sizeof(ErrList) * count_);

	Error* e = mHead;
	
	int i = 0;
	while(e)
	{
		list[i++].err = e;
		e = e->prev;
	}
	
	qsort((void*)list, count_, sizeof(ErrList), comp);
	
	return list;
}





int ErrorList::comp(const void *v1, const void *v2)
{
	int first = ((ErrorList::ErrList*)v1)->err->l;
	int second = ((ErrorList::ErrList*)v2)->err->l;
	
	if(first > second)
		return 1;
	if(first < second)
		return -1;
	return 0;
}

