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


/* 
	Internal error codes
	
	NOTE: 0-99 are reserved.
	
*/
#ifndef _SOL_CEP_ERR_H_
#define _SOL_CEP_ERR_H_

static const int OK = 0;

static const int OK_VER = 1;


// Common Errors

static const int ERR_CM_READFILE = 901;
static const int ERR_CM_WRITEFILE = 902;
static const int ERR_CM_FILEIO = 903;
static const int ERR_EVAL_FALSE = 904;

// XML Config Errors
static const int ERR_CFX_NODE_NOT_FOUND = 300;

// Framework Builder
static const int ERR_EB_EVENT_NOT_FOUND = 101;
static const int ERR_EB_EXPECTED_EVENT = 102;
static const int ERR_EB_EXPECTED_EA_LIST = 103;
static const int ERR_EB_EXPECTED_EXT_DECL = 104;
static const int ERR_EB_UNEXPECTED_TYPE = 105;
static const int ERR_EB_COMPLEX_EVENT_BUILD_ERR = 106;

// Parser
static const int ERR_PA_PARSE_ERRORS = 200;
static const int ERR_PA_UNRESOLVED_REFERENCE = 201;
static const int ERR_PA_INTERNAL_ERROR = 202;

// Analyzer
static const int ERR_AN_INTERNAL_ERR = 400;
static const int ERR_AN_LASTS_ERR = 401;
static const int ERR_AN_DETECT_REF_ERR = 402;

// Infrastructure errors
static const int ERR_THREAD_CREATE = 500;

// Decoder errors
static const int ERR_DECODER = 600;

// Coordinator Errors
static const int ERR_CO_SYSTEM = 700;
static const int ERR_CO_OPT = 707;
static const int ERR_CO_CONFIG = 706;
static const int ERR_CO_ENV = 701;
static const int ERR_CO_LOGGING = 702;
static const int ERR_CO_PROCESS_START = 703;
static const int ERR_CO_SCRIPT = 704;
static const int ERR_CO_MESSAGING = 705; // general messaging err.


// Admin Interface Errors
static const int ERR_CC_CONNECTION = 800;
static const int ERR_ADM_SOLCEP_ROOT_EXPECTED = 801;
static const int ERR_ADM_UNKNOWN_RESOURCE = 802;
static const int ERR_ADM_BAD_OPERATION = 803;
static const int ERR_ADM_SERVER_ERROR = 804;


// Instruction Evaluator errors
static const int ERR_IE_ABORT = 1000;
static const int ERR_IE_INV_PROG_VER = 1001;

// Plugin errors
static const int ERR_PLUGIN_OPEN = 1200;
static const int ERR_PLUGIN_SYMBOL = 1201;
static const int ERR_PLUGIN_INIT = 1202;
static const int ERR_PLUGIN_CREATE = 1203;
static const int ERR_PLUGIN_PARAM = 1204;


// Logger errors
static const int ERR_LOG_FILE = 1100;

// Infrastructure errors
static const int ERR_CANNOT_START_WORKER = 1300;
static const int ERR_LISTENER_NOT_STARTED = 1301;
static const int ERR_DECODER_NOT_STARTED = 1302;
static const int ERR_ENCODER_NOT_STARTED = 1303;
static const int ERR_EMITTER_NOT_STARTED = 1304;
static const int ERR_NO_DECODER_FOR_LISTENER = 1311;
static const int ERR_NO_ENCODER_FOR_EMITTER = 1312;
static const int ERR_DECODER_INIT = 1313;
static const int ERR_LISTENER_INIT = 1314;
static const int ERR_NO_FREE_PORT = 1315;
static const int ERR_ENCODER_INIT = 1316;
static const int ERR_EMITTER_INIT = 1317;


// Message infrastructure errors
static const int ERR_MSG_CONTEXT = 1400;
static const int ERR_MSG_NO_PORT = 1401;
static const int ERR_MSG_CONTROL_SOCKET = 1402;
static const int ERR_MSG_SOCKET = 1403;
static const int ERR_MSG_BIND = 1404;
static const int ERR_MSG_CONNECT = 1405;
static const int ERR_MSG_NO_MEM = 1406;
static const int ERR_MSG_SEND = 1407;

// Complex Event Detector
static const int ERR_CED_DECODE = 1500;

// REST
static const int ERR_REST_LISTENER_NOT_STARTED = 2000;

#endif
