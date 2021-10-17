//
// Created by ywang3781 on 10/16/2021.
//

#ifndef LAB3_TEXTWRAPPER_H
#define LAB3_TEXTWRAPPER_H

#pragma once

#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;
using namespace std;

/*
 * Static text wrapper of Text class
 * */
class TextWrapper {

public:
    TextWrapper(const string &fontPath, const string &message, const int size, const char color,
                const float px, const float py) {
        font.loadFromFile(fontPath);
        text.setFont(font);
        text.setString(message);
        text.setCharacterSize(size);
        if (color == 'r')
            text.setFillColor(Color::Red);
        else
            text.setFillColor(Color::White);
        FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left +
                       textRect.width / 2.0f,
                       textRect.top +
                       textRect.height / 2.0f);
        text.setPosition(px, py);
    }

    Text text;
private:
    Font font;

};


#endif //LAB3_TEXTWRAPPER_H
