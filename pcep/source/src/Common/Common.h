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

struct TPosLiteral;

/* 
    Common functions	
 */
class Common
{
public:

    virtual ~Common() {}

    /*!
        \brief Builds a path from a dir and a name, adding
                a path separator
        \param _dir The directory part. Must NOT end with path separator
        \param _name The file name
        \param _sep The path separator
        \returns A pointer to the full path. The caller is responsible for free()-ing the
                memory occupied by the path.
     */
///    static char* BuildPath(const char* _dir, const char* _name, const char _sep = '/');

///    static char* CopyStr(const char* _str);
///    static void ReleaseStr(const char* _str);
};


