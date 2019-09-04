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

class KSU
{
private:
    Eigen::MatrixXd Compute_Tc(const Eigen::VectorXd& vxPredict_x, 
            const Eigen::VectorXd& vxPredict_z, const Eigen::MatrixXd& mxSigma_x,
            const Eigen::MatrixXd& mxSigma_z);

public:
    KSU() { ; }
    ~KSU() { ; }

    void Process(const Eigen::VectorXd& predicted_x, const Eigen::VectorXd& predicted_z, 
            const Eigen::VectorXd& z, const Eigen::MatrixXd& S, 
            const Eigen::MatrixXd& predicted_P, const Eigen::MatrixXd& sigma_x,
            const Eigen::MatrixXd& sigma_z);
    void Update(const Eigen::VectorXd& vxZ, const Eigen::MatrixXd& mxS, 
            const Eigen::MatrixXd& mxTc, const Eigen::VectorXd& vxPredict_z,
            const Eigen::VectorXd& vxPredict_x, const Eigen::MatrixXd& mxPredict_P);

    Eigen::MatrixXd _mxX;
    Eigen::MatrixXd _mxP;
    double _NIS;    // Normalized Innovations Squared
};
