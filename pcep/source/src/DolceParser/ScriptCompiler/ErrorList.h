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

/*
	Unified compiler error list 
*/
#ifndef _ERROR_LIST_H_
#define _ERROR_LIST_H_

#include <stdlib.h>
#include <assert.h>

/*!
	\brief Error list for use with Compiler
*/
class ErrorList
{
	friend class ErrorListItr;
public:

	ErrorList();
	
	~ErrorList();

	void addError(	const int _lineNum, 
					const int _colNum, 
					const char* _err,
					...);
	
	void print();
	
	bool hasError() const { return (mHead != 0); }
	
	//LL
	struct Error
	{
		int l;
		int c;
		char* s;
		
		struct Error* prev;
	};
	
	struct ErrList
	{
		int lineNum;
		
		Error* err;
	};

        Error* mHead;  ///RP: was 
        
protected:
	ErrorList(const ErrorList& rhs)	{}
	ErrorList& operator= (const ErrorList &rhs)	{ return *this; }
	
	ErrList* createSortedList(int& count_);
	
private:


	static const int MAX_MSG_SIZE = 256;

	

//	Error* mHead;           ///RP: to show more detail
	int mCount;
		
	static int comp(const void *v1, const void *v2);
};


//------------------------------------------------------------------------

/*
	Iterator for Error List.

	Permits independent iteration, e.g. from threads.
	
	Usage:
		ErrorListItr itr(errorListInstance);
		ErrorList::Error* err = itr.first();
		while(err)
		{
			err = itr.next();
		}
*/
class ErrorListItr
{
public:

	ErrorListItr(ErrorList* const _errList)
	{
		assert(_errList);
		
		mCurr = 0;
		
		mErrList = _errList->createSortedList(mCount);
	}
	
	
	~ErrorListItr()
	{
		free(mErrList);
	}
		
	
	ErrorList::Error* const first() const
	{
		return next();
	}
	
	
	ErrorList::Error* const next() const
	{
		ErrorList::Error* ret = 0;
		
		if(mCurr < mCount)
			ret = mErrList[mCurr].err;
			
		mCurr++;
		
		return ret;
	}
	
protected:

private:
	
	int mCount;
	mutable int mCurr;
	ErrorList::ErrList* mErrList;
};



#endif

