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

//#include "../Trace.h"
#include "Trace.h"
#include <iostream>
#include <fstream>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#endif
///#include <iomanip>

namespace solcep{

/*
 *  Propagates trace messages to standard error output
 */
class StdOutTracer : public ITracer
{   
public:
    void Trace(std::string message) 
    {
        std::cerr << message;
    }
};

/**
 * Propagates log message to a file
 */
class FileTracer: public ITracer
{

	void Trace(std::string message)
	{
		if(LogFile)
		{
			if(LogFile->is_open())
			{
				*LogFile << message;
			}
		}
	}


public:

	/**
	 * \brief Destroyer. It checks if the log file is still open. If it is, this methods closes it and frees the memory.
	 */
	~FileTracer()
	{
		if(LogFile)
		{
			if(LogFile->is_open())
			{
				LogFile->close();
				free(LogFile);
			}
		}
	}

	/**
	 * \brief It sets the log file
	 * \param logFileName Log file path
	 */
	static void SetLogFileName(std::string logFileName)
	{
		// File path is extracted
		std::string path=SplitFileName(logFileName);
		int valid_path=0;

		//If path is empty, the file will be placed in the working directory
		if(!path.empty())
		{
			// We check if the path exists. If not, we create it.
			valid_path =CheckPath(path);
		}

		if(valid_path==0)
		{
			std::ofstream* temp_stream= new std::ofstream();
			temp_stream->open(logFileName,std::ofstream::out | std::ofstream::app);

			if(!temp_stream->is_open())
			{
				std::cerr << "Cannot open logfile "<< logFileName <<" for writing";
			}
			else
			{
				LogFile=temp_stream;
			}
		}
		else
		{
			std::cerr << "Cannot open path "<< path <<" for writing";
		}
	}

private:

	/**
	 * \brief This methods checks if a path exists. If not, it creates this path
	 * \param Path to be checked
	 * \return O if succeeded, 1 if not
	 */
    static int CheckPath(std::string pathName)
    {
        int result = -1;

        struct stat sb;
        bool bStat = (stat(pathName.c_str(), &sb) == 0);

#ifndef _WIN32
        bool isDir = S_ISDIR(sb.st_mode);
#else
        bool isDir = sb.st_mode & S_IFDIR;
#endif

        if (bStat)
        {
            if (isDir)
            {
                result = 0;
            }
        }
        else    // Path does not exist, so try to create it
        {
#ifndef _WIN32
            result = mkdir(pathName.c_str(), 0777);
#else
            result = _mkdir(pathName.c_str());
#endif
        }

        return result;
    }

	/**
	 * \brief This method extracts the path from a file name
	 * \param fileName
	 * \return File path if exists
	 */
	static std::string SplitFileName(std::string fileName)
	{
		size_t found;
		std::string path="";
		found= fileName.find_last_of("/\\");
		if(found!=std::string::npos)
		{
			path=fileName.substr(0,found);
		}

		return path;

	}

	static std::ofstream* LogFile;
};

/*  
 *  Propagates log messages to remote log writer.
 */

///RP
/*
class SockTracer : public ITracer
{
public:
    SockTracer(EndPoint& endPoint, std::unique_ptr<ITracer> tracer)
        : mTracer(std::move(tracer))
    {
        mSocket = make_unique<zmq::socket_t>(endPoint.getContext(), ZMQ_PUSH);
        std::string conStr = endPoint.getConnStr();
        mSocket->connect(conStr.c_str());
    }
    
    void Trace(std::string message)
    {     
        if(!mSocket->send(message.data(), message.size(), ZMQ_DONTWAIT))
            WARN() << "Socket busy, message was not sent!" << std::endl;
        if(mTracer)
            mTracer->Trace(message);
    }
    
private:
    std::unique_ptr<zmq::socket_t> mSocket;
    std::unique_ptr<ITracer> mTracer;
};
*/


}

