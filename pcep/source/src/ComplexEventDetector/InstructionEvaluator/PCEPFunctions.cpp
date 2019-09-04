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

#define _USE_MATH_DEFINES   // M_PI
//#include <cmath>
#include <math.h>

#include <stdlib.h>
#include <ctime>
#include <cstring>

#include "PCEPFunctions.h"
#include <omp.h>
#include <vector>

#define OMP_PARALLEL 1
#define NUM_THREADS 3

//#define TEST_FFT_SIZE	256
//#define TEST_FFT_SIZE	262144  // ~150 ms without parallelization, C code (malloc)... etc. 
#define TEST_FFT_SIZE	1048576


/**
* @brief Reverse bits
* @param x Number to reverse bits
* @param n Number of bits
*/
size_t PCEPFunctions::reverse_bits(size_t x, unsigned int n)
{
    size_t result = 0;

    for (unsigned int i = 0; i < n; i++, x >>= 1)
        result = (result << 1) | (x & 1);

    return result;
}

/**
* @brief Bit-reversed addressing permutation
* @param real[] Array of real values
* @param imag[] Array of image reals
* @param levels Binary levels
* @param n Number of elements
*/
void PCEPFunctions::bitPermutation(double real[], double imag[], unsigned int levels, size_t n)
{
    for (size_t i = 0; i < n; i++) 
    {
        size_t j = reverse_bits(i, levels);
        if (j > i) 
        {
            double temp = real[i];
            real[i] = real[j];
            real[j] = temp;
            temp = imag[i];
            imag[i] = imag[j];
            imag[j] = temp;
        }
    }
}

/**
* @brief Trignometric tables
* @param cos_table Return Cosine table
* @param sin_table Return Sine table
* @param n Number of elements
*/
bool PCEPFunctions::tables(double *cos_table, double *sin_table, size_t n)
{
    for (size_t i = 0; i < n / 2; i++) 
    {
        cos_table[i] = cos(2 * M_PI * i / n);	// real part
        sin_table[i] = sin(2 * M_PI * i / n);   // imag part
    }

    return true;
}

/**
* @brief Calculate binary number of levels
* @param n Number of elements
*/
unsigned int PCEPFunctions::computeLevels(size_t n)
{
    unsigned int levels = 0;
    size_t temp = n;

    while (temp > 1) {
        levels++;
        temp >>= 1;
    }
    if (1u << levels != n)
        return false;  // n is not a power of 2

    return levels;
}

/**
* @brief Cooley Tuckey FFT function: step 1
* @param real[] Array of real values
* @param imag[] Array of image reals
* @param cos_table[] Cosine table
* @param sin_table[] Sine table
* @param n Number of elements
*/
void PCEPFunctions::cooleyTukeyFFT_step1(double real[], double imag[], double cos_table[],
    double sin_table[], size_t n)
{
    // Cooley-Tukey decimation-in-time radix-2 FFT

#if OMP_PARALLEL
#pragma omp parallel num_threads(NUM_THREADS)
    {
#endif

        for (size_t size = 2; size <= 2; size *= 2) {

#if OMP_PARALLEL
#pragma omp for
#endif
            for (int i = 0; i < n; i += size) {
                size_t halfsize = size / 2;
                size_t tablestep = n / size;
                for (size_t j = i, k = 0; j < i + halfsize; j++, k += tablestep) {
                    double tpre = real[j + halfsize] * cos_table[k] + imag[j + halfsize] * sin_table[k];
                    double tpim = -real[j + halfsize] * sin_table[k] + imag[j + halfsize] * cos_table[k];
                    real[j + halfsize] = real[j] - tpre;
                    imag[j + halfsize] = imag[j] - tpim;
                    real[j] += tpre;
                    imag[j] += tpim;
                }
            }
            if (size == n)  // Prevent overflow in 'size *= 2'
                break;
        }

#if OMP_PARALLEL
    }
#endif

}

/**
* @brief Cooley Tuckey FFT function: step 2a
* @param real[] Array of real values
* @param imag[] Array of image reals
* @param cos_table[] Cosine table
* @param sin_table[] Sine table
* @param n Number of elements
*/
void PCEPFunctions::cooleyTukeyFFT_step2_a(double real[], double imag[], double cos_table[], 
    double sin_table[], size_t n)
{
    // Cooley-Tukey decimation-in-time radix-2 FFT

#if OMP_PARALLEL
#pragma omp parallel num_threads(NUM_THREADS)
    {
#endif

        for (size_t size = 4; size <= 4; size *= 2) {

#if OMP_PARALLEL
#pragma omp for
#endif
            for (int i = 0; i < n / 2; i += size) {
                size_t halfsize = size / 2;
                size_t tablestep = n / size;
                for (size_t j = i, k = 0; j < i + halfsize; j++, k += tablestep) {
                    double tpre = real[j + halfsize] * cos_table[k] + imag[j + halfsize] * sin_table[k];
                    double tpim = -real[j + halfsize] * sin_table[k] + imag[j + halfsize] * cos_table[k];
                    real[j + halfsize] = real[j] - tpre;
                    imag[j + halfsize] = imag[j] - tpim;
                    real[j] += tpre;
                    imag[j] += tpim;
                }
            }
            if (size == n)  // Prevent overflow in 'size *= 2'
                break;
        }

#if OMP_PARALLEL
    }
#endif
}

/**
* @brief Cooley Tuckey FFT function: step 2b
* @param real[] Array of real values
* @param imag[] Array of image reals
* @param cos_table[] Cosine table
* @param sin_table[] Sine table
* @param n Number of elements
*/
void PCEPFunctions::cooleyTukeyFFT_step2_b(double real[], double imag[], double cos_table[], 
    double sin_table[], size_t n)
{
    // Cooley-Tukey decimation-in-time radix-2 FFT

#if OMP_PARALLEL
#pragma omp parallel num_threads(NUM_THREADS)
    {
#endif
        for (size_t size = 4; size <= 4; size *= 2) {

#if OMP_PARALLEL
#pragma omp for
#endif
            for (int i = n / 2; i < n; i += size) {
                size_t halfsize = size / 2;
                size_t tablestep = n / size;
                for (size_t j = i, k = 0; j < i + halfsize; j++, k += tablestep) {
                    double tpre = real[j + halfsize] * cos_table[k] + imag[j + halfsize] * sin_table[k];
                    double tpim = -real[j + halfsize] * sin_table[k] + imag[j + halfsize] * cos_table[k];
                    real[j + halfsize] = real[j] - tpre;
                    imag[j + halfsize] = imag[j] - tpim;
                    real[j] += tpre;
                    imag[j] += tpim;
                }
            }
            if (size == n)  // Prevent overflow in 'size *= 2'
                break;
        }

#if OMP_PARALLEL
    }
#endif
}

/**
* @brief Cooley Tuckey FFT function: step 3
* @param real[] Array of real values
* @param imag[] Array of image reals
* @param cos_table[] Cosine table
* @param sin_table[] Sine table
* @param n Number of elements
*/
void PCEPFunctions::cooleyTukeyFFT_step3(double real[], double imag[], double cos_table[],
    double sin_table[], size_t n)
{
    // Cooley-Tukey decimation-in-time radix-2 FFT
#if OMP_PARALLEL
#pragma omp parallel num_threads(NUM_THREADS)
    {
#endif

        for (size_t size = 8; size <= n; size *= 2) {

#if OMP_PARALLEL
#pragma omp for
#endif
            for (int i = 0; i < n; i += size) {
                size_t halfsize = size / 2;
                size_t tablestep = n / size;
                for (size_t j = i, k = 0; j < i + halfsize; j++, k += tablestep) {
                    double tpre = real[j + halfsize] * cos_table[k] + imag[j + halfsize] * sin_table[k];
                    double tpim = -real[j + halfsize] * sin_table[k] + imag[j + halfsize] * cos_table[k];
                    real[j + halfsize] = real[j] - tpre;
                    imag[j + halfsize] = imag[j] - tpim;
                    real[j] += tpre;
                    imag[j] += tpim;
                }
            }
            if (size == n)  // Prevent overflow in 'size *= 2'
                break;
        }
#if OMP_PARALLEL
    }
#endif
}

/**
* @brief Allocate and copy memory with malloc()
* @param src Source elements
* @param n Number of elements
*/
void* PCEPFunctions::memdup_ext(const void *src, size_t n)
{
    void *dest = malloc(n);

    if (dest != NULL)
        memcpy(dest, src, n);

    return dest;
}

/**
* @brief Allocate and copy memory with new
* @param src Source elements
* @param n Number of elements
*/
void* memdup_ext2(const void *src, size_t n)
{
    void* dest = ::operator new(n);

    if (dest != NULL)
        memcpy(dest, src, n);

    return dest;
}

/**
* @brief Generate reandom values
* @param n Number of elements
*/
double* PCEPFunctions::random_reals_ext(int n)
{
    double *result = (double*)malloc(n * sizeof(double));

    for (int i = 0; i < n; i++)
        result[i] = (rand() / (RAND_MAX + 1.0)) * 2 - 1;

    return result;
}

/**
* @brief Generate test values
* @param n Number of elements
* @param image If it's the image file generate different test values
*/
double* PCEPFunctions::test_reals(int n, bool image /*= false*/)
{
    double* result = new double[n];

    if (!image)
        for (int i = 0; i < n; ++i)
            result[i] = ((i + 1) % (n / 10)) + static_cast<double>(i)/100.0;
    else
        for (int i = 0; i < n; ++i)
            result[i] = ((i + 1) % (n / 10)) - static_cast<double>(i)/100.0;
    
    return result;
}

/**
* @brief Cooley Tuckey FFT function
*/
float PCEPFunctions::func_c_fft()
{
    int n = TEST_FFT_SIZE;

    double *inputreal, *inputimag;
    double *actualoutreal, *actualoutimag;

    unsigned int levels;
    double *cos_table, *sin_table;

    //inputreal = random_reals_ext(n);
    //inputimag = random_reals_ext(n);
    inputreal = test_reals(n);
    inputimag = test_reals(n, true);

    //actualoutreal = (double*)memdup_ext(inputreal, n * sizeof(double));
    //actualoutimag = (double*)memdup_ext(inputimag, n * sizeof(double));
    actualoutreal = (double*)memdup_ext2(inputreal, n * sizeof(double));
    actualoutimag = (double*)memdup_ext2(inputimag, n * sizeof(double));

    levels = computeLevels(n);
    bitPermutation(actualoutreal, actualoutimag, levels, (size_t)n);

    //cos_table = (double*)malloc(n / 2 * sizeof(double));
    //sin_table = (double*)malloc(n / 2 * sizeof(double));
    cos_table = new double[n/2];
    sin_table = new double[n/2];

    tables(cos_table, sin_table, n);

    cooleyTukeyFFT_step1(actualoutreal, actualoutimag, cos_table, sin_table, n);
    cooleyTukeyFFT_step2_a(actualoutreal, actualoutimag, cos_table, sin_table, n);
    cooleyTukeyFFT_step2_b(actualoutreal, actualoutimag, cos_table, sin_table, n);
    cooleyTukeyFFT_step3(actualoutreal, actualoutimag, cos_table, sin_table, n);

    //float retVal = sqrt(actualoutreal[50] * actualoutreal[50] + actualoutimag[50] * actualoutimag[50]);
    //float retVal = sqrt(actualoutreal[13426] * actualoutreal[13426] + actualoutimag[13426] * actualoutimag[13426]);     // 63364.9 for TEST_FFT_SIZE 262144
    float retVal = sqrt(actualoutreal[27445] * actualoutreal[27445] + actualoutimag[27445] * actualoutimag[27445]);     // 364131 for TEST_FFT_SIZE 1048576

    //srand((unsigned)time(NULL));    // seed rand() with current time
    //int randidx = rand() % TEST_FFT_SIZE;
    //float randVal = sqrt(actualoutreal[randidx] * actualoutreal[randidx] + actualoutimag[randidx] * actualoutimag[randidx]);
    //std::cout << "randidx = " << randidx << std::endl;

    //free(inputreal);
    //free(inputimag);
    delete[] inputreal;
    delete[] inputimag;

    //free(actualoutreal);
    //free(actualoutimag);
    ::operator delete(actualoutreal);
    ::operator delete(actualoutimag);

    //free(cos_table);
    //free(sin_table);
    delete[] cos_table;
    delete[] sin_table;

    //return randVal;
    return retVal;
}
