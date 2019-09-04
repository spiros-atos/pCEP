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
///#include <vector>
///#include <map>

namespace solcep {
namespace Config {

/* brief Declares plugin setting. Applies to all kind of plugins,
        emitters, decoders, encoders and listeners.
 */
struct Plugin
{
    Plugin() : cannotLoad(false)
    {
    }

    /*!
            \brief The name of the plugin as found on the file system,
            excluding the path.
            It is the actual name of the shared library, not some free form
            identifier
     */
    std::string name;

    /*!
            \brief Workaround for a bug in linux/glibc. Not stored as part
            of the configuration

            (dlopen()-ing a DLL a second time succeeds when it should fail
            It is supposedly fixed in GlibC 2.1, but it is still an issue.
            (Google for: "dlopen() succeeds when it shoudln't")
     */
    bool cannotLoad;

    /*!
        \brief User defined parameters for the plugin
     */
    std::map<std::string, std::string> parameters;
};

/*brief Listener, Emitter, Encoder and Decoder configurations
        are all grouped under the same structure due to their simularity,
        and are called Handlers.

        The THandlerType distinguishes the type to which the configuration
        applies.
 */
enum THandlerType
{
    listener,
    decoder,
    encoder,
    emitter,
    unknown
};

/*!
        \brief Defines an entry for a collection of
                Listeners/Decoders/Emitters/Encoders declared in the config file
 */
struct Handler
{

    Handler() : type(unknown), plugin(0)
    {
    }

    THandlerType type;

    //! True if the handler is enabled.
    bool enabled;

    //! Identifies the handler. Referenced by other configuration settings.
    std::string id;

    //! Reference to the plugin used.
    Plugin* plugin;

    // Encoder/Decoder reference identifier
    std::string codingRef;
};

/*!
        \brief Declares Coordinator configuration
 */
struct Coordinator
{
    //! Start of IP port range
    int portRangeStart = 29000;

    //! Size of IP port range
    int portRangeSize = 29100;

    //! Listening port for REST administration interface
    int adminPort = portRangeSize + 1;

    //! Internal IP port for logger subsystem.
    int logPort = 5554;

    //! Log level
    int logLevel;

    //! Name of the log file
    std::string logFileName;
};

/*!
        \brief Event Collector configuration
 */
struct EventCollector
{
    std::vector<Handler*> listeners;
    std::vector<Handler*> decoders;
    
    bool enabled() {return listeners.size() > 0;}
};

struct HandlerRef
{
    std::string name;
    std::map<std::string, std::string> params;
};

/*!
    \brief Detection Specification configuration

    This is where the Complex Event Detections
    are specified (i.e. the Dolce language spec
 */
struct DetectionSpecification
{
    std::string id;
    bool enabled = true;
    std::string port;
    
    std::vector<HandlerRef> listeners;
    std::vector<HandlerRef> emmiters;
};

/*!
    \brief A Complex Event Detector groups multiple Detection
    Specifications. This allows multiple complex event detector
    engines running in parallel
 */
struct ComplexEventDetector
{
    std::vector<DetectionSpecification*> dectSpecs;
};

/*!
        \brief Complex Event Publisher configuration
 */
struct ComplexEventPublisher
{
    std::vector<Handler*> encoders;
    std::vector<Handler*> emitters;
    
    bool enabled() {return emitters.size() > 0;}
};

}
} //namespace solcep::Config


