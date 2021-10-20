/*
Author: Yinuo Wang
Class: ECE 6122
Last Date Modified: 10/16/2021

Description:
 This is the wrapper class of Text and Font classes.
*/

#ifndef LAB3_TEXTWRAPPER_H
#define LAB3_TEXTWRAPPER_H

#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>

using namespace sf;
using namespace std;

/*
 * This is the wrapper class of Text and Font classes
 * */
class TextWrapper
{
public:
//    Constructor
    TextWrapper(const string &fontPath, const string &message, const int size, const char color,
                const float px, const float py)
    {
        font.loadFromFile(fontPath);
        text.setFont(font);
        text.setString(message);
        text.setCharacterSize(size);
        if (color == 'r')
            text.setFillColor(Color::Red);
        else
            text.setFillColor(Color::White);
        FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f,
                       textRect.top + textRect.height / 2.0f);
        text.setPosition(px, py);
    }
/*  set the current score text
 *  @param scores is the current score
 */
    void setScore(const int scores)
    {
        stringstream ss;
        ss << "Score: " << scores;
        text.setString(ss.str());
    }
/*  draw the text object
 * @param window is the RenderWindow object in which the sprite is drawn
 */
    void draw(RenderWindow &window)
    {
        window.draw(text);
    }

private:
    Font font;
    Text text;
};

#endif //LAB3_TEXTWRAPPER_H
