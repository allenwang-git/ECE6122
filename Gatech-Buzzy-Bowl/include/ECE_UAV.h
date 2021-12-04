/*
Author: Yinuo Wang
Class: ECE 6122
Last Date Modified: 11/25/2021

Description:
 This program is the header of ECE_UAV.cpp
 to clarify the class and define some useful constants and variables.
*/

#ifndef LAB1_ECE_UAV_H
#define LAB1_ECE_UAV_H
#pragma once
#include <vector>
#include <eigen3/Eigen/StdVector>
#include <thread>
#include <chrono>
#include <cmath>
#include <mutex>
#include <atomic>
using namespace Eigen;
using namespace std;

// constant number
#define PI 3.1415926
#define WAIT_TIME 0.1

class ECE_UAV
{
public:
    // thread function
    ECE_UAV(const double px, const double py);
    void start();
    void join();

    // getter and setter of flags
    void setEndFlag();
    bool getEndFlag();
    void setWaitFlag();
    bool getWaitFlag();
    void setArriveFlag();
    bool getArriveFlag();
    void setInitSphereFlag();
    bool getInitSphereFlag();

    // kinematic function
    void initialize(double *maxV, double *acc, double *theta, double *alpha, Vector4d *unitScale);
    void initSphereMovement(const Vector4d unitScale);
    void updateLinearMovement(const double &maxV, double &a, const double &theta, const double &alpha);
    void updateSphereMovement(const double initV);
    bool checkReachSphere();
    bool checkInitSphere(const double initSpeed);
    bool checkCollision(Vector3d p);
    void updateCurrentV();
    Vector3d* getPosition();
    double getDistance(Vector3d start, Vector3d end);
    Vector3d getUnitDirection(Vector3d start, Vector3d end);
    Vector3d getUnitDirection(Vector3d vec);
    Vector4d genRandomUnit();

    // variable member
    mutex mutexUav;
    thread thuav;
    const chrono::high_resolution_clock::time_point startTimestamp = chrono::high_resolution_clock::now();
private:
    // kinematics variable
    const double mass = 1.0f;
    const double g = 9.81f;
    const double dt = 0.01f;
    const Vector3d destPosition{0.,0.,20.f};
    double currentV = 0.f;
    Vector3d iniPosition;
    Vector3d position;
    Vector3d velocity;
    Vector3d acceleration;
    Vector3d unitLinearDirection;

    // process flag
    bool waitFlag = true;
    bool arriveFlag = false;
    bool initSphereFlag = false;
    bool endFlag = false;
};


#endif //LAB1_ECE_UAV_H
