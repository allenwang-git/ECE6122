//
// Created by allen on 10/22/21.
//

#ifndef LAB1_LAB3_PROBLEM1_H
#define LAB1_LAB3_PROBLEM1_H
#pragma once
#include <vector>
#include <cmath>
#include <random>
#include "SpriteWrapper.h"
#include "TextWrapper.h"
#include "BarWrapper.h"
#include "SoundWrapper.h"
#include "Motion.h"
#include "Buzzy.h"

// define of some useful constant value
#define BUZZY_X 150.F
#define BUZZY_Y 520.F
#define BUZZY_AGL -45.F
#define COLUMN1 1500
#define COLUMN2 1750

// the animal set
vector<SpriteWrapper *> mascots, animals, woodlandCol1, woodlandCol2;

// game control logic boolean
bool pausedFlag = true;
bool roundOver = true;
bool gameOverFlag = false;
bool loseLife = false;
bool column1 = true;
bool column2 = true;
bool col1Spin = false;
bool col2Spin = false;

// initial configuration
float power = 0.f;
int lives = 5;
int score = 99;

/*
 * This function is used to set animals initial position
 * according to randomization result
 *
 * @param i is a random number from random generator
 * @return the initial position
 * */
Vector2f getInitPos(const int i)
{
    Vector2f initPos;
    switch (i)
    {
        case 0:
            initPos.x = COLUMN1;
            initPos.y = 300;
            break;
        case 1:
            initPos.x = COLUMN1;
            initPos.y = 450;
            break;
        case 2:
            initPos.x = COLUMN1;
            initPos.y = 600;
            break;
        case 3:
            initPos.x = COLUMN1;
            initPos.y = 750;
            break;
        case 4:
            initPos.x = COLUMN1;
            initPos.y = 900;
            break;
        case 5:
            initPos.x = COLUMN2;
            initPos.y = 300;
            break;
        case 6:
            initPos.x = COLUMN2;
            initPos.y = 450;
            break;
        case 7:
            initPos.x = COLUMN2;
            initPos.y = 600;
            break;
        case 8:
            initPos.x = COLUMN2;
            initPos.y = 750;
            break;
        case 9:
            initPos.x = COLUMN2;
            initPos.y = 900;
            break;

    }
    return initPos;
}

/*
 * This function is used to set animals' position randomly
 * at the begining of the game
 * */
void resetImage()
{
    vector<int> vec;
    for (int i = 0; i < 10; ++i)
    {
        vec.push_back(i);
    }
    random_device ri;
    default_random_engine e(ri());
    uniform_int_distribution<int> pos(0, 9);
    int n = vec.size();
//    generate new position
    do
    {
        for (int i = 0; i < n; i++)
        {
            int index = i + pos(e) % (n - i);
            swap(vec[index], vec[i]);
        }
    }
    while (!(vec[0] < 5 && vec[1] >= 5));
//    set new position
    for (int i = 0; i < mascots.size(); i++)
    {
        (*mascots[i]).setPosition(getInitPos(vec[i]));
        (*mascots[i]).setRotation(0.f);
        (*mascots[i]).posTag = vec[i];
        (*mascots[i]).hitFlag = false;
    }
    for (int j = 0; j < animals.size(); j++)
    {
        (*animals[j]).setPosition(getInitPos(vec[j + 2]));
        (*animals[j]).setRotation(0.f);
        (*animals[j]).posTag = vec[j + 2];
        (*animals[j]).hitFlag = false;
    }
//    set woodland animals
    woodlandCol1.clear();
    woodlandCol2.clear();
    for (int i = 0; i < animals.size(); ++i)
    {
//        unicorn doesn't belong to woodland
        if (i != 4)
        {
            if ((*animals[i]).posTag < 5)
                woodlandCol1.push_back(animals[i]);
            else if ((*animals[i]).posTag >= 5)
                woodlandCol2.push_back(animals[i]);
        }
    }
}
/*
 * Initialize the game's configuration when press Enter
 * */
void initializeGame()
{
    loseLife = false;
    column2 = true;
    column1 = true;
    resetImage();
    lives = 5;
    score = 0;
    power = 0.;
}
/*
 * Spin the animals column
 *
 * @param col1 is the spin state of the column 1
 * @param col2 is the spin state of the column 2
 * @param unicorn is a spriteWrapper object
 * */
void spinAnimals(bool col1, bool col2, SpriteWrapper &unicorn)
{
//    SPIN COLUMN 1
    if (!column1 && (*woodlandCol1[0]).getPosition().y <= 1080)
    {
        col1Spin = true;
        for (auto w: woodlandCol1)
        {
            (*w).setPosition((*w).getPosition().x, (*w).getPosition().y + 0.5f);
            (*w).rotate(0.5f);
        }
        if (unicorn.posTag < 5)
        {
            unicorn.setPosition(unicorn.getPosition().x, unicorn.getPosition().y + 0.5f);
            unicorn.rotate(0.5f);
        }
        for (auto m: mascots)
        {
            if ((*m).posTag < 5)
            {
                (*m).setPosition((*m).getPosition().x, (*m).getPosition().y + 0.5f);
                (*m).rotate(0.5f);
            }
        }
    }
    else if (!column1 && (*woodlandCol1[0]).getPosition().y > 1080)
    {
        col1Spin = false;

    }
// SPIN COLUMN 2
    if (!column2 && (*woodlandCol2[0]).getPosition().y <= 1080)
    {
        col2Spin = true;
        for (auto w: woodlandCol2)
        {
            (*w).setPosition((*w).getPosition().x, (*w).getPosition().y + 0.5f);
            (*w).rotate(0.5f);
        }
        if (unicorn.posTag >= 5)
        {
            unicorn.setPosition(unicorn.getPosition().x, unicorn.getPosition().y + 0.5f);
            unicorn.rotate(0.5f);
        }
        for (auto m: mascots)
        {
            if ((*m).posTag >= 5)
            {
                (*m).setPosition((*m).getPosition().x, (*m).getPosition().y + 0.5f);
                (*m).rotate(0.5f);
            }
        }

    }
    else if (!column2 && (*woodlandCol2[0]).getPosition().y > 1080)
    {
        col2Spin = false;
    }
}
/*
 * Drop down the animal above unicorn by one
 * @param unicorn is the unicorn sprite object
 * */
    void dropAnimals(SpriteWrapper &unicorn)
    {
        if (unicorn.posTag < 5)
        {
            for (auto w: woodlandCol1)
            {
                if ((*w).posTag < unicorn.posTag)
                {
                    (*w).posTag++;
                    (*w).setPosition((*w).getPosition().x, (*w).getPosition().y + 150);
                }
            }
        }
        else
        {
            for (auto w: woodlandCol2)
            {
                if ((*w).posTag < unicorn.posTag)
                {
                    (*w).posTag++;
                    (*w).setPosition((*w).getPosition().x, (*w).getPosition().y + 150);
                }
            }
        }
        for (auto m: mascots)
        {
            if ((*m).posTag < unicorn.posTag)
            {
                (*m).posTag++;
                (*m).setPosition((*m).getPosition().x, (*m).getPosition().y + 150);
            }
        }
    }

#endif //LAB1_LAB3_PROBLEM1_H
