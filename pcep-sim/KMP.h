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

#include "Eigen/Dense"
#include "datapoint.h"

class KMP
{
private:
    void Initialize(const DataPointType sensor_type);

    DataPointType _curr_type;
    int _nz;

public:
    void Process(const Eigen::MatrixXd& sigma_x, const DataPointType sensor_type);
    Eigen::MatrixXd ComputeSigma_z(const Eigen::MatrixXd& mxSigma_x);
    Eigen::MatrixXd Compute_z(const Eigen::MatrixXd& mxSigma_z);
    Eigen::MatrixXd Compute_S(const Eigen::MatrixXd& mxSigma_z, 
            const Eigen::MatrixXd& mxPredict_z);

    Eigen::VectorXd _vxZ;
    Eigen::MatrixXd _mxS;
    Eigen::MatrixXd _mxSigma_z;
    Eigen::MatrixXd _mxR;
};

