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
#include <mutex>
#include <atomic>
using namespace Eigen;
using namespace std;


// constant number
#define PI 3.1415926
#define K 20.0
#define WAIT_TIME 0.1
class ECE_UAV
{
public:
    ECE_UAV(const double px, const double py );
    Vector3d* getPosition();
    Vector3d getVelocity(){
        return velocity;
    };
    Vector3d getAcceleration(){
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
    void setInitSphereFlag();
    bool getInitSphereFlag();

    void updateLinearMovement(const double &maxV, double &a, const double &theta, const double &alpha);
    void updateSphereMovement(const double initV);
//    double computeTheta();
    void initLinearMovement(double *maxV, double *acc, double *theta, double *alpha, Vector4d *unitScale);
    void initSphereMovement(const Vector4d unitScale);
    bool checkReachSphere();
    bool checkInitSphere(const double initSpeed);
    void updateCurrentV();
    double getDistance(Vector3d start, Vector3d end){
        double distance = 0.;
        for (int i = 0; i < 3; ++i) {
            distance += ((end[i] - start[i]) * (end[i] - start[i]));
        }
        if(distance>0.) {
            distance = sqrt(distance);
        }
        return distance;

    }
    Vector3d getUnitDirection(Vector3d start, Vector3d end){
        double scalar = 0.;
        for (int i = 0; i < 3; ++i) {
            scalar += ((end[i]-start[i])*(end[i]-start[i]));
        }
        Vector3d unitVector;
        unitVector.setZero();
        if(scalar>0.){
            scalar = sqrt(scalar);
            for (int i = 0; i < 3; ++i) {
                unitVector[i] = (end[i]-start[i]) / scalar;
            }
        }

        return unitVector;
    }
    Vector3d getUnitDirection(Vector3d vec){
        double scalar = 0.;
        for (int i = 0; i < 3; ++i) {
            scalar += (vec[i]*vec[i]);
        }
        Vector3d unitVector;
        unitVector.setZero();
        if(scalar>0.){
            scalar = sqrt(scalar);
            for (int i = 0; i < 3; ++i) {
                unitVector[i] = vec[i] / scalar;
            }
        }

        return unitVector;
    }
    Vector4d genRandomUnit();
    bool checkCollision(Vector3d p);
    mutex mutexUav;
    thread thuav;
    const chrono::high_resolution_clock::time_point startTimestamp = chrono::high_resolution_clock::now();
private:
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
    bool endFlag = false;
    bool waitFlag = true;
    bool arriveFlag = false;
    bool initSphereFlag = false;

};


#endif //LAB1_ECE_UAV_H
