/*
Author: Yinuo Wang
Class: ECE 6122
Last Date Modified: 10/18/2021

Description:
 This is the derived class of SpriteWrapper class, which is used to
 handle the buzzy's special functionality.
*/

#ifndef LAB1_BUZZY_H
#define LAB1_BUZZY_H

#pragma once
#include "SpriteWrapper.h"
#define UPPER_BOUND 270.F
#define LOWER_BOUND 45.F
/*
 * This is the derived class of SpriteWrapper for buzzy object
 * */
class Buzzy : public SpriteWrapper
{
public:
//    Constructor
    Buzzy(const string loadPath, const float px, const float py,
          const float scale_x, const float scale_y)
    {
        texture.loadFromFile(loadPath);
        sprite.setTexture(texture);
        sprite.setPosition(px, py);
        sprite.setScale(scale_x, scale_y);
        this->px0 = px;
        this->py0 = py;
    }

/*
 * Set buzzy's initial direction up
 */
    void headUp()
    {
        if ((this->getRotation() <= 360.f && this->getRotation() > UPPER_BOUND)
            || (this->getRotation() >= 0.f && this->getRotation() <= LOWER_BOUND))
            this->rotate(-5.f);
        else
            this->setRotation(round(this->getRotation()) + 5.f);
    }

/*
 * Set buzzy's initial direction down
 */
    void headDown()
    {
        if ((this->getRotation() <= 360.f && this->getRotation() >= UPPER_BOUND)
            || (this->getRotation() >= 0.f && this->getRotation() < LOWER_BOUND))
            this->rotate(5.f);
        else
            this->setRotation(round(this->getRotation()) - 5.f);
    }

/*  check if buzzy has intersected with other objects
 *
 * @param v2f is the other object's position
 * @return true if intersect or false if not
 * */
    bool intersect(Vector2f v2f)
    {
        if (abs(this->getPosition().x - v2f.x) < 70 &&
            abs(this->getPosition().y - v2f.y) < 70)
            return true;
        else
            return false;
    }
/*  check if buzzy has intersected with small objects
 *
 * @param v2f is the other object's position
 * @return true if intersects or false if not
 * */
    bool intersectSmall(Vector2f v2f)
    {
        if (abs(this->getPosition().x - v2f.x) < 45 &&
            abs(this->getPosition().y - v2f.y) < 45)
            return true;
        else
            return false;
    }
    /*
     * This function is used to check if buzzy has hit a woodland creature or not
     *
     * @param woodlanCreatures is the vector contains some SpriteWrapper objects
     * @return true if hit anyone in the vector or false if not
     * */
    bool hitWoodlandCreature(const vector<SpriteWrapper *> woodlandCreatures)
    {
        for (int i = 0; i < woodlandCreatures.size(); ++i)
        {
            if (abs(this->getPosition().x - (*woodlandCreatures[i]).getPosition().x) < 70
                && abs(this->getPosition().y - (*woodlandCreatures[i]).getPosition().y) < 70)
                return true;
        }
        return false;
    }
};

#endif //LAB1_BUZZY_H
