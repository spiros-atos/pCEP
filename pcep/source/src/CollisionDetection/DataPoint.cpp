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

#include "datapoint.h"

using namespace std;

DataPoint::DataPoint(const long long timestamp, const DataPointType& data_type, 
        const Eigen::VectorXd& raw, const short& id)
{
    Set(timestamp, data_type, raw, id);
}

void DataPoint::Set(const long long timestamp, const DataPointType& data_type, 
        const Eigen::VectorXd& raw, const short& id)
{
    _vid = id;
    _timestamp = timestamp;
    _data_type = data_type;
    _raw = raw;
}

Eigen::VectorXd DataPoint::get_state() const
{
    Eigen::VectorXd state(NX);

    if (_data_type == DataPointType::LIDAR)
    {
        double px = _raw(0);
        double py = _raw(1);
        state << px, py, 0.0, 0.0, 0.0;
    } 
    else if (_data_type == DataPointType::RADAR)
    {
        double rho = _raw(0);
        double phi = _raw(1);

        double px = rho * cos(phi);
        double py = rho * sin(phi);
        state << px, py, 0.0, 0.0, 0.0;
    } 
    else if (_data_type == DataPointType::STATE)
    {
        state = _raw;
    } 
    else if (_data_type == DataPointType::TRUTH)
    {
        double px = _raw(0);
        double py = _raw(1);
        double vx = _raw(2);
        double vy = _raw(3);

        double v = sqrt(vx * vx + vy * vy);
        double yaw = atan2(vy, vx);
        state << px, py, v, yaw, 0.0;
    }

    return state;
}

Eigen::VectorXd DataPoint::get_vec() const
{
    Eigen::VectorXd vec(NX - 1);

    if (_data_type == DataPointType::LIDAR)
    {
        double px = _raw(0);
        double py = _raw(1);
        vec << px, py, 0.0, 0.0;
    } 
    else if (_data_type == DataPointType::RADAR)
    {
        double rho = _raw(0);
        double phi = _raw(1);
        double px = rho * cos(phi);
        double py = rho * sin(phi);

        vec << px, py, 0.0, 0.0;
    } 
    else if (_data_type == DataPointType::STATE)
    {
        double px = _raw(0);
        double py = _raw(1);
        double v = _raw(3);
        double yaw = _raw(4);

        double vx = v * cos(yaw);
        double vy = v * sin(yaw);

        vec << px, py, vx, vy;
    } 
    else if (_data_type == DataPointType::TRUTH)
    {
        vec = _raw;
    }

    return vec;
}
