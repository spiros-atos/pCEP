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
	Common types 
*/
#ifndef _SOL_CEP_GEO_HELPER_H_
#define _SOL_CEP_GEO_HELPER_H_



class GeoHelper
{
	// PI/180
	static constexpr double DEG_TO_RAD = 0.017453292519943295769236907684886;
	static constexpr double EARTH_RADIUS_KM = 6371.0; 
	
public:

	
	/*!
		\brief Calculates the distance in between two 
			geographical points, accordng to the haversine formula
		\param _lat1 Latitude of first position
		\param _lon1 Longitude of first position
		\param _lat2 Latitude of second position
		\param _lon2 Longitude of second position
		
		\see http://www.movable-type.co.uk/scripts/latlong.html

		\return the distance between the two positions in meters
	*/
	static float Distance(	const double _lat1,
							const double _lon1,
							const double _lat2,
							const double _lon2)
	{
		double dLat = (_lat2-_lat1) * DEG_TO_RAD;
		double dLon = (_lon2-_lon1) * DEG_TO_RAD;

		double lat1 = _lat1 * DEG_TO_RAD;
		double lat2 = _lat2 * DEG_TO_RAD;

		double sinLat = sin(dLat/2.0); sinLat *= sinLat;
		double sinLon = sin(dLon/2.0); sinLon *= sinLon;
		
		double a = sinLat +	sinLon * cos(lat1) * cos(lat2); 
		double c = 2.0 * atan2(sqrt(a), sqrt(1.0-a)); 
		
		return (float)(EARTH_RADIUS_KM * c * 1000.0);
	}
				

				
	/*!
		\brief Calculates a new position by adding a distance
			with a bearing to a current position
		\param _lat_ Latitude of current position and that of the 
				new position on return.
		\param _lon1 Longitude of current position and that of the 
			new position on return.
		\param _distance Distance in meters 
		\param _bearing The bearing (direction) in which to travel (0-360 clockwise)
		
		\see http://www.movable-type.co.uk/scripts/latlong.html
	*/
	static void AddDistance(	float& _lat_,
								float& _lon_,
								const float _distance,
								const float _bearing)
	{
		double bearing = _bearing * DEG_TO_RAD;
		double angularDist = ((_distance / 1000.0) / EARTH_RADIUS_KM);
		
		double latOrig = _lat_ * DEG_TO_RAD;
		double lonOrig = _lon_ * DEG_TO_RAD;

		double sinDist = sin(angularDist);
		double cosDist = cos(angularDist);
		
		double lat = asin( sin(latOrig) * cosDist + 
					cos(latOrig) * sinDist * cos(bearing) );
			  
		double lon = lonOrig + atan2(
						sin(bearing) * sinDist * cos(latOrig), 
						cosDist - sin(latOrig) * sin(lat));
						
		_lat_ = lat / DEG_TO_RAD;
		_lon_ = lon / DEG_TO_RAD;
	}
};


#endif 
