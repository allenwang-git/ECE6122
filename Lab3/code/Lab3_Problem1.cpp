#include <iostream>
#include "include/SpriteWrapper.h"
#include "include/TextWrapper.h"
#include "include/BarWrapper.h"
#include "include/Motion.h"
#include <vector>
//#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>

using namespace sf;
using namespace std;

#define UPPER_BOUND 270.F
#define LOWER_BOUND 45.F
#define BUZZY_X 150.F
#define BUZZY_Y 520.F
#define BUZZY_AGL -45.F

int main() {
//    create the window
    VideoMode vm(960, 540);
    RenderWindow window(vm, "Buzzy's Revenge");
    View view(FloatRect(0, 0, 1920, 1080));
    window.setView(view);

//    create Sprite objects
    SpriteWrapper background("../graphics/background.png", 0, 0, 1, 1);
    SpriteWrapper buzzy("../graphics/smallbuzzy.png", 150, 520, 1.4f, 1.4f);
    SpriteWrapper insect("../graphics/bee.png", 1600, 200, 1.5f, 1.5f);
    vector<SpriteWrapper> buzzy_lifes;
    SpriteWrapper life1("../graphics/buzzy_life.png", 150 + 0 * 60, 20, 0.5f, 0.5f);
    buzzy_lifes.push_back(life1);
    SpriteWrapper life2("../graphics/buzzy_life.png", 150 + 1 * 60, 20, 0.5f, 0.5f);
    buzzy_lifes.push_back(life2);
    SpriteWrapper life3("../graphics/buzzy_life.png", 150 + 2 * 60, 20, 0.5f, 0.5f);
    buzzy_lifes.push_back(life3);
    SpriteWrapper life4("../graphics/buzzy_life.png", 150 + 3 * 60, 20, 0.5f, 0.5f);
    buzzy_lifes.push_back(life4);
    SpriteWrapper life5("../graphics/buzzy_life.png", 150 + 4 * 60, 20, 0.5f, 0.5f);
    buzzy_lifes.push_back(life5);

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
    buzzy.rotate(BUZZY_AGL);
    buzzy.setOrigin(200, 40);

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
    bool roundOver = true;
    float power = 100.f;
    int lives = 5;
    int score = 0;
    bool loseLife = false;
    bool column1 = true;
    bool column2 = true;
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
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
            roundOver = false;
            buzzy.movingFlag = true;
            loseLife = false;
            buzzyMotion.reset();
            buzzyMotion.initialize(buzzy.px0, buzzy.py0, buzzy.movingSpeed, buzzy.getRotation());
            clock.restart();
            tScore.setScore(0);
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
//          Moving the bee
            if (insect.movingFlag) {
                insect.setPosition(insect.getPosition().x - dt.asSeconds() * insect.movingSpeed,
                                   insect.getPosition().y);
                if (insect.getPosition().x < -100.)
                    insect.movingFlag = false;
            } else {
                random_device rd;
                default_random_engine e(rd());
                uniform_int_distribution<int> h(200, 800);
                insect.setPosition(1920, h(e));
                uniform_int_distribution<int> speed(200, 400);
                insect.movingSpeed = speed(e);
                insect.movingFlag = true;
            }
            if (!roundOver) {

                buzzyMotion.t += dt.asSeconds();
//        Moving the buzzy
                if (buzzy.movingFlag) {
                    buzzy.setPosition(buzzyMotion.getXPosition(), buzzyMotion.getYPosition());
                    buzzy.setRotation(buzzyMotion.getVelocityDirection());
//                buzzyMotion.show();
                }

                //   hit Tiger
                if (!evilTiger.hitFlag && evilTiger.intersects(buzzy.getPosition())) {
                    score += 25;
                    tScore.setScore(score);
                    evilTiger.hitFlag = true;
                    column1 = false;
                    buzzy.reset(BUZZY_X, BUZZY_Y, BUZZY_AGL);
                    roundOver = true;
//                cout<<"happy night"<<endl;
                }
                    //   hit Bulldog
                else if (!evilBulldog.hitFlag && evilBulldog.intersects(buzzy.getPosition())) {
                    score += 25;
                    tScore.setScore(score);
                    evilBulldog.hitFlag = true;
                    column2 = false;
                    buzzy.reset(BUZZY_X, BUZZY_Y, BUZZY_AGL);
                    roundOver = true;
//                cout<<"happy night"<<endl;
                }
                    //    hit Insect
                else if (!insect.hitFlag && insect.intersects(buzzy.getPosition())) {
                    score += 75;
                    tScore.setScore(score);
                    insect.hitFlag = true;
                    buzzy.reset(BUZZY_X, BUZZY_Y, BUZZY_AGL);
                    roundOver = true;
//                cout<<"happy night"<<endl;
                }
                    //     hit Tiger
                else if (!unicorn.hitFlag && unicorn.intersects(buzzy.getPosition())) {

                    if (lives < 5) lives += 1;
                    unicorn.hitFlag = true;
                    buzzy.reset(BUZZY_X, BUZZY_Y, BUZZY_AGL);
                    roundOver = true;
//                cout<<"happy night"<<endl;
                } else if (!loseLife && (buzzy.getPosition().x > 1920 || buzzy.getPosition().x < 0
                                         || buzzy.getPosition().y > 1080 || buzzy.getPosition().y < 0)) {
                    if (lives > 0) lives -= 1;
                    loseLife = true;
                    buzzy.reset(BUZZY_X, BUZZY_Y, BUZZY_AGL);
                    roundOver = true;
                }
            }

//            else
//                roundOver = true;


        }



        /*******************************
         Draw our game scene here
        ****************************/
        // Clear everything from the last frame
        window.clear();

        background.draw(window);
        if (!insect.hitFlag)
            insect.draw(window);
        buzzy.draw(window);

        for (int i = 0; i < lives; ++i) {
            buzzy_lifes[i].draw(window);
        }
        if (column1) {
            sheep.draw(window);
            bunny.draw(window);
            dog.draw(window);
            evilTiger.draw(window);
            mouse.draw(window);
        }

        if (column2) {
            if (!unicorn.hitFlag)
                unicorn.draw(window);
            frog.draw(window);
            evilBulldog.draw(window);
            pig.draw(window);
            chicken.draw(window);
        }
        powerBar.drawBar(window);
//        draw texts
        tLives.draw(window);
        tPower.draw(window);
        tScore.draw(window);
        if (pausedFlag) {
            tTitle.draw(window);
            tRestart.draw(window);
            tExit.draw(window);
            tPowerup.draw(window);
            tAim.draw(window);
            tAuthor.draw(window);
        }
        // Show everything we just drew
        window.display();
    }


//    std::cout << "Hello, World!" << std::endl;
    return 0;
}
