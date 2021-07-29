#include <iostream>
#include <vector>
#include "operations.cuh"
#define BLOCK_SIZE 512

__global__ void addKernel(const float* a, const float* b, float* c)
{
	int i = blockDim.x * blockIdx.x + threadIdx.x;
	c[i] = a[i] + b[i];
}

__global__ void multiplyKernel(const float* firstInput, const float* secondInput, float* output)
{
	int index = blockDim.x * blockIdx.x + threadIdx.x;
	output[index] = firstInput[index] * secondInput[index];
}

__global__ void maskedAddKernel(const float* firstInput, const float* secondInput, const int* mask, float* output)
{
	int index = blockDim.x * blockIdx.x + threadIdx.x;
	output[index] = firstInput[index] + secondInput[index] * mask[index];
}

__global__ void softplusKernel(const float* input, float* output)
{
	int index = blockDim.x * blockIdx.x + threadIdx.x;
	output[index] = logf(1 + expf(input[index]));
}

__global__ void dotProductCuda(const float* firstInput, const float* secondInput, float* output)
{
	// Dynamically allocate the share memory
	int THREADS_PER_BLOCK = __cudaGet_blockDim().x;
	__shared__ float sharedMemory[BLOCK_SIZE];
	int multiplyX = blockDim.x * blockIdx.x + threadIdx.x;
	sharedMemory[threadIdx.x] = firstInput[multiplyX] * secondInput[multiplyX];

	// All of the threads must be done with the multiplication
	__syncthreads();

	// Add the values in the block
	if (threadIdx.x == 0)
	{
		float blockProduct = 0;
		for (int i = 0; i < THREADS_PER_BLOCK; ++i)
		{
			blockProduct += sharedMemory[i];
			// printf("%s", sharedMemory[i]);
		}
		atomicAdd(output, blockProduct);
	}
}

__global__ void dotProductWindowCuda(const float* firstInput, const float* secondInput, const int N, float* output)
{
	// Dynamically allocate the share memory
	//int THREADS_PER_BLOCK = __cudaGet_blockDim().x;
	__shared__ float sharedMemory[BLOCK_SIZE];
	int multiplyX = blockDim.x * blockIdx.x + threadIdx.x;
	sharedMemory[threadIdx.x] = firstInput[multiplyX] * secondInput[multiplyX % N];
	//printf(" (%i) %f * %f  = %f \n", multiplyX, firstInput[multiplyX], secondInput[multiplyX % N], sharedMemory[threadIdx.x]);

	int tIdx = threadIdx.x;

	// All of the threads must be done with the multiplication
	__syncthreads();

	// Add the values in the block
	int offset = blockDim.x * blockIdx.x;
	if (threadIdx.x == 0)
	{
		for (int i = 0; i < BLOCK_SIZE; ++i)
		{
			output[(offset + i) / N] += sharedMemory[i];
		 }
	}	
	__syncthreads();

	//printf("(%i) out: %f, %f, %f, %f\n", multiplyX, output[0], output[1], output[2], output[3]);
}

namespace CudaFunctions
{
	void dotProductWindow(float* firstInput, float* secondInput, int N, float* output, int outputSize)
	{
		int inputSize = N * outputSize;
		// Allocate memory for the CUDA operations
		float* dFirstInput;
		float* dSecondInput;
		float* dOutput;
		cudaMalloc((void**)& dFirstInput, inputSize * sizeof(float));
		cudaMalloc((void**)& dSecondInput, N * sizeof(float));
		cudaMalloc((void**)& dOutput, outputSize * sizeof(float));

		// Copy the values to GPU
		cudaMemcpy(dFirstInput, firstInput, inputSize * sizeof(float), cudaMemcpyHostToDevice);
		cudaMemcpy(dSecondInput, secondInput, N * sizeof(float), cudaMemcpyHostToDevice);
		cudaMemcpy(dOutput, output, outputSize * sizeof(float), cudaMemcpyHostToDevice);

		// Set the blocks to use
		dim3 dimBlock(BLOCK_SIZE);
		dim3 dimGrid((inputSize + BLOCK_SIZE - 1)/ BLOCK_SIZE);

		// Run the kernel
		dotProductWindowCuda<<<dimGrid, dimBlock>>> (dFirstInput, dSecondInput, N, dOutput);
		cudaDeviceSynchronize();

		// Copy the values to host
		cudaMemcpy(output, dOutput, outputSize * sizeof(float), cudaMemcpyDeviceToHost);

		cudaError_t error = cudaGetLastError();
		if (error != cudaSuccess) {
			fprintf(stderr, "ERROR CUDA: %s \n", cudaGetErrorString(error));
			return;
		}	

		// Clean up the values
		cudaFree(dFirstInput); cudaFree(dSecondInput); cudaFree(dOutput);
	}

	float dotProduct(float* firstInput, float* secondInput, int N)
	{
		// Allocate memory for the CUDA operations
		float output;
		float* dFirstInput;
		float* dSecondInput;
		float* dOutput;
		cudaMalloc((void**)&dFirstInput, N * sizeof(float));
		cudaMalloc((void**)&dSecondInput, N * sizeof(float));
		cudaMalloc((void**)&dOutput, sizeof(float));

		// Copy the values to GPU
		cudaMemcpy(dFirstInput, firstInput, N * sizeof(float), cudaMemcpyHostToDevice);
		cudaMemcpy(dSecondInput, secondInput, N * sizeof(float), cudaMemcpyHostToDevice);

		// Set the blocks to use
		dim3 dimBlock(BLOCK_SIZE);
		dim3 dimGrid(N / BLOCK_SIZE + 1);

		// Run the kernel
		dotProductCuda << <dimGrid, dimBlock >> > (dFirstInput, dSecondInput, dOutput);
		cudaDeviceSynchronize();

		cudaError_t error = cudaGetLastError();
		if (error != cudaSuccess) {
			fprintf(stderr, "ERROR: %s \n", cudaGetErrorString(error));
			return 1;
		}

		// Copy the values to host
		cudaMemcpy(&output, dOutput, sizeof(float), cudaMemcpyDeviceToHost);

		// Clean up the values
		cudaFree(dFirstInput); cudaFree(dSecondInput); cudaFree(dOutput);

		return output;
	}

	void add(float* firstInput, float* secondInput, int N, float* output)
	{
		// Allocate memory for the CUDA operations
		float* dFirstInput;
		float* dSecondInput;
		float* dOutput;
		cudaMalloc((void**)& dFirstInput, N * sizeof(float));
		cudaMalloc((void**)& dSecondInput, N * sizeof(float));
		cudaMalloc((void**)& dOutput, N * sizeof(float));

		// Copy the values to GPU
		cudaMemcpy(dFirstInput, firstInput, N * sizeof(float), cudaMemcpyHostToDevice);
		cudaMemcpy(dSecondInput, secondInput, N * sizeof(float), cudaMemcpyHostToDevice);

		// Set the blocks to use
		dim3 dimBlock(N);
		dim3 dimGrid(N / BLOCK_SIZE + 1);

		// Run the kernel
		addKernel <<<dimGrid, dimBlock >>> (dFirstInput, dSecondInput, dOutput);
		cudaDeviceSynchronize();

		// Copy the values to host
		cudaMemcpy(output, dOutput, N * sizeof(float), cudaMemcpyDeviceToHost);
		cudaError_t error = cudaGetLastError();
		if (error != cudaSuccess) {
			fprintf(stderr, "ERROR: %s \n", cudaGetErrorString(error));
			return;
		}
		// Clean up the values
		cudaFree(dFirstInput); cudaFree(dSecondInput); cudaFree(dOutput);
	}

	void multiply(float* firstInput, float* secondInput, int N, float* output)
	{
		// Allocate memory for the CUDA operations
		float* dFirstInput;
		float* dSecondInput;
		float* dOutput;
		cudaMalloc((void**)& dFirstInput, N * sizeof(float));
		cudaMalloc((void**)& dSecondInput, N * sizeof(float));
		cudaMalloc((void**)& dOutput, N * sizeof(float));

		// Copy the values to GPU
		cudaMemcpy(dFirstInput, firstInput, N * sizeof(float), cudaMemcpyHostToDevice);
		cudaMemcpy(dSecondInput, secondInput, N * sizeof(float), cudaMemcpyHostToDevice);

		// Set the blocks to use
		dim3 dimBlock(BLOCK_SIZE);
		dim3 dimGrid(N / BLOCK_SIZE + 1);

		// Run the kernel
		multiplyKernel <<<dimGrid, dimBlock >>> (dFirstInput, dSecondInput, dOutput);
		cudaDeviceSynchronize();

		cudaError_t error = cudaGetLastError();
		if (error != cudaSuccess) {
			fprintf(stderr, "ERROR: %s \n", cudaGetErrorString(error));
			return;
		}

		// Copy the values to host
		cudaMemcpy(output, dOutput, N * sizeof(float), cudaMemcpyDeviceToHost);

		// Clean up the values
		cudaFree(dFirstInput); cudaFree(dSecondInput); cudaFree(dOutput);
	}

	void maskedAdd(float* firstInput, float* secondInput, int* mask, int N, float* output)
	{
		// Allocate memory for the CUDA operations
		float* dFirstInput;
		float* dSecondInput;
		float* dOutput;
		int* dMask;
		cudaMalloc((void**)& dFirstInput, N * sizeof(float));
		cudaMalloc((void**)& dSecondInput, N * sizeof(float));
		cudaMalloc((void**)& dMask, N * sizeof(int));
		cudaMalloc((void**)& dOutput, N * sizeof(float));

		// Copy the values to GPU
		cudaMemcpy(dFirstInput, firstInput, N * sizeof(float), cudaMemcpyHostToDevice);
		cudaMemcpy(dSecondInput, secondInput, N * sizeof(float), cudaMemcpyHostToDevice);
		cudaMemcpy(dMask, mask, N * sizeof(int), cudaMemcpyHostToDevice);

		// Set the blocks to use
		dim3 dimBlock(BLOCK_SIZE);
		dim3 dimGrid(N / BLOCK_SIZE + 1);

		// Run the kernel
		maskedAddKernel<<<dimGrid, dimBlock >>> (dFirstInput, dSecondInput, dMask, dOutput);
		cudaDeviceSynchronize();

		cudaError_t error = cudaGetLastError();
		if (error != cudaSuccess) {
			fprintf(stderr, "ERROR: %s \n", cudaGetErrorString(error));
			return;
		}

		// Copy the values to host
		cudaMemcpy(output, dOutput, N * sizeof(float), cudaMemcpyDeviceToHost);

		// Clean up the values
		cudaFree(dFirstInput); cudaFree(dSecondInput); cudaFree(dOutput);
	}


	void softplus(float* input, int N,  float* output)
	{
		// Allocate memory for the CUDA operations
		float* dInput;
		float* dOutput;
		cudaMalloc((void**)& dInput, N * sizeof(float));
		cudaMalloc((void**)& dOutput, N * sizeof(float));

		// Copy the values to GPU
		cudaMemcpy(dInput, input, N * sizeof(float), cudaMemcpyHostToDevice);

		// Set the blocks to use
		dim3 dimBlock(BLOCK_SIZE);
		dim3 dimGrid(N / BLOCK_SIZE + 1);

		// Run the kernel
		softplusKernel<<<dimGrid, dimBlock>>> (dInput, dOutput);
		cudaDeviceSynchronize();

		cudaError_t error = cudaGetLastError();
		if (error != cudaSuccess) {
			fprintf(stderr, "ERROR: %s \n", cudaGetErrorString(error));
			return;
		}

		// Copy the values to host
		cudaMemcpy(output, dOutput, N * sizeof(float), cudaMemcpyDeviceToHost);

		// Clean up the values
		cudaFree(dInput);  cudaFree(dOutput);
	}
}
