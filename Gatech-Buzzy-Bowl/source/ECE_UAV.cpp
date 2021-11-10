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
    float acc, theta, alpha;
    pUAV->computeAcc(&acc, &theta, &alpha);

    while(!pUAV->getEndFlag())
    {
        if(pUAV->getWaitFlag())
        {
            // wait 5s to launch
            if(duration_cast<duration<float>>(high_resolution_clock::now()-pUAV->startTimestamp).count()>=5.0)
                pUAV->setWaitFlag();
        }
        else if(!pUAV->getArriveFlag())
        {
//            cout<<"ready to launch!"<<endl;
            pUAV->updateMovement(acc, theta, alpha);
//            cout<<"vel:"<<pUAV->getVelocity()<<endl<<"ACC: "<<pUAV->getAcceleration()<<endl;
            if(pUAV->checkArrive())
                pUAV->setArriveFlag();
        }
        else if (!pUAV->getWaitFlag() && pUAV->getArriveFlag())
        {

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

void ECE_UAV::computeAcc(float *acc, float *theta, float *alpha)
{
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
void ECE_UAV::updateMovement(float &a, float theta, float alpha)
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
    float currentV = 0;
    for (int i = 0; i < 3; ++i) {
        velocity[i] += acceleration[i] * dt;
        currentV += pow(velocity[i],2);
    }
    currentV = sqrt(currentV);
    if(currentV>2.)
    {
        currentV = 2.;
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