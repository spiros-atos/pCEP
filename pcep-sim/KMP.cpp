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


#include "KMP.h"
#include "kutils.h"


void KMP::Process(const Eigen::MatrixXd& mxSigma_x, const DataPointType sensor_type)
{
    Initialize(sensor_type);
    _mxSigma_z = ComputeSigma_z(mxSigma_x); // transform predicted sigma_x into measurement space
    _vxZ = Compute_z(_mxSigma_z); // get the mean predicted measurement vector z
    _mxS = Compute_S(_mxSigma_z, _vxZ); // get the measurement covariance matrix S
}

void KMP::Initialize(const DataPointType sensor_type) 
{
    _curr_type = sensor_type;

    if (_curr_type == DataPointType::RADAR) 
    {
        _nz = NZ_RADAR;
        _mxR = Eigen::MatrixXd(_nz, _nz);
        _mxR << VAR_RHO,    0,          0, 
              0,            VAR_PHI,    0,
              0,            0,          VAR_RHODOT;
    }
    else if (_curr_type == DataPointType::LIDAR) 
    {
        _nz = NZ_LIDAR;
        _mxR = Eigen::MatrixXd(_nz, _nz);
        _mxR << VAR_PX,   0,
                0,        VAR_PY;
    }
}

Eigen::MatrixXd KMP::ComputeSigma_z(const Eigen::MatrixXd& mxSigma_x) 
{
    const double THRESH = 1e-4;
    Eigen::MatrixXd mxSigma = Eigen::MatrixXd::Zero(_nz, NSIGMA);

    for (int c = 0; c < NSIGMA; ++c) 
    {
        if (_curr_type == DataPointType::RADAR) 
        {
            const double px = mxSigma_x(0, c);
            const double py = mxSigma_x(1, c);
            const double v = mxSigma_x(2, c);
            const double yaw = mxSigma_x(3, c);

            const double vx = cos(yaw) * v;
            const double vy = sin(yaw) * v;

            const double rho = sqrt(px * px + py * py);
            const double phi = atan2(py, px);

            // avoid division by zero
            const double rhodot = (rho > THRESH) ? ((px * vx + py * vy) / rho) : 0.0;

            mxSigma(0, c) = rho;
            mxSigma(1, c) = phi;
            mxSigma(2, c) = rhodot;
        }
        else if (_curr_type == DataPointType::LIDAR) 
        {
            mxSigma(0, c) = mxSigma_x(0, c); //px
            mxSigma(1, c) = mxSigma_x(1, c); //py
        }
    }

    return mxSigma;
}

Eigen::MatrixXd KMP::Compute_z(const Eigen::MatrixXd& mxSigma)
{
    Eigen::VectorXd vxZ = Eigen::VectorXd::Zero(_nz);

    for (int c = 0; c < NSIGMA; ++c) 
    {
        vxZ += WEIGHTS[c] * mxSigma.col(c);
    }

    return vxZ;
}

Eigen::MatrixXd KMP::Compute_S(const Eigen::MatrixXd& mxSigma, const Eigen::MatrixXd& mxZ) 
{
    Eigen::VectorXd vxDz;
    Eigen::MatrixXd mxS = Eigen::MatrixXd::Zero(_nz, _nz);

    for (int c = 0; c < NSIGMA; ++c) 
    {
        vxDz = mxSigma.col(c) - mxZ;
        if (_curr_type == DataPointType::RADAR) 
            vxDz(1) = KUtils::Normalize(vxDz(1));

        mxS += WEIGHTS[c] * vxDz * vxDz.transpose();
    }

    mxS += _mxR;
    return mxS;
}
