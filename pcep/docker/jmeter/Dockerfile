FROM debian:latest

ENV JMETER_VERSION=2.13
ENV JMETER_HOME=/usr/local/apache-jmeter-${JMETER_VERSION}
ENV PATH=${JMETER_HOME}/bin:${PATH}

RUN apt-get update 
RUN apt-get -y install wget openjdk-7-jre

RUN wget https://archive.apache.org/dist/jmeter/binaries/apache-jmeter-${JMETER_VERSION}.tgz && \
	tar -xzf apache-jmeter-${JMETER_VERSION}.tgz -C /usr/local/

RUN wget http://central.maven.org/maven2/kg/apc/jmeter-plugins-extras-libs/1.1.3/jmeter-plugins-extras-libs-1.1.3.jar -P ${JMETER_HOME}/lib/ext/

RUN rm -rf apache-jmeter-${JMETER_VERSION}.tgz \
			${JMETER_HOME}/bin/examples \
			${JMETER_HOME}/bin/templates \
			${JMETER_HOME}/bin/*.cmd \
			${JMETER_HOME}/bin/*.bat \
			${JMETER_HOME}/docs \
			${JMETER_HOME}/printable_docs

COPY docker/jmeter/UDPRequest.jmx ${JMETER_HOME}
EXPOSE 29654/udp
WORKDIR ${JMETER_HOME}
