/*
Author: Yinuo Wang
Class: ECE 6122
Last Date Modified: 10/16/2021

Description:
 This is the wrapper class of Sprite and Texture class.
 Overload some functions of above wrapped classes.
*/
#ifndef LAB3_SPRITEWRAPPER_H
#define LAB3_SPRITEWRAPPER_H
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

using namespace sf;
using namespace std;

/*
 * This is the SpriteWrapper class
 * */
class SpriteWrapper {
public:
    SpriteWrapper() {};

    SpriteWrapper(const string loadPath, const float px, const float py,
                  const float scale_x, const float scale_y) {
        texture.loadFromFile(loadPath);
        sprite.setTexture(texture);
        sprite.setPosition(px, py);
        sprite.setScale(scale_x, scale_y);
        sprite.setOrigin(50, 50);
        this->px0 = px;
        this->py0 = py;
    }

//  rotation transformation
    void setRotation(const float angle) {
        sprite.setRotation(angle);
    }

    void rotate(const float angle) {
        sprite.rotate(angle);
    }

    float getRotation() {
        return sprite.getRotation();
    }

//    position transformation
    void setPosition(const float x, const float y) {
        sprite.setPosition(x, y);
    }

    void setPosition(const Vector2f &pos) {
        sprite.setPosition(pos);
    }

    Vector2f getPosition() {
        return sprite.getPosition();
    }

//  set object center for transformation
    void setOrigin(const float x, const float y) {
        sprite.setOrigin(x, y);
    }

//    set sprite scale
    void setScale(const float x, const float y) {
        sprite.setScale(x, y);
    }

//  draw the sprite
    void draw(RenderWindow &window) {
        window.draw(sprite);
    }

//  reset the sprite with origin position and angular
    void reset(const float x, const float y, const float agl) {
        sprite.setPosition(x, y);
        sprite.setRotation(agl);
    }

    int posTag;
    float px0, py0;
    bool movingFlag = false;
    bool spinningFlag = false;
    float movingSpeed = 0.0f;
    bool hitFlag = false;
protected:
    Texture texture;
    Sprite sprite;
};

#endif //LAB3_SPRITEWRAPPER_H
