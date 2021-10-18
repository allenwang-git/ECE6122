//
// Created by ywang3781 on 10/16/2021.
//

#ifndef LAB3_SPRITEWRAPPER_H
#define LAB3_SPRITEWRAPPER_H
#pragma once
#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;
using namespace std;

class SpriteWrapper {
public:
    SpriteWrapper() {};

    SpriteWrapper(const string loadPath,
                  const float px, const float py, const float scale_x, const float scale_y) {
        texture.loadFromFile(loadPath);
        sprite.setTexture(texture);
        sprite.setPosition(px, py);
        sprite.setScale(scale_x, scale_y);
        this->px0 = px;
        this->py0 = py;
    }

//    SpriteWrapper(Texture text,float px, float py, float scale_x, float scale_y)
//    {
//        sprite.setTexture(text);
//        sprite.setPosition(px, py);
//        sprite.setScale(scale_x, scale_y);
//
//    }
    void setRotation(const float angle) {
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

    Vector2f getPosition() {
        return sprite.getPosition();
    }

//  set object center for transformation
    void setOrigin(const float x, const float y) {
        sprite.setOrigin(x, y);
    }

    void draw(RenderWindow &window) {
        window.draw(sprite);
    }

    bool intersects(Vector2f v2f) {
        if (abs(this->getPosition().x - v2f.x) < 120 &&
            abs(this->getPosition().y - v2f.y) < 120)
            return true;
        else
            return false;
    }

    void reset(const float x, const float y, const float agl) {
        sprite.setPosition(x, y);
        sprite.setRotation(agl);
    }

    float px0, py0;
    bool movingFlag = false;
    float movingSpeed = 0.0f;
    bool hitFlag = false;
private:
    Texture texture;
    Sprite sprite;
};

#endif //LAB3_SPRITEWRAPPER_H
