/*
Author: Yinuo Wang
Class: ECE 6122
Last Date Modified: 10/10/2021

Description:
This program use openmp to calculate an integral expression within 30secs.
If input is not a valid whole number, the output will be "Invalid Input".
The final result will be output into file "Lab2Prob1.txt".
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include "omp.h"

using namespace std;

/*
 * This function is used to check whether an input is valid or not.
 *
 * @param strInput input string need to be checked
 * @param validOutput output the valid number
 * @return true for valid input and false for invalid input
 */
bool isValidInput(const string strInput, unsigned long &validOutput)
{
    unsigned long tmpValid;
//    check the input is a number or not
    for (auto chInput: strInput)
    {
        if (!isdigit(chInput))
            return false;
    }
//    check the number is valid or not
    stringstream(strInput) >> tmpValid;
    if (tmpValid < 1)
    {
        return false;
    }
    else
    {
        validOutput = tmpValid;
        return true;
    }
}

/*
 * This is the Main function of this program.
 *
 * @param argc the number of command line arguments
 * @param argv the array of arguments
 * */
int main(int argc, char *argv[])
{
    // Create a file for output result
    fstream outputFile("Lab2Prob2.txt", ios::out | ios::trunc);
    outputFile.precision(6);
    fixed(outputFile);
    double integralValue = 0.;
    unsigned long N = 0;
    double upperBound = log(2) / 7;
    double lowerBound = 0.;
    double delta_x, F_x;

//    Parse parameters from command line and check invalid input
    if (argc != 2) //  check the number of arguments
    {
        cout << "[ECE6122-Lab2-2] Please input one and only one argument for this program.\n";
        return 1;
    }
//    Run the parallel computing if input is valid
    if (isValidInput(argv[1], N))
    {
        delta_x = (upperBound - lowerBound) / N;
        // Allocate the for loop assignment to several threads
#pragma omp parallel for reduction (+:integralValue)
        for (long i = 1; i <= N; ++i)
        {
            F_x = 14. * exp(7. * (lowerBound + (i - 0.5) * delta_x));
            integralValue += (delta_x * F_x);
        }
    } else
    {
        cout << "[ECE6122-Lab2-2] Please input a WHOLE NUMBER for this program.\n";
    }

    //    Output prime factors to file
    if (outputFile.good())
    {
        outputFile << integralValue;
    }
    else
    {
        cout << "[ECE6122-Lab2-2] Can not open output file.\n";
    }
    return 0;
}

