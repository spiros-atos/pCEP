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
#include <chrono>
#include <vector>
#include <deque>
#include <algorithm>

#include "datapoint.h"


class DataSource
{
public:
    DataSource(int max_samples = 4096,
        std::chrono::milliseconds fetch_interval = std::chrono::milliseconds{ 100 }) { ; }

    ~DataSource() { ; }
    void Fetch();
    DataPoint At(int core, int sample) const noexcept { return _samples[core][sample]; }
    int SamplesCount() const noexcept { return std::max(int(_samples[0].size()) - 2, 0); };
    void GetDataPoints(const std::string& infile);
    std::vector<DataPoint> _vct_truth_data_points;

private:
    void Consume(DataPoint dp);
    static void Smooth(std::deque<std::pair<float, float>>& data);

    std::chrono::high_resolution_clock::time_point _lastFetched;
    std::chrono::milliseconds _fetchInterval;
    std::vector<std::deque<DataPoint>> _samples;
    std::vector<DataPoint> _vct_eigen_data_points;
};

