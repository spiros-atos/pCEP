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
///#include <thread>
///#include <mutex>



namespace solcep
{

/*!
	\brief Sends periodic ticks to the Engine
	
*/
class Ticker
{
public:
	Ticker(const int _interval = 1);
	virtual ~Ticker();
        bool canExecute();
        void publishData(std::string message);       
        void sleepMs(int mSec) 
        {
///            std::this_thread::sleep_for(std::chrono::milliseconds(mSec));
        }
        
        

protected:
	
	int mInterval; //seconds
	virtual void processPublishedMessage(std::string message);
	void work();
        
        
private:
    bool mCanExecute;
///    std::mutex mMutex;
///    std::unique_ptr<std::thread> mThread;

        
};

}


