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
        setSubVelocity();
    }

    void computeSubVelocity() {
        vx = vx0;
        vy = vy0 + a * t;
    }

    double getVelocityDirection() {
        computeSubVelocity();
        theta = atan(vy / vx) / 3.14 * 180.;
        if (theta < 0) theta += 360.;
        return theta;
    }

    float getYPosition() {
        return y = y0 + vy0 * t + 0.5 * a * t * t;
    }

    float getXPosition() {
        return x = x0 + vx0 * t;
    }

    void show() {
        cout << "x0" << x0 << " y0" << y0 << " vx0" << vx0 << " vy0" << vy0 << " theta0:" << theta0;
        cout << " x" << x << " y" << y << " vx" << vx << " vy" << vy << " theta:" << theta << endl;
    }

    float t = 0.f;
private:
    const float a = 9.80f * 10.;
    float x, y, v0, vx0, vy0, v, vx, vy, x0, y0;
    double theta0, theta;

    void setSubVelocity() {
        vx0 = v0 * cos(theta0 / 180 * 3.14);
        vy0 = v0 * sin(theta0 / 180 * 3.14);
    }

};


#endif //LAB1_MOTION_H
