#include <iostream>
#include "include/SpriteWrapper.h"
#include "include/TextWrapper.h"
#include "include/BarWrapper.h"
#include "include/Motion.h"
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <random>
using namespace sf;
using namespace std;

#define UPPER_BOUND 270.F
#define LOWER_BOUND 45.F

int main() {
//    create the window
    RenderWindow window(VideoMode(960, 540), "Buzzy's Revenge");
    View view(FloatRect(0, 0, 1920, 1080));
    window.setView(view);

//    create Sprite objects
    SpriteWrapper background("../graphics/background.png", 0, 0, 1, 1);
    SpriteWrapper buzzy("../graphics/smallbuzzy.png", 150, 520, 1.4f, 1.4f);
    SpriteWrapper buzzy_life("../graphics/buzzy life.png", 150, 20, 0.5f, 0.5f);
    SpriteWrapper insect("../graphics/bee.png", 1600, 200, 1.5f, 1.5f);
//    column 1
    SpriteWrapper sheep("../graphics/sheep.png", 1500, 300, 1.5f, 1.5f);
    SpriteWrapper bunny("../graphics/bunny.png", 1500, 450, 1.6f, 1.6f);
    SpriteWrapper dog("../graphics/dog.png", 1500, 600, 1.5f, 1.5f);
    SpriteWrapper evilTiger("../graphics/tiger.png", 1500, 750, 0.17f, 0.17f);
    SpriteWrapper mouse("../graphics/mouse.png", 1500, 900, 1.4f, 1.4f);
//    column 2
    SpriteWrapper unicorn("../graphics/angry_unicorn.png", 1700, 300, 1.5f, 1.5f);
    SpriteWrapper frog("../graphics/frog.png", 1750, 450, 1.4f, 1.4f);
    SpriteWrapper evilBulldog("../graphics/Bulldog.png", 1720, 590, 1.8f, 1.8f);
    SpriteWrapper pig("../graphics/pig.png", 1750, 750, 1.4f, 1.4f);
    SpriteWrapper chicken("../graphics/chicken.png", 1750, 900, 1.8f, 1.6f);

//  Modification
    buzzy.rotate(-45.f);
    buzzy.setOrigin(150, 80);

//  create a power bar
    BarWrapper powerBar(500, 50, 'r', 150, 925);

//  create text
    TextWrapper tTitle("../fonts/KOMIKAP_.ttf", "Buzzy's Revenge", 80, 'r', 960, 200);
    TextWrapper tRestart("../fonts/KOMIKAP_.ttf", "Press Enter to Restart Game", 50, 'w', 960, 400);
    TextWrapper tExit("../fonts/KOMIKAP_.ttf", "Press Esc to Exit", 50, 'w', 960, 460);
    TextWrapper tPowerup("../fonts/KOMIKAP_.ttf", "Press Space to Powerup", 50, 'w', 960, 520);
    TextWrapper tAim("../fonts/KOMIKAP_.ttf", "Press up/down arrow to Aim", 50, 'w', 960, 580);
    TextWrapper tAuthor("../fonts/KOMIKAP_.ttf", "Created by\n Yinuo Wang", 25, 'w', 960, 750);
    TextWrapper tPower("../fonts/KOMIKAP_.ttf", "Power", 30, 'w', 80, 950);
    TextWrapper tLives("../fonts/KOMIKAP_.ttf", "Lives", 30, 'w', 80, 50);
    TextWrapper tScore("../fonts/KOMIKAP_.ttf", "Score: 99", 30, 'w', 1750, 50);

//  create the motion object to handle buzzy motion
    Motion buzzyMotion;


// Variables to control time itself
    Clock clock;
// Track whether the game is running
    bool pausedFlag = true;
    float power = 100.f;

    while (window.isOpen()) {
        buzzy.movingSpeed = power * 4.f;
        powerBar.setSize(power);

        /******************************
         Handle the player's input
         ******************************/
//        exit the game
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
        }
//        start the game
        if (Keyboard::isKeyPressed(Keyboard::Enter)) {
            pausedFlag = false;
            buzzy.movingFlag = true;
            buzzyMotion.initialize(buzzy.px0, buzzy.py0, buzzy.movingSpeed, buzzy.getRotation());
            clock.restart();
        }
//        change buzz initial launch power
        if (Keyboard::isKeyPressed(Keyboard::Space)) {
            if (power < powerBar.barLength) {
                power += .5f;
            }
        }
//        change buzz initial launch direction
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            if ((buzzy.getRotation() <= 360.f && buzzy.getRotation() > UPPER_BOUND)
                || (buzzy.getRotation() >= 0.f && buzzy.getRotation() <= LOWER_BOUND))
                buzzy.rotate(-.1f);
            else
                buzzy.setRotation(round(buzzy.getRotation()) + 0.1f);
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            if ((buzzy.getRotation() <= 360.f && buzzy.getRotation() >= UPPER_BOUND)
                || (buzzy.getRotation() >= 0.f && buzzy.getRotation() < LOWER_BOUND))
                buzzy.rotate(.1f);
            else
                buzzy.setRotation(round(buzzy.getRotation()) - 0.1f);
        }


        /****************************
         Update the screen
        ****************************/
        if (!pausedFlag) {
            Time dt = clock.restart();
            buzzyMotion.t += dt.asSeconds();
//        Moving the buzzy
            if (buzzy.movingFlag) {
                buzzy.setPosition(buzzyMotion.getXPosition(), buzzyMotion.getYPosition());
                buzzy.setRotation(buzzyMotion.getVelocityDirection());
//                buzzyMotion.show();
            }
//          Moving the bee
            if (insect.movingFlag) {
                insect.setPosition(insect.getPosition().x
                                   - dt.asSeconds() * insect.movingSpeed, insect.getPosition().y);

                if (insect.getPosition().x < -100.)
                    insect.movingFlag = false;
            } else {
                random_device rd;
                default_random_engine e(rd());
                uniform_int_distribution<int> h(100, 800);
                insect.setPosition(2020, h(e));
                uniform_int_distribution<int> speed(200, 400);
                insect.movingSpeed = speed(e);
                insect.movingFlag = true;
            }
        }



        /*******************************
         Draw our game scene here
        ****************************/
        // Clear everything from the last frame
        window.clear();
//        draw pictures
        background.draw(window);
        buzzy.draw(window);
        buzzy_life.draw(window);
        insect.draw(window);

        sheep.draw(window);
        bunny.draw(window);
        dog.draw(window);
        evilTiger.draw(window);
        mouse.draw(window);

        unicorn.draw(window);
        frog.draw(window);
        evilBulldog.draw(window);
        pig.draw(window);
        chicken.draw(window);
/*        window.draw(background.sprite);
        window.draw(buzzy.sprite);
        window.draw(buzzy_life.sprite);
        window.draw(insect.sprite);

        window.draw(sheep.sprite);
        window.draw(bunny.sprite);
        window.draw(dog.sprite);
        window.draw(evilTiger.sprite);
        window.draw(mouse.sprite);

        window.draw(unicorn.sprite);
        window.draw(frog.sprite);
        window.draw(evilBulldog.sprite);
        window.draw(pig.sprite);
        window.draw(chicken.sprite);*/

        powerBar.drawBar(window);
//        draw texts
        window.draw(tLives.text);
        window.draw(tPower.text);
        window.draw(tScore.text);
        if (pausedFlag) {
            window.draw(tTitle.text);
            window.draw(tRestart.text);
            window.draw(tExit.text);
            window.draw(tPowerup.text);
            window.draw(tAim.text);
            window.draw(tAuthor.text);
        }

        // Show everything we just drew
        window.display();
    }


//    std::cout << "Hello, World!" << std::endl;
    return 0;
}
