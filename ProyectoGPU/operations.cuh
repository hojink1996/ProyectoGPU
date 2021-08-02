#pragma once
#include <stdio.h>
#include <cuda.h>
#include <cuda_runtime_api.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <device_functions.h>
#include <math.h>

namespace CudaFunctions {
	/*
	Function that computes the dot product between two vectors. Note that both vectors must have the same size.

	@param firstInput:		Pointer to the first vector.
	@param secondInput:		Pointer to the second vector.
	@param N:				Size of the vector.
	*/
	float dotProduct(float* firstInput, float* secondInput, int N);

	/*
	Function that computes the piecewise dot product between two vectors.
	Both vectors have size NxM, and each segment of size N is used to compute the dot product, 
	and saved in a position of a vector of size M.

	@param firstInput:		Pointer to the first vector of size NxM.
	@param secondInput:		Pointer to the second vector of size NxM.
	@param N:				Size of the segment for which the dot product is computed.
	@param output:			Pointer to the output vector of size M.
	@param outputSize:		The size M of the output
	*/
	void dotProductWindow(float* firstInput, float* secondInput, int N, float* output, int outputSize);
	
	/*
	Function that adds two vectors in an element-wise manner. Note that both vectors must have the same size.

	@param firstInput:		Pointer to the first vector.
	@param secondInput:		Pointer to the second vector.
	@param N:				Size of the vector.
	@param output:			Pointer to the output vector.
	*/
	void add(float* firstInput, float* secondInput, int N, float* output);

	/*
	Function that multiplies two vectors in an element-wise manner. Note that both vectors must have the same size.

	@param firstInput:		Pointer to the first vector.
	@param secondInput:		Pointer to the second vector.
	@param N:				Size of the vector.
	@param output:			Pointer to the output vector.
	*/
	void multiply(float* firstInput, float* secondInput, int N, float* output);

	/*
	Function that adds two vectors in an element-wise manner given a mask. Note that all vectors must have the same size.

	@param firstInput:		Pointer to the first vector.
	@param secondInput:		Pointer to the second vector.
	@param mask:			Pointer to the mask vector.
	@param N:				Size of the vector.
	@param output:			Pointer to the output vector.
	*/
	void maskedAdd(float* firstInput, float* secondInput, int* mask, int N, float* output);


	/*
	Function that computes the softplus function for each element of a vector.

	@param input:			Pointer to the input vector.
	@param N:				Size of the vector.
	@param output:			Pointer to the output vector.
	*/
	void softplus(float* input, int N, float* output);
}

