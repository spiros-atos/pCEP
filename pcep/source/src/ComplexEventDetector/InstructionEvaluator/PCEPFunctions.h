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

#ifndef PCEPFUNCTIONS_H
#define	PCEPFUNCTIONS_H

class ECapturedEvent;


struct PCEPFunctions {

    static double* random_reals_ext(int n);
    static double* test_reals(int n, bool image = false);
    static void* memdup_ext(const void *src, size_t n);
    static unsigned int computeLevels(size_t n);
    static void bitPermutation(double real[], double imag[], unsigned int levels, size_t n);
    static bool tables(double *cos_table, double *sin_table, size_t n);
    static size_t reverse_bits(size_t x, unsigned int n);

    static void cooleyTukeyFFT_step1(double real[], double imag[], double cos_table[], double sin_table[], size_t n);
    static void cooleyTukeyFFT_step2_a(double real[], double imag[], double cos_table[], double sin_table[], size_t n);
    static void cooleyTukeyFFT_step2_b(double real[], double imag[], double cos_table[], double sin_table[], size_t n);
    static void cooleyTukeyFFT_step3(double real[], double imag[], double cos_table[], double sin_table[], size_t n);

    static float func_c_fft();
};

#endif  //PCEPFUNCTIONS_H
