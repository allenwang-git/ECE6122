//
// Created by allen on 10/17/21.
//

#ifndef LAB1_BARWRAPPER_H
#define LAB1_BARWRAPPER_H

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

    void setSize(const float len) {
        bar.setSize(Vector2f(len, barWidth));
    }

    void drawBar(RenderWindow &window) {
        window.draw(bar);
        window.draw(shell);
    }

    float barLength, barWidth;
private:
    VertexArray shell;
    RectangleShape bar;

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
