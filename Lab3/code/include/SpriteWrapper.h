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

    float px0, py0;
    bool movingFlag = false;
    float movingSpeed = 0.0f;
private:
    Texture texture;
    Sprite sprite;
};

#endif //LAB3_SPRITEWRAPPER_H
