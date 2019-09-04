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


#include "Stack.h"

#include "../../Types/TValue.h"

Stack::Stack()
{
	mSP = 0;
	mHead = 0;
	
}

Stack::~Stack()
{
	
	while(pop())
		;
		
}

void Stack::push(const TValue& _val)
{
#ifdef CEP_DEBUG
	printf("Stack::push | %s\n", _val.asString());
#endif	

	Item* newItem = new Item();
	
	newItem->val = new TValue(_val);
	
	newItem->prev = mHead;
	mHead = newItem;
}	
	
bool Stack::pop(TValue* val_)
{
    if(mHead)
    {
        if(val_)
        {
            *val_ = *(mHead->val);
			
#ifdef CEP_DEBUG
            printf("Stack::pop | %s\n", val_->asString());
#endif	
        }
			
        Item* curr = mHead;
		
        mHead = mHead->prev;

        delete curr->val;
        delete curr;
		
        return true;
    }
	
    return false;
}

TValue* Stack::peek()
{
	if(mHead)
	{
#ifdef CEP_DEBUG
		printf("Stack::peek | %s\n", mHead->val->asString());
#endif		
		return mHead->val;
	}
	
	return 0;
}



