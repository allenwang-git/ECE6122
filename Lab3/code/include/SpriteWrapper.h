//
// Created by ywang3781 on 10/16/2021.
//

#ifndef LAB3_SPRITEWRAPPER_H
#define LAB3_SPRITEWRAPPER_H
#pragma once

#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>
#include <string>

using namespace sf;
using namespace std;

class SpriteWrapper : public Transformable {
public:
    SpriteWrapper(const string &loadPath,
                  const float px, const float py, const float scale_x, const float scale_y) {
        texture.loadFromFile(loadPath);
        sprite.setTexture(texture);
        sprite.setPosition(px, py);
        sprite.setScale(scale_x, scale_y);
        this->px0 = px;
        this->py0 = py;
    }

    void setRotation(float angle) {
        sprite.setRotation(angle);
    }

    void rotate(float angle) {
        sprite.rotate(angle);
    }

    float getRotation() {
        return sprite.getRotation();
    }

    void setPosition(const float x, const float y) {
        sprite.setPosition(x, y);
    }

    void draw(RenderWindow &window) {
        window.draw(sprite);
    }

    float px0, py0;
    bool movingFlag = false;
    float movingSpeed = 0.0f;
private:
    Texture texture;
    Sprite sprite;
};

/*
// Is the buzzy currently moving?
bool buzzyActive = false;
bool unicornActive = false;
bool insectActive = false;

// How fast can the buzzy fly
float buzzySpeed = 0.0f;
float unicornSpeed = 0.0f;
float insectSpeed = 0.0f;
*/

#endif //LAB3_SPRITEWRAPPER_H
