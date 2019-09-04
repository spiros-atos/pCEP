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
# import glob
# import os


def receive_udp_messages(output_test_file):    

    UDP_IP="127.0.0.1"
    UDP_PORT= 50000
    SHUTDOWN_COMMAND= '7 Shutdown int Result 0'
    sock= socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
    sock.bind((UDP_IP,UDP_PORT))
    ofile = open(output_test_file, 'a')
    while(True):
        data, addr = sock.recvfrom(1024)
        ofile.write(data)
        print data, addr
        if SHUTDOWN_COMMAND in data:
            break
                        

def main():

    # pdb.set_trace()
    test = r'C:\Users\a696457\dev\CLASS\branch_collision_detection\pcep\source\test\OC\temp'
    output_file = '.'.join([test, 'out'])
    receive_udp_messages(output_file)


# e.g.
# %run "C:/Users/a696457/dev/deveo/bcep_port2win/source/test/send_udp_msgs.py"
# 
if __name__ == '__main__':

    main()
