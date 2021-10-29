/*
Author: Yinuo Wang
Class: ECE 6122
Last Date Modified: 09/18/2021

Description:
This program can compute the number of all black squares after the ant moves a specific steps.
If input is not a number or is a negative number, the output will be "Invalid Input".
The black square number or message will output to fill "output2.txt".
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

using namespace std;
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
    if (tmpValid < 0)
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
 * This class is used to construct a customized type of Map Key
 * */
class Coordinate
{
public:
    long x ,y;
    //    constructor
    Coordinate();
    Coordinate(long _x, long _y)
    {
        x = _x;
        y = _y;
    }
/*
 * This is an operator overload function
 * @return true for equal and false for unequal
 * */
    bool operator==(const Coordinate & coord) const
    {
       return (this->x==coord.x && this->y ==coord.y);
    }
};
/*
 *  Overload Hash function for unordered map
 * */
struct Hash{
    size_t operator()(const Coordinate & c) const
    {
        return hash<long>()(c.x) ^ hash<long>()(c.y);
    }
};
/*
 * Define move direction
 * */
enum Direction
{
    UP = 1,
    DOWN = 3,
    RIGHT = 2,
    LEFT = 4
};

/*
 * This function is used to calculate the number of black squares after specific moving steps.
 *
 * @param totalSteps the number of steps ant need to move
 * @param numBlackSquares the number of black squares after all of movements
 * */
void getBlackSquares(const unsigned long totalSteps, string & numBlackSquares)
{
    //    Initialization
    Coordinate currentCoord(0,0);
    unsigned long currentStep = 0;
    Direction currentDirection = UP;
    unordered_map<Coordinate,string,Hash> gridMap;
    //    ant moving loop
    while (currentStep < totalSteps)
    {
            unordered_map<Coordinate,string,Hash>::const_iterator iter = gridMap.find(currentCoord);
            if ( iter == gridMap.end()) // white grid
            {
                gridMap.insert({currentCoord,"black"});
                    switch(currentDirection){ // clockwise 90 degrees rotation logic
                        case 1:
                            currentDirection=RIGHT;
                            currentCoord.x++ ;
                            break;
                        case 2:
                            currentDirection=DOWN;
                            currentCoord.y--;
                            break;
                        case 3:
                            currentDirection=LEFT;
                            currentCoord.x--;
                            break;
                        case 4:
                            currentDirection= UP;
                            currentCoord.y++;
                            break;
                    }
            }
            else //black grid
            {
                gridMap.erase(iter->first);
                switch(currentDirection)
                { // counterclockwise 90 degrees rotation logic
                    case 3:
                        currentDirection = RIGHT;
                        currentCoord.x++;
                        break;
                    case 4:
                        currentDirection = DOWN;
                        currentCoord.y--;
                        break;
                    case 1:
                        currentDirection = LEFT;
                        currentCoord.x--;
                        break;
                    case 2:
                        currentDirection = UP;
                        currentCoord.y++;
                        break;
                }
            }
        currentStep++;
    }
    numBlackSquares = to_string(gridMap.size());
}
/*
 * This is the Main function of this program, which is used to parse and check input parameters and call functions.
 *
 * @param argc the number of command line arguments
 * @param argv the array of arguments
 *
 * */
int main(int argc, char* argv[])
{
    string strOutput;
    unsigned long ulInputSteps{0};
    fstream outputFile("output2.txt", ios::out | ios::trunc);

    //    Parse parameters from command line and check invalid input
    if (argc != 2) //  check the number of arguments
    {
        strOutput = "Invalid Input";
        cout << "[ECE6122-Lab1-2] Please input one and only one number for this program.\n";
    }
    else if (isValidInput(argv[1],ulInputSteps))
    {
        getBlackSquares(ulInputSteps, strOutput);
    }
    else
    {
        strOutput = "Invalid Input";
        cout << "[ECE6122-Lab1-2] Your input is invalid, please input a positive number for this program.\n";
    }
    //    Output prime factors to file
    if (outputFile.good())
    {
        outputFile << strOutput;
    }
    else
    {
        cout << "[ECE6122-Lab1-2] Can not open output file.\n";
    }
    return 0;
}
