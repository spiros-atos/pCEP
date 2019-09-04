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

#include <vector>
#include <fstream>
#include <string>
//#include <algorithm>

#include "datapoint.h"

std::vector<std::pair<float, float>> 
getObjLocs(std::vector<std::pair<float, float>> vct_obj_loc)
{
    std::vector<std::pair<float, float>> result(1);
    static int idx_obj_loc = 0;

    result[0] = vct_obj_loc[idx_obj_loc++];

    if (idx_obj_loc >= vct_obj_loc.size())
        idx_obj_loc = 0;        // read file from beginning

    return result;    
}

DataPoint GetEigenDataPoint(std::vector<DataPoint> vct_eigen_data_points)
{
    static int idx_eigen_data_point = 0;

    DataPoint dp = vct_eigen_data_points[idx_eigen_data_point++];

    if (idx_eigen_data_point >= vct_eigen_data_points.size())
        idx_eigen_data_point = 0;        // read file from beginning

    return dp;
}

void getLidarLine(
    std::vector<DataPoint>& vct_eigen_data_points,
    //std::vector<std::vector<DataPoint>>& vct_of_eigens,
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
    //vct_of_eigens[id].push_back(sensor_data);

    infile >> val_type >> val_name >> truth_x >> val_type >> val_name >> truth_y;
    infile >> val_type >> val_name >> truth_vx >> val_type >> val_name >> truth_vy;

    truth_v = sqrt(truth_vx*truth_vx + truth_vy * truth_vy);

    Eigen::VectorXd truth_vec(NX);
    truth_vec << truth_x, truth_y, truth_v, truth_yaw, truth_yawrate;

    DataPoint truth_data;
    truth_data.Set(ftime, DataPointType::STATE, truth_vec,
        static_cast<short>(obj_id));
    vct_truth_data_points.push_back(truth_data);
}

void getRadarLine(
    std::vector<DataPoint>& vct_eigen_data_points,
    std::vector<DataPoint>& vct_truth_data_points,
    std::ifstream& infile, std::string val_type, std::string val_name,
    double phi, double rho, double drho, long long ftime, int obj_id,
    double truth_x, double truth_y,
    double truth_vx, double truth_vy, double truth_v,
    double truth_yaw, double truth_yawrate)
{
    infile >> val_type >> val_name >> rho >> val_type >> val_name >> phi;
    infile >> val_type >> val_name >> drho >> val_type >> val_name >> ftime;

    Eigen::VectorXd eigen_vec(NZ_RADAR);
    eigen_vec << rho, phi, drho;

    DataPoint sensor_data(ftime, DataPointType::RADAR, eigen_vec,
        static_cast<short>(obj_id));
    vct_eigen_data_points.push_back(sensor_data);
    //vct_of_eigens[id].push_back(sensor_data);

    infile >> val_type >> val_name >> truth_x >> val_type >> val_name >> truth_y;
    infile >> val_type >> val_name >> truth_vx >> val_type >> val_name >> truth_vy;
    infile >> val_type >> val_name >> truth_yaw >> val_type >> val_name >> truth_yawrate;

    truth_v = sqrt(truth_vx*truth_vx + truth_vy * truth_vy);

    Eigen::VectorXd truth_vec(NX);
    truth_vec << truth_x, truth_y, truth_v, truth_yaw, truth_yawrate;

    DataPoint truth_data;
    truth_data.Set(ftime, DataPointType::STATE, truth_vec,
        static_cast<short>(obj_id));
    vct_truth_data_points.push_back(truth_data);
}

std::vector<DataPoint> getEigenDataPointsFromFile(const std::string& filename,
    std::vector<DataPoint>& vct_eigen_data_points, 
    std::vector<DataPoint>& vct_truth_data_points)
{
    //std::vector<std::vector<DataPoint>> vct_of_eigens;

    static int idx_obj_loc = 0;
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

        int i = 0;

        std::vector<int> vct_of_obj_ids;

        while (infile >> event_type >> event_name >> val_type >> val_name >> obj_id >>
            val_type >> val_name >> obj_type)
        {
            bool isFound = false;
            auto it = vct_of_obj_ids.begin();
            while (it != vct_of_obj_ids.end())
            {
                if (obj_id == *it)
                {
                    isFound = true;
                    break;
                }
                ++it;
            }

            //int id = vct_of_eigens.size()-1; // the id of this vehicle/object in vct_of_eigens

            if (obj_type == "L")
            {
                getLidarLine(vct_eigen_data_points, vct_truth_data_points,
                //getLidarLine( id, vct_truth_data_points,
                    infile, val_type, val_name, loc_x, loc_y, ftime, obj_id,
                    truth_x, truth_y, truth_vx, truth_vy, truth_v, truth_yaw,
                    truth_yawrate);
            }
            else if (obj_type == "R")
            {
                getRadarLine(vct_eigen_data_points, vct_truth_data_points,
                    infile, val_type, val_name, rho, phi, drho, ftime, obj_id,
                    truth_x, truth_y, truth_vx, truth_vy, truth_v,
                    truth_yaw, truth_yawrate);
            }
        }
    }

    return vct_eigen_data_points;
    //return vct_of_eigens;
}
