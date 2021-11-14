/*
Author: Yinuo Wang
Class: ECE 6122
Last Date Modified: 11/13/2021

Description:
This program can simulate the temperature diffusing process of a plate.
User neesd to specify the interior points dimension and iteration times of the simulation.
The program will check the user input and print message if any invalid arguments are input.
The final temperature matrix will be ouput to the "finalTemperatures.csv".

Reference:
[NVIDIA CUDA sample programs](https://github.com/NVIDIA/cuda-samples)

*/


#include <cuda_runtime.h>
#include "main.h"

/*
 * This function is the gpu kernel function to update the temperature
 * @param H is the temperature matrix of points
 * @param N is the matrix dimension
 */
 __global__ void updateTemperatureCUDA(double *H, int N)
{
    // generate index
    int tID_x = blockIdx.x * blockDim.x + threadIdx.x;
    int tID_y = blockIdx.y * blockDim.y + threadIdx.y;

    double currentTemperature = 0.;
    // update the temperature
    if (tID_x >= 1 && tID_x <= (N-2) && tID_y >= 1 && tID_y <= (N-2))
        currentTemperature = 0.25 *(H[(tID_x+1)+tID_y*N] + H[(tID_x-1)+tID_y*N] + H[tID_x+(tID_y-1)*N] + H[tID_x+(tID_y+1)*N]);
    else
        currentTemperature = H[tID_x+tID_y*N];

    H[tID_x + tID_y*N] = currentTemperature;

    __syncthreads();
}

/*
 * This function is used to simulate the temperature diffusion process
 * @param N is the points in the plate
 * @param iteration is the total updating iteration of the simulation
 * */
void computeTemperature(const int &N, const long iteration)
{
    //Alloacte host memory
    unsigned long mem_size = sizeof(double)* N * N;
    double *H;
    cudaMallocManaged((void**)&H, sizeof(double) * mem_size);
    if (H == NULL)
    {
        fprintf(stderr, "Failed to allocate memory!\n");
        exit(EXIT_FAILURE);
    }

    // Initialize the temperature
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if(i==0&&j<0.7*(N-1)&&j>0.3*(N-1))
                H[i*N+j] = 100.0;

            else
                H[i*N+j] = 20.0;
        }
    }

    cudaError_t error;

    // Allocate CUDA events that we'll use for timing
    cudaEvent_t start;
    error = cudaEventCreate(&start);

    if (error != cudaSuccess)
    {
        fprintf(stderr, "Failed to create start event (error code %s)!\n", cudaGetErrorString(error));
        exit(EXIT_FAILURE);
    }

    cudaEvent_t stop;
    error = cudaEventCreate(&stop);

    if (error != cudaSuccess)
    {
        fprintf(stderr, "Failed to create stop event (error code %s)!\n", cudaGetErrorString(error));
        exit(EXIT_FAILURE);
    }

    // Record the start event
    error = cudaEventRecord(start, NULL);

    if (error != cudaSuccess)
    {
        fprintf(stderr, "Failed to record start event (error code %s)!\n", cudaGetErrorString(error));
        exit(EXIT_FAILURE);
    }

    // Do the iteration
    dim3 blocks(N,1);
    dim3 threads(1,N);
    for (int iter = 0; iter < iteration; ++iter)
    {
        // call gpu cuda function
        updateTemperatureCUDA<<<blocks, threads>>>(H, N);
    }

    // Record the stop event
    error = cudaEventRecord(stop, NULL);
    if (error != cudaSuccess)
    {
        fprintf(stderr, "Failed to record stop event (error code %s)!\n", cudaGetErrorString(error));
        exit(EXIT_FAILURE);
    }

    // Wait for the stop event to complete
    error = cudaEventSynchronize(stop);
    if (error != cudaSuccess)
    {
        fprintf(stderr, "Failed to synchronize on the stop event (error code %s)!\n", cudaGetErrorString(error));
        exit(EXIT_FAILURE);
    }

    // Compute and print the performance
    float msecTotal = 0.0f;
    error = cudaEventElapsedTime(&msecTotal, start, stop);
    if (error != cudaSuccess)
    {
        fprintf(stderr, "Failed to get time elapsed between events (error code %s)!\n", cudaGetErrorString(error));
        exit(EXIT_FAILURE);
    }
    printf("Cuda Kernel Execute Time: %.2f msec\n", msecTotal);

    // Synchronize between device and host
    cudaDeviceSynchronize();

    //   Create a file for output result
    fstream outputFile("finalTemperatures.csv", ios::out | ios::trunc);
    if (outputFile.good())
    {
        for (int i = 0; i < N; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                outputFile <<H[j+i*N] <<",";
            }
            outputFile<<endl;
        }
    }
    else
    {
        cout << "[ECE6122-Lab5] Can not open output file.\n";
    }

}

/*
 *  Main function to check the imput and call the temperature update function
 */
int main(int argc, char **argv)
{
    printf("[2D Steady State Heat Conduction]\n");

    // Set the point number and iteration as what is provided at the command line
    int N = 100, iteration=1000, argValue, argIndex;

    // check -N argument
    if (checkCmdLineArg(argc, (const char **)argv, "-N", argIndex))
    {
        if(argIndex+1 < argc && isValidArg(argv[argIndex+1],argValue))
        {
            N = argValue + 2;
        }
        else
        {
            printf("Invalid parameters, please check your values for '-N'.\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        N = N + 2;
    }
    // check -I argument
    if (checkCmdLineArg(argc, (const char **)argv, "-I", argIndex))
    {
        if(argIndex+1 < argc && isValidArg(argv[argIndex+1],argValue))
        {
            iteration = argValue;
        }
        else
        {
            printf("Invalid parameters,please check your values for '-I'.\n");
            exit(EXIT_FAILURE);
        }
    }

    // check device properties
    int devID = 0;
    cudaError_t error;
    cudaDeviceProp deviceProp;
    error = cudaGetDeviceProperties(&deviceProp, devID);
    if (error != cudaSuccess)
    {
        printf("cudaGetDeviceProperties returned error %s (code %d), line(%d)\n", cudaGetErrorString(error), error, __LINE__);
    }
    else
    {
        printf("GPU Device: \"%s\" \n", deviceProp.name);
    }

    cout<<"Total Points: "<<N<<"*"<<N <<" Iteration times: "<<iteration<<endl;

    // Beigin to compute the temperature
    computeTemperature(N, iteration);

    return 0;
}