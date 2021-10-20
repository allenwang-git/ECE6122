/*
Author: Yinuo Wang
Class: ECE 6122
Last Date Modified: 10/17/2021

Description:
 This file is the definition of class Motion,
 which is used to handle the movement of buzzy after the launch
*/
#ifndef LAB1_MOTION_H
#define LAB1_MOTION_H

#pragma once
#include <cmath>
/*
 * This class is used to handle the buzzy's movement
 * */
class Motion
{
public:
/*
 * Set the buzzy's initial state
 * @param x0 is the initial x position
 * @param y0 is the initial y position
 * @param t0 is the initial time
 * @param theta0 is the initial absolute angular
 *
 * */
    void initialize(float x0, float y0, float v0, float theta0)
    {
        this->x0 = x0;
        this->y0 = y0;
        this->v0 = v0;
        this->theta0 = theta0;
        setSubVelocity();
    }

//  compute current sub-velocity
    void computeSubVelocity()
    {
        vx = vx0;
        vy = vy0 + a * t;
    }

/*  compute current velocity direction
 * @return theta is the current velocity' direction
 */
    double getVelocityDirection()
    {
        computeSubVelocity();
        theta = atan(vy / vx) / 3.14 * 180.;
        if (theta < 0) theta += 360.;
        return theta;
    }

/*  compute current Y position
 *  @return current y's coordinate
 */
    float getYPosition()
    {
        return y = y0 + vy0 * t + 0.5 * a * t * t;
    }

/*  compute current X position
 *  @return current x's coordinate
 */
    float getXPosition()
    {
        return x = x0 + vx0 * t;
    }

//  debug show function
    void show()
    {
        cout << "x0" << x0 << " y0" << y0 << " vx0" << vx0 << " vy0" << vy0 << " theta0:" << theta0;
        cout << " x" << x << " y" << y << " vx" << vx << " vy" << vy << " theta:" << theta << endl;
    }

//  reset current time position and velocity
    void reset()
    {
        v = v0;
        x = x0;
        y = y0;
        theta = theta0;
        t = 0.f;
    }
    float t = 0.f;
private:
    const float a = 9.80f * 10.;
    float x, y, v0, vx0, vy0, v, vx, vy, x0, y0;
    double theta0, theta;

//  compute the initial sub-velocity
    void setSubVelocity() {
        vx0 = v0 * cos(theta0 / 180 * 3.14);
        vy0 = v0 * sin(theta0 / 180 * 3.14);
    }
};

#endif //LAB1_MOTION_H
