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



#ifndef _SOL_CEP_TYPES_ADDRESS_H_
#define _SOL_CEP_TYPES_ADDRESS_H_

///#include <stdio.h>
#include <string.h>
///#include <stdlib.h>

/*
	Address to locate attribtes, externals etc.
	
	Usage of members in addressing:
	
	Attributes
		oid - attribute oid
		ownerOid - event oid
		ref - windowOffset
		
	Externals
		oid - external oid
		
	Complex event function value
		oid - complex event function value
	
*/
struct TAddress
{
	TAddress(): oid(-1L), ownerOid(-1L), ref(0)
	{}
	
	TAddress(long _attrOid, long _eventOid, int _windowOffset)
		: oid(_attrOid)
		, ownerOid(_eventOid)
		, ref(_windowOffset)
	{}
	
	TAddress(long _oid)
		: oid(_oid)
		, ownerOid(-1L)
		, ref(0)
	{}
		
	long oid;
	long ownerOid;
	int ref;
	

	/*!
		\brief Serializes the object into a memory location.
		\param mem_ memory location. must be big enough to hold a TAddress
		\param calc_ if non-null, store size needed for serialization.
		\returns address of serialized data
	*/
	unsigned char* serialize(unsigned char* mem_, size_t* calc_ = 0) const
	{
		size_t offs = (size_t)0;
		
		if(!calc_) memcpy(mem_ + offs, (void*)&oid, sizeof(oid)); offs += sizeof(oid);
		if(!calc_) memcpy(mem_ + offs, &ownerOid, sizeof(ownerOid)); offs += sizeof(ownerOid);
		if(!calc_) memcpy(mem_  + offs, &ref, sizeof(ref)); offs += sizeof(ref);

		if(calc_)
			*calc_ = offs;
			
		return mem_;
	}
	
	
	TAddress* deserialize(unsigned char* _mem)
	{
		size_t offs = (size_t)0;
		
		memcpy(&oid, _mem + offs, sizeof(oid)); offs += sizeof(oid);
		memcpy(&ownerOid, _mem + offs, sizeof(ownerOid)); offs += sizeof(ownerOid);
		memcpy(&ref, _mem + offs, sizeof(ref)); 
		
		return this;
	}
	
};

#endif

