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


#include "Queue.h"

Queue::Queue() {
    mQueue = 0;
    mHead = 0;
}

Queue::~Queue() {
    
    while(pop().size()>0)
        ;
}

void Queue::push(std::string _msg)
{
	Item* newItem = new Item();

	newItem->msg = _msg;
	newItem->next = 0;

	if (mHead)
		mQueue->next = newItem;
	else
		mHead = newItem;

	mQueue = newItem;

	cv.notify_one();
        
}


std::string Queue::wait_and_pop()
{
    std::unique_lock<std::mutex> guard(m);

    if(!mHead)
    {
    	cv.wait(guard);
    }
    
    return pop();
}

std::string Queue::pop()
{
    std::string strToReturn;
    if(mHead) 
    {
        if(mHead->msg.size()>0)
           strToReturn = mHead->msg;
        
        Item *curr = mHead;
        mHead = mHead->next;
///		delete curr->msg;
        delete curr;		
    }
	
    return strToReturn;
}

