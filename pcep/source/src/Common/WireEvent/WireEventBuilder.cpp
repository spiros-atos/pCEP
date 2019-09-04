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

#ifdef _WIN32
#include <time.h>
#include <iomanip>
#include <sstream>
#endif

#include "WireEventBuilder.h"
#include "../../Types/TPosLiteral.h"

namespace solcep
{
using namespace std;

WireEventBuilder::WireEventBuilder() { }

WireEventBuilder::~WireEventBuilder() {
    
    reset();
    
}

void WireEventBuilder::create(const int _type, string _name)
{
    reset();

    add(0x01);	// version 1
    add(_type);
    add(0x00);	// little endian (TODO: relialbly detect this info!!)

    if(_name.length() > 0)
        add(_name);
        
    string tmp = mStream.str();
    mPairPos = mStream.str().length();
    unsigned short pairs = 0;
    mStream.write(reinterpret_cast<const char*>(&pairs), sizeof(pairs));
///is used by the decode too
}
	
string WireEventBuilder::finalize()
{
    return mStream.str();
}

void WireEventBuilder::reset()
{
    mStream.str("");
    mStream.clear();
}

void WireEventBuilder::add(string name)
{
    auto length = static_cast<unsigned int>(name.length() | ID_MASK);
    mStream.write(reinterpret_cast<const char*>(&length), sizeof(length));
    mStream << name;
}
	
void WireEventBuilder::add(unsigned char byte)
{
    mStream << byte;
}

void WireEventBuilder::add(string name, const int value)
{
    add(name);	
    TValue v((int)value);
    serializeVar(v);
    
///    updatePairsCount();
    
///RP is neccesary for the count used in the Pairs
/*  moved to a new method
    std::string temp = mStream.str();
    short count=0;
    
    char* cPair = new char(2);
    temp.copy(cPair, sizeof(count), mPairPos);
    count= static_cast<short>(*cPair);
    
    count++;
    delete cPair;
    
    temp.replace(mPairPos, sizeof(count), (char*)&count, sizeof(count));
    reset();
    mStream << temp;
*/
    
}

void WireEventBuilder::add(string name, const float value)
{
    add(name);
    TValue v((float)value);
    serializeVar(v);
///	updatePairsCount();    
}

void WireEventBuilder::add(string name, const char* value)
{
    add(name);
    TValue v(value);
    serializeVar(v);
}

///RP
void WireEventBuilder::add(string name, const char value)
{
    add(name);
    TValue v(value);
    serializeVar(v);
}

///RP
void WireEventBuilder::add(string name, const TPosLiteral value)
{
    add(name);
    TValue v(value);
    serializeVar(v);
}

///RP
void WireEventBuilder::add(string name, const TAreaLiteral value)
{
    add(name);
    TValue v(value);
    serializeVar(v);
}
//Works with time data type (not is a timestamp) used to encode
void WireEventBuilder::add(string name, const TTimeLiteral value)
{
    add(name);
    TValue v(value);
    serializeVar(v);

}


///RP
void WireEventBuilder::addTPL(string name, const char* value)
{
    add(name);
    char *sep;
    sep= strtok((char*)value, "\\\\");    
    TPosLiteral tpl;
    tpl.lat = (float)atof(sep);
    sep= strtok(NULL, "\\\\");
    tpl.lon = (float)atof(sep);
            
    TValue v(tpl);
    serializeVar(v);    
}

void WireEventBuilder::addArea(string name, const char* value)
{
    add(name);
    char *sep;
    sep= strtok((char*)value, "\\\\");    
    
    TAreaLiteral area;
    area.lat = (float)atof(sep);
    
    sep= strtok(NULL, "\\\\");
    area.lon = (float)atof(sep);

    sep= strtok(NULL, "\\\\");    
    area.radius = (float)atof(sep);
    area.type = 0;
    
    sep= strtok(NULL, "\\\\");
    if (sep != NULL) {
        area.lat2 = area.radius;
        area.lon2 = (float)atof(sep);
        area.type = 1;
    }
    
    TValue v(area);
    serializeVar(v);
}

#ifdef _WIN32
char* WireEventBuilder::strptime(const char* s, const char* f, struct tm* tm)
{
    std::istringstream input(s);
    input.imbue(std::locale(setlocale(LC_ALL, nullptr)));
    input >> std::get_time(tm, f);
    if (input.fail()) {
        return nullptr;
    }
    return (char*)(s + input.tellg());
}
#endif

//Works with time data type (not is a timestamp) used to decode
void WireEventBuilder::addTime(string name, const char* value)
{
    add(name);
    TTimeLiteral v_time;
    struct tm tm {};

    if (strchr(value, '@') && strlen(value) > 8)
        strptime(value, "%Y-%m-%d@%H:%M:%S", &tm);
    else
        strptime(value, "%H:%M:%S", &tm);
    
    v_time.time = tm;
    
    TValue v(v_time);
    serializeVar(v);    
}

void WireEventBuilder::serializeVar(TValue& value)
{
    size_t count;	
    value.serialize(0, &count);
    auto buff = (unsigned char*)malloc(count);
    value.serialize(buff);
    mStream << string(reinterpret_cast<const char*>(buff), count);
    free(buff);
}

///RP: To update the count of Pairs in the Wire Event; could use the addNVPairCount(unsigned short count) method!!
void WireEventBuilder::updatePairsCount() {
    
    std::string temp = mStream.str();
    short count=0;
    
    char* cPair = new char(2);
    temp.copy(cPair, sizeof(count), mPairPos);
    count= static_cast<short>(*cPair);
    
    count++;
    delete cPair;
    
    temp.replace(mPairPos, sizeof(count), (char*)&count, sizeof(count));
    reset();
    mStream << temp;

    
}

}

