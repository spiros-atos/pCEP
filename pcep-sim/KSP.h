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


class KSP   // Kalman State Predictor
{
private:
    Eigen::MatrixXd ComputeAugmented_Sigma(const Eigen::VectorXd& vxCurr_x, 
            const Eigen::MatrixXd& mxCurr_P);
    Eigen::MatrixXd Predict_Sigma(const Eigen::MatrixXd& mxAug_Sigma, double dt);
    Eigen::VectorXd Predict_x(const Eigen::MatrixXd& mxPredict_Sigma);
    Eigen::MatrixXd Predict_P(const Eigen::MatrixXd& mxPredict_Sigma, 
            const Eigen::VectorXd& vxPredict_x);

public:
    void Process(Eigen::VectorXd& vxCurr_x, Eigen::MatrixXd& mxCurr_P, double dt);

    Eigen::VectorXd _vxX = Eigen::VectorXd(5);
    Eigen::MatrixXd _mxP = Eigen::MatrixXd(5, 5);
    Eigen::MatrixXd _mxSigma = Eigen::MatrixXd(5, 15);
};
