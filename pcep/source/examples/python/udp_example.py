#!/usr/local/bin/python

# /*
#  * Copyright (C) 2019  Atos Spain SA. All rights reserved.
#  *
#  * This file is part of pCEP.
#  *
#  * pCEP is free software: you can redistribute it and/or modify it under the
#  * terms of the Apache License, Version 2.0 (the License);
#  *
#  * http://www.apache.org/licenses/LICENSE-2.0
#  *
#  * The software is provided "AS IS", without any warranty of any kind, express or implied,
#  * including but not limited to the warranties of merchantability, fitness for a particular
#  * purpose and noninfringement, in no event shall the authors or copyright holders be
#  * liable for any claim, damages or other liability, whether in action of contract, tort or
#  * otherwise, arising from, out of or in connection with the software or the use or other
#  * dealings in the software.
#  *
#  * See README file for the full disclaimer information and LICENSE file for full license
#  * information in the project root.
#  *
#  * Authors:  Atos Research and Innovation, Atos SPAIN SA
#  */

import bcolors
import udp_python_connectors
import os, sys                            # get unix, python services
import time
import multiprocessing
import signal
import psutil
from stat import ST_SIZE                  # or use os.path.getsize
from glob import glob                     # file name expansion
from os.path import exists                # file exists test
from multiprocessing import Process
from bcolors import bcolors
#rom time import time, ctime              # time functions

## @package udp_example
# This python module aims to create examples for BCEP application. To use it: python udp_example.py {path_to_example_file}
# @param A command line argument to receive the example file definition
#
# The BCEP application must be started with UDP default UDP configuration
# The example file must include the set of events to be sent to BCEP
#

print 'Example start.'
print 'path:', os.getcwd(  )              # current directory
print 'time:', time.ctime(), '\n'


if len(sys.argv)<2:
	print bcolors.FAIL+'FAILED: To use this script: python udp_example.py {example_file}'+ bcolors.ENDC
else:
	example = sys.argv[1]

	if exists(example):
		#Example file is read first.
		with open(example) as f:
			content = f.read().splitlines()
		if len(content)<1:
			print bcolors.FAIL+'FAILED: '+example+'must contain at least one event'+ bcolors.ENDC
		else:

				p1= Process(target=udp_python_connectors.udp_publisher,args=())
				p2= Process(target=udp_python_connectors.udp_collector,args=(content,))

				p1.start()
				time.sleep(1)
				p2.start()

				p1.join()
				p1.terminate
				p2.terminate
	else:
		print bcolors.FAIL+'FAILED:'+ example+' does not exist'+ bcolors.ENDC

print 'Example finished:', time.ctime()
