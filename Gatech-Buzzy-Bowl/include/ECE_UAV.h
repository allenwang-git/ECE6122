//
// Created by allen on 11/6/21.
//

#ifndef LAB1_ECE_UAV_H
#define LAB1_ECE_UAV_H

#include <vector>
#include <eigen3/Eigen/StdVector>
#include <thread>

using namespace Eigen;
using namespace std;



class ECE_UAV
{
public:
    ECE_UAV(const float px, const float py );
    Vector3f getPosition();
    void start();
    void join();
    thread thuav;
private:
    const float mass = 1.0;
    const float g = 9.81;
    Vector3f position;
    Vector3f velocity;
    Vector3f acceleration;



};


#endif //LAB1_ECE_UAV_H
