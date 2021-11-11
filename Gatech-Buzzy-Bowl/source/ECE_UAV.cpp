//
// Created by allen on 11/6/21.
//

#include "../include/ECE_UAV.h"
#include <iostream>
#include <random>

using namespace std;
using namespace chrono;

void threadFunction(ECE_UAV* pUAV)
{
    // Do some initializing computing
    float acc, theta, alpha, maxV;
    pUAV->initLinearMovement(&maxV, &acc, &theta, &alpha);

    while(!pUAV->getEndFlag())
    {
        if(pUAV->getWaitFlag())
        {
            // wait 5s to launch
            if(duration_cast<duration<float>>(high_resolution_clock::now()-pUAV->startTimestamp).count()>=5.0)
            {
                pUAV->setWaitFlag();
                cout<<"Ready to launch!"<<endl;
            }
        }
        else if(!pUAV->getArriveFlag())
        {
            pUAV->updateLinearMovement(maxV,acc, theta, alpha);
//            cout<<"vel:"<<pUAV->getVelocity()<<endl<<"ACC: "<<pUAV->getAcceleration()<<endl;
            if(pUAV->checkArrive())
                pUAV->setArriveFlag();
        }
        else if (!pUAV->getWaitFlag() && pUAV->getArriveFlag())
        {
//            pUAV->initSphereMovement();
            pUAV->updateSphereMovement();
        }
        // set the update rate
        this_thread::sleep_for(chrono::milliseconds(10));
    }


}
ECE_UAV::ECE_UAV(const float px, const float py)
{
    iniPosition.setZero();
    iniPosition[0] = px;
    iniPosition[1] = py;
    iniPosition[2] = 0.6f;
    position = iniPosition;
    velocity.setZero();
    acceleration.setZero();
    acceleration[2] = -g;
//    this->start();
//    cout<<chrono::system_clock::to_time_t(this->startTimestamp)<<endl;
}
Vector3f* ECE_UAV::getPosition()
{
    return &position;
}

void ECE_UAV::setEndFlag()
{
    this->endFlag = true;
}

bool ECE_UAV::getEndFlag()
{
    return this->endFlag;
}
void ECE_UAV::setWaitFlag()
{
    this->waitFlag = false;
}
bool ECE_UAV::getWaitFlag()
{
    return this->waitFlag;
}
void ECE_UAV::setArriveFlag()
{
    this->arriveFlag = true;
}
bool ECE_UAV::getArriveFlag()
{
    return this->arriveFlag;
}
void ECE_UAV::start()
{
    thread t(threadFunction,this);
    thuav = move(t);

}

void ECE_UAV::join()
{
    if(thuav.joinable())
        thuav.join();
}

void ECE_UAV::initLinearMovement(float *maxV, float *acc, float *theta, float *alpha)
{
    unsigned seed = hash<thread::id>()(this_thread::get_id());
    default_random_engine e(seed);
    uniform_real_distribution<float> u(1.0, 2.0);
    *maxV = u(e);
    cout<<*maxV<<endl;

    float F = 20.0;
    if(iniPosition[0]==0.f && iniPosition[1] ==0.f)
    {
        *acc = F/mass + (-g);
        *theta = PI/2;
        *alpha = PI/2;
    }
    else
    {
        float xyDistance = sqrt(pow(iniPosition[0],2)+ pow(iniPosition[1],2));
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
        float beta = acos(mass *g /F);
        *acc = F * sin(beta)/(mass * cos(*theta));

    }
}
void ECE_UAV::updateCurrentV()
{
    currentV = sqrt(velocity[0]*velocity[0] +  velocity[1]*velocity[1] + velocity[2]*velocity[2]);
}
void ECE_UAV::updateLinearMovement(const float &maxV, float &a, const float &theta, const float &alpha)
{
//    computeAcc(a,theta,alpha);
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

    float currentT = duration_cast<duration<float>>(high_resolution_clock::now()-this->startTimestamp).count() - 5.f;

    for (int i = 0; i < 3; ++i)
    {
        velocity[i] += acceleration[i] * dt;
//        currentV += pow(velocity[i],2);
    }
//    currentV = sqrt(currentV);
    updateCurrentV();
    if(currentV > maxV)
    {
        currentV = maxV;
        a = 0;
        for (int i = 0; i < 3; ++i) {
            position[i] += velocity[i] * dt;
        }
    }
    else
    {
        for (int i = 0; i < 3; ++i)
        {
//            cout<<"ct"<<currentT<<endl;
            position[i] = iniPosition[i] + 0.5f * acceleration[i] * currentT * currentT;
        }
    }
//cout<<currentT<<endl;
}
/*
 *
 * */
bool ECE_UAV::checkArrive()
{
    if(abs(position[0]-destPosition[0])<0.02 && abs(position[1]-destPosition[1])<0.02 && abs(position[2]-destPosition[2])<0.02)
        return true;
    else
        return false;
}

void ECE_UAV::updateSphereMovement()
{
    // Compute distance and direction
    float distance, f;
    Vector3f direction_3D, unitDirection_3D;
    for (int i = 0; i < 3; ++i)
    {
        distance += pow((position[i] - destPosition[i]),2);
        direction_3D[i] = position[i] - destPosition[i];
    }
    distance = sqrt(distance);
    cout<<"distance "<<distance<<endl;
    // if uav at the (0,0,50) then generate a random unit direction for F
    if (abs(direction_3D[0])<1e-6 && abs(direction_3D[1]) <1e-6 && abs(direction_3D[2])<1e-6)
    {
        float unit=0.f;
        default_random_engine e;
        uniform_real_distribution<float> u(-1.0, 1.0);
        for (int i = 0; i < 3; ++i) {
            direction_3D[i] = u(e);
            unit += direction_3D[i] * direction_3D[i];
        }

        unit = sqrt(unit);
        for (int i = 0; i < 3 ; ++i) {
            unitDirection_3D[i] = unitDirection_3D[i] / unit;
        }
    }
    else
    {
        // compute unit vector of direction
        for (int i = 0; i < 3; ++i) {
//            float tmp = sqrt(direction_3D[0]*direction_3D[0] + direction_3D[1]*direction_3D[1] + direction_3D[2]*direction_3D[2]);
            unitDirection_3D[i] = direction_3D[i] / distance;
        }
    }
//    cout<<"unitdirection"<<unitDirection_3D[0]<<" "<<unitDirection_3D[1]<<" "<<unitDirection_3D[2]<<endl;

    updateCurrentV();
    // compute F according to distance
    f = K * (10.f - distance) - currentV;// + 1*(2-currentV) + 1*(10-currentV);
    cout << f<<" "<<currentV<<" ";
    //
    Vector3f F_vector;
    for (int i = 0; i < 3; ++i) {
        F_vector[i] = f * unitDirection_3D[i];

    }
    // check the F from UAV is less than 20N or not
    Vector3f G_vector = {0.,0.,-mass*g};
    Vector3f F_uav = F_vector - G_vector;
    float f_uav = sqrt(F_uav[0]*F_uav[0]+F_uav[1]*F_uav[1]+F_uav[2]*F_uav[2]);
    if(f_uav > 20.f)
    {
        // reset F_uav to 20N
        for (int i = 0; i < 3; ++i)
        {
            F_uav[i] = F_uav[i] / f_uav * 20.f;
        }
        f_uav = 20.f;
        F_vector = G_vector + F_uav;
    }
    // compute the Acc
    acceleration  = F_vector / mass;


    for (int i = 0; i < 3; ++i)
    {cout<<F_vector[i]<<" ";
        velocity[i] += acceleration[i] * dt;
        position[i] += velocity[i] * dt;
    }
    cout<<endl;

}

void ECE_UAV::initSphereMovement()
{
    // Compute distance and direction
    float distance, F;
    Vector3f direction_3D;
    for (int i = 0; i < 3; ++i)
    {
        distance += pow((position[i] - destPosition[i]),2);
        direction_3D[i] = destPosition[i] - position[i];
    }
    distance = sqrt(distance);

    updateCurrentV();

    // compute F according to distance
    F = K * (10.f - distance);// + 2*(2-currentV) + 2*(10-currentV);
    if (F > 20.f)
        F = 20.f;
    else if(F < -20.f)
        F = -20.f;
    // compute angles for F decomposition
    Vector3f angle;


    angle[0] = atan(direction_3D[1] / direction_3D[0]);
    angle[1] = atan(direction_3D[0] / direction_3D[1]);
    angle[2] = atan(sqrt(pow(direction_3D[0],2)+pow(direction_3D[1],2))/direction_3D[2]);
    if (abs(direction_3D[2])<1e-3){
        angle[2] = 0;
    }
    // compute F_3D and Acc
    Vector3f F_3D;
    F_3D[2] = F * cos(angle[2]) - mass * g;
    F_3D[0] = F * sin(angle[2]) * cos(angle[0]);
    F_3D[1] = F * sin(angle[2]) * cos(angle[1]);
    cout<<currentV<<" "<<distance<<" "<<F_3D[0]<<" "<<F_3D[1]<<" "<<F_3D[2]<<endl;
    if (abs(F_3D[0])<1e-3 && abs(F_3D[1]) <1e-3 && abs(F_3D[2])<1e-3)
    {
        float unit;
        default_random_engine e;
        uniform_real_distribution<float> u(-1.0, 1.0);
        for (int i = 0; i < 3; ++i) {
            F_3D[i] = u(e);
            unit += F_3D[i] * F_3D[i];
        }

        unit = sqrt(unit);
        for (int i = 0; i < 3 ; ++i) {
            F_3D[i] = 10.f * F_3D[i] / unit;
        }
    }
    acceleration  = F_3D / mass;


}