/*
Author: Yinuo Wang
Class: ECE 6122
Last Date Modified: 10/18/2021

Description:
 This is the wrapper class of Sound and SoundBuffer class.
*/
#ifndef LAB1_SOUNDWRAPPER_H
#define LAB1_SOUNDWRAPPER_H

#pragma once

#include <SFML/Audio.hpp>
#include <string>

using namespace sf;
using namespace std;

class SoundWrapper {
public:
    SoundWrapper(const string loadPath) {
        sBuffer.loadFromFile(loadPath);
        sound.setBuffer(sBuffer);
    }

//    wrapper to play the sound
    void play() {
        sound.play();
    }

private:
    Sound sound;
    SoundBuffer sBuffer;
};

#endif //LAB1_SOUNDWRAPPER_H
