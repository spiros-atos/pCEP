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

#include <deque>
#include <fstream>

#include "line.h"
#include "vehicle.h"
#include "kalman.h"

#include "Eigen/Dense"
#include "datapoint.h"

//#include "object_factory.h"
//#include "vehicle_object.h"
//#include "animal_object.h"
#include "car.h"
#include <map>


using namespace std::experimental::io2d;

class CEPSim
{
public:
    CEPSim(const std::string& infile);
    void Run();
    void Run(std::ofstream& out_file);

private:
    void Display(output_surface& surface/*, const short& vid*/);
    void Display(output_surface& surface, std::ofstream& out_file/*, const short& vid*/);
    void DrawBackground(output_surface& surface) const;
    void DrawVerticalGridLines(output_surface& surface) const;
    void DrawHorizontalGridLines(output_surface& surface) const;
    void DrawVehicles(output_surface& surface, std::ofstream& out_file/*, const short& vid*/);
    bool TestForCollisionsWithBoundaries(const Vehicle& v) const;
    void DrawCurrentVehicle(Vehicle& v, output_surface& surface, const short& vid);
	void DrawProjectedVehicle(output_surface& surface, std::ofstream& out_file, const short& vid);
	void DrawProjectedVehicle(output_surface& surface, const int& idx, std::ofstream& out_file);
    Vehicle CalcProjectedVehicleUKF(const int& idx, std::ofstream& out_file);
    Vehicle CalcProjectedVehicleUKF(std::ofstream& out_file);
	std::pair<double, double> CalcProjectedVehicleQuadraticRegression(const double& x, 
		const double& y, std::ofstream& out_file);
	//double calcQuadReg(const double& x, const double& y, const double& z);
	//double calcQuadReg(const double& x, const double& z);
	double calcQuadReg(std::vector<double>& vct_x, std::vector<double>& vct_y, 
		const double& z);
	//double calcCircleFit(const double& x, const double& y, const double& z);
	//double calcCircleFit(const double& xin);
	double calcCircleFit(std::vector<double>& vct_x,
		std::vector<double>& vct_y, const double& xin);
	//void getXYVectors(std::vector<double>& vct_x, std::vector<double>& vct_y);
	void getXYVectors(std::vector<double>& vct_x, std::vector<double>& vct_y, 
		const bool& fromTime = false);

    brush           _backgroundFill{ rgba_color::alice_blue };
    stroke_props    _gridStrokeProps{ 1.f };
    brush           _verticalLinesBrush{ rgba_color::cornflower_blue };
    brush           _horizontalLinesBrush{ rgba_color::blue };
    brush           _projVehicleLinesBrush{ rgba_color::green };
    brush           _vehicleLinesBrushCrashing{ rgba_color::yellow };
    brush           _vehicleLinesBrushCrashingProjected{ rgba_color::red };
    brush           _fillBrush{ { 0, 0 },{ 0, 1 },{ { 0.f, rgba_color::green },{ 0.4f, rgba_color::yellow },{ 1.0f, rgba_color::red } } };

    const std::vector<Line> vct_vertical_boundaries = { 
        Line(Point(0.f, 0.f), Point(0.f, 400.f)),
        Line(Point(100.f, 0.f), Point(100.f, 100.f)),
        Line(Point(100.f, 250.f), Point(100.f, 400.f)),
        Line(Point(250.f, 0.f), Point(250.f, 100.f)),
        Line(Point(250.f, 250.f), Point(250.f, 400.f)),
        Line(Point(400.f, 0.f), Point(400.f, 400.f))
    };

    const std::vector<Line> vct_horizontal_boundaries = {
        Line(Point(0.f, 0.f), Point(400.f, 0.f)),
        Line(Point(0.f, 100.f), Point(100.f, 100.f)),
        Line(Point(250.f, 100.f), Point(400.f, 100.f)),
        Line(Point(0.f, 250.f), Point(100.f, 250.f)),
        Line(Point(250.f, 250.f), Point(400.f, 250.f)),
        Line(Point(0.f, 400.f), Point(400.f, 400.f))
    };

    std::deque<Point> _dqFxFy;
    std::vector<DataPoint> _vctEigenDataPoints;

    std::deque<DataPoint> _dqEigenDataPoints;
    std::deque<DataPoint> _dqEigenPredDataPoints;

    std::deque<Point> _dqPredFxFy;

    std::deque<std::pair<double, double>> _dqQuadRegPoints;
    double _RSquared;

    Kalman _kalman;

    // 
    // After refactoring:
    //

    //ObjectFactory* _factory;
    //VehicleObject _vehicles;
    //AnimalObject _animals;
    std::map<int, Car*> _cars;
    void GetCars(const std::string& filename);
    Car* GetCarByID(const int& id);
    void GetLidarLine(std::vector<DataPoint>& vct_eigen_data_points,
        std::vector<DataPoint>& vct_truth_data_points,
        std::ifstream& infile, std::string val_type, std::string val_name,
        double loc_x, double loc_y, long long ftime, int obj_id,
        double truth_x, double truth_y,
        double truth_vx, double truth_vy, double truth_v,
        double truth_yaw, double truth_yawrate);
    void GetRadarLine(std::vector<DataPoint>& vct_eigen_data_points,
        std::vector<DataPoint>& vct_truth_data_points,
        std::ifstream& infile, std::string val_type, std::string val_name,
        double rho, double phi, double drho, long long ftime, int obj_id,
        double truth_x, double truth_y,
        double truth_vx, double truth_vy, double truth_v,
        double truth_yaw, double truth_yawrate);
	//void DrawCar(output_surface& surface, std::map<int, Car*>::iterator cit, int& i);
	void DrawCar(output_surface& surface, std::map<int, Car*>::iterator cit, int& i, 
		std::ofstream& out_file);
    void DrawEgoVehicle(Vehicle& v, output_surface& surface, const short& vid);
    bool TestForCollisionsWithOtherVehicles(Car* pEgo);
};
