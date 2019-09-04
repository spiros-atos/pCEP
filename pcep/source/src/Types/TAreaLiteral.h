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



#ifndef _SOL_CEP_TYPES_AREA_LITERAL_H_
#define _SOL_CEP_TYPES_AREA_LITERAL_H_

///#include <stdio.h>
///#include <string.h>
///#include <stdlib.h>
///#include <time.h>
#include <math.h>
///#include <assert.h>

#include "../Common/GeoHelper.h"


struct TAreaLiteral
{
	char type; // 0-circular, 1-area
	
	float lat;
	float lon;
	float lat2;
	
	union 
	{
		float lon2;	//type 1
		float radius;	// type 0 (circular)
	};
	
	enum 
	{
		KAreaCircular = 0,
		KAreaRectangular = 1
	};
	
	bool operator==(const TAreaLiteral& rhs) 
	{
		if((type != rhs.type)
		|| (lat != rhs.lat)
		|| (lon != rhs.lon)
		|| (lon2 != rhs.lon2))
			return false;
			
		return (type == KAreaRectangular) ? 
			(lat2 == rhs.lat2) : true;
	}

	TAreaLiteral operator+=(const float& rhs) 
	{
		if(type == KAreaCircular)
			radius += rhs;
		else
		{
			// Really, the spec should have been made 'easier' by
			// specifying that the top-left and bottom-right corners
			// will be moved N/2 meters north-west and south-east, respectively
			// just like TPosLiteral +/- float.
			//
			// Instead, we follow the spec, by moving the top-left north by N/2
			// meters, them west by N/2 meters. The same procedure for 
			// bottom-left (first south, then east, by N/2 meters)
			//
			// (Same comment applies top operator-)
			//
			float dist = rhs / 2.0;
			
			GeoHelper::AddDistance(lat, lon, dist, 0.0);		//North
			GeoHelper::AddDistance(lat, lon, dist, 270.0); 	//West
			GeoHelper::AddDistance(lat2, lon2, dist, 180.0);	//South
			GeoHelper::AddDistance(lat2, lon2, dist, 90.0);	//East
		}
			
		sanityCheck();
		
		return *this;
		
	}
	
	TAreaLiteral operator-=(const float& rhs) 
	{
		if(type == KAreaCircular)
			radius -= rhs;
		else
		{
			float dist = rhs / 2.0;
			
			GeoHelper::AddDistance(lat, lon, dist, 180.0);	//South
			GeoHelper::AddDistance(lat, lon, dist, 90.0);	//East
			GeoHelper::AddDistance(lat2, lon2, dist, 0.0);		//North
			GeoHelper::AddDistance(lat2, lon2, dist, 270.0); 	//West
			
		}
			
		sanityCheck();
		
		return *this;
	}
	
	
	void sanityCheck()
	{
		if(type == KAreaCircular)
		{
			if(radius < 0.0)
				radius = 0.0;
		}
		else
		{
			if(lat < -90.0) lat = -90.0;
			if(lat > +90.0) lat = +90.0;
			if(lon < -180.0) lon = -180.0;
			if(lon > +180.0) lon = +180.0;
			//FIXME: geo sanity check. 
		}
	}
	
};


#endif

