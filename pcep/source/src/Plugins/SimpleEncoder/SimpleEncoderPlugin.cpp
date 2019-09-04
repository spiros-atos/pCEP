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


#include "SimpleEncoderPlugin.h"

///#include <string>
///RP: #include <cstring>
#include <stdexcept>
#include "../../Types/SEvent.h"

using namespace std;

int SimpleEncoderPlugin::load() 
{ 
	mWD = new solcep::WireEventDecoder();
	return 0;
}

void SimpleEncoderPlugin::unload()
{
    mWD->externalDestroy(1);
    delete mWD;
}

string SimpleEncoderPlugin::encode(string message)
{
	SEvent* e = mWD->create((unsigned char*)message.c_str(), message.length());

	if(!e)
            throw runtime_error("Unable to encode message!");

        char buff[256];
///        sprintf(buff, "%s", e->name);
        sprintf(buff, "%d %s", e->type, e->name); 
	
        string str = buff;
        
	SNVPairs* attr = e->nvPairs;
	
	while(attr)
	{
		if(attr->val.t == t_int)
			str += " int";
		else if(attr->val.t == t_float)
			str += " float";
		else if(attr->val.t == t_string)
			str += " string";
        	else if(attr->val.t == t_char)
			str += " char";
                else if(attr->val.t == t_pos)
			str += " pos";
                else if(attr->val.t == t_area)
			str += " area";
		else if(attr->val.t == t_time)
			str += " time";
		else
                    str += " [UNSUPPORTED-TYPE]";

		
                sprintf(buff, " %s %s", attr->name, attr->val.asStringRaw());
///RP::28102015                                sprintf(buff, " %s %s ", attr->name, attr->val.asString());
		str += buff;

		attr = attr->prev;
	}
	
	
	str += "\n";
        
        return str;
}






