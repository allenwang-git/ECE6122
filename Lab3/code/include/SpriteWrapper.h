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
 * This is the Wrapper class for Sprite and Texture class
 * */
class SpriteWrapper
{
public:
    //    Constructor
    SpriteWrapper() {};
    SpriteWrapper(const string loadPath, const float px, const float py,
                  const float scale_x, const float scale_y)
    {
        texture.loadFromFile(loadPath);
        sprite.setTexture(texture);
        sprite.setPosition(px, py);
        sprite.setScale(scale_x, scale_y);
        sprite.setOrigin(50, 50);
        this->px0 = px;
        this->py0 = py;
    }

    /*  set a rotation angular of sprite object
     * @param angle is the absolute angular of the sprie
     */
    void setRotation(const float angle)
    {
        sprite.setRotation(angle);
    }
    /*  rotate the sprite from current position
     * @param angle is the relative angular of the rotation
     */
    void rotate(const float angle)
    {
        sprite.rotate(angle);
    }
    /*
     * Get the sprite's current angular
     * @return sprite's current angular
     */
    float getRotation()
    {
        return sprite.getRotation();
    }
    /*    position transformation
     *  @param x is the goal position's x coordinate
     *  @param y is the goal position's y coordinate
     */
    void setPosition(const float x, const float y)
    {
        sprite.setPosition(x, y);
    }
    /*    position transformation
     *  @param pos is the goal position
     */
    void setPosition(const Vector2f &pos)
    {
        sprite.setPosition(pos);
    }
    /*
     * Get the sprite's current position
     * @return sprite's current position
     */
    Vector2f getPosition()
    {
        return sprite.getPosition();
    }

    /*  set object center for transformation
     *  @param x is the origin position's x coordinate
     *  @param y is the origin position's y coordinate
     */
    void setOrigin(const float x, const float y)
    {
        sprite.setOrigin(x, y);
    }

    /*    set sprite scale
     *  @param x is the x direction's scale
     *  @param y is the y direction's scale
     */
    void setScale(const float x, const float y)
    {
        sprite.setScale(x, y);
    }

    /*  draw the sprite
     * @param window is the RenderWindow object in which the sprite is drawn
     */
    void draw(RenderWindow &window)
    {
        window.draw(sprite);
    }

    /*  reset the sprite with origin position and angular
     *  @param x is the original position's x coordinate
     *  @param y is the original position's y coordinate
     *  @param agl is the absolute angular of the sprie
     */
    void reset(const float x, const float y, const float agl)
    {
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
