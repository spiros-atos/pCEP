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

enum class DataPointType
{
  LIDAR, RADAR, STATE, TRUTH
};

const int NZ_RADAR = 3; // number of radar measurements
const int NZ_LIDAR = 2; // number of lidar measurements
const int NX = 5; // number of states
const int NAUGMENTED = NX + 2; // number of states plus two noise values
const int NSIGMA = NAUGMENTED * 2 + 1; // number of sigma points

//process noise standard deviations
const double STD_SPEED_NOISE = 0.9; // longitudinal acceleration in m/s^2
const double STD_YAWRATE_NOISE = 0.6; // yaw acceleration in rad/s^2
const double VAR_SPEED_NOISE = STD_SPEED_NOISE * STD_SPEED_NOISE;
const double VAR_YAWRATE_NOISE = STD_YAWRATE_NOISE * STD_YAWRATE_NOISE;

//RADAR measurements noise standard deviations
const double STD_RHO = 0.3; // meters
const double STD_PHI = 0.03; // radians
const double STD_RHODOT = 0.3; // meters / second
const double VAR_RHO = STD_RHO * STD_RHO;
const double VAR_PHI = STD_PHI * STD_PHI;
const double VAR_RHODOT = STD_RHODOT * STD_RHODOT;

//LIDAR measurements noise standard deviations
const double STD_PX = 0.15; // meters
const double STD_PY = 0.15; // meters
const double VAR_PX = STD_PX * STD_PX;
const double VAR_PY = STD_PY * STD_PX;

const int LAMBDA = 3 - NAUGMENTED; // parameter for tuning
//const double SCALE = sqrt(LAMBDA + NAUGMENTED); // used to create augmented sigma points
const double W = 0.5 / (LAMBDA + NAUGMENTED);
const double W0 = LAMBDA / double(LAMBDA + NAUGMENTED);
const double WEIGHTS[NSIGMA] = {W0, W, W, W, W, W, W, W, W, W, W, W, W, W, W};
