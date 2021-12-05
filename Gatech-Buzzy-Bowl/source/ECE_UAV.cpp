/*
Author: Yinuo Wang
Class: ECE 6122
Last Date Modified: 11/25/2021

Description:
 This program implements all member functions in ECE_UAV class
 and defines the UAV thread function to handle its movement.
*/
#pragma once
#include "../include/ECE_UAV.h"
#include <iostream>
#include <random>

using namespace std;
using namespace chrono;
/*
 * thread function to handle the UAV's movement control
 * */
void threadFunction(ECE_UAV* pUAV)
{
    // Do some initializing computing
    double acc, theta, alpha, maxV;
    Vector4d unitScale;
    pUAV->initialize(&maxV, &acc, &theta, &alpha, &unitScale);


    while(!pUAV->getEndFlag())
    {
        if(pUAV->getWaitFlag())
        {
            // wait 5s to launch
            if(duration_cast<duration<double>>(high_resolution_clock::now()-pUAV->startTimestamp).count() >= WAIT_TIME)
            {
                pUAV->setWaitFlag();
            }
        }
        else if(!pUAV->getArriveFlag())
        {
            // linear movement to the sphere surface
            pUAV->updateLinearMovement(maxV,acc, theta, alpha);

            if(pUAV->checkReachSphere())
                pUAV->setArriveFlag();
        }
        else if (!pUAV->getInitSphereFlag())
        {
            // initialize the velocity on the sphere surface
            pUAV->initSphereMovement(unitScale);
            if(pUAV->checkInitSphere(unitScale[3]))
                pUAV->setInitSphereFlag();

        }
        else if (!pUAV->getEndFlag())
        {
            // Move around the sphere surface
            pUAV->updateSphereMovement(unitScale[3]);
        }
        else if(pUAV->getEndFlag())
        {
            // exit the loop and end the thread
            break;
        }
        // set the update rate
        this_thread::sleep_for(chrono::milliseconds(10));
    }
    return;

}
/*
 * Constructor of UAV and start the thread
 * */
ECE_UAV::ECE_UAV(const double px, const double py)
{
    iniPosition.setZero();
    iniPosition[0] = px;
    iniPosition[1] = py;
    iniPosition[2] = 0.6f;
    position = iniPosition;
    velocity.setZero();
    acceleration.setZero();
    acceleration[2] = -g;
    this->start();
}
/*
 * get the uav's position
 * */
Vector3d* ECE_UAV::getPosition()
{
    return &position;
}
/*
 * set the state of endFlag and join the thread
 * */
void ECE_UAV::setEndFlag()
{
    this->endFlag = true;
    join();
}
/*
 * get the state of endFlag
 * */
bool ECE_UAV::getEndFlag()
{
    return this->endFlag;
}
/*
 * set the state of waitFlag
 * */
void ECE_UAV::setWaitFlag()
{
    this->waitFlag = false;
}
/*
 * get the state of waitFlag
 * */
bool ECE_UAV::getWaitFlag()
{
    return this->waitFlag;
}
/*
 * set the state of arriveFlag
 * */
void ECE_UAV::setArriveFlag()
{
    this->arriveFlag = true;
}
/*
 * get the state of arriveFlag
 * */
bool ECE_UAV::getArriveFlag()
{
    return this->arriveFlag;
}
/*
 * get the state of initSphereFlag
 * */
void ECE_UAV::setInitSphereFlag()
{
    this->initSphereFlag = true;
}
/*
 * get the state of initSphereFlag
 * */
bool ECE_UAV::getInitSphereFlag()
{
    return this->initSphereFlag;
}
/*
 * Start the thread
 * */
void ECE_UAV::start()
{
    thread t(threadFunction,this);
    thuav = move(t);
}
/*
 * Wrapper of join()
 * */
void ECE_UAV::join()
{
    if(thuav.joinable())
        thuav.join();
}
/*
 * Initialize the uav's kinematics and dynamics
 * */
void ECE_UAV::initialize(double *maxV, double *acc, double *theta, double *alpha, Vector4d *unitScale)
{
    //    generate a max speed for uav between 1.0~2.0
    unsigned seed = hash<thread::id>()(this_thread::get_id());
    default_random_engine e(seed);
    uniform_real_distribution<double> u(1.0, 2.0);
    *maxV = u(e);

    // compute the desired acceleration and angle for linear movement
    position = iniPosition;
    double f_uav = 20.0;
    if(iniPosition[0]==0.f && iniPosition[1] ==0.f)
    {
        *acc = f_uav/mass + (-g);
        *theta = PI/2;
        *alpha = PI/2;
        unitLinearDirection = {0.,0.,1.0};
    }
    else
    {
        double xyDistance = sqrt(pow(iniPosition[0],2)+ pow(iniPosition[1],2));
        *theta = atan(abs(destPosition[2] - iniPosition[2])/xyDistance);
        if(isnan(*theta))
            *theta = PI/2;
        *alpha = atan(abs(iniPosition[0]) / abs(iniPosition[1]));
        if(isnan(*alpha))
            *alpha = PI/2;
        /*****************************
         * ma = F+ G
         * Fcos(beta) = mg
         * Fsin(beta) = ma*cos(theta)
         *******************************/
        double beta = acos(mass *g /f_uav);
        *acc = f_uav * sin(beta)/(mass * cos(*theta));
        // compute the linear direction
        unitLinearDirection = getUnitDirection(iniPosition, destPosition);
    }
    // generate the random and initial force when uav reach the sphere surface
    *unitScale = genRandomUnit();
}
/*
 *  compute velocity scalar
 */
void ECE_UAV::updateCurrentV()
{
    currentV = sqrt(velocity[0]*velocity[0] +  velocity[1]*velocity[1] + velocity[2]*velocity[2]);
}

/*
 * Update the uav's kinematics of linear movement
 * */
void ECE_UAV::updateLinearMovement(const double &maxV, double &a, const double &theta, const double &alpha)
{
    //  computeAcc (a,theta,alpha);
    acceleration[0] = a * cos(theta) * sin(alpha);
    acceleration[1] = a * cos(theta) * cos(alpha);
    acceleration[2] = a * sin(theta);

    if(destPosition[0]<=iniPosition[0] && destPosition[1]<=iniPosition[1])
    {
        acceleration[0] = -acceleration[0];
        acceleration[1] = -acceleration[1];
    }
    else if (destPosition[0]<=iniPosition[0] && destPosition[1]>=iniPosition[1])
    {
        acceleration[0] = -acceleration[0];
    }
    else if (destPosition[0]>=iniPosition[0] && destPosition[1]<=iniPosition[1])
    {
        acceleration[1] = -acceleration[1];
    }

    /*
     * Control the uav's linear movement according to current distance and velocity
     */
    double currentD = getDistance(position,destPosition);
    updateCurrentV();
    if(currentV >= maxV && currentD > 20.) // uniform motion
    {
        currentV = maxV;
        Vector3d unitVelocity = getUnitDirection(velocity);
        a = 0;
        for (int i = 0; i < 3; ++i)
        {
            velocity[i] = maxV * unitVelocity[i];
            position[i] += velocity[i] * dt;
        }
    }
    else if(currentV< maxV && currentD > 20.) // accelerated motion
    {
        for (int i = 0; i < 3; ++i)
        {
            velocity[i] += acceleration[i] * dt;
            position[i] += velocity[i] * dt;
        }
    }
    else if(currentD <= 20. && currentV>=1e-2) // decelerated motion
    {
        a = -maxV*maxV*0.1*0.5;
        for (int i = 0; i < 3; ++i)
        {
            velocity[i] += acceleration[i] * dt;
            position[i] += velocity[i] * dt;
        }
    }
    else // has reach the sphere
    {
        a=0;
        velocity.setZero();
        for (int i = 0; i < 3; ++i)
        {
            mutexUav.lock();
            position[i] = destPosition[i] - 10.0 * unitLinearDirection[i];
            mutexUav.unlock();
        }
    }

}
/*
 * Check the uav arrives the (0,0,50) or not
 * */
bool ECE_UAV::checkReachSphere()
{
    double d = getDistance(position,destPosition);
    if(d < (10+1e-2))
    {
        velocity.setZero();
        return true;
    }
    else
        return false;
}
/*
 * Give each uav an initial and random speed
 * */
void ECE_UAV::initSphereMovement(const Vector4d unit)
{
    Vector3d F_uav, G;
    double f_uav = 20.;
    G = {0,0,-9.81};
    F_uav.setZero();
    for (int i = 0; i < 3; ++i)
    {
        F_uav[i] = unit[i] * f_uav;
    }
    // compute the Acc
    acceleration  = (F_uav+G) / mass;
    // update position and velocity
    for (int i = 0; i < 3; ++i)
    {
        mutexUav.lock();
        velocity[i] += acceleration[i] * dt;
        position[i] += velocity[i] * dt;
        mutexUav.unlock();
    }
    updateCurrentV();

}
/*
 * Check if the uav has obtained the initial speed around the sphere
 * @return true if it has reach the desired speed
 * */
bool ECE_UAV::checkInitSphere(const double initSpeed)
{
    updateCurrentV();
    if(abs(currentV-initSpeed)<0.2)
    {
        return true;
    }
    else
        return false;
}
/*
 * Update the uav's kinematics of movement around the sphere
 * */
void ECE_UAV::updateSphereMovement(const double initV)
{
    // Compute distance and direction
    double distance = getDistance(position,destPosition);
    Vector3d unitDirection_3D = getUnitDirection(destPosition,position);

    updateCurrentV();
    // compute Force value according to distance
    double f_hook = 50 * currentV * (10. - distance);

    // compute the Hooke Force vector
    Vector3d F_hook;
    F_hook.setZero();
    for (int i = 0; i < 3; ++i)
    {
        F_hook[i] = f_hook * unitDirection_3D[i];
    }
    // The F_uav will overcome G and provide F_hook
    Vector3d G_vector = {0.,0.,-mass*g};
    Vector3d F_uav = F_hook +(-G_vector);
    // check the F from UAV is less than 20N or not
    double f_uav = sqrt(F_uav[0]*F_uav[0]+F_uav[1]*F_uav[1]+F_uav[2]*F_uav[2]);
    if(f_uav > 20.)
    {
        // reset F_uav to 20N
        F_uav = getUnitDirection(F_uav);
        for (int i = 0; i < 3; ++i)
        {
            F_uav[i] = 20. * F_uav[i];
        }
    }
    // compute the Acc
    acceleration  = (F_uav+G_vector) / mass;
    // update position and velocity
    for (int i = 0; i < 3; ++i)
    {
        mutexUav.lock();
        velocity[i] += acceleration[i] * dt;
        position[i] += velocity[i] * dt;
        mutexUav.unlock();
    }
    updateCurrentV();
}

/*
 * Generate the random initial force unit direction and desirable speed value for each UAV
 * */
Vector4d ECE_UAV::genRandomUnit()
{
    double unit = 0.;
    Vector4d unit_vector;
    unsigned seed = hash<thread::id>()(this_thread::get_id());
    default_random_engine e(seed);
    uniform_real_distribution<double> u(-1.0, 1.0);
    uniform_real_distribution<double> s(2.0,5.0);
    unit_vector.setZero();
    Vector3d normalVector = getUnitDirection(iniPosition,destPosition);
    while(true)
    {
        unit = 0.;
        for (int i = 0; i < 3; ++i)
        {
            unit_vector[i] = u(e);
            unit += unit_vector[i] * unit_vector[i];
        }
        if(unit>0.)
        {
            unit = sqrt(unit);
            for (int i = 0; i < 3; ++i)
            {
                unit_vector[i] = unit_vector[i] / unit;
            }
        }
        if(abs((normalVector[0]*unit_vector[0] )+(normalVector[1]*unit_vector[1]) +(normalVector[2]*unit_vector[2]))<1e-3)
            break;
    }
    unit_vector[3] = s(e);
    return unit_vector;
}
/*
 * Check if the uav collides with other one
 * @return true if collision happens
 * */
bool ECE_UAV::checkCollision(Vector3d pos)
{
    double dis = getDistance(pos, position);
    if(dis<=0.2*2)
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*
 * Compute the Euclidean distance of two points in 3D space
 * @param start is one point coordinate
 * @param start is another point coordinate
 * @return the distance between two points
 * */
double ECE_UAV::getDistance(Vector3d start, Vector3d end)
{
    double distance = 0.;
    for (int i = 0; i < 3; ++i)
    {
        distance += ((end[i] - start[i]) * (end[i] - start[i]));
    }
    if(distance>0.)
    {
        distance = sqrt(distance);
    }
    return distance;

}
/*
 * Compute the unit of the vector of two points in 3D space
 * @param start is one point coordinate
 * @param start is another point coordinate
 * @return the unit vector
 * */
Vector3d ECE_UAV::getUnitDirection(Vector3d start, Vector3d end)
{
    double scalar = 0.;
    for (int i = 0; i < 3; ++i)
    {
        scalar += ((end[i]-start[i])*(end[i]-start[i]));
    }
    Vector3d unitVector;
    unitVector.setZero();
    if(scalar>0.)
    {
        scalar = sqrt(scalar);
        for (int i = 0; i < 3; ++i)
        {
            unitVector[i] = (end[i]-start[i]) / scalar;
        }
    }
    return unitVector;
}
/*
 * Compute the unit of a vector in 3D space
 * @param vec is the vector in 3D space
 * @return the unit vector
 * */
Vector3d ECE_UAV::getUnitDirection(Vector3d vec)
{
    double scalar = 0.;
    for (int i = 0; i < 3; ++i)
    {
        scalar += (vec[i]*vec[i]);
    }
    Vector3d unitVector;
    unitVector.setZero();
    if(scalar>0.)
    {
        scalar = sqrt(scalar);
        for (int i = 0; i < 3; ++i)
        {
            unitVector[i] = vec[i] / scalar;
        }
    }
    return unitVector;
}