#pragma once
#include <stdio.h>
#include <cuda.h>
#include <cuda_runtime_api.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <device_functions.h>
#include <math.h>
namespace CudaFunctions {
	float dotProduct(float* firstInput, float* secondInput, int N);
	void dotProductWindow(float* firstInput, float* secondInput, int N, float* output, int outputSize);
	void add(float* firstInput, float* secondInput, int N, float* output);
	void multiply(float* firstInput, float* secondInput, int N, float* output);
	void maskedAdd(float* firstInput, float* secondInput, int* mask, int N, float* output);
	void softplus(float* input, int N, float* output);
}

