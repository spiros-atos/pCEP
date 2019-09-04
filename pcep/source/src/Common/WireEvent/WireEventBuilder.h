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

#pragma once

///#include <string>
#include <sstream>
#include "../../Types/TValue.h"

namespace solcep {

/*!
        \brief Builds a flat, binary representation of SEvent, so it can
        be sent over the wire, between threads and/or processes.
	
        Supports version 1.
	
        \note IDs (non-string identifiers) are NOT supported
	
 */
class WireEventBuilder
{
public:
    
    WireEventBuilder();
    virtual ~WireEventBuilder();

    /*!
            \brief Creates a new event, discarding any
            other event being built
     */
    void create(const int _type, std::string _name = "");

    //! Adds number of NV-pairs
    void addNVPairCount(unsigned short count)
    {
        //Hack need to re-write!
        std::string temp = mStream.str();
        temp.replace(mPairPos, sizeof(count), (char*)&count, sizeof(count));
        reset();
        mStream << temp;
    }
    
    ///RP: 17122014
    void updatePairsCount();

    /*!
        Finalizes the event building.
        The reference to the internal event is kept in memory
        by the builder and is overwritten on create()

        \returns A reference to the built event.
     */
    std::string finalize();
    static const unsigned int ID_MASK = 0x80000000UL;
    void add(std::string name);
    void add(std::string name, const int value);
    void add(std::string name, const float value);
    void add(std::string name, const char* value);
    void add(std::string name, const char value);       ///RP
    void addTPL(std::string name, const char* value);
    void addArea(std::string name, const char* value);
    void addTime(std::string name, const char* value);

#ifdef _WIN32
    char* strptime(const char* s, const char* f, struct tm* tm);
#endif

    void add(std::string name, const TPosLiteral value);
    void add(std::string name, const TAreaLiteral value);
    void add(std::string name, const TTimeLiteral value);
    void serializeVar(TValue& value);
    void reset();
    
protected:

    void add(unsigned char byte);
///    void reset();
    void checkSize(const int _addedDataSize);

private:
    int mPairPos = 0;
    std::ostringstream mStream;
};

}


