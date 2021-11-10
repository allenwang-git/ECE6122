//
// Created by allen on 11/6/21.
//

#ifndef LAB1_ECE_UAV_H
#define LAB1_ECE_UAV_H

#include <vector>
#include <eigen3/Eigen/StdVector>
#include <thread>
#include <chrono>
#include <cmath>
using namespace Eigen;
using namespace std;


// constant number
#define PI 3.1415926

class ECE_UAV
{
public:
    ECE_UAV(const float px, const float py );
    Vector3f* getPosition();
    Vector3f getVelocity(){
        return velocity;
    };
    Vector3f getAcceleration(){
        return acceleration;
    };
    void start();
    void join();

    void setEndFlag();
    bool getEndFlag();
    void setWaitFlag();
    bool getWaitFlag();
    void setArriveFlag();
    bool getArriveFlag();

    void updateMovement(float & a, float theta, float alpha);
//    float computeTheta();
    void computeAcc(float *acc, float *theta, float *alpha);
    bool checkArrive();



    thread thuav;
    const chrono::high_resolution_clock::time_point startTimestamp = chrono::high_resolution_clock::now();
private:
    const float mass = 1.0;
    const float g = 9.81;
    const float dt = 0.01;
    const Vector3f destPosition{0.,0.,50.};
    Vector3f iniPosition;
    Vector3f position;
    Vector3f velocity;
    Vector3f acceleration;
    bool endFlag = false;
    bool waitFlag = true;
    bool arriveFlag = false;

};


#endif //LAB1_ECE_UAV_H
