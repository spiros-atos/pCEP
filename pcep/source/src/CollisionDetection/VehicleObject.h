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
#include <vector>
#include "DataPoint.h"
#include "Point.h"

class VehicleObject
{
public:
    VehicleObject() {}
    virtual ~VehicleObject() {}

    std::vector<DataPoint> _vct_eigen_data_points;
    std::vector<DataPoint> _vct_truth_data_points;

    //private:
    Point _c;   // center point of vehicle object
    //float r[2]; // radius rx, ry
    float _length;
    float _width;

    inline void SetCurrentPosition(double x, double y) {
        _curr_x = x; _curr_y = y;
    }

    inline std::pair<double, double> GetCurrentPosition(void) {
        return std::make_pair(_curr_x, _curr_y);
    }

private:
    double _curr_x;
    double _curr_y;
};

