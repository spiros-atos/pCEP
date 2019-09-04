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


///#include <stdio.h>
///#include <stdlib.h>
///#include <string.h>
///#include <math.h>
///#include <assert.h>

#include "Common.h"

/*
char* Common::BuildPath(const char* _dir, const char* _name, const char _sep)
{
	assert(_dir);
	assert(_name);
	
///	char* path = (char*)malloc(strlen(_dir) + strlen(_name) + 2);
	
///	sprintf(path, "%s%c%s", _dir, _sep, _name);
	
	return path;
}
*/

/*
char* Common::CopyStr(const char* _str)
{
    if(_str)
    {
///        char* newStr = (char*)malloc(strlen(_str) + 1);

        strcpy(newStr, _str);

        return newStr;
    }

    return 0;
}
*/


///void Common::ReleaseStr(const char* _str)
///{
///    if(_str)
///        free((void*)_str);
///}

static const double DEG_TO_RAD = 0.017453292519943295769236907684886;
static const double EARTH_RADIUS_KM = 6371.0;