# -*- coding: utf-8 -*-

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

import pdb
import socket
import sys
import time
# import glob
# import os


def send_udp_message(events):

    UDP_IP="127.0.0.1"
    UDP_PORT= 29654
    SHUTDOWN_MESSAGE="153 Shutdown int scope 0"
    sock=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
    for event in events:
#        sock.sendto(event,(UDP_IP,UDP_PORT))	
        sock.sendto(str.encode(event),(UDP_IP,UDP_PORT))	
        time.sleep(0.1)
    sock.sendto(str.encode(SHUTDOWN_MESSAGE),(UDP_IP,UDP_PORT))


def main(testfile):

#    pdb.set_trace()    
            
    with open(testfile) as f:
        content = f.read().splitlines() 
               
    send_udp_message(content[1:])


# e.g.
# %run "C:/Users/a696457/dev/deveo/bcep_port2win/source/test/send_udp_msgs.py" "C:\Users\a696457\dev\gitlab\bcep\source\test\test_1.in"
# 
if __name__ == '__main__':
    
#    pdb.set_trace()
    testfile = sys.argv[1]
    main(testfile)
