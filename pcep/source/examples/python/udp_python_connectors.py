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
import socket
from bcolors import bcolors

##
# It receives complex events  udp messages from bcep and prints them in the console.
# It will break when a SHUTDOWN command is received
#
def udp_publisher():
	UDP_IP="127.0.0.1"
	UDP_PORT= 50000
	SHUTDOWN_COMMAND= '7 Shutdown int Result 0'
	sock= socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
	sock.bind((UDP_IP,UDP_PORT))
	while(True):
		data,addr=sock.recvfrom(1024)
		print bcolors.OKGREEN+'[Publisher]'+data+ bcolors.ENDC
		if SHUTDOWN_COMMAND in data:
			break

##
# It sends events to BCEP through default UDP socket. After having sent all the commands, it will send a shutdown special event
# @param events Events to be sent to the BCEP
#
def udp_collector(events):
	UDP_IP="127.0.0.1"
	UDP_PORT= 29654
	SHUTDOWN_MESSAGE="153 Shutdown int scope 0"
	sock=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
	for event in events:
		print bcolors.OKBLUE+'[Collector]'+event+ bcolors.ENDC
		sock.sendto(event,(UDP_IP,UDP_PORT))
	sock.sendto(SHUTDOWN_MESSAGE,(UDP_IP,UDP_PORT))
