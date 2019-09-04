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
	Time 
*/
#ifndef _SOL_CEP_TYPES_TIME_LITERAL_H_
#define _SOL_CEP_TYPES_TIME_LITERAL_H_

#include <time.h>
///#include <assert.h>

#include "TDurationLiteral.h"

struct TTimeLiteral
{
	/* future:
	char mustRepeat; - Y if no YYYY-MM-DD specified 

	char yy;
	char mm;
	char dd;
	

	char hh;
	char mm;
	char ss;
	*/

	tm time;
	

	TTimeLiteral operator+=(const int& rhs) 
	{
		addTime((time_t)rhs);
		return *this;
	}
	
	
	TTimeLiteral operator+=(const TDurationLiteral& rhs) 
	{
		addTime(rhs);
		return *this;
	}

	TTimeLiteral operator-=(const int& rhs) 
	{
		substractTime(rhs);
		return *this;
	}
	
	TTimeLiteral operator-=(const TDurationLiteral& rhs) 
	{
		substractTime(rhs);
		return *this;
	}
	
	bool operator<(const TTimeLiteral& rhs)
	{
		time_t t1=mktime(&(this->time));
		time_t t2=mktime((tm*)&(rhs.time));
		bool result= t1<t2;
		return result;
	}
	
	bool operator==(const TTimeLiteral& rhs) 
	{
		time_t t1= mktime(&(this->time));
		tm rhs_tm=rhs.time;
		time_t t2=mktime(&rhs_tm);
		bool result= t1==t2;
		return result;
	}
	
	
	bool operator!=(const TTimeLiteral& rhs) 
	{
		return !(*this == rhs);
	}


	void addTime(const time_t _seconds)
	{
		time_t t = mktime(&(this->time));
		
		t += _seconds;		
		this->time=*(localtime(&t));
	}

	void substractTime(const time_t _seconds)
	{
		time_t t = mktime(&(this->time));
		t -= _seconds;
		this->time=*(localtime(&t));

	}

	
};



#endif

