/*
Author: Yinuo Wang
Class: ECE 6122
Last Date Modified: 09/13/2021

Description:
This program can find all prime factors of the input number.
If input is a prime number, the output will be "No prime factors".
All prime factors or message will be output to fill "output1.txt".
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

/*
 * Test whether the input number is a prime number or not,
 *
 * @param testPrimeInput the number will be tested whether is a prime or not
 * @return true for prime and false for composite.
 * */
bool IsPrime (unsigned long testPrimeInput){
    for (int i = 2; i < testPrimeInput; ++i)
    {
        if (testPrimeInput % i ==0)
        {
            return false;
        }
    }
    return true;
}

/*
 * This function determines the prime factors of ulInputNumber.
 *
 * @param ulInputNumber the input number that need to be found factors
 * @param strOutput the string of all prime factors
 * @return true for success and false for failure
 * */
bool GetPrimeFactors (const unsigned long ulInputNumber, string &strOutput){
//    Check invalid input
    if (ulInputNumber < 2)
    {
        cout << "This number is neither prime nor composite.\nPlease change a number.\n";
        return false;
    }
//    Get every prime factors
    if (IsPrime(ulInputNumber))
    {
        strOutput += to_string(ulInputNumber); // last update of factors string
        return true;
    }
    else
    {
        for (unsigned long i = 2; i < ulInputNumber; ++i)
        {
            if (ulInputNumber % i == 0)
            {
                strOutput += (to_string(i) + ",");  // update factors string
                GetPrimeFactors(ulInputNumber / i,strOutput); // recurse to find all factors
                break;
            }
        }
        return true;
    }
}

/*
 * This is the Main function of this program, which is used to parse input parameters and call functions.
 *
 * @param argc the number of command line arguments
 * @param argv the array of arguments
 *
 * */
int main(int argc, char* argv[])
{
    string strOutput;
    unsigned long ulInputNumber;
    fstream outputFile("output1.txt", ios::out | ios::trunc);
//    Parse parameters from command line
    if (argc != 2)
    {
        cout << "Usage: Please input one number when run this program.\n";
    }
    else
    {
        stringstream(argv[1]) >> ulInputNumber;
    }
//    Get the prime factors of valid input
    if (IsPrime(ulInputNumber))
    {
        strOutput = "No prime factors";
    }
    else
    {
        GetPrimeFactors(ulInputNumber, strOutput);
    }
//    Output prime factors to file
    if (outputFile.bad())
    {
        cout << "Can not open output file.\n";
    }
    else
    {
        outputFile << strOutput;
    }
    return 0;
}
