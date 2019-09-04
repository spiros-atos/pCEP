
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

#include <sstream>
#include <memory>
#include <ctime>

#define DEBUG() solcep::Trace().Debug()
#define INFO() solcep::Trace().Info()
#define WARN() solcep::Trace().Warn()
#define ERROR() solcep::Trace().Err()

namespace solcep {

template<typename T, typename ...Args>
std::unique_ptr<T> make_unique(Args&& ...args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

class ITracer
{
public:
    virtual void Trace(std::string message) = 0;
    virtual ~ITracer() {}
};
    
class Trace {
public:
    
    static int LogLevel;
    
    Trace(){};
    virtual ~Trace()
    {
        if(Trace::mTracer && Trace::LogLevel >= mLogLevel)
            Trace::mTracer->Trace(mStream.str());
    }

    static std::unique_ptr<ITracer> GetTracer()
    {
        return std::move(Trace::mTracer);
    }
    
    static void SetTracer(std::unique_ptr<ITracer> tracer)
    {
        Trace::mTracer = std::move(tracer);
    }

    virtual std::ostringstream& Debug()
    {
        mLogLevel = 3;
        mStream << GetCurrentTime() << " <DEBUG> :";
        return mStream;
    }

    virtual std::ostringstream& Info() 
    {
        mLogLevel = 2;
        mStream << GetCurrentTime() << " <INFO> :";
        return mStream;        
    }
    virtual std::ostringstream& Warn() 
    {
        mLogLevel = 1;
        mStream << GetCurrentTime() << " <WARNING> :";
        return mStream; 
    }
    virtual std::ostringstream& Err() 
    {
        mStream << GetCurrentTime() << " <ERROR> :";
        return mStream; 
    };
    
    void Terminate()
    {
        mStream << "$stop$";
    }

private:
///    int mLogLevel = 0;
        int mLogLevel = 4;
    std::ostringstream mStream;
    static std::unique_ptr<ITracer> mTracer;
    
    std::string GetCurrentTime()
    {
        char buffer[128];
        std::time_t now = std::time(nullptr);
        strftime(buffer, sizeof(buffer), "%a %b %e %X %y", std::localtime(&now));
        return std::string(buffer);
    }
};

} /* namespace solcep */
