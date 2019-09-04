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

#include "KSP.h"
#include "constants.h"
#include "kutils.h"


void KSP::Process(Eigen::VectorXd& vxCurr_x, Eigen::MatrixXd& mxCurr_P, double dt)
{
    Eigen::MatrixXd mxAug_Sigma = ComputeAugmented_Sigma(vxCurr_x, mxCurr_P);
    _mxSigma = Predict_Sigma(mxAug_Sigma, dt);
    _vxX = Predict_x(_mxSigma);
    _mxP = Predict_P(_mxSigma, _vxX);
}

Eigen::MatrixXd KSP::Predict_Sigma(const Eigen::MatrixXd& mxAug_Sigma, double dt)
{
    const double THRESH = 0.001;
    Eigen::MatrixXd mxPredict_Sigma = Eigen::MatrixXd(NX, NSIGMA);

    for (int c = 0; c < NSIGMA; ++c) 
    {
        const double px = mxAug_Sigma(0, c);
        const double py = mxAug_Sigma(1, c);
        const double speed = mxAug_Sigma(2, c);
        const double yaw = mxAug_Sigma(3, c);
        const double yawrate = mxAug_Sigma(4, c);
        const double speed_noise = mxAug_Sigma(5, c);
        const double yawrate_noise = mxAug_Sigma(6, c);

        /*************************************
        * predict the next state with noise
        * USING THE CTRV MODEL
        *************************************/
        const double cos_yaw = cos(yaw);
        const double sin_yaw = sin(yaw);
        const double dt2 = dt * dt;
        const double p_noise = 0.5 * speed_noise * dt2;  // predicted position noise
        const double y_noise = 0.5 * yawrate_noise * dt2; // predicted yaw noise
        const double dyaw = yawrate * dt; //change in yaw
        const double dspeed = speed * dt; //change in speed

        const double p_speed = speed + speed_noise * dt; // predicted speed = assumed constant speed + noise
        const double p_yaw = yaw + dyaw + y_noise; // predicted yaw
        const double p_yawrate = yawrate + yawrate_noise * dt; // predicted yaw rate = assumed constant yawrate + noise

        double p_px, p_py; // where predicted positions will be stored

        if (fabs(yawrate) <= THRESH) 
        {
            // moving straight
            p_px = px + dspeed * cos_yaw + p_noise * cos_yaw;
            p_py = py + dspeed * sin_yaw + p_noise * sin_yaw;

        }
        else 
        {
            const double k = speed / yawrate;
            const double theta = yaw + dyaw;
            p_px = px + k * (sin(theta) - sin_yaw) + p_noise * cos_yaw;
            p_py = py + k * (cos_yaw - cos(theta)) + p_noise * sin_yaw;
        }

        /*************************************
        * Write the prediction to the appropriate column
        *************************************/
        mxPredict_Sigma(0, c) = p_px;
        mxPredict_Sigma(1, c) = p_py;
        mxPredict_Sigma(2, c) = p_speed;
        mxPredict_Sigma(3, c) = p_yaw;
        mxPredict_Sigma(4, c) = p_yawrate;
    }

    return mxPredict_Sigma;
}

Eigen::MatrixXd KSP::ComputeAugmented_Sigma(const Eigen::VectorXd& vxCurr_x, 
        const Eigen::MatrixXd& mxCurr_P) 
{
    Eigen::MatrixXd mxAug_Sigma = Eigen::MatrixXd::Zero(NAUGMENTED, NSIGMA);
    Eigen::VectorXd vxAug_x = Eigen::VectorXd::Zero(NAUGMENTED);
    Eigen::MatrixXd mxAug_P = Eigen::MatrixXd::Zero(NAUGMENTED, NAUGMENTED);

    vxAug_x.head(NX) = vxCurr_x;

    mxAug_P.topLeftCorner(NX, NX) = mxCurr_P;
    mxAug_P(NX, NX) = VAR_SPEED_NOISE;
    mxAug_P(NX + 1, NX + 1) = VAR_YAWRATE_NOISE;

    const Eigen::MatrixXd L = mxAug_P.llt().matrixL();
    mxAug_Sigma.col(0) = vxAug_x;

    for (int c = 0; c < NAUGMENTED; c++) {
        const int i = c + 1;
        mxAug_Sigma.col(i) = vxAug_x + SCALE * L.col(c);
        mxAug_Sigma.col(i + NAUGMENTED) = vxAug_x - SCALE * L.col(c);
    }

    return mxAug_Sigma;
}

Eigen::VectorXd KSP::Predict_x(const Eigen::MatrixXd& mxPredict_Sigma)
{
    Eigen::VectorXd vxPredict_x = Eigen::VectorXd::Zero(NX);

    for (int c = 0; c < NSIGMA; ++c) 
    {
        vxPredict_x += WEIGHTS[c] * mxPredict_Sigma.col(c);
    }

    return vxPredict_x;
}

Eigen::MatrixXd KSP::Predict_P(const Eigen::MatrixXd& mxPredict_Sigma,
    const Eigen::VectorXd& vxPredict_x)
{
    Eigen::MatrixXd mxPredict_P = Eigen::MatrixXd::Zero(NX, NX);

    Eigen::VectorXd vxDx = Eigen::VectorXd(NX);

    for (int c = 0; c < NSIGMA; ++c) 
    {
        vxDx = mxPredict_Sigma.col(c) - vxPredict_x;
        vxDx(3) = KUtils::Normalize(vxDx(3));
        mxPredict_P += WEIGHTS[c] * vxDx * vxDx.transpose();
    }

    return mxPredict_P;
}
