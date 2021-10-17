//
// Created by allen on 10/17/21.
//

#ifndef LAB1_MOTION_H
#define LAB1_MOTION_H

#pragma once

#include <cmath>

class Motion {
public:

    void initialize(float x0, float y0, float v0, float theta0) {
        this->x0 = x0;
        this->y0 = y0;
        this->v0 = v0;
        this->theta0 = theta0;
    }

    void getSubVelocity() {
        vx0 = v0 * cos(theta0);
        vy0 = v0 * sin(theta0);
    }

    double getVelocityDirection() {
        vx = vx0;
        vy = vy0 + a * t;
        theta = atan(vx / vy) - 45.f;
        return theta;
    }

    void getYPosition() {
        y = y0 + vy0 * t + 0.5 * a * t * t;
    }

    void getXPosition() {
        x = x0 + vx0 * t;
    }
    float t = 0.f;
private:
    const float a = -9.80f;
    float x, y, v0, vx0, vy0, v, vx, vy, x0, y0;
    double theta0, theta;


};


#endif //LAB1_MOTION_H
