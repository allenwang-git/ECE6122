/*
Author: Yinuo Wang
Class: ECE 6122
Last Date Modified: 09/15/2021

Description:
This is the header of the main.pp to define some useful functions
*/

#ifndef LAB1_MAIN_H
#define LAB1_MAIN_H

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>

// common includes
#include "common/inc/helper_cuda.h"
#include "common/inc/helper_functions.h"


#ifndef STRNCASECMP
#define STRNCASECMP strncasecmp
#endif

using namespace std;

/*
 * This function is used to detect if any acceptable arguments is specified or not
 * @param argc is the number of command line arguments
 * @param argv is the char array of arguments
 * @param string_ref is the acceptable arguments identifier
 * @param arg_index is the acceptable argument's index in argv
 *
 * */
inline bool checkCmdLineArg(const int argc, const char **argv, const char *string_ref, int &arg_index)
{
    bool bFound = false;

    if (argc >= 1)
    {
        for (int i=1; i < argc; i++)
        {
            if(argv[i][0] == '-')
            {
                //Input arg length
                int argv_length = (int) strlen(argv[i]) ;
                //Reference arg name length
                int length = (int)strlen(string_ref);
                // compare string
                if (length == argv_length && !STRNCASECMP(argv[i], string_ref, length))
                {
                    bFound = true;
                    arg_index = i;
                    continue;
                }
            }
        }
    }
    return bFound;
}

/*
 * This function is used to check whether an input argument is valid or not.
 *
 * @param strInput input string need to be checked
 * @param validOutput output the valid number
 * @return true for valid input and false for invalid input
 */
bool isValidArg(const string & strInput,int & validOutput)
{
    int tmpValid;
    //    check the input is a number or not
    for (char chInput : strInput)
    {
        if(!isdigit(chInput))
            return false;
    }
    //    check the number is valid or not
    stringstream(strInput) >> tmpValid;
    if (tmpValid > 0)
    {
        validOutput = tmpValid;
        return true;
    }
    else
    {
        return false;
    }
}


#endif //LAB1_MAIN_H
