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

struct SEvent;

namespace solcep{
/*!
    \brief Decodes a flat, binary version of SEvent into an SEvent.
 */
class WireEventDecoder
{
public:
    
    WireEventDecoder();
    virtual ~WireEventDecoder();
    /*!
            \brief Creates a new event, discarding any
            other event being built
            \param _data The wire event data, i.e. the internal SOL/CEP event
            \param _dataLen The size of the wire event data
            \returns A reference to the decoded event. 
                    The memory is owned by WireEventDecoder.
     */
    SEvent* create(const unsigned char* _data, const int _dataLen);
///        SEvent* create(char* _data, const int _dataLen);
    
///RP to manage memory leak    
    void externalDestroy(int discard);
        
protected:

    void reset();
    bool getVersion();
    bool getType();
    bool getByteOrder();
    bool getEventId();

    bool getNVPairCount(int& nvPairCount_);

    /*!
        \brief Gets and identifier, which is either an ID or a string
        \param id_ The id
        \param s_ Name. 0 if it's an ID, a pointer to a new allocated string 
                otherwise. The caller must be clean up this string
     */
    bool getIdentifier(unsigned long& id_, char*& s_);

private:

    const unsigned char* mData;
///    unsigned char* mData;
    unsigned int mDataLen;
    unsigned long mOffset;
    SEvent* mEvent;
};

}