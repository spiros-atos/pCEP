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
#include "line.h"
#include <io2d.h>


class Vehicle
{
public:
    Vehicle(const double& fx, const double& fy);
    void Draw(std::experimental::io2d::v1::path_builder& graph, const short& vid);
    void DrawID(std::experimental::io2d::v1::path_builder& graph, const short& vid);
    std::vector<Line> GetLines() const { return _lines; }
    Point GetPoint();

    Vehicle(const double& fcx, const double& fcy, const float& fw, const float& fl);
    void ConstructLines(const Point points[4]);

private:
    std::vector<Line> _lines;

/*
Christer, pg. 78:
    The last representation specifies the AABB as a center point C and halfwidth
        extents or radii rx, ry, and rz along its axes :
    // region R = {(x, y, z) | |c.x-x|<=rx, |c.y-y|<=ry, |c.z-z|<=rz }
    struct AABB {
        Point c; // center point of AABB
        float r[3]; // radius or halfwidth extents (rx, ry, rz)
    };
*/

    //Point _c;   // center point of vehicle
    ////float r[2]; // radius rx, ry
    //float _length;
    //float _width;

/*
Christer pg. 80:

    Finally, the center-radius representation results in the following overlap test:
    int TestAABBAABB(AABB a, AABB b)
    {
    if (Abs(a.c[0] - b.c[0]) > (a.r[0] + b.r[0])) return 0;
    if (Abs(a.c[1] - b.c[1]) > (a.r[1] + b.r[1])) return 0;
    if (Abs(a.c[2] - b.c[2]) > (a.r[2] + b.r[2])) return 0;
    return 1;
}*/

    //friend int testVehiclesIntersection(Vehicle a, Vehicle b) {};
};


