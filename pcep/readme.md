# Description
BCEP is a Complex Event Processor (CEP). It's software that performs operations on events. Common event processing operations include reading, creating, transforming, and deleting events (ETZION & NIBLETT, 2011). It can also be considered as a method of analyzing real-time streams of data about things that have happened and deriving a conclusion from them (LUCKHAM, 2011). It typically processes input events in order to produce a smaller set of output events.

BCEP uses DOLCE to define the rules of the CEP engine. DOLCE stands for “Description Language for Complex Events”. You can see below an example in *"How to use it"* section.

BCEP is the core of SOLCEP but it has been extracted and improved.

ATOS proposes a distributed, scalable and "cloudified" architecture for Complex Event Processing which is the outcome of several years of research from European projects:

![CEP Architecture](https://bscclass01.bsc.es/class/pcep/raw/master/pcep/docs/CEP_architecture.png "CEP architecture")

In general, these functional blocks are not software modules and they need to be specifically implemented for each project.

The only implemented (and reusable) block in this architecture is the Complex Event Detector, called microCEP or BCEP. This project targets this specific component.

This page is only the "Getting Started" for BCEP project. Further information is available in https://bscclass01.bsc.es/class/pcep/wikis/home

# How to use it

Note that BCEP supports two different communication protocols to receive events and publish detected complex events:
* UDP
* MQTT (Linux only)

This tutorial is focused on the first option. Please, refer to the complete guide for more detaled information about MQTT.

## Using docker

Launch an instance of BCEP using Docker and listening on port 29654. Complex events will be published by default on port 50000.

### Linux

```
$ sudo apt-get install docker-engine
$ sudo docker run -p 29654:29654/udp -t danielcalvo/cep-udp
```
### Windows

Download and install docker [here](https://store.docker.com/editions/community/docker-ce-desktop-windows), then from a command prompt:

```
>docker-machine start default
>docker run -p 29654:29654/udp -t danielcalvo/cep-udp
```

## Example of usage

We use [docker-compose](https://docs.docker.com/compose/), [docker](https://www.docker.com/) and some python scripts to provide a complete functional example of BCEP.
For more examples, please visit https://bscclass01.bsc.es/class/pcep/wikis/home

### Requirements

* docker
* docker-compose

#### Linux

You can install them using a package manager:

```
$ sudo apt-get install docker-engine
$ sudo apt-get install docker-compose
```
#### Windows

You can download them and install them from [here](https://store.docker.com/editions/community/docker-ce-desktop-windows)

### Running the example

* Clone bcep repository in your workspace
* In the root folder of the repository, execute:

#### Linux

```
$ sudo docker-compose -f source/examples/docker-compose.yml up
```

#### Windows

```
>docker-machine start default
>docker-compose -f source/examples/docker-compose.yml up
```

It will download the needed docker images and execute them with the appropriate configuration.

### Understanding the example

The contents of the docker-compose file are:

```
bcep:
    image: danielcalvo/cep-udp
    volumes:
      - ./:/tmp/examples
    net: "host"
    ports:
      - "29654:29654/udp"
    command: -d /tmp/examples/detect_1.dolce
connectors:
    build: ./python
    volumes:
      - ./:/tmp/examples
    command: python2 /tmp/examples/python/udp_example.py /tmp/examples/example_1.in
    net: "host"
```

As it can be seen, it runs two images:
1. bcep: BCEP application which listens on port 29654 and executes [detect_1.dolce](https://bscclass01.bsc.es/class/pcep/raw/master/pcep/source/examples/detect_1.dolce)

2. connectors: A python [script](https://bscclass01.bsc.es/class/pcep/raw/master/pcep/source/examples/python/udp_example.py). It creates the following processes:
 * A collector: It reads the events from [example_1.in](https://bscclass01.bsc.es/class/pcep/raw/master/pcep/source/examples/example_1.in) and sends them through UDP to BCEP.
 * A publisher: It listens on UDP port 50000 (default for BCEP) for complex events published by BCEP and prints them in the console.

Below, the content of the dolce file is presented. Comments have been added to improve understandability.

```
/*
Definition of temperature event.
It has the following attributes:
- a SensorId: To identify the sensor
- a Description: To add some information about the sensor
- a Position: Where the sensor is placed
- a ReadingTime: When the sensor has performed this reading
- a Value: The temperature value
The accept statement allows filtering. In this case, we are going to consider only some sensor Ids
*/
event TemperatureReading
{
    use
    {
        int SensorId,
        string Description,
				pos Position,
				time ReadingTime,
        float Value
    };

    accept { SensorId > 10 };
}

/*
This complex definition allows calculating the average temperature.
- The detect statement defines that this Complex will consider only as input the TemperatureReading signal
- The where statement allows also to filter some values. In this case, we only want to consider values greater than -10
- The payload defines the attributes of the complex event. Now, only the average temperature.
*/
complex AverageTemperature
{
	payload {
		float average = avg(Value)
   	};
   	detect TemperatureReading where (Value > -10) in [10];
}
```

BCEP waits for inputs events in the follwing format:

```
1 {event_name} att_type_1 att_id_1 att_value_1 att_type_2 att_id_2 att_value_2 ... att_type_n att_id_n att_value_n
```

Examples are presented in example_1.in file:

```
1 TemperatureReading int SensorId 10 string Description "Temperature sensor placed in Madrid, central train station" pos Position 7.675946\45.069258 time ReadingTime 2016-05-13T11:47:37Z float Value 25.7
1 TemperatureReading int SensorId 11 string Description "Temperature sensor placed in Seville, central train station" pos Position 45.675946\100.069258 time ReadingTime 2016-05-13T11:47:37Z float Value 35.7
1 TemperatureReading int SensorId 14 string Description "Temperature sensor placed in Madrid, airport" pos Position 8.675946\47.069258 time ReadingTime 2016-05-13T11:47:37Z float Value 28.7
```

In the output of the example, you can see how events are sent (connectors_1 | [Collector]), the output of the bcep (bcep_1) and the complex events (connectors_1 | [Publisher]).

Specifically, the complex events detected are:

```
connectors_1 | [Publisher]2 AverageTemperature float average 35.700001
connectors_1 | 
connectors_1 | [Publisher]2 AverageTemperature float average 32.200001

```

Their structure follows what is defined for AverageTemperature in the dolce file.

## License

Apache 2.0

By downloading this software, the downloader agrees with the specified terms and conditions of the License Agreement and the particularities of the license provided.
