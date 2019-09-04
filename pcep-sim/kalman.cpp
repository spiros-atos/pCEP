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


#include <fstream>

#include "kalman.h"


void Kalman::Process(const DataPoint& data)
{
    _is_initialized ? Update(data) : Initialize(data);
}

void Kalman::Process(const DataPoint& data, std::ofstream& out_file)
{
    //_is_initialized ? Update(data) : Initialize(data);
    _is_initialized ? Update(data, out_file) : Initialize(data);
}

void Kalman::Initialize(const DataPoint& data)
{
    _vxX = data.get_state();
    _mxP = Eigen::MatrixXd::Identity(NX, NX);
    _timestamp = data._timestamp;
    _is_initialized = true;
}

void Kalman::Update(const DataPoint& data)
{
    Eigen::VectorXd vxPredict_z;
    Eigen::MatrixXd mxSigma_x;
    Eigen::MatrixXd mxSigma_z;
    Eigen::MatrixXd mxS;

    // get the time difference in seconds
    double dt = (data._timestamp - _timestamp) / 1.0e4;

    _state_predict.Process(_vxX, _mxP, dt);
    _vxX = _state_predict._vxX;
    _mxP = _state_predict._mxP;
    mxSigma_x = _state_predict._mxSigma;

    _measure_predict.Process(mxSigma_x, data._data_type);
    vxPredict_z = _measure_predict._vxZ;
    mxS = _measure_predict._mxS;
    mxSigma_z = _measure_predict._mxSigma_z;

    _state_update.Process(_vxX, vxPredict_z, data._raw, mxS, _mxP, mxSigma_x, mxSigma_z);
    _vxX = _state_update._mxX;
    _mxP = _state_update._mxP;
    _NIS = _state_update._NIS;

    _timestamp = data._timestamp;
}

void Kalman::Update(const DataPoint& data, std::ofstream& out_file)
{
    Eigen::VectorXd vxPredict_z;
    Eigen::MatrixXd mxSigma_x;
    Eigen::MatrixXd mxSigma_z;
    Eigen::MatrixXd mxS;

    // get the time difference in seconds
    double dt = (data._timestamp - _timestamp) / 1.0e4;

    _state_predict.Process(_vxX, _mxP, dt);
    _vxX = _state_predict._vxX;
    _mxP = _state_predict._mxP;
    mxSigma_x = _state_predict._mxSigma;

    //for (int i = 0; i < _vxX.size(); ++i)
    //    out_file << _vxX(i) << "\t";
    //for (int i = 0; i < _mxP.cols(); ++i)
    //    for (int j = 0; j < _mxP.rows(); ++j)
    //        out_file << _mxP(j, i) << "\t";
    //for (int j = 0; j < mxSigma_x.rows(); ++j)
    //    for (int i = 0; i < mxSigma_x.cols(); ++i)
    //        out_file << mxSigma_x(j, i) << "\t";

    _measure_predict.Process(mxSigma_x, data._data_type);
    vxPredict_z = _measure_predict._vxZ;
    mxS = _measure_predict._mxS;
    mxSigma_z = _measure_predict._mxSigma_z;

    _state_update.Process(_vxX, vxPredict_z, data._raw, mxS, _mxP, mxSigma_x, mxSigma_z);
    _vxX = _state_update._mxX;
    _mxP = _state_update._mxP;
    _NIS = _state_update._NIS;

    _timestamp = data._timestamp;
}
