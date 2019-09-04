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

#include "vehicle.h"

Vehicle::Vehicle(const double& fx, const double& fy)
{
    Point points[4];
    points[0] = Point(fx, fy);
    points[1] = Point(fx + 50.f, fy);
    points[2] = Point(fx + 50.f, fy + 50.f); 
    points[3] = Point(fx, fy + 50.f);

    ConstructLines(points);

    //_lines.push_back(Line(points[0], points[1]));
    //_lines.push_back(Line(points[1], points[2]));
    //_lines.push_back(Line(points[2], points[3]));
    //_lines.push_back(Line(points[3], points[0]));
}

Vehicle::Vehicle(const double & fcx, const double & fcy, const float & fw, const float & fl)
{
    //double fw = 50;
    //double fl = 50;

    Point points[4];
    points[0] = Point(fcx - fw / 2.f, fcy - fl / 2.f);
    points[1] = Point(fcx + fw / 2.f, fcy - fl / 2.f);
    points[2] = Point(fcx + fw / 2.f, fcy + fl / 2.f);
    points[3] = Point(fcx - fw / 2.f, fcy + fl / 2.f);

    ConstructLines(points);
}

void Vehicle::ConstructLines(const Point points[4])
{
    _lines.push_back(Line(points[0], points[1]));
    _lines.push_back(Line(points[1], points[2]));
    _lines.push_back(Line(points[2], points[3]));
    _lines.push_back(Line(points[3], points[0]));
}

void Vehicle::DrawID(std::experimental::io2d::v1::path_builder& graph, const short& vid)
{
    float x2 = 0.0, y2 = 0.0, y2_prime = 0.0;
    float x0 = float(_lines[0]._start._x);
    float x1 = float(_lines[1]._end._x);
    float y0 = float(_lines[0]._start._y);
    float y1 = float(_lines[1]._end._y);

    if (vid == 1)
    {
        x2 = x0 + (x1 - x0) / 2;
        y2 = y0 + 2 * (y1 - y0) / 3;
        y2_prime = y0 + (y1 - y0) / 3;
    }
    else if (vid == 2)
    {
        x2 = x0 + 20;
        y2 = y0 + 2 * (y1 - y0) / 3;
        y2_prime = y0 + (y1 - y0) / 3;
        graph.new_figure({ x2, y2 });
        graph.line({ x2, y2_prime });

        x2 = x0 + 30;
        y2 = y0 + 2 * (y1 - y0) / 3;
        y2_prime = y0 + (y1 - y0) / 3;
    }

    graph.new_figure({ x2, y2 });
    graph.line({ x2, y2_prime });
}

void Vehicle::Draw(std::experimental::io2d::v1::path_builder & graph, const short& vid)
{
    graph.new_figure({ float(_lines[0]._start._x), float(_lines[0]._start._y) });

    //if (vid != 2)
    //    return;

    graph.line({ float(_lines[0]._end._x), float(_lines[0]._end._y) });
    graph.line({ float(_lines[1]._end._x), float(_lines[1]._end._y) });
    graph.line({ float(_lines[2]._end._x), float(_lines[2]._end._y) });
    graph.line({ float(_lines[3]._end._x), float(_lines[3]._end._y) });

    // "draw" ID, the library doesn't have text rendering functionality yet
    // so just draw roman numerals
    DrawID(graph, vid);
}

Point Vehicle::GetPoint()
{
    Point pt = Point(_lines[0]._start);
    return pt;
}

//int testVehiclesIntersection(Vehicle a, Vehicle b)
//{
//
//}
