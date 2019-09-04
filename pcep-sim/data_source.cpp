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

#include "data_source.h"
#include "profiler.h"
#include <assert.h>

void DataSource::GetDataPoints(const std::string& infile)
{
    getEigenDataPointsFromFile(infile, _vct_eigen_data_points, _vct_truth_data_points);

    DataPoint dp = GetEigenDataPoint(_vct_eigen_data_points);
    _samples.resize(1);
    Consume(dp);

    _lastFetched = std::chrono::high_resolution_clock::now();
}

void DataSource::Fetch()
{
    //auto now = std::chrono::high_resolution_clock::now();
    //if (now - _lastFetched >= _fetchInterval) {
    //    Consume(GetEigenDataPoint(_vct_eigen_data_points));
    //    _lastFetched = now;
    //}
}

void DataSource::Consume(DataPoint dp)
{
    for (auto i = 0; i < 1; ++i) 
    {
        auto &samples = _samples[i];
        samples.emplace_back(dp);
        if (samples.size() > 1000)
            samples.pop_front();
    }
}


void DataSource::Smooth(std::deque<std::pair<float, float>>& data)
{
    if (data.size() < 6)
        return;

    auto p = std::end(data);
    auto v = (*--p);
    v.first += (*--p).first;
    v.first += (*--p).first * 2.f;
    v.first += (*--p).first;
    v.first += (*--p).first;
    (*std::prev(std::end(data), 3)).first = v.first / 6.f;

    auto vy = (*--p);
    vy.second += (*--p).second;
    vy.second += (*--p).second * 2.f;
    vy.second += (*--p).second;
    vy.second += (*--p).second;
    (*std::prev(std::end(data), 3)).second = vy.second / 6.f;
}
