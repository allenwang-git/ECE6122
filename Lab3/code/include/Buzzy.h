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

class Buzzy : public SpriteWrapper {
public:
    Buzzy(const string loadPath, const float px, const float py,
          const float scale_x, const float scale_y) {
        texture.loadFromFile(loadPath);
        sprite.setTexture(texture);
        sprite.setPosition(px, py);
        sprite.setScale(scale_x, scale_y);
        this->px0 = px;
        this->py0 = py;
    }

/*  check if buzzy has intersected with other objects
 *
 * @param v2f is the other object's position
 * @return true if intersects or false if not
 * */
    bool intersects(Vector2f v2f) {
        if (abs(this->getPosition().x - v2f.x) < 75 &&
            abs(this->getPosition().y - v2f.y) < 75)
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
    bool hitWoodlandCreature(const vector<SpriteWrapper *> woodlandCreatures) {
        for (int i = 0; i < woodlandCreatures.size(); ++i) {
            if (abs(this->getPosition().x - (*woodlandCreatures[i]).getPosition().x) < 75
                && abs(this->getPosition().y - (*woodlandCreatures[i]).getPosition().y) < 75) {
//                cout<<this->getPosition().x<<" "<<this->getPosition().y<< " "<<(*woodlandCreatures[i]).getPosition().x<<" "<<(*woodlandCreatures[i]).getPosition().y<<endl;
                return true;
            }
        }
        return false;
    }

};

#endif //LAB1_BUZZY_H
