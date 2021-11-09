//
// Created by allen on 11/6/21.
//

#include "../include/ECE_UAV.h"
#include <iostream>
using namespace std;

void threadFunction(ECE_UAV* pUAV)
{
    for (int i=0; i<1;i++)
    {
        Vector3f pos = pUAV->getPosition();
        cout<< pos[0]<<pos[1]<<endl;
//        cout<<endl;
    }



}
ECE_UAV::ECE_UAV(const float px, const float py)
{
    position.setZero();
    position[0] = px;
    position[1] = py;
    position[2] = 0.6f;
    velocity.setZero();
    acceleration.setZero();
    acceleration[2] = g;
    this->start();
//    cout<<acceleration[2]<<endl;
}
Vector3f ECE_UAV::getPosition()
{
    return position;
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