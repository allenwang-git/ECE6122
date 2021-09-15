/*
Author: Yinuo Wang
Class: ECE 6122
Last Date Modified: 09/15/2021

Description:
This program can find all prime factors of the input number.
If input is a prime number, the output will be "No prime factors".
All prime factors or message will be output to fill "output1.txt".
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>

using namespace std;

/*
 * Test whether the input number is a prime number or not,
 *
 * @param testPrimeInput the number will be tested whether is a prime or not
 * @return true for prime and false for composite.
 * */
bool isPrime(const unsigned long testPrimeInput){

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
 * This function is used to check whether an input is valid or not.
 *
 * @param strInput input string need to be checked
 * @param validOutput output the valid number
 * @return true for valid input and false for invalid input
 */
bool isValidInput(const string & strInput, unsigned long & validOutput)
{
    unsigned long tmpValid;
//    check the input is a number or not
    for (auto chInput : strInput)
    {
        if(!isdigit(chInput))
            return false;
    }
//    check the number is valid or not
    stringstream(strInput) >> tmpValid;
    if (tmpValid < 2)
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
 * This function determines the prime factors of ulInputNumber.
 *
 * @param ulInputNumber the input number that need to be found factors
 * @param strOutput the string of all prime factors
 * @return true for success and false for failure
 * */
bool GetPrimeFactors (const unsigned long ulInputNumber, string &strOutput)
{
//    Get every prime factors
    if (isPrime(ulInputNumber))
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
    unsigned long ulInputNumber{0};
    fstream outputFile("output1.txt", ios::out | ios::trunc);

//    Parse parameters from command line and check invalid input
    if (argc != 2) //  check the number of arguments
    {
        strOutput =  "Invalid Input";
        cout << "Please input one and only one number for this program.\n";
    }
//    bool isValid = isValid;
    if(!isValidInput(argv[1], ulInputNumber))  //  check if input is valid
    {
        strOutput =  "Invalid Input";
        cout << "Your input is invalid, please input a digital number for this program.\n";
    }
    else if (isPrime(ulInputNumber))  //   Check if input is a prime
    {
        strOutput = "No prime factors";
        cout << "Your input is a prime, which has no prime factors.\n";
    }
    else   //    Get the prime factors of valid input
    {
        GetPrimeFactors(ulInputNumber, strOutput);
    }

//    Output prime factors to file
    if (outputFile.good())
    {
        outputFile << strOutput;
    }
    else
    {
        cout << "Can not open output file.\n";
    }
    return 0;
}
