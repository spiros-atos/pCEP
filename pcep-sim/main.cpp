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

#include <iostream>
#include <fstream>
#include "cep_sim.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
        return 0;

    std::string infile(argv[1]);
    CEPSim sim(infile);

    if (argc == 2)
    {
        sim.Run();
    }
    else if (argc == 3)
    {
        std::string outfile(argv[2]);
        std::ofstream out_file(outfile.c_str(), std::ofstream::out);
        sim.Run(out_file);
        out_file.close();
    }
    else
    {
        std::cout << "Check the program arguments!!!" << std::endl;
    }
}
