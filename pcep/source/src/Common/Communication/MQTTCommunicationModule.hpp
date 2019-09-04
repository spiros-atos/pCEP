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



#ifndef MQTTCOMMUNICATIONMODULE_HPP
#define	MQTTCOMMUNICATIONMODULE_HPP

#include "CommunicationModule.hpp"
#include <stdio.h>
#include "mosquitto.h"
#include "../Queue.h"
#include "../Configuration/ReadIniFile.h"

namespace solcep {

class MQTTCommunicationModule : public CommunicationModule {
public:
    struct mosquitto *mosq = NULL;
    static Queue mosqQueue;

    MQTTCommunicationModule(IniFileSection iniFileSection);
    ~MQTTCommunicationModule();
    virtual char * receiveData();
    virtual void sendData(const char* message);



private:

    static char * topicCollect;
    static char * topicPublish;
    static void my_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str);
    static void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);
    static void my_connect_callback(struct mosquitto *mosq, void *userdata, int result);
    static void my_subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos);
    static void addMessage(void * message);
    static string format(void * vmessage);
    static char * stringToCharArr(std::string cad);

};


Queue MQTTCommunicationModule::mosqQueue;
char * MQTTCommunicationModule::topicCollect = NULL;
char * MQTTCommunicationModule::topicPublish = NULL;
int collectQoS= 2;
int publishQoS= 2;

MQTTCommunicationModule::MQTTCommunicationModule(IniFileSection iniFileSection) {
    //mosqQueue = new Queue();
    IniFileSection fileSection= iniFileSection;
    
    const char * host = fileSection.hostMQTT.c_str();
    int port = fileSection.portMQTT;
    topicCollect = stringToCharArr(fileSection.topicMQTTCollect);
    topicPublish = stringToCharArr(fileSection.topicMQTTPublish);
    
    collectQoS= fileSection.mqttCollectQoS;
    publishQoS= fileSection.mqttPublishQoS;

    mosquitto_lib_init();
    int keepAlive = fileSection.mqttKeepAlive;
    bool clean_session = true;

    mosq = mosquitto_new(iniFileSection.mqttClientId.c_str(), clean_session, NULL);
    if ((!fileSection.userMQTT.empty()) || (!fileSection.passwordMQTT.empty())){
            mosquitto_username_pw_set(mosq,fileSection.userMQTT.c_str(),fileSection.passwordMQTT.c_str());
    }

    if (!mosq) {
        fprintf(stderr, "Error: Out of memory.\n");
    }
    if (mosquitto_connect(mosq, host, port, keepAlive)) {
        fprintf(stderr, "Unable to connect.\n");
    }
    mosquitto_log_callback_set(mosq, my_log_callback);
    mosquitto_connect_callback_set(mosq, my_connect_callback);
    mosquitto_message_callback_set(mosq, my_message_callback);
    mosquitto_subscribe_callback_set(mosq, my_subscribe_callback);
    
    mosquitto_loop_start(mosq);
}

MQTTCommunicationModule::~MQTTCommunicationModule() {
    mosquitto_loop_stop(mosq,true);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    delete topicCollect;
    delete topicPublish;
}

char * MQTTCommunicationModule::receiveData() {
    return  stringToCharArr(mosqQueue.wait_and_pop());
}

void MQTTCommunicationModule::sendData(const char* message){
    mosquitto_publish(mosq,NULL,topicPublish,strlen(message),message,publishQoS,false);
}

void MQTTCommunicationModule::my_log_callback(mosquitto* mosq, void* userdata, int level, const char* str) {
    printf("%s\n", str);
}

void MQTTCommunicationModule::my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message) {
    if (message->payloadlen) {
        printf("%s %s\n", message->topic, (char *) message->payload);
        addMessage(message->payload);
    } else {
        printf("%s (null)\n", message->topic);
    }
    //fflush(stdout);
}

void MQTTCommunicationModule::my_connect_callback(struct mosquitto *mosq, void *userdata, int result) {
    if (!result) {
        // Subscribe to broker information topics on successful connect
        //mosquitto_subscribe(mosq, NULL, "$SYS/#", 2);
        mosquitto_subscribe(mosq, NULL, topicCollect, collectQoS);
    } else {
        fprintf(stderr, "Connect failed\n");
    }
}

void MQTTCommunicationModule::my_subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos) {
    int i;

    printf("Subscribed (mid: %d): %d", mid, granted_qos[0]);
    for (i = 1; i < qos_count; i++) {
        printf(", %d", granted_qos[i]);
    }
    printf("\n");
}

void MQTTCommunicationModule::addMessage(void * message) {
    mosqQueue.push(format(message));
}

std::string MQTTCommunicationModule::format(void* vmessage){
    return std::string((char *)vmessage);
}

char * MQTTCommunicationModule::stringToCharArr(std::string cad){
    char * cop = new char[cad.length() + 1];
    strcpy(cop, cad.c_str());
    return cop;
    
}



#endif	/* MQTTCOMMUNICATIONMODULE_HPP */

}

/* for testing
int main(int argc, char** argv) {
    
    CommunicationModule * cm = new MQTTCommunicationModule();

    while (true){
       string scad = cm->receiveData();
       if (scad.size()>0){
           const char * cad = scad.c_str();
            printf("received data \n");           
            printf(cad);
            printf("...............\n");
            cm->sendData("mandado datito");
       }

    }

    
    
    return 0;

}

*/
