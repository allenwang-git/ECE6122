/*
Author: Yinuo Wang
Class: ECE 6122
Last Date Modified: 09/20/2021

Description:
This program can find the reflection number after the laser exits a triangle.
If input is the  initial reflection’s x location, the output will be the number of reflection times.
And the number or error message will output to file "output3.txt".
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>

#define AB 1
#define BC 2
#define AC 3

using namespace std;
/*
 *  This class is used to handle reflection task
 * */
class Reflection{
public:
    Reflection(const double posInput){
        xPosABInput = posInput;
    }
//    void initialization();
    void getReflectioinTimes();
    int reflectTimes{0};
    double xPosABInput;
private:
    void get2PointsLine(double x1, double y1, double x2, double y2);
    bool get2LinesIntersection(double A1, double B1, double C1, double A2, double B2, double C2, double& x, double& y);
    void getSymmetryLine();
    void getReflectLine();

    const double yPosAB = 10. * sqrt(3.);
    const double origin[2]={0.,0.};

    double incidentA{}, incidentB{}, incidentC{};
    double reflectA{}, reflectB{}, reflectC{};
    double symmetryA{}, symmetryB{}, symmetryC{};

    const double lineAB_A{0.}, lineAB_B{1.}, lineAB_C{-yPosAB};
    const double lineBC_A{sqrt(3.)}, lineBC_B{-1.},lineBC_C{0.};
    const double lineAC_A{-sqrt(3.)},lineAC_B{-1.},lineAC_C{0.};

    const double perpendicularAB_A{1.}, perpendicularAB_B{0.};
    const double perpendicularBC_A{-1./ sqrt(3.)}, perpendicularBC_B{-1.};
    const double perpendicularAC_A{1./ sqrt(3.)},perpendicularAC_B{-1.};

    const double perpendicularA_A{-1./ sqrt(3.)},perpendicularA_B{-1.},perpendicularA_C{20./ sqrt(3.)};
    const double perpendicularB_A{1./ sqrt(3.)},perpendicularB_B{-1.},perpendicularB_C{20./ sqrt(3.)};

    double xAB,yAB,xBC,yBC,xAC,yAC;
    bool firstTime{true};
    int pre_intersectLine;
};

/*
 * This function can compute a line equation given 2 points
 * @param x1, y1 are the coordinates of one point
 * @param x2, y2 are the coordinates of another point
 * */
void Reflection::get2PointsLine(double x1, double y1, double x2, double y2){
//   Line: (y-y1)/(x-x1) = (y-y2)/(x-x2)
//   =>  Ax + By + C =0
    incidentA = y1 - y2;
    incidentB = x2 - x1;
    incidentC = x1 * y2 - x2 * y1;
}
/*
 * This function can compute the intersection point given 2 lines
 * @param A1, B1, C1 are factors of one line
 * @param A2, B2, C2 are factors of another line
 * @param x,y are intersection point coordinates
 * @return true if intersection exits and false if lines are parallel
 * */
bool Reflection::get2LinesIntersection(double A1, double B1, double C1, double A2, double B2, double C2, double &x, double &y){
//  A1x + B1y + C1 =0
//  A2x + B2y + C2 =0
    double m = A1 * B2 - A2 * B1;
    if (m==0)
    {
        cout << "No intersection\n";
        return false;
    }
    else{
        x = (C2*B1 - C1*B2) / m;
        y = (C1*A2 - C2*A1) / m;
    }
    return true;
}
/*
 * This function can compute the symmetry line equation of the reflection
 * given the incident line equation and intersection point.
 *
 * */
#include <iomanip>
void Reflection::getSymmetryLine(){

    double xx,yy;
//    compute intersection point with every line
    this->get2LinesIntersection(incidentA,incidentB,incidentC,lineAB_A,lineAB_B,lineAB_C,xAB,yAB);
    this->get2LinesIntersection(incidentA,incidentB,incidentC,lineBC_A,lineBC_B,lineBC_C,xBC,yBC);
    this->get2LinesIntersection(incidentA,incidentB,incidentC,lineAC_A,lineAC_B,lineAC_C,xAC,yAC);
//    decide the correct intersection point and compute symmetry line
    if (xAB< 10. && xAB> -10. && yAB - yPosAB < 1e-12 && pre_intersectLine!=AB)
    {
        symmetryA = perpendicularAB_A;
        symmetryB = perpendicularAB_B;
        symmetryC = -symmetryA * xAB - symmetryB * yAB;
        pre_intersectLine = AB;
    }
    else if (xBC < 10. && xBC > 0. && yBC > 0.01 && yBC <= yPosAB && pre_intersectLine!=BC)
    {
        symmetryA = perpendicularBC_A;
        symmetryB = perpendicularBC_B;
        symmetryC = -symmetryA * xBC - symmetryB * yBC;
        pre_intersectLine = BC;
    }
    else if (xAC > -10. && xAC < 0. && yAC > 0.01 && yAC <= yPosAB && pre_intersectLine!=AC)
    {
        symmetryA = perpendicularAC_A;
        symmetryB = perpendicularAC_B;
        symmetryC = -symmetryA * xAC - symmetryB * yAC;
        pre_intersectLine = AC;
    }
    else if ((xAB==10. || xBC==10.) && yAB == yPosAB && pre_intersectLine==AC) // POINT B
    {
        symmetryA = perpendicularB_A;
        symmetryB = perpendicularB_B;
        symmetryC = perpendicularB_C;
        pre_intersectLine = AB;
    }
    else if ((xAB==-10. || xAC==-10.) && yAB == yPosAB && pre_intersectLine==BC)// POINT A
    {
        symmetryA = perpendicularA_A;
        symmetryB = perpendicularA_B;
        symmetryC = perpendicularA_C;
        pre_intersectLine = AB;
    }
}
/*
 * This funtion can compute the reflection line equation
 * given the incident line and symmetry line equations
 *
 * */
void Reflection::getReflectLine()
{
//    compute reflection line
    reflectA = incidentA * (pow(symmetryB,2) - pow(symmetryA,2)) - 2.*symmetryA*symmetryB*incidentB;
    reflectB = incidentB * (pow(symmetryA,2) - pow(symmetryB,2)) - 2.*symmetryA*symmetryB*incidentA;
    reflectC = incidentC * (pow(symmetryA,2) + pow(symmetryB,2)) - 2.*symmetryC*(symmetryA*incidentA + symmetryB*incidentB);
//    update incident line
    incidentA = reflectA;
    incidentB = reflectB;
    incidentC = reflectC;
}
/*
 * This function can compute the number of reflection times
 * after the beam exits the triangle.
 * */
void Reflection::getReflectioinTimes(){
    if (firstTime){
        this->get2PointsLine(origin[0],origin[1], xPosABInput, yPosAB);
        firstTime =false;
    }
    this->getSymmetryLine();
    this->getReflectLine();
//    check if the beam can exit this time
    if ((reflectC/reflectB) <= 0.01 && abs(reflectC/reflectA) <= (0.01* sqrt(3.)) && reflectA!=0. && reflectB!=0.)
    {
        reflectTimes++;
    }
    else if (reflectB ==0. && reflectA!=0. && abs(reflectC/reflectA) <= (0.01* sqrt(3.)))
    {
        reflectTimes++;
    }
    else if (reflectA == 0. && reflectB!=0. && (reflectC/reflectB) <= 0.01)
    {
        reflectTimes++;
    }
    else // If not exit, reflect again
    {
        reflectTimes++;
        this->getReflectioinTimes();
    }
}

/*
 * This is the Main function of this program,
 * which is used to parse input parameters, check input, and call functions.
 *
 * @param argc the number of command line arguments
 * @param argv the array of arguments
 * */
int main(int argc, char* argv[])
{
    string strOutput;
    fstream outputFile("output3.txt", ios::out | ios::trunc);
    double tmpXPosInput;
    //    Parse parameters from command line and check it
    if (argc != 2)
    {
        cout << "[ECE6122-Lab1-3] Please input a X coordination(cm) from -10 to 10 when run this program.\n";
    }
    else
    {
        stringstream(argv[1]) >> tmpXPosInput;
    }
    Reflection refObj(tmpXPosInput);

    if (refObj.xPosABInput > -10. && refObj.xPosABInput < 10.){
        refObj.getReflectioinTimes(); // begin the reflection
        strOutput = to_string(refObj.reflectTimes); // get the reflection times
    }
    else
    {
        strOutput = "Invalid Input";
        cout << "[ECE6122-Lab1-3] Invalid initial reflection's x location, please change a location.\n";
    }
    //    Output prime factors to file
    if (outputFile.good())
    {
        outputFile << strOutput;
    }
    else
    {
        cout << "[ECE6122-Lab1-1] Can not open output file.\n";
    }
    return 0;
}
