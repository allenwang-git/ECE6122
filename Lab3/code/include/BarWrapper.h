/*
Author: Yinuo Wang
Class: ECE 6122
Last Date Modified: 10/17/2021

Description:
 This is the wrapper class of RectangleShape class
 and add a rectangle shell for each bar shape.
*/

#ifndef LAB1_BARWRAPPER_H
#define LAB1_BARWRAPPER_H

#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class BarWrapper {
public:
    BarWrapper(const int length, const int width, const char color,
               const float px, const float py) {
        bar.setSize(Vector2f(length, width));
        if (color == 'r')
            bar.setFillColor(Color::Red);
        bar.setPosition(px, py);
        shell = setBarShell(length, width, px, py);
        barWidth = width;
        barLength = length;
    }

//  set the rectangle bar's length
    void setSize(const float len) {
        bar.setSize(Vector2f(len, barWidth));
    }

//  draw the bar and its shell in window
    void drawBar(RenderWindow &window) {
        window.draw(bar);
        window.draw(shell);
    }

    float barLength, barWidth;
private:
    VertexArray shell;
    RectangleShape bar;

/*
 * set the bar's shell by using 5 vertices to create 4 segments
 *
 * @param len is the shell's length
 * @param wid is the shell's width
 * @param x is the x coordinate of the rectangle's left top vertex
 * @param y is the y coordinate of the rectangle's left top vertex
 * @return a VertexArray object
*/
    VertexArray setBarShell(const int len, const int wid, const float x, const float y) {
        VertexArray lines(sf::LineStrip, 5);
        lines[0].position = Vector2f(x, y);
        lines[1].position = Vector2f(x + len, y);
        lines[2].position = Vector2f(x + len, y + wid);
        lines[3].position = Vector2f(x, y + wid);
        lines[4].position = Vector2f(x, y);
        return lines;
    }
};

#endif //LAB1_BARWRAPPER_H
