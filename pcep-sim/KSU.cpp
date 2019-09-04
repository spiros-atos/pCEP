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

#include "KSU.h"
#include "kutils.h"
#include "constants.h"


void KSU::Process(const Eigen::VectorXd& vxPredict_x, const Eigen::VectorXd& vxPredict_z,
    const Eigen::VectorXd& vxZ, const Eigen::MatrixXd& mxS,
    const Eigen::MatrixXd& mxPredict_P, const Eigen::MatrixXd& mxSigma_x,
    const Eigen::MatrixXd& mxSigma_z)
{
    Eigen::MatrixXd mxTc = Compute_Tc(vxPredict_x, vxPredict_z, mxSigma_x, mxSigma_z);
    Update(vxZ, mxS, mxTc, vxPredict_z, vxPredict_x, mxPredict_P);
}

Eigen::MatrixXd KSU::Compute_Tc(const Eigen::VectorXd& vxPredict_x,
        const Eigen::VectorXd& vxPredict_z, const Eigen::MatrixXd& mxSigma_x, 
        const Eigen::MatrixXd& mxSigma_z)
{
    int NZ = vxPredict_z.size();
    Eigen::VectorXd vxDz;
    Eigen::VectorXd vxDx;
    Eigen::MatrixXd mxTc = Eigen::MatrixXd::Zero(NX, NZ);

    for (int c = 0; c < NSIGMA; ++c) 
    {
        vxDx = mxSigma_x.col(c) - vxPredict_x;
        vxDx(3) = KUtils::Normalize(vxDx(3));

        vxDz = mxSigma_z.col(c) - vxPredict_z;
        if (NZ == NZ_RADAR) vxDz(1) = KUtils::Normalize(vxDz(1));

        mxTc += WEIGHTS[c] * vxDx * vxDz.transpose();
    }

    return mxTc;
}

void KSU::Update(const Eigen::VectorXd& vxZ, const Eigen::MatrixXd& mxS,
    const Eigen::MatrixXd& mxTc, const Eigen::VectorXd& vxPredict_z,
    const Eigen::VectorXd& vxPredict_x, const Eigen::MatrixXd& mxPredict_P)
{
    Eigen::MatrixXd mxSi = mxS.inverse();
    Eigen::MatrixXd mxK = mxTc * mxSi;

    Eigen::VectorXd vxDz = vxZ - vxPredict_z;
    if (vxDz.size() == NZ_RADAR) vxDz(1) = KUtils::Normalize(vxDz(1)); // yaw/phi in radians

    _mxX = vxPredict_x + mxK * vxDz;
    _mxP = mxPredict_P - mxK * mxS * mxK.transpose();
    _NIS = vxDz.transpose() * mxSi * vxDz;
}
