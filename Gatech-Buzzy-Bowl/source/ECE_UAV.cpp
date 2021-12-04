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
    double acc, theta, alpha, maxV;
    Vector4d unitScale;
    pUAV->initLinearMovement(&maxV, &acc, &theta, &alpha, &unitScale);
//    pUAV->initSphereMovement();

    while(!pUAV->getEndFlag())
    {
        if(pUAV->getWaitFlag())
        {
            // wait 5s to launch
            if(duration_cast<duration<double>>(high_resolution_clock::now()-pUAV->startTimestamp).count() >= WAIT_TIME)
            {
                pUAV->setWaitFlag();
                cout<<"Ready to launch!"<<endl;
            }
        }
        else if(!pUAV->getArriveFlag())
        {

            pUAV->updateLinearMovement(maxV,acc, theta, alpha);
//            cout<<"vel:"<<pUAV->getVelocity()<<endl<<"ACC: "<<pUAV->getAcceleration()<<endl;
            if(pUAV->checkReachSphere())
                pUAV->setArriveFlag();
        }
        else if (!pUAV->getInitSphereFlag())
        {
            pUAV->initSphereMovement(unitScale);
            if(pUAV->checkInitSphere(unitScale[3]))
                pUAV->setInitSphereFlag();

        } else if (!pUAV->getEndFlag()){

            pUAV->updateSphereMovement(unitScale[3]);
        }
        // set the update rate
        this_thread::sleep_for(chrono::milliseconds(10));
    }


}
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
//    this->start();
//    cout<<chrono::system_clock::to_time_t(this->startTimestamp)<<endl;
}
Vector3d* ECE_UAV::getPosition()
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
void ECE_UAV::setInitSphereFlag()
{
    this->initSphereFlag = true;
}
bool ECE_UAV::getInitSphereFlag()
{
    return this->initSphereFlag;
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

void ECE_UAV::initLinearMovement(double *maxV, double *acc, double *theta, double *alpha, Vector4d *unitScale)
{
    unsigned seed = hash<thread::id>()(this_thread::get_id());
    default_random_engine e(seed);
    uniform_real_distribution<double> u(1.9, 2.0);
    *maxV = u(e);
//    cout<<*maxV<<endl;
    position = iniPosition;
    double F = 20.0;
    if(iniPosition[0]==0.f && iniPosition[1] ==0.f)
    {
        *acc = F/mass + (-g);
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
        double beta = acos(mass *g /F);
        *acc = F * sin(beta)/(mass * cos(*theta));
        // compute the linear direction
        unitLinearDirection = getUnitDirection(iniPosition, destPosition);
//        for (int i = 0; i < 3; ++i) {
//            cout<<"v"<<v0[i] <<" ";
//        }cout<<endl;
    }
    *unitScale = genRandomUnit();
}
/*
 *  compute velocity scalar
 * */
void ECE_UAV::updateCurrentV()
{
    currentV = sqrt(velocity[0]*velocity[0] +  velocity[1]*velocity[1] + velocity[2]*velocity[2]);
//    Vector3d unitdirection = getUnitDirection(velocity);
//    for (int i = 0; i < 3; ++i) {
//        cout<<unitdirection[i]<<" ";
//    }cout<<endl;

}
void ECE_UAV::updateLinearMovement(const double &maxV, double &a, const double &theta, const double &alpha)
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

//    double currentT = duration_cast<duration<double>>(high_resolution_clock::now()-this->startTimestamp).count() - WAIT_TIME;


    double currentD = getDistance(position,destPosition);
    updateCurrentV();
    if(currentV >= maxV && currentD > (20.+ 5e-2))
    {
        currentV = maxV;
        Vector3d unitVelocity = getUnitDirection(velocity);
        a = 0;
        for (int i = 0; i < 3; ++i) {
            velocity[i] = maxV * unitVelocity[i];
            position[i] += velocity[i] * dt;
        }
//        cout<<2<<endl;
    }
    else if(currentV< maxV && currentD > (20.+ 5e-2))
    {
        for (int i = 0; i < 3; ++i)
        {
            velocity[i] += acceleration[i] * dt;
            position[i] += velocity[i] * dt;//iniPosition[i] + 0.5f * acceleration[i] * currentT * currentT;
        }
//        cout<<"v"<<getUnitDirection(velocity)<<endl;
//        cout<<"p"<<getUnitDirection(iniPosition,destPosition)<<endl;
//        cout<<a<<" "<<position[0]<<" "<<position[1]<<" "<<position[2]<<" "<<velocity[0]<<" "<<velocity[1]<<" "<<velocity[2]<<endl;
//        cout<<1<<endl;
    }
    else if(currentD <= (20.+ 5e-2) && currentV>=1e-2)
    {
        a = -maxV*maxV*0.1*0.5;
        for (int i = 0; i < 3; ++i)
        {
            velocity[i] += acceleration[i] * dt;
            position[i] += velocity[i] * dt;
        }
//        cout<<a<<" "<<position[0]<<" "<<position[1]<<" "<<position[2]<<" "<<velocity[0]<<" "<<velocity[1]<<" "<<velocity[2]<<endl;
//        cout<<3<<"v"<<currentV<<"a"<<a<<"d"<<currentD<<endl;
    }
    else{
        a=0;
        velocity.setZero();
        for (int i = 0; i < 3; ++i) {
            mutexUav.lock();
            position[i] = destPosition[i] - 10.0 * unitLinearDirection[i];
            mutexUav.unlock();
        }
//        currentD = getDistance(position,destPosition);
//        cout<<4<<"v"<<currentV<<"a"<<a<<"d"<<currentD<<endl;
    }

}
/*
 * Check the uav arrives the (0,0,50) or not
 * */
bool ECE_UAV::checkReachSphere()
{
    double d = getDistance(position,destPosition);
    if(d < (10+1e-2)){
        cout<<"arrive"<<endl;
        velocity.setZero();
        return true;
    }
    else
        return false;
}
bool ECE_UAV::checkInitSphere(const double initSpeed){

    updateCurrentV();
    if(abs(currentV-initSpeed)<0.2){

        return true;
    }
    else
        return false;
}
void ECE_UAV::updateSphereMovement(const double initV)
{
    // Compute distance and direction
    double distance = getDistance(position,destPosition);
    Vector3d unitDirection_3D = getUnitDirection(destPosition,position);



//    for (int i = 0; i < 3; ++i)
//    {
//        distance += pow((position[i] - destPosition[i]),2);
//        direction_3D[i] = position[i] - destPosition[i];
//    }
//    distance = sqrt(distance);
//    cout<<"distance "<<distance<<endl;

    // if uav at the (0,0,50) then generate a random unit direction for F
//    if (abs(direction_3D[0])<5e-6 && abs(direction_3D[1]) <5e-6 && abs(direction_3D[2])<5e-6)
//    {
//        double unit=0.f;
//        default_random_engine e;
//        uniform_real_distribution<double> u(-1.0, 1.0);
//        for (int i = 0; i < 3; ++i) {
//            direction_3D[i] = u(e);
//            unit += direction_3D[i] * direction_3D[i];
//        }
//
//        unit = sqrt(unit);
//        for (int i = 0; i < 3 ; ++i) {
//            unitDirection_3D[i] = unitDirection_3D[i] / unit;
//        }
//    }
//    else
//    {
//        // compute unit vector of direction
//        for (int i = 0; i < 3; ++i) {
////            double tmp = sqrt(direction_3D[0]*direction_3D[0] + direction_3D[1]*direction_3D[1] + direction_3D[2]*direction_3D[2]);
//            unitDirection_3D[i] = direction_3D[i] / distance;
//        }
//    }
//    cout<<"unitdirection"<<unitDirection_3D[0]<<" "<<unitDirection_3D[1]<<" "<<unitDirection_3D[2]<<endl;

    updateCurrentV();
    // compute F according to distance
    double f_hook = initV * initV * initV * (10.f - distance) + initV * initV *(initV- currentV);
//    cout << f<<" "<<currentV<<" ";

    Vector3d F_hook;
    F_hook.setZero();
    for (int i = 0; i < 3; ++i) {
//        if(firstSphereFlag)
//        {
//            F_hook[i] = f_hook * unitLinearDirection[i];//unitDirection_3D[i];
//            firstSphereFlag = false;
//        }
//        else{
            F_hook[i] = f_hook * unitDirection_3D[i];
//        }

    }
    /*
     * The F_uav will overcome G and provide F_hook
     * */
    Vector3d G_vector = {0.,0.,-mass*g};
    Vector3d F_uav = F_hook +(-G_vector);
    // check the F from UAV is less than 20N or not
    double f_uav = sqrt(F_uav[0]*F_uav[0]+F_uav[1]*F_uav[1]+F_uav[2]*F_uav[2]);
    if(f_uav > 20.f)
    {
        // reset F_uav to 20N
//        for (int i = 0; i < 3; ++i)
//        {
//            F_uav[i] = F_uav[i] / f_uav * 20.f;
//        }
//        f_uav = 20.f;
//        F_vector = G_vector + F_uav;
    }
    // compute the Acc
    acceleration  = (F_uav+G_vector) / mass;


    for (int i = 0; i < 3; ++i)
    {
//        cout<<F_uav[i]<<" ";
        mutexUav.lock();
        velocity[i] += acceleration[i] * dt;
        position[i] += velocity[i] * dt;
        mutexUav.unlock();
    }
    updateCurrentV();
//    cout<<"v"<<currentV<<endl;

}

void ECE_UAV::initSphereMovement(const Vector4d unit)
{
/*    // Compute distance and direction
    double distance, F;
    Vector3d direction_3D;
    for (int i = 0; i < 3; ++i)
    {
        distance += pow((position[i] - destPosition[i]),2);
        direction_3D[i] = destPosition[i] - position[i];
    }
    // the oushi distance between uav and sphere origin
    distance = sqrt(distance);

    updateCurrentV();

    // compute F according to distance
    F = K * (10.f - distance);// + 2*(2-currentV) + 2*(10-currentV);
    if (F > 20.f)
        F = 20.f;
    else if(F < -20.f)
        F = -20.f;
    // compute angles for F decomposition
    Vector3d angle;


    angle[0] = atan(direction_3D[1] / direction_3D[0]);
    angle[1] = atan(direction_3D[0] / direction_3D[1]);
    angle[2] = atan(sqrt(pow(direction_3D[0],2)+pow(direction_3D[1],2))/direction_3D[2]);
    if (abs(direction_3D[2])<5e-3){
        angle[2] = 0;
    }
    // compute F_3D and Acc
    Vector3d F_3D;
    F_3D[2] = F * cos(angle[2]) - mass * g;
    F_3D[0] = F * sin(angle[2]) * cos(angle[0]);
    F_3D[1] = F * sin(angle[2]) * cos(angle[1]);
    cout<<currentV<<" "<<distance<<" "<<F_3D[0]<<" "<<F_3D[1]<<" "<<F_3D[2]<<endl;
    if (abs(F_3D[0])<5e-3 && abs(F_3D[1]) <5e-3 && abs(F_3D[2])<5e-3)
    {
        double unit;
        default_random_engine e;
        uniform_real_distribution<double> u(-1.0, 1.0);
        for (int i = 0; i < 3; ++i) {
            F_3D[i] = u(e);
            unit += F_3D[i] * F_3D[i];
        }

        unit = sqrt(unit);
        for (int i = 0; i < 3 ; ++i) {
            F_3D[i] = 10.f * F_3D[i] / unit;
        }
    }
    acceleration  = F_3D / mass;*/
    Vector3d F_uav, G;
    double f_uav = 20.;
    G = {0,0,-9.81};
    F_uav.setZero();
    for (int i = 0; i < 3; ++i) {
        F_uav[i] = unit[i] * f_uav;
    }
    // compute the Acc
    acceleration  = (F_uav+G) / mass;


    for (int i = 0; i < 3; ++i)
    {
//        cout<<F_uav[i]<<" ";
        mutexUav.lock();
        velocity[i] += acceleration[i] * dt;
        position[i] += velocity[i] * dt;
        mutexUav.unlock();
    }
    updateCurrentV();
//    cout<<currentV<<" "<<unit[3]<<endl;

}

Vector4d ECE_UAV::genRandomUnit() {
    double unit = 0.;
    Vector4d unit_vector;
    unsigned seed = hash<thread::id>()(this_thread::get_id());
    default_random_engine e(seed);
    uniform_real_distribution<double> u(-1.0, 1.0);
    uniform_real_distribution<double> s(2.0,5.0);
    unit_vector.setZero();
    for (int i = 0; i < 3; ++i) {
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
    unit_vector[3] = s(e);
    cout<<unit_vector[3]<<endl;
    return unit_vector;
}

bool ECE_UAV::checkCollision(Vector3d pos){

    double dis = getDistance(pos, position);
    if(dis<=0.2*2)
    {
        return true;
    }else{
        return false;
    }

}