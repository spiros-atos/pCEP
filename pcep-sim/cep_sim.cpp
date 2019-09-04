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

#include "cep_sim.h"
#include "line.h"
#include "kalman.h"
#include "constants.h"

#include <vector>
#include <numeric>


CEPSim::CEPSim(const std::string& infile)
{
    _kalman = Kalman();
    GetCars(infile);
}

void CEPSim::Run()
{
    auto display = output_surface{ 
        400, 400, format::argb32, scaling::none, refresh_style::fixed, 30 
    };

    display.size_change_callback([](output_surface& surface) {
        surface.dimensions(surface.display_dimensions());
    });

    display.draw_callback([&](output_surface& surface) {
        Display(surface);
    });

    display.begin_show();
}

void CEPSim::Run(std::ofstream& out_file)
{
    auto display = output_surface{ 
        400, 400, format::argb32, scaling::none, refresh_style::fixed, 30 
    };

	display.size_change_callback([](output_surface& surface){
        surface.dimensions(surface.display_dimensions());
    });

	display.draw_callback([&](output_surface& surface){
        Display(surface, out_file);
    });

	display.begin_show();
}

void CEPSim::Display(output_surface& surface)
{
    DrawBackground(surface);
    DrawVerticalGridLines(surface);
    DrawHorizontalGridLines(surface);

    std::string dummy("");
    std::ofstream dummy_file(dummy.c_str(), std::ofstream::out);

    DrawVehicles(surface, dummy_file/*, vid*/);
}

void CEPSim::Display(output_surface& surface, std::ofstream& out_file/*, const short& vid*/)
{
    DrawBackground(surface);
    DrawVerticalGridLines(surface);
    DrawHorizontalGridLines(surface);
    DrawVehicles(surface, out_file/*, vid*/);
}

void CEPSim::DrawBackground(output_surface& surface) const
{
    surface.paint(_backgroundFill);
}

void CEPSim::DrawVerticalGridLines(output_surface& surface) const
{
    auto pb = path_builder{};

    for (auto it = vct_vertical_boundaries.begin();
        it != vct_vertical_boundaries.end(); ++it)
    {
        pb.new_figure({ float(it->_start._x), float(it->_start._y) });
        pb.line({ float(it->_end._x), float(it->_end._y) });
    }

    pb.close_figure();

    using namespace std;
    surface.stroke(_verticalLinesBrush, pb, nullopt, nullopt, nullopt, nullopt);

    for (int i = 0; i < 400; i += 40)
    {
        pb.new_figure({ 175.f, float(i)});
        pb.line({ 175.f, float(i) + 10.f });

        pb.new_figure({ 175.f, float(i) + 20.f });
        pb.line({ 175.f, float(i) + 30.f });
    }

    pb.close_figure();
    surface.stroke(_verticalLinesBrush, pb, nullopt, nullopt, nullopt, nullopt);
}

void CEPSim::DrawHorizontalGridLines(output_surface& surface) const
{
    auto pb = path_builder{};

    for (auto it = vct_horizontal_boundaries.begin();
        it != vct_horizontal_boundaries.end(); ++it)
    {
        pb.new_figure({ float(it->_start._x), float(it->_start._y) });
        pb.line({ float(it->_end._x), float(it->_end._y) });
    }

    for (int i = 0; i < 400; i += 40)
    {
        pb.new_figure({ float(i) + 10.f, 175.f });
        pb.line({ float(i) + 20.f, 175.f });

        pb.new_figure({ float(i) + 30.f, 175.f });
        pb.line({ float(i) + 40.f, 175.f });
    }

    pb.close_figure();

    using namespace std;
    surface.stroke(_horizontalLinesBrush, pb, nullopt, nullopt, nullopt, nullopt);
}

void CEPSim::DrawCar(output_surface& surface, std::map<int, Car*>::iterator it, int& i, 
	std::ofstream& out_file)
{
    int carid = it->first;
    Car* pCar = it->second;

	if (i >= pCar->_vct_eigen_data_points.size())
	{
		i = 0;
		if (out_file)
		{
			out_file.close();
			exit(0);
		}
	}

	double x, y;

	DataPointType dptype = pCar->_vct_eigen_data_points[i]._data_type;
	if (dptype == DataPointType::LIDAR)
	{
		x = pCar->_vct_eigen_data_points[i]._raw(0);
		y = pCar->_vct_eigen_data_points[i]._raw(1);
	}
	else if (dptype == DataPointType::RADAR)
	{
		double rho = pCar->_vct_eigen_data_points[i]._raw(0);
		double phi = pCar->_vct_eigen_data_points[i]._raw(1);
		x = rho * cos(phi);
		y = rho * sin(phi);
	}

    Eigen::VectorXd raw = _cars[carid]->_vct_eigen_data_points[i].get_state();

    if (carid == 1)       // move along the y axis
    {
        x = x * 10 + 135;
        y = y * 10 + 25;
        //Vehicle v(fx, fy, 17.f, 42.f);
        Vehicle v(x, y, pCar->_width, pCar->_length);

        pCar->SetCurrentPosition(x, y);

        //DrawCurrentVehicle(v, surface, carid);
        DrawEgoVehicle(v, surface, carid);

		DrawProjectedVehicle(surface, i, out_file);
    }
    else if (carid == 2)  // move along the x axis
    {
        x = raw(0) * 10 + 25;
        y = raw(1) * 10 + 210;
        //Vehicle v(fx, fy, 42.f, 17.f);
        //Vehicle v(x, y, 84.f, 34.f);
        Vehicle v(x, y, pCar->_width, pCar->_length);

        pCar->SetCurrentPosition(x, y);

        DrawCurrentVehicle(v, surface, carid);
    }
    else if (carid == 3)  // move along the x axis
    {
        x = raw(0) * 10 + 25;
        y = raw(1) * 10 + 135;
        //Vehicle v(fx, fy, 42.f, 17.f);
        //Vehicle v(x, y, 84.f, 34.f);
        Vehicle v(x, y, pCar->_width, pCar->_length);

        pCar->SetCurrentPosition(x, y);

        DrawCurrentVehicle(v, surface, carid);
    }
}

void CEPSim::DrawVehicles(output_surface& surface, std::ofstream& out_file)
{
    static int i = 0;
    if (i > -1)
    {
        auto it = _cars.begin();
        while (it != _cars.end())
        {
			//DrawCar(surface, it, i);
			DrawCar(surface, it, i, out_file);
            ++it;
        }
        ++i;
    }
}

bool GetLinesIntersection(Line l0, Line l1, Point& p)
{
    double p0_x = l0._start._x;
    double p0_y = l0._start._y;
    double p1_x = l0._end._x;
    double p1_y = l0._end._y;

    double p2_x = l1._start._x;
    double p2_y = l1._start._y;
    double p3_x = l1._end._x;
    double p3_y = l1._end._y;

    double s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

    double s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        p._x = p0_x + (t * s1_x);
        p._y = p0_y + (t * s1_y);
        return true;
    }

    return false;
}

/*
Finally, the center - radius representation results in the following overlap test :
*/
bool CEPSim::TestForCollisionsWithOtherVehicles(Car* pEgo)
{
    bool bRes = false;

    std::pair<double, double> ac = pEgo->GetCurrentPosition();
    std::pair<double, double> ar = std::make_pair(pEgo->_width, pEgo->_length);

    //
    // from Christer:
    // Two AABBs only overlap if they overlap on all three axes, where their extent along..
    // ..each dimension is seen as an interval on the corresponding axis.
    //

    auto it = _cars.begin();
    while (it != _cars.end())
    {
        if (it->first != 1)
        {
            auto bc = it->second->GetCurrentPosition();
            std::pair<double, double> br = std::make_pair(_cars[2]->_width, _cars[2]->_length);

            if ( (std::abs<double>(ac.first - bc.first) <= (ar.first / 2 + br.first / 2)) &&
                 (std::abs<double>(ac.second - bc.second) <= (ar.second / 2 + br.second / 2)) )
                bRes = true;
        }
        ++it;
    }

    return bRes;
}

bool CEPSim::TestForCollisionsWithBoundaries(const Vehicle& v) const
{
    auto lines = v.GetLines();
    auto it = lines.begin();

    // for each side of vehicle
    while (it != lines.end())
    {
        // for each vertical boundary
        auto vbound = vct_vertical_boundaries;
        auto vbit = vbound.begin();
        while (vbit != vbound.end())
        {
            Point intersect_point;
            // check for interesction
            // if interesct, return true
            if (GetLinesIntersection(*it, *vbit, intersect_point))
                return true;
            ++vbit;
        }

        // for each horizontal boundary
        auto hbound = vct_horizontal_boundaries;
        auto hbit = hbound.begin();
        while (hbit != hbound.end())
        {
            Point intersect_point;
            // check for interesction
            // if interesct, return true
            if (GetLinesIntersection(*it, *hbit, intersect_point))
                return true;
            ++hbit;
        }

        ++it;
    }

    return false;
}

void CEPSim::DrawEgoVehicle(Vehicle& v, output_surface& surface, const short& vid)
{
    brush vehicle_brush = _horizontalLinesBrush;

    // see if there is a collision
    //if (TestForCollisionsWithBoundaries(v))
    if (TestForCollisionsWithOtherVehicles(GetCarByID(1)))
    {
        // set red box
        vehicle_brush = _vehicleLinesBrushCrashing;
    }

    // now draw the graph
    auto graph = path_builder{};
    v.Draw(graph, vid);
    graph.close_figure();

    using namespace std;
    surface.stroke(vehicle_brush, graph, nullopt, nullopt, nullopt, nullopt);
}

void CEPSim::DrawCurrentVehicle(Vehicle& v, output_surface& surface, const short& vid)
{
    brush vehicle_brush = _horizontalLinesBrush;

    //// see if there is a collision
    //if (TestForCollisionsWithBoundaries(v))
    ////if (TestForCollisionsWithOtherVehicles(v))
    //{
    //    // set red box
    //    vehicle_brush = _vehicleLinesBrushCrashing;
    //}

    // now draw the graph
    auto graph = path_builder{};
    v.Draw(graph, vid);
    graph.close_figure();

    using namespace std;
    surface.stroke(vehicle_brush, graph, nullopt, nullopt, nullopt, nullopt);
}

//Vehicle CEPSim::ReadProjectedVehicle()
//{
//    //if (_dqPredFxFy.size() > 9)
//    //{
//    //    Point predPt = _dqPredFxFy[9];
//    //    Vehicle v(predPt._x, predPt._y);
//    //    return v;
//    //}
//    //else
//    {
//        Point predPt = _dqPredFxFy[0];
//        Vehicle v(predPt._x, predPt._y);
//        return v;
//    }
//}

//Vehicle CEPSim::CalcProjectedVehicle()
//{
//    // calculate projected vehicle
//    Point currPredPt = _dqPredFxFy[0];
//    Point prevPredPt = _dqPredFxFy[1];
//    float xdiff = 0;
//    float ydiff = 0;
//
//    // in 10 time steps:
//    short time_steps = 10;
//
//    //Vehicle v(currPredPt._x + time_steps * xdiff, currPredPt._y + time_steps * ydiff);
//    Vehicle v(currPredPt._x + time_steps, currPredPt._y + time_steps);
//    //Vehicle v(predPt._x, predPt._y);
//
//    return v;
//}

/*
    calculate quadratic regression equations taken from here:
        https://www.varsitytutors.com/hotmath/hotmath_help/topics/quadratic-regression
*/
//double CEPSim::calcQuadReg(const double& x, const double& y, const double& z)
//double CEPSim::calcQuadReg(const double& x, const double& z)
double CEPSim::calcQuadReg(std::vector<double>& vct_x, 
	std::vector<double>& vct_y, const double& z)
{
    // if x == -1, get 'y' from time t
    // else, get y from x
   // std::vector<double> vct_x;
   // std::vector<double> vct_y;

   // if (x == -1)
   // {
   //     for (int i = 0; i < QUAD_REG_LEN; ++i)
   //     {
   //         vct_x.push_back(i);
			////vct_y.push_back(_dqQuadRegPoints[QUAD_REG_LEN - 1 - i].first);
			//vct_y.push_back(_dqQuadRegPoints[i].first);
   //     }
   // }
   // else
   // {
   //     for (int i = 0; i < QUAD_REG_LEN; ++i)
   //     {
   //         vct_x.push_back(_dqQuadRegPoints[i].first);
   //         vct_y.push_back(_dqQuadRegPoints[i].second);
   //     }
   // }

    double fx = 0.0, fy = 0.0;
    double sum_xi2_by_yi = 0.0, sum_xi_by_yi = 0.0, sum_yi = 0.0;
    double sum_xi = 0.0, sum_xi2 = 0.0, sum_xi3 = 0.0, sum_xi4 = 0.0;
    for (int i = 0; i < _dqQuadRegPoints.size(); ++i)
    {
        sum_xi += vct_x[i];
        sum_xi2 += std::pow(vct_x[i], 2);
        sum_xi3 += std::pow(vct_x[i], 3);
        sum_xi4 += std::pow(vct_x[i], 4);

        sum_yi += vct_y[i];
        sum_xi_by_yi += vct_x[i] * vct_y[i];
        sum_xi2_by_yi +=
            std::pow(vct_x[i], 2)*vct_y[i];
    }

    Eigen::Matrix3d A;
    Eigen::Vector3d B;
    A << sum_xi4, sum_xi3, sum_xi2, sum_xi3, sum_xi2, sum_xi, sum_xi2, sum_xi, static_cast<double>(_dqQuadRegPoints.size());
    B << sum_xi2_by_yi, sum_xi_by_yi, sum_yi;
    Eigen::Vector3d x_prime = A.colPivHouseholderQr().solve(B);

    double y_ave = 0.0;
    for (int i = 0; i < _dqQuadRegPoints.size(); ++i)
        y_ave += vct_y[i];
    y_ave = y_ave / _dqQuadRegPoints.size();

    double a = x_prime(0);
    double b = x_prime(1);
    double c = x_prime(2);

    double SSE = 0.0;
    double SST = 0.0;

    for (int i = 0; i < _dqQuadRegPoints.size(); ++i)
    {
        SST += std::pow(vct_y[i] - y_ave, 2);
        SSE += std::pow(vct_y[i] - a * vct_x[i] * vct_x[i] - b * vct_x[i] - c, 2);
    }

    double R2 = 1 - SSE / SST;
    _RSquared = R2;

    fx = z;
    fy = a * (fx*fx) + b * fx + c;

    return fy;
}

//double CEPSim::calcCircleFit(const double& x, const double& y, const double& z)
double CEPSim::calcCircleFit(std::vector<double>& vct_x, 
	std::vector<double>& vct_y, const double& xin)
{
	//std::vector<double> vct_x;
	//std::vector<double> vct_y;

	//for (int i = 0; i < QUAD_REG_LEN; ++i)
	//{
	//	vct_x.push_back(_dqQuadRegPoints[i].first);
	//	vct_y.push_back(_dqQuadRegPoints[i].second);
	//}

	double x_bar = std::accumulate(vct_x.begin(), vct_x.end(), 0.0) / 
		vct_x.size();
	double y_bar = std::accumulate(vct_y.begin(), vct_y.end(), 0.0) / 
		vct_y.size();

	std::vector<double> u, v;

	for (size_t i = 0; i < vct_x.size(); ++i)
	{
		u.push_back(vct_x[i] - x_bar);
		v.push_back(vct_y[i] - y_bar);
	}

	//double u_c, v_c;	// centre of circle (u_c, v_c)
	double S_uu = 0.0;
	double S_vv = 0.0;
	double S_uuu = 0.0;
	double S_vvv = 0.0;
	double S_uv = 0.0;
	double S_uvv = 0.0;
	double S_vuu = 0.0;

	for (size_t i = 0; i < vct_x.size(); ++i)
	{
		S_uu += u[i] * u[i];
		S_vv += v[i] * v[i];
		S_uuu += u[i] * u[i] * u[i];
		S_vvv += v[i] * v[i] * v[i];
		S_uv += u[i] * v[i];
		S_uvv += u[i] * v[i] * v[i];
		S_vuu += v[i] * u[i] * u[i];
	}

	double v_c = (S_uv * (S_uuu + S_uvv) - S_uu * (S_vvv + S_vuu)) /
		(2 * (S_uv * S_uv - S_uu * S_vv));
	double u_c = (0.5 * (S_uuu + S_uvv) - v_c * S_uv) / S_uu;
	double x_c = u_c + x_bar;
	double y_c = v_c + y_bar;

	double a = u_c * u_c + v_c * v_c + (S_uu + S_vv) / vct_x.size();
	double R = std::sqrt(a);

	//std::cout << "Centre of circle (x_c, y_c) = (";
	//std::cout << x_c << ", " << y_c << ")" << std::endl;
	//std::cout << "Radius of circle: " << R << std::endl;

	//std::cout << "Circle equation: " << std::endl;
	//std::cout << "(x - " << x_c << ")^2 + (y - " << y_c << ")^2 = ";
	//std::cout << R << "^2" << std::endl;

	//double xin;
	//std::cin >> xin;
	//std::cout << "you entered x = " << xin << std::endl;

	auto b = -2 * y_c;
	auto xdiff = xin - x_c;
	auto c = y_c * y_c - R * R + xdiff * xdiff;
	auto sr = b * b - 4 * c;

	double yout1 = 0.0, yout2 = 0.0;

	if (sr < 0)
	{
		auto yout = calcQuadReg(vct_x, vct_y, xin);    // get y from x
		return yout;
	}
	else
	{
		yout1 = (-b + std::sqrt(b * b - 4 * c)) / 2;
		yout2 = (-b - std::sqrt(b * b - 4 * c)) / 2;

		//std::cout << "for point x = " << xin << " the solutions are:\n";
		//std::cout << yout1 << ", " << yout2 << std::endl;

		auto [min, max] = std::minmax_element(vct_y.begin(), vct_y.end());
		if (std::clamp(yout1, *min, *max) != yout1)
			return yout1;
		else
			return yout2;
	}
}

void CEPSim::getXYVectors(std::vector<double>& vct_x, 
	std::vector<double>& vct_y, const bool& fromTime/* = false*/)
{
	vct_x.clear();
	vct_y.clear();

	if (fromTime)
	{
		for (int i = 0; i < QUAD_REG_LEN; ++i)
		{
			vct_x.push_back(i);
			vct_y.push_back(_dqQuadRegPoints[i].first);
		}
	}
	else
	{
		for (int i = 0; i < QUAD_REG_LEN; ++i)
		{
			vct_x.push_back(_dqQuadRegPoints[i].first);
			vct_y.push_back(_dqQuadRegPoints[i].second);
		}
	}
}

std::pair<double, double> 
CEPSim::CalcProjectedVehicleQuadraticRegression(const double& x, const double& y, 
	std::ofstream& out_file)
{
    double fx = 0.0, fy = 0.0;

    std::pair<double, double> pt(x, y);
	//_dqQuadRegPoints.push_front(pt);
	_dqQuadRegPoints.push_back(pt);

    if (_dqQuadRegPoints.size() >= QUAD_REG_LEN + 1)
    {
		out_file << x << "\t" << y << "\t";

		//_dqQuadRegPoints.pop_back();
		_dqQuadRegPoints.pop_front();

		std::vector<double> vct_x;
		std::vector<double> vct_y;
		getXYVectors(vct_x, vct_y, true);

		out_file << vct_x[0] << "\t" << vct_x[vct_x.size() - 1] << "\t";
		out_file << vct_y[0] << "\t" << vct_y[vct_y.size() - 1] << "\t";

		//fx = calcQuadReg(-1.0, x, QUAD_REG_LEN + 9);  // get x from time
		fx = calcQuadReg(vct_x, vct_y, QUAD_REG_LEN+QUAD_REG_OFFSET-1);  // get x from time
		getXYVectors(vct_x, vct_y, false);

		out_file << fx << "\t";
		out_file << vct_x[0] << "\t" << vct_x[vct_x.size() - 1] << "\t";
		out_file << vct_y[0] << "\t" << vct_y[vct_y.size() - 1] << "\t";
		out_file << std::endl;

		// if fx is not within the range of the data points x's:
		auto [min, max] = std::minmax_element(vct_x.begin(), vct_x.end());
		if (std::clamp(fx, *min, *max) != fx)
		{
			if (_RSquared < 0.9 || _RSquared > 1.0)
			{
				//calcLinearReg(-1.0, x, 29);

				// if all else fails...
				return std::pair<double, double>(0.0, 0.0);
			}

			//fy = calcQuadReg(x, y, fx);    // get y from x
			fy = calcQuadReg(vct_x, vct_y, fx);    // get y from x
		}
		// else if fx is within the range, calculate a circle
		else
		{
			//fy = calcCircleFit(x, y, fx);
			fy = calcCircleFit(vct_x, vct_y, fx);
		}
    }

    return std::pair<double, double>(fx, fy);
}

Vehicle CEPSim::CalcProjectedVehicleUKF(const int& idx, std::ofstream& out_file)
{
	//DataPoint dpSensorData = _dqEigenDataPoints[0];
	Car* pCar = _cars[1];
	DataPoint dpSensorData = pCar->_vct_eigen_data_points[idx];

// spiros start
	//DataPointType dptype = pCar->_vct_eigen_data_points[i]._data_type;
	//if (dptype == DataPointType::LIDAR)
	//{
	//	x = pCar->_vct_eigen_data_points[i]._raw(0);
	//	y = pCar->_vct_eigen_data_points[i]._raw(1);
	//}
	//else if (dptype == DataPointType::RADAR)
	//{
	//	double rho = pCar->_vct_eigen_data_points[i]._raw(0);
	//	double phi = pCar->_vct_eigen_data_points[i]._raw(1);
	//	x = rho * cos(phi);
	//	y = rho * sin(phi);
	//}
	//dpSensorData.Set(dpSensorData._timestamp, dptype, (x, y), id);
// spiros end

	//double dTemp = dpSensorData._raw(0);

    //_kalman.Process(dpSensorData);

    //Eigen::VectorXd vxPredict;
    //vxPredict = _kalman._vxX;

    //double NIS = _kalman._NIS;

	//std::pair<double, double> pt =
	//	CalcProjectedVehicleQuadraticRegression(vxPredict(0), vxPredict(1));
	std::pair<double, double> pt =
		CalcProjectedVehicleQuadraticRegression(dpSensorData._raw(0), 
			dpSensorData._raw(1), out_file);

    //if (dpSensorData._data_type == DataPointType::LIDAR)
    {
        //double x = vxPredict(0);
        //double y = vxPredict(1);
		double x = pt.first;
		double y = pt.second;

		//double fx = x * 10 + 150;
		//double fy = y * 10 + 150;
		double fx = x * 10 + 135;
		double fy = y * 10 + 25;

        //Vehicle v(fx, fy);
		Vehicle v(fx, fy, pCar->_width, pCar->_length);
		return v;
    }
    //else if (dpSensorData._data_type == DataPointType::RADAR)
    //{
    //    double rho = vxPredict(0);
    //    double phi = vxPredict(1);

    //    double fx = rho * 10 + 150;
    //    double fy = phi * 10 + 150;

    //    Vehicle v(fx, fy);
    //    return v;
    //}
    //else
    //{
    //    return Vehicle(0, 0);
    //}
}

Vehicle CEPSim::CalcProjectedVehicleUKF(std::ofstream& out_file)
{
    DataPoint dpSensorData = _dqEigenDataPoints[0];

    _kalman.Process(dpSensorData, out_file);

    Eigen::VectorXd vxPredict = _kalman._vxX;
    std::pair<double, double> pt = 
        CalcProjectedVehicleQuadraticRegression(vxPredict(0), vxPredict(1), out_file);

    if (dpSensorData._data_type == DataPointType::LIDAR)
    {
        double x = pt.first;
        double y = pt.second;

        double fx = x * 10 + 150;
        double fy = y * 10 + 150;

        Vehicle v(fx, fy);
        return v;
    }
    else if (dpSensorData._data_type == DataPointType::RADAR)
    {
        double rho = pt.first;
        double phi = pt.second;

        double fx = rho * 10 + 150;
        double fy = phi * 10 + 150;

        Vehicle v(fx, fy);
        return v;
    }
    else
    {
        return Vehicle(0, 0);
    }
}

void CEPSim::DrawProjectedVehicle(output_surface& surface, std::ofstream& out_file, 
        const short& vid)
{
    size_t idx = _dqEigenDataPoints.size();
    if (idx < 1)
        return;

    Vehicle v = CalcProjectedVehicleUKF(out_file);
    Point pt = v.GetPoint();
    out_file << pt._x << "\t";
    out_file << pt._y << "\t";

    if (_dqQuadRegPoints.size() >= QUAD_REG_LEN && _RSquared >= 0.9 && _RSquared <= 1.0)
    {
        brush vehicle_brush = _projVehicleLinesBrush;

        // see if there is a collision
        if (TestForCollisionsWithBoundaries(v))
        {
            // set red box
            vehicle_brush = _vehicleLinesBrushCrashingProjected;
        }

        // now draw the graph
        auto graph = path_builder{};
        v.Draw(graph, vid);
        graph.close_figure();

        using namespace std;
        surface.stroke(vehicle_brush, graph, nullopt, nullopt, nullopt, nullopt);
    }
}

void CEPSim::DrawProjectedVehicle(output_surface& surface, const int& idx, 
	std::ofstream& out_file)
{
	//size_t idx = _dqEigenDataPoints.size();
	//if (idx < 1)
	//	return;

	Vehicle v = CalcProjectedVehicleUKF(idx, out_file);
	Point pt = v.GetPoint();
	//out_file << pt._x << "\t";
	//out_file << pt._y << "\t";

	//if (_dqQuadRegPoints.size() >= QUAD_REG_LEN && _RSquared >= 0.9 && _RSquared <= 1.0)
	if (_dqQuadRegPoints.size() >= QUAD_REG_LEN)
	{
		brush vehicle_brush = _projVehicleLinesBrush;

		// see if there is a collision
		if (TestForCollisionsWithBoundaries(v))
		{
			// set red box
			vehicle_brush = _vehicleLinesBrushCrashingProjected;
		}

		// now draw the graph
		auto graph = path_builder{};
		v.Draw(graph, 0);
		graph.close_figure();

		using namespace std;
		surface.stroke(vehicle_brush, graph, nullopt, nullopt, nullopt, nullopt);
	}
}

void CEPSim::GetCars(const std::string& filename)
{
    std::vector<DataPoint> vct_eigen_data_points;
    std::vector<DataPoint> vct_truth_data_points;

    std::ifstream infile(filename.c_str());
    if (infile.is_open())
    {
        DataPoint sensor_data;
        using std::string;
        int event_type;
        string event_name, val_type, val_name;
        int obj_id;
        string obj_type;
        long long ftime = 0;
        double loc_x = 0.0, loc_y = 0.0, rho = 0.0, phi = 0.0, drho = 0.0,
            truth_x = 0.0, truth_y = 0.0, truth_vx = 0.0, truth_vy = 0.0,
            truth_v = 0.0, truth_yaw = 0.0, truth_yawrate = 0.0;

        while (infile >> event_type >> event_name >> val_type >> val_name >> 
            obj_id >> val_type >> val_name >> obj_type)
        {

//1 obj_loc int obj_id 1 string type L float loc_x	0.3122				float loc_y 0.5803				float t 14770104430000
//1 obj_loc int obj_id 1 string type R float rho	0.658951386674313	float phi	1.07721469597045	float t 14770104430000 


            if (GetCarByID(obj_id) == nullptr)
            {
                // new car
                Car* pCar = new Car();

                if (obj_id == 1) // ego
                {
                    // hardcode the width and length for now
                    //pCar->_width = 25.0;
                    //pCar->_length = 50.0;
                    pCar->_width = 40.0;
                    pCar->_length = 80.0;
                }
                else if (obj_id == 2 || obj_id == 3)
                {
                    // hardcode the width and length for now
                    //pCar->_width = 25.0;
                    //pCar->_length = 50.0;
                    pCar->_width = 80.0;
                    pCar->_length = 40.0;
                }

                _cars[obj_id] = pCar;
            }

            if (obj_type == "L")
            {
                GetLidarLine(vct_eigen_data_points, vct_truth_data_points,
                    infile, val_type, val_name, loc_x, loc_y, ftime, obj_id,
                    truth_x, truth_y, truth_vx, truth_vy, truth_v, truth_yaw,
                    truth_yawrate);
            }
            else if (obj_type == "R")
            {
                GetRadarLine(vct_eigen_data_points, vct_truth_data_points,
                    infile, val_type, val_name, rho, phi, drho, ftime, obj_id,
                    truth_x, truth_y, truth_vx, truth_vy, truth_v,
                    truth_yaw, truth_yawrate);
            }
        }
    }
}


void CEPSim::GetLidarLine(std::vector<DataPoint>& vct_eigen_data_points,
    std::vector<DataPoint>& vct_truth_data_points,
    std::ifstream& infile, std::string val_type, std::string val_name,
    double loc_x, double loc_y, long long ftime, int obj_id,
    double truth_x, double truth_y,
    double truth_vx, double truth_vy, double truth_v,
    double truth_yaw, double truth_yawrate)
{
    infile >> val_type >> val_name >> loc_x >> val_type >> val_name >> loc_y;
    infile >> val_type >> val_name >> ftime;

    Eigen::VectorXd eigen_vec(NZ_LIDAR);
    eigen_vec << loc_x, loc_y;

    DataPoint sensor_data(ftime, DataPointType::LIDAR, eigen_vec,
        static_cast<short>(obj_id));
    vct_eigen_data_points.push_back(sensor_data);
    _cars[obj_id]->_vct_eigen_data_points.push_back(sensor_data);

    infile >> val_type >> val_name >> truth_x >> val_type >> val_name >> truth_y;
    infile >> val_type >> val_name >> truth_vx >> val_type >> val_name >> truth_vy;

    truth_v = sqrt(truth_vx*truth_vx + truth_vy * truth_vy);

    Eigen::VectorXd truth_vec(NX);
    truth_vec << truth_x, truth_y, truth_v, truth_yaw, truth_yawrate;

    DataPoint truth_data;
    truth_data.Set(ftime, DataPointType::STATE, truth_vec,
        static_cast<short>(obj_id));
    vct_truth_data_points.push_back(truth_data);
    _cars[obj_id]->_vct_truth_data_points.push_back(truth_data);
}

void CEPSim::GetRadarLine(std::vector<DataPoint>& vct_eigen_data_points,
    std::vector<DataPoint>& vct_truth_data_points,
    std::ifstream& infile, std::string val_type, std::string val_name,
    double rho, double phi, double drho, long long ftime, int obj_id,
    double truth_x, double truth_y,
    double truth_vx, double truth_vy, double truth_v,
    double truth_yaw, double truth_yawrate)
{
    infile >> val_type >> val_name >> rho >> val_type >> val_name >> phi;
    infile >> val_type >> val_name >> drho >> val_type >> val_name >> ftime;

	//double x, y;
    Eigen::VectorXd eigen_vec(NZ_RADAR);

	//x = rho * cos(phi);
	//y = rho * sin(phi);

	eigen_vec << rho, phi, drho;
	//eigen_vec << x, y, drho;

// spiros hack: don't forget have done this for that file lines_1_car.txt
	ftime += 500;

    DataPoint sensor_data(ftime, DataPointType::RADAR, eigen_vec,
        static_cast<short>(obj_id));
    vct_eigen_data_points.push_back(sensor_data);
    _cars[obj_id]->_vct_eigen_data_points.push_back(sensor_data);

    infile >> val_type >> val_name >> truth_x >> val_type >> val_name >> truth_y;
    infile >> val_type >> val_name >> truth_vx >> val_type >> val_name >> truth_vy;

    truth_v = sqrt(truth_vx*truth_vx + truth_vy * truth_vy);

    Eigen::VectorXd truth_vec(NX);
    truth_vec << truth_x, truth_y, truth_v, truth_yaw, truth_yawrate;

    DataPoint truth_data;
    truth_data.Set(ftime, DataPointType::STATE, truth_vec,
        static_cast<short>(obj_id));
    vct_truth_data_points.push_back(truth_data);
    _cars[obj_id]->_vct_truth_data_points.push_back(truth_data);
}

Car* CEPSim::GetCarByID(const int& id)
{
    if (_cars.find(id) == _cars.end())
        return nullptr;
    else
        return _cars[id];
}
