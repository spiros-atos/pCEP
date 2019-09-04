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
	TValue - This is the main type used throughout the CEP 
*/
#ifndef _SOL_CEP_TYPES_VALUE_H_
#define _SOL_CEP_TYPES_VALUE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
///#include <time.h>
///#include <math.h>
#include <assert.h>

#include "TType.h"
#include "TTimeLiteral.h"
#include "TDurationLiteral.h"
#include "TPosLiteral.h"
#include "TAreaLiteral.h"

#include "../Common/GeoHelper.h"




enum TDurationUnit
{	
	d_years = 10,
	d_months,
	d_weeks,
	d_days,
	d_hours,
	d_minutes,
	d_seconds
};


union TContent
{
	char				charVal;
	int					integerVal;
	float				floatVal;
	TTimeLiteral		timeVal;
	TDurationLiteral	durationVal;
	TPosLiteral			posVal;
	TAreaLiteral		areaVal;
	TDurationUnit 		duration_unit;	// Yacc wants this here. It is not used otherwise.
	char*				stringVal;
};

/*
	FIXME: Move to doc SOL-CEP-0002 Technical Design
	behaviour of ersatz (E) with variable x
	
	E + x = E
	E - x = E
	E / x = E
	E * x = E
	
	E <= x = false
	E >= x = false
	E == x = false
	E != x = false
	
	E && x = false && x
	E || x = false || x
*/


struct TValue
{
public:
	TValue() { t = t_invalid; }

	TValue(const char _c) 
		{ v.charVal = _c; t = t_char; }

	TValue(const int _v)
		// FIXME: check if lexer / parser deal correctly with sign
		{ v.integerVal = _v; t = t_int; }
	
	TValue(const float _v) 
		{ v.floatVal = _v; t = t_float;  }

	TValue(const TTimeLiteral& _val) 
		{ v.timeVal = _val; t = t_time;   }
		
	// Ctor for ersatz value. Some random signature was
	// chosen (not trying to occupy a future 'bool' type)
	TValue(const int _er, const int _sa, const int _tz)
		{ t = t_ersatz; }


	TValue(const TDurationLiteral& _duration)
		{ v.durationVal = _duration; t = t_duration; }

	TValue(const TPosLiteral& _val) 
		{ v.posVal = _val;  t = t_pos;  }

	TValue(const TAreaLiteral& _val) 
		{ v.areaVal = _val;  t = t_area;  }

	// TODO: Caller is responsible for cleaning up source string.
	TValue(const char* _v)
	{
		v.stringVal = (char *)_v;
		if(_v)
		{
			v.stringVal = (char*)malloc(strlen(_v) + 1);
			strcpy(v.stringVal, _v);
		}
		t = t_string;
	}

	TType t;
	TContent v;
	
	bool isValid() const { return t != t_invalid; }
	
	float asFloat()
	{
		if(t == t_float)
			return v.floatVal;
		else if(t == t_int)
			return (float)v.integerVal;
		else
		{	
			return 0.0;
			//XXX ERR handling
		}
	}
	
	// Caller must check validity with isValid()
	TValue operator+=(const TValue& rhs) 
	{
		TType rhsType = rhs.t;
		
		if(rhsType == t_ersatz)
		{
			t = t_ersatz;	
			return *this;
		}

		if(t == t_int)
		{
			if(rhsType == t_int)
				v.integerVal += rhs.v.integerVal;
			else if(rhsType == t_float)
			{
				t = t_float; // promote to float
				v.floatVal  = (float)v.integerVal + rhs.v.floatVal;
			}
		}
		else if(t == t_float)
		{
			if(rhsType == t_float)
				v.floatVal += rhs.v.floatVal;
			else if(rhsType == t_int)
				v.floatVal += (float)rhs.v.integerVal;
		}
		else if(t == t_duration)
		{
			if(rhsType == t_int)
				v.durationVal += (TDurationLiteral)rhs.v.integerVal;
			else if(rhsType == t_duration)
				v.durationVal += rhs.v.durationVal;
				
		}
		else if(t == t_time)
		{
			if(rhsType == t_int)
				v.timeVal += rhs.v.integerVal;
			else if(rhsType == t_duration)
				v.timeVal += rhs.v.durationVal;
		}
		else if(t == t_pos)
		{
			if(rhsType == t_float)
			{
				GeoHelper::AddDistance(v.posVal.lat, 
									v.posVal.lon,
									rhs.v.floatVal, 
									135.0); //southeast-bearing
			}
		}
		else if(t == t_area)
		{
			if(rhsType == t_float)
				v.areaVal += rhs.v.floatVal;
		}
			
		return *this;
	}
	
	
	TValue operator+(const TValue& rhs) 
	{
		TValue result = *this;
		result += rhs;
		
		return result;
	}
	
	
	TValue operator-=(const TValue& rhs) 
	{
		TType rhsType = rhs.t;
		
		if(rhsType == t_ersatz)
		{
			t = t_ersatz;	
			return *this;
		}

		if(t == t_int)
		{
			if(rhsType == t_int)
				v.integerVal -= rhs.v.integerVal;
			else if(rhsType == t_float)
			{
				t = t_float; // promote to float
				v.floatVal  = (float)v.integerVal - rhs.v.floatVal;
			}
		}
		else if(t == t_float)
		{
			if(rhsType == t_float)
				v.floatVal -= rhs.v.floatVal;
			else if(rhsType == t_int)
				v.floatVal -= (float)rhs.v.integerVal;
		}
		else if(t == t_duration)
		{
			if(rhsType == t_int)
				v.durationVal -= (TDurationLiteral)rhs.v.integerVal;
			else if(rhsType == t_duration)
				v.durationVal -= rhs.v.durationVal;
				
		}
		else if(t == t_time)
		{
			if(rhsType == t_int)
				v.timeVal -= rhs.v.integerVal;
			else if(rhsType == t_duration)
				v.timeVal -= rhs.v.durationVal;
		}
		else if(t == t_pos)
		{
			if(rhsType == t_pos)
			{
				t = t_float;
				v.floatVal = GeoHelper::Distance(v.posVal.lat,
										v.posVal.lon,
										rhs.v.posVal.lat,
										rhs.v.posVal.lon);
			}
			else if(rhsType == t_float)
			{
				GeoHelper::AddDistance(v.posVal.lat,
									v.posVal.lon,
									rhs.v.floatVal, 
									315.0); //northwest-bearing
			}
		}
		else if(t == t_area)
		{
			if(rhsType == t_float)
				v.areaVal -= rhs.v.floatVal;
		}
		
		return *this;
	}
	
		
	TValue operator-(const TValue& rhs) 
	{
		TValue result = *this;
		result -= rhs;
		
		return result;
	}

	
	TValue operator*=(const TValue& rhs) 
	{
		TType rhsType = rhs.t;
		
		if(rhsType == t_ersatz)
		{
			t = t_ersatz;	
			return *this;
		}

		if(t == t_int)
		{
			if(rhsType == t_int)
			{
				v.integerVal *= rhs.v.integerVal;
			}
			else if(rhsType == t_float)
			{
				t = t_float; // promote to float
				v.floatVal  = (float)v.integerVal * rhs.v.floatVal;
			}
		}
		else if(t == t_float)
		{
			if(rhsType == t_float)
				v.floatVal *= rhs.v.floatVal;
			else if(rhsType == t_int)
				v.floatVal *= (float)rhs.v.integerVal;
		}
#ifdef _SUPPORTED_
		add more types
#endif //_SUPPORTED_
			
		return *this;
	}
	
		
	TValue operator*(const TValue& rhs) 
	{
		TValue result = *this;
		result *= rhs;
		
		return result;
	}	
	
	
	TValue operator/=(const TValue& rhs) 
	{
		TType rhsType = rhs.t;
		
		if(rhsType == t_ersatz)
		{
			t = t_ersatz;	
			return *this;
		}

		// Catch div/0
		if(((rhsType == t_int) && (rhs.v.integerVal == 0L))
		 ||((rhsType == t_float) && (rhs.v.floatVal == 0.0)))
		{
			t = t_invalid;
			return *this;
		}
		
		if(t == t_int)
		{
			if(rhsType == t_int)
				v.integerVal /= rhs.v.integerVal;
			else if(rhsType == t_float)
			{
				t = t_float; // promote to float
				v.floatVal  = (float)v.integerVal / rhs.v.floatVal;
			}
		}
		else if(t == t_float)
		{
			if(rhsType == t_float)
				v.floatVal /= rhs.v.floatVal;
			else if(rhsType == t_int)
				v.floatVal /= (float)rhs.v.integerVal;
		}
			
		return *this;
	}
	
		
	TValue operator/(const TValue& rhs) 
	{
		TValue result = *this;
		result /= rhs;
		
		return result;
	}	

	
	bool operator==(const TValue& rhs) 
	{
		TType rhsType = rhs.t;

		if(rhsType == t_ersatz)
		{
			return false;
		}

		if(t == t_int)
		{
			if(rhsType == t_int)
			{
				return v.integerVal == rhs.v.integerVal;
			}
			else if(rhsType == t_float)
				return v.integerVal == (int)rhs.v.floatVal;
		}
		else if(t == t_float)
		{
			if(rhsType == t_float)
				return v.floatVal == rhs.v.floatVal;
			else if(rhsType == t_int)
				return v.floatVal == (float)rhs.v.integerVal;
		}
		else if(t == t_duration)
		{
			if(rhsType == t_int)
				return v.durationVal == (TDurationLiteral)rhs.v.durationVal;
			else if(rhsType == t_duration)
				return v.durationVal == rhs.v.durationVal;
		}
		else if(t == t_time)
		{
			if(rhsType == t_time)
				return v.timeVal == rhs.v.timeVal;
			else
			{
				assert(0);
			}
		}
		else if(t == t_pos)
		{
			if(rhsType == t_pos)
				return v.posVal == rhs.v.posVal;
			else
				assert(0);
		}
		else if(t == t_area)
		{
			if(rhsType == t_area)
				return v.areaVal == rhs.v.areaVal;
		}
///RP:
                else if(t == t_string)
		{
///printf("COMPARA %s - %s ", v.stringVal, rhs.v.stringVal);
			if(rhsType == t_string)
				return !strcmp(v.stringVal, rhs.v.stringVal);
		}                

		return false;
	}
	
	
	bool operator!=(const TValue& rhs) 
	{
		return !(*this == rhs);
	}
	
	
	bool operator<(const TValue& rhs) 
	{
		TType rhsType = rhs.t;

		if(rhsType == t_ersatz)
		{
			return false;
		}

		if(t == t_int)
		{
			if(rhsType == t_int)
				return v.integerVal < rhs.v.integerVal;
			else if(rhsType == t_float)
				return v.integerVal < (int)rhs.v.floatVal;
		}
		else if(t == t_float)
		{
			if(rhsType == t_float)
				return v.floatVal < rhs.v.floatVal;
			else if(rhsType == t_int)
				return v.floatVal < (float)rhs.v.integerVal;
		}
		else if(t == t_time)
		{
			if(rhsType == t_time)
			{
				return v.timeVal < rhs.v.timeVal;
			}
			else
			{
				assert(0);
			}
		}
#ifdef _SUPPORTED_
	//FIXME-TODO: add support for this
		else if(t == t_duration)
		{
			if(rhsType == t_int)
				v.durationVal += (TDurationLiteral)rhs.v.integerVal;
			else if(rhsType == t_duration)
				v.durationVal += rhs.v.durationVal;
		}
		else if(t == t_pos)
		{

		}
#endif // _SUPPORTED_
		return false;
	}
	
	bool operator>(const TValue& rhs) 
	{
		return (TValue)rhs < *this;
	}
	
	bool operator<=(const TValue& rhs) 
	{
            if (this->t == t_ersatz || rhs.t == t_ersatz)
                return false;

            return !((TValue)rhs < *this);
	}

	bool operator>=(const TValue& rhs) 
	{
            if (this->t == t_ersatz || rhs.t == t_ersatz)
                return false;

            return !(*this < rhs);
	}

	TValue TV_abs()
	{
		if(t == t_int)
		{
			v.integerVal= abs((int)v.integerVal);
		}

		return *this;
	}

#ifdef CEP_DEBUG
	/*!
		\brief Sanity check if serialize/deserialize will succeeed with the
			Wire Event Protocol.
	*/
    void assertMachineTypes() const
	{
		assert(sizeof(unsigned char) == 1);
		assert(sizeof(v.charVal) == 1);
		assert(sizeof(v.integerVal) == 4);
		assert(sizeof(v.floatVal) == 4);
		
		// Avoid calc errors due to struct alignment
		assert(sizeof(v.posVal.lat) * 2 == 8);
			
		// Avoid calc errs due to struct alignment
		assert(sizeof(v.areaVal.type) + 4*sizeof(v.areaVal.lat) == 17);
		
		assert(sizeof(v.durationVal) == 4);
#ifndef _WIN32
        assert(sizeof(v.timeVal.time) == 56);
#else
        assert(sizeof(v.timeVal.time) == 36);
        //printf("YEAH*** YEAH*** YEAH***\n");
        //printf("%d", sizeof(v.timeVal.time));
#endif
	}
#endif

	/*!
		\brief Serializes the TValue into a memory location. Serialization follows the Wire Event
			Protocol.
		\param mem_ memory location. must be big enough to hold a TAddress
		\param calc_ if not null, only calculate the needed size, and store result.Do not serialize.
		\returns address of serialized data
	*/
	unsigned char* serialize(unsigned char* mem_, size_t* calc_ = 0) const
	{
#ifdef CEP_DEBUG
		assertMachineTypes();
#endif

		size_t offs = (size_t)0;
		
		unsigned char ttype = (unsigned char)t;
		
		if(!calc_) memcpy(mem_ + offs, &ttype, sizeof(unsigned char)); 
		offs += sizeof(unsigned char);
		
		//NOTE: string is not serialized as it is handled specially 
		//  in the expression evaluator programs.
		// FIXME-TODO: MUST serialze it, so we can use this method for the wire event protocol
		//
		if(t == t_char)
		{
			if(!calc_) memcpy(mem_ + offs, &v.charVal, sizeof(v.charVal));
			offs += sizeof(TType);
		}
		else if(t == t_string)
		{
			if(!calc_){
                            memcpy(mem_ + offs, &v.stringVal, sizeof(v.stringVal));
                        }
			offs += sizeof(v.stringVal);
		}
		else if(t == t_int)
		{
			if(!calc_) memcpy(mem_ + offs, &v.integerVal, sizeof(v.integerVal));
			offs += sizeof(TType);
		}
		else if(t == t_float)
		{
			if(!calc_) memcpy(mem_ + offs, &v.floatVal, sizeof(v.floatVal));
			offs += sizeof(TType);
		}
		else if(t == t_pos)
		{
			if(!calc_) memcpy(mem_ + offs, &v.posVal.lat, sizeof(v.posVal.lat));
			offs += sizeof(v.posVal.lat);
			if(!calc_) memcpy(mem_ + offs, &v.posVal.lon, sizeof(v.posVal.lon));
			offs += sizeof(v.posVal.lon);
		}
		else if(t == t_area)
		{
			if(!calc_) memcpy(mem_ + offs, &v.areaVal.type, sizeof(v.areaVal.type));
			offs += sizeof(v.areaVal.type);
			if(!calc_) memcpy(mem_ + offs, &v.areaVal.lat, sizeof(v.areaVal.lat));
			offs += sizeof(v.areaVal.lat);
			if(!calc_) memcpy(mem_ + offs, &v.areaVal.lon, sizeof(v.areaVal.lon));
			offs += sizeof(v.areaVal.lon);
			if(!calc_) memcpy(mem_ + offs, &v.areaVal.lat2, sizeof(v.areaVal.lat2));
			offs += sizeof(v.areaVal.lat2);
			if(!calc_) memcpy(mem_ + offs, &v.areaVal.lon2, sizeof(v.areaVal.lon2));
			offs += sizeof(v.areaVal.lon2);
		}
		else if(t == t_duration)
		{
			if(!calc_) memcpy(mem_ + offs, &v.durationVal, sizeof(v.durationVal));
			offs += sizeof(v.durationVal);
		}
		else if(t == t_time)
		{
			if(!calc_) memcpy(mem_ + offs, &v.timeVal.time.tm_year, sizeof(v.timeVal.time.tm_year));
			offs += sizeof(v.timeVal.time.tm_year);
			if(!calc_) memcpy(mem_ + offs, &v.timeVal.time.tm_mon, sizeof(v.timeVal.time.tm_mon));
			offs += sizeof(v.timeVal.time.tm_mon);
			if(!calc_) memcpy(mem_ + offs, &v.timeVal.time.tm_mday, sizeof(v.timeVal.time.tm_mday));
			offs += sizeof(v.timeVal.time.tm_mday);

			if(!calc_) memcpy(mem_ + offs, &v.timeVal.time.tm_hour, sizeof(v.timeVal.time.tm_hour));
			offs += sizeof(v.timeVal.time.tm_hour);
			if(!calc_) memcpy(mem_ + offs, &v.timeVal.time.tm_min, sizeof(v.timeVal.time.tm_min));
			offs += sizeof(v.timeVal.time.tm_min);
			if(!calc_) memcpy(mem_ + offs, &v.timeVal.time.tm_sec, sizeof(v.timeVal.time.tm_sec));
			offs += sizeof(v.timeVal.time.tm_sec);
		}
		
		if(calc_)
			*calc_ = offs;
			
		return mem_;
	}

	
	
	/*!
		\brief Deserializes from a memory location. The deserialized content is assumed
		to be in the Wire Event Protocol format.
		\param mem_ memory location from which to deserialize.
		\param count_ if not null, contains the number of bytes that was deserialized.
		\returns address of serialized data
	*/
        TValue* deserialize(const unsigned char* _mem, size_t* count_ = 0)
	{
#ifdef CEP_DEBUG
		assertMachineTypes();
#endif

		size_t offs = (size_t)0;
		
		char ttype;
		memcpy(&ttype, _mem + offs, 1); offs += 1;
		t = (TType)ttype;
		
		//NOTE: string is not serialized as it is handled specially 
		//  in the expression evaluator programs.
		//
		if(t == t_char)
		{
			memcpy(&v.charVal, _mem + offs, sizeof(v.charVal));
			offs += sizeof(TType);
		}
		else if(t == t_string)
		{
			memcpy(&v.stringVal, _mem + offs, sizeof(v.stringVal));
			offs += sizeof(v.stringVal);
		}
		else if(t == t_int)
		{
			memcpy(&v.integerVal, _mem + offs, sizeof(v.integerVal));
			offs += sizeof(TType);
		}
		else if(t == t_float)
		{
			memcpy(&v.floatVal, _mem + offs, sizeof(v.floatVal));
			offs += sizeof(TType);
		}
		else if(t == t_pos)
		{
			memcpy(&v.posVal.lat, _mem + offs, sizeof(v.posVal.lat));
			offs += sizeof(v.posVal.lat);
			memcpy(&v.posVal.lon, _mem + offs, sizeof(v.posVal.lon));
			offs += sizeof(v.posVal.lon);
		}
		else if(t == t_area)
		{
			memcpy(&v.areaVal.type, _mem + offs, sizeof(v.areaVal.type));
			offs += sizeof(v.areaVal.type);
			memcpy(&v.areaVal.lat, _mem + offs, sizeof(v.areaVal.lat));
			offs += sizeof(v.areaVal.lat);
			memcpy(&v.areaVal.lon, _mem + offs, sizeof(v.areaVal.lon));
			offs += sizeof(v.areaVal.lon);
			memcpy(&v.areaVal.lat2, _mem + offs, sizeof(v.areaVal.lat2));
			offs += sizeof(v.areaVal.lat2);
			memcpy(&v.areaVal.lon2, _mem + offs, sizeof(v.areaVal.lon2));
			offs += sizeof(v.areaVal.lon2);
		}
		else if(t == t_duration)
		{
			memcpy(&v.durationVal, _mem + offs, sizeof(v.durationVal));
			offs += sizeof(v.durationVal);
		}
		else if(t == t_time)
		{
			v.timeVal.time={0};
			memcpy(&v.timeVal.time.tm_year, _mem + offs, sizeof(v.timeVal.time.tm_year));
			offs += sizeof(v.timeVal.time.tm_year);
			memcpy(&v.timeVal.time.tm_mon, _mem + offs, sizeof(v.timeVal.time.tm_mon));
			offs += sizeof(v.timeVal.time.tm_mon);
			memcpy(&v.timeVal.time.tm_mday, _mem + offs, sizeof(v.timeVal.time.tm_mday));
			offs += sizeof(v.timeVal.time.tm_mday);

			memcpy(&v.timeVal.time.tm_hour, _mem + offs, sizeof(v.timeVal.time.tm_hour));
			offs += sizeof(v.timeVal.time.tm_hour);
			memcpy(&v.timeVal.time.tm_min, _mem + offs, sizeof(v.timeVal.time.tm_min));
			offs += sizeof(v.timeVal.time.tm_min);
			memcpy(&v.timeVal.time.tm_sec, _mem + offs, sizeof(v.timeVal.time.tm_sec));
			offs += sizeof(v.timeVal.time.tm_sec);
		}
		
		if(count_)
			*count_ = offs;
			
		return this;
	}
	

	// NOTE: Really for debug purposes, but now used by SimpleEncoderPlugin
	//
	const char* asString() const
	{
		static char fmt[256]; //FIXME: NOT THREAD SAFE!
		
		if(t == t_char)	{ sprintf(fmt, "%c/c", v.charVal); return fmt;}
		if(t == t_int)	{ sprintf(fmt, "%d/i", v.integerVal);  return fmt;}
		if(t == t_float)	{ sprintf(fmt, "%f/f", v.floatVal); return fmt; }
		if(t == t_time)	
		{ 
			sprintf(fmt, "%02d:%02d:%02d/t", (int)v.timeVal.time.tm_hour,
									(int)v.timeVal.time.tm_min,
									(int)v.timeVal.time.tm_sec);
			return fmt;
		}
		if(t == t_duration)	
		{ 
/*
			sprintf(fmt, "%d %s", (int)v.durationVal.duration, 
									durationTypeAsString(v.durationVal.units));
*/
			sprintf(fmt, "%d/d", (TDurationLiteral)v.durationVal);
			return fmt;
		}
		if(t == t_area)
		{
			if(v.areaVal.type == TAreaLiteral::KAreaCircular)
			{
				sprintf(fmt, "lat=%f/lon=%f,rad=%f/a", 
						v.areaVal.lat, 
						v.areaVal.lon, 
						v.areaVal.radius);
			}
			else if(v.areaVal.type == TAreaLiteral::KAreaRectangular)
			{
				sprintf(fmt, "lat=%f/lon=%f,lat=%f/lon=%f/ra", 
				v.areaVal.lat,
				v.areaVal.lon,
				v.areaVal.lat,
				v.areaVal.lon);
			}
			return fmt;
		}
		if(t == t_pos)
		{
			sprintf(fmt, "lat=%f/lon=%f/p", v.posVal.lat, v.posVal.lon);
			return fmt;
		}
		if(t == t_string)
		{
			sprintf(fmt, "'%s'", v.stringVal);
			return fmt;
		}
		if(t == t_ersatz)
		{
			sprintf(fmt, "Ersatz");
			return fmt;
		}
	
		sprintf(fmt,"asString??");
		return fmt;
	}


	const char* asStringRaw() const
	{
		static char fmt[256]; //FIXME: NOT THREAD SAFE!
		
		if(t == t_char)	{ sprintf(fmt, "%c", v.charVal); return fmt;}
		if(t == t_int)	{ sprintf(fmt, "%d", v.integerVal);  return fmt;}
		if(t == t_float)	{ sprintf(fmt, "%f", v.floatVal); return fmt; }
		if(t == t_time)	
		{ 
			if(v.timeVal.time.tm_year!=0 && v.timeVal.time.tm_mon>=0
					&& v.timeVal.time.tm_mday>0)
			{
				strftime(fmt,sizeof(fmt),"%Y-%m-%d@%X",&v.timeVal.time);
			}
			else
			{
				sprintf(fmt, "%02d:%02d:%02d", (int)v.timeVal.time.tm_hour,
										(int)v.timeVal.time.tm_min,
										(int)v.timeVal.time.tm_sec);
			}

			return fmt;
		}
		if(t == t_duration)	
		{ 
/*
			sprintf(fmt, "%d %s", (int)v.durationVal.duration, 
									durationTypeAsString(v.durationVal.units));
*/
			sprintf(fmt, "%d", (TDurationLiteral)v.durationVal);
			return fmt;
		}
		if(t == t_area)
		{
			if(v.areaVal.type == TAreaLiteral::KAreaCircular)
			{
///				sprintf(fmt, "lat=%f/lon=%f,rad=%f/a", 
///                                sprintf(fmt, "%f\\%f\\%f", 
                                sprintf(fmt, "%f\\%f\\%f",
						v.areaVal.lat, 
						v.areaVal.lon, 
						v.areaVal.radius);
			}
			else if(v.areaVal.type == TAreaLiteral::KAreaRectangular)
			{
///				sprintf(fmt, "lat=%f/lon=%f,lat=%f/lon=%f/ra",
                                sprintf(fmt, "%f\\%f\\%f\\%f",
				v.areaVal.lat,
				v.areaVal.lon,
				v.areaVal.lat2,
				v.areaVal.lon2);
			}
			return fmt;
		}
		if(t == t_pos)
		{
///			sprintf(fmt, "lat=%f/lon=%f/p", v.posVal.lat, v.posVal.lon);
                    	sprintf(fmt, "%f\\%f", v.posVal.lat, v.posVal.lon);
			return fmt;
		}
		if(t == t_string)
		{
			sprintf(fmt, "%s", v.stringVal);
			return fmt;
		}
		if(t == t_ersatz)
		{
			sprintf(fmt, "Ersatz");
			return fmt;
		}
	
		sprintf(fmt,"asString??");
		return fmt;
	}
	
	const char* durationTypeAsString(const TDurationUnit _unit) const
	{
		if(_unit == d_years) return "years";
		if(_unit == d_months) return "months";
		if(_unit == d_weeks) return "weeks";
		if(_unit == d_days) return "days";
		if(_unit == d_hours) return "hours";
		if(_unit == d_minutes) return "minutes";
		if(_unit == d_seconds) return "seconds";

		return "durationTypeAsString:???";
	}


	

};

#endif

