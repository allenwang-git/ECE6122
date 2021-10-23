/*
Author: Yinuo Wang
Class: ECE 6122
Last Date Modified: 10/19/2021

Description:
 This program use SFML to create a 2D game called "Buzzy Revenge",
 In this game, the user launches buzzy across the field attempting
 to hit the evil mascots from other universities, while avoiding
 the cute woodland creatures.

 Press Enter to start/restart the game, press Space to modify the buzzy's
 initial velocity and press up/down to modify buzzy's initial direction.
 When releasing the Space, the buzzy will be launched.

 If user win or lose the game, the page will reset to the initial page.
*/
#include <iostream>
#include "include/Lab3_Problem1.h"

using namespace sf;
using namespace std;

/*
 * This is the main function to handle the game logic
 * */
int main()
{
//    create the window
    VideoMode vm(960, 540);
    RenderWindow window(vm, "Buzzy's Revenge");
    View view(FloatRect(0, 0, 1920, 1080));
    window.setView(view);

//    create Sprite objects
    SpriteWrapper background("../graphics/background.png", 0, 0, 1, 1);
    background.setOrigin(0, 0);
    Buzzy buzzy("../graphics/smallbuzzy.png", 250, 500, 1.4f, 1.4f);
    buzzy.rotate(BUZZY_AGL);
    buzzy.setOrigin(200, 40);
    SpriteWrapper insect("../graphics/insect.png", 1600, 200, 1.5f, 1.5f);
    insect.setScale(-1.5, 1.5);
    vector<SpriteWrapper> buzzy_lifes;
    SpriteWrapper life1("../graphics/buzzy_life.png", 170 + 0 * 60, 50, 0.5f, 0.5f);
    buzzy_lifes.push_back(life1);
    SpriteWrapper life2("../graphics/buzzy_life.png", 170 + 1 * 60, 50, 0.5f, 0.5f);
    buzzy_lifes.push_back(life2);
    SpriteWrapper life3("../graphics/buzzy_life.png", 170 + 2 * 60, 50, 0.5f, 0.5f);
    buzzy_lifes.push_back(life3);
    SpriteWrapper life4("../graphics/buzzy_life.png", 170 + 3 * 60, 50, 0.5f, 0.5f);
    buzzy_lifes.push_back(life4);
    SpriteWrapper life5("../graphics/buzzy_life.png", 170 + 4 * 60, 50, 0.5f, 0.5f);
    buzzy_lifes.push_back(life5);

//  create column 1 and add woodland creatures to list
    SpriteWrapper sheep("../graphics/sheep.png", COLUMN1, 300, 1.5f, 1.5f);
    animals.push_back(&sheep);
    woodlandCol1.push_back(&sheep);
    SpriteWrapper bunny("../graphics/bunny.png", COLUMN1, 450, 1.6f, 1.6f);
    animals.push_back(&bunny);
    woodlandCol1.push_back(&bunny);
    SpriteWrapper dog("../graphics/dog.png", COLUMN1, 600, 1.5f, 1.5f);
    animals.push_back(&dog);
    woodlandCol1.push_back(&dog);
    SpriteWrapper evilTiger("../graphics/tiger.png", COLUMN1, 750, 0.17f, 0.17f);
    evilTiger.setOrigin(500, 500);
    mascots.push_back(&evilTiger);
    SpriteWrapper mouse("../graphics/mouse.png", COLUMN1, 900, 1.4f, 1.4f);
    animals.push_back(&mouse);
    woodlandCol1.push_back(&mouse);

//    create column 2 and add to woodland creatures
    SpriteWrapper unicorn("../graphics/angry_unicorn.png", COLUMN2, 300, 1.5f, 1.5f);
    unicorn.setOrigin(90, 50);
    animals.push_back(&unicorn);
    SpriteWrapper frog("../graphics/frog.png", COLUMN2, 450, 1.4f, 1.4f);
    animals.push_back(&frog);
    woodlandCol2.push_back(&frog);
    SpriteWrapper evilBulldog("../graphics/Bulldog.png", COLUMN2, 590, 1.8f, 1.8f);
    evilBulldog.setOrigin(50, 50);
    mascots.push_back(&evilBulldog);
    SpriteWrapper pig("../graphics/pig.png", COLUMN2, 750, 1.4f, 1.4f);
    animals.push_back(&pig);
    woodlandCol2.push_back(&pig);
    SpriteWrapper chicken("../graphics/chicken.png", COLUMN2, 900, 1.8f, 1.6f);
    chicken.setOrigin(30, 50);
    animals.push_back(&chicken);
    woodlandCol2.push_back(&chicken);

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
    TextWrapper tOver("../fonts/KOMIKAP_.ttf", "Game Over", 80, 'r', 960, 200);
//  create the motion object to handle buzzy motion
    Motion buzzyMotion;

//  create sound
    SoundWrapper hit("../sound/chop.wav");
    SoundWrapper win("../sound/out_of_time.wav");
    SoundWrapper gameOver("../sound/death.wav");

// Variables to control time itself
    Clock clock;
// Game main loop
    while (window.isOpen())
    {
        /******************************
         Handle the player's input
         ******************************/
        Event event;
        while (window.pollEvent(event))
        {
            //        exit the game
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                //        exit the game
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                //        start the game
                if (event.key.code == Keyboard::Enter)
                {
                    initializeGame();
                    pausedFlag = false;
                    buzzyMotion.reset();
                    insect.hitFlag = false;
                    powerBar.setSize(power);
                    tScore.setScore(score);
                }
//                Start the new game
                if (!pausedFlag)
                {
                    //        change buzz initial launch power
                    if (event.key.code == Keyboard::Space)
                    {
                        if (power < powerBar.barLength)
                        {
                            power += 5.f;
                            powerBar.setSize(power);
                        }
                    }
                    //        change buzz initial launch direction
                    if (Keyboard::isKeyPressed(Keyboard::Up))
                    {
                        buzzy.headUp();
                    }
                    if (Keyboard::isKeyPressed(Keyboard::Down))
                    {
                        buzzy.headDown();
                    }
                }
            }
            if (event.type == Event::KeyReleased && !pausedFlag)
            {
                //    launch the buzzy to begin a round
                if (event.key.code == sf::Keyboard::Space)
                {
                    clock.restart();
                    buzzyMotion.reset();
                    buzzyMotion.initialize(buzzy.px0, buzzy.py0, buzzy.movingSpeed, buzzy.getRotation());
                    buzzy.movingFlag = true;
                    roundOver = false;
                    powerBar.setSize(0);
                }
            }
        }
//      set the buzz speed and update powerbar
        buzzy.movingSpeed = power * 2.f + 600;
//      Game over, prepare to restart
        if (lives == 0 && !buzzy.spinningFlag) {
            gameOverFlag = true;
            gameOver.play();
            initializeGame();
            pausedFlag = true;
            insect.hitFlag = false;
            buzzyMotion.reset();
            powerBar.setSize(power);
            tScore.setScore(score);
        }
//      Win the game, prepare to restart
        if (!column1 && !column2 && lives > 0 && !col1Spin && !col2Spin && !buzzy.spinningFlag)
        {
            gameOverFlag = false;
            win.play();
            initializeGame();
            pausedFlag = true;
            insect.hitFlag = false;
            buzzyMotion.reset();
            powerBar.setSize(power);
            tScore.setScore(score);
        }

        /****************************
         Update the screen
        ****************************/
        if (!pausedFlag)
        {
            Time dt = clock.restart();
            //   Moving the bee randomly
            random_device rd;
            default_random_engine e(rd());
            if (insect.movingFlag)
            {
                int height = insect.getPosition().y;
                uniform_int_distribution<int> deltaH(0, 2);
                switch (deltaH(e))
                {
                    case 0:
                        break;
                    case 1:
                        height += 2;
                        break;
                    case 2:
                        height -= 2;
                        break;
                }
                insect.setPosition(insect.getPosition().x - dt.asSeconds() * insect.movingSpeed, height);
                if (insect.getPosition().x < -100. || insect.getPosition().y < 0. || insect.getPosition().y > 1080.)
                    insect.movingFlag = false;
            }
            else
            {
                uniform_int_distribution<int> h(200, 800);
                insect.setPosition(1920, h(e));
                uniform_int_distribution<int> speed(200, 400);
                insect.movingSpeed = speed(e);
                insect.movingFlag = true;
            }
//            begin a new round
            if (!roundOver)
            {
                buzzyMotion.t += dt.asSeconds();
                //   Moving the buzzy
                if (buzzy.movingFlag)
                {
                    buzzy.setPosition(buzzyMotion.getXPosition(), buzzyMotion.getYPosition());
                    buzzy.setRotation(buzzyMotion.getVelocityDirection());
//                buzzyMotion.show();
                }
                //   hit Tiger
                if (!evilTiger.hitFlag && buzzy.intersect(evilTiger.getPosition()))
                {
                    hit.play();
                    score += 25;
                    tScore.setScore(score);
                    evilTiger.hitFlag = true;
                    if (evilTiger.posTag < 5)
                        column1 = false;
                    else
                        column2 = false;
                    column1 = false;
                    roundOver = true;
                    power = 0;
                    buzzy.spinningFlag = true;
                }
                    //   hit Bulldog
                else if (!evilBulldog.hitFlag && buzzy.intersect(evilBulldog.getPosition()))
                {
                    hit.play();
                    score += 25;
                    tScore.setScore(score);
                    evilBulldog.hitFlag = true;
                    if (evilBulldog.posTag < 5)
                        column1 = false;
                    else
                        column2 = false;
//                    buzzy.reset(BUZZY_X, BUZZY_Y, BUZZY_AGL);
                    roundOver = true;
                    power = 0;
                    buzzy.spinningFlag = true;
                }
                    //    hit Insect
                else if (!insect.hitFlag && buzzy.intersectSmall(insect.getPosition()))
                {
                    score += (25 * 3);
                    hit.play();
                    tScore.setScore(score);
                    insect.hitFlag = true;
                    roundOver = true;
                    power = 0;
                    buzzy.spinningFlag = true;
                }
                    //     hit Unicorn
                else if (!unicorn.hitFlag && buzzy.intersect(unicorn.getPosition()))
                {
                    if (lives < 5) lives += 1;
                    unicorn.hitFlag = true;
                    hit.play();
//                    drop down the animals above unicorn by one
                    dropAnimals(unicorn);
                    roundOver = true;
                    power = 0;
                    buzzy.spinningFlag = true;
                }
//                hit woodland creatures
                else if ((column1 && buzzy.hitWoodlandCreature(woodlandCol1))
                         || (column2 && buzzy.hitWoodlandCreature(woodlandCol2)))
                {
                    if (lives > 0) lives -= 1;
                    hit.play();
                    loseLife = true;
                    roundOver = true;
                    power = 0;
                    buzzy.spinningFlag = true;
                }
//              fly beyond the window
                else if ((buzzy.getPosition().x > 1900 || buzzy.getPosition().x < 0
                          || buzzy.getPosition().y > 1050 || buzzy.getPosition().y < 0))
                {
                    if (lives > 0) lives -= 1;
                    loseLife = true;
                    buzzy.reset(BUZZY_X, BUZZY_Y, BUZZY_AGL);
                    roundOver = true;
                    power = 0;
                }
            }
//            spin the buzzy to bottom before resetting
            if (buzzy.spinningFlag && buzzy.getPosition().y <= 1080)
            {
                buzzy.setPosition(buzzy.getPosition().x, buzzy.getPosition().y + 0.5f);
                buzzy.rotate(0.5f);
            }
            else if (buzzy.spinningFlag && buzzy.getPosition().y > 1080)
            {
                buzzy.spinningFlag = false;
                buzzy.reset(BUZZY_X, BUZZY_Y, BUZZY_AGL);
            }
//            spin the column to bottom before disappearing
            spinAnimals(column1, column2, unicorn);
        }
        /*******************************
         Draw our game scene here
        ****************************/
        // Clear everything from the last frame
        window.clear();

        background.draw(window);
        buzzy.draw(window);
        if (!insect.hitFlag)
            insect.draw(window);

        for (int i = 0; i < lives; ++i)
            buzzy_lifes[i].draw(window);

        if (column1 || col1Spin)
        {
            for (int i = 0; i < woodlandCol1.size(); ++i)
                (*woodlandCol1[i]).draw(window);
            if (unicorn.posTag < 5 && !unicorn.hitFlag)
                unicorn.draw(window);
            if (evilBulldog.posTag < 5)
                evilBulldog.draw(window);
            if (evilTiger.posTag < 5)
                evilTiger.draw(window);
        }
        if (column2 || col2Spin)
        {
            for (int i = 0; i < woodlandCol2.size(); ++i)
                (*woodlandCol2[i]).draw(window);
            if (unicorn.posTag >= 5 && !unicorn.hitFlag)
                unicorn.draw(window);
            if (evilTiger.posTag >= 5)
                evilTiger.draw(window);
            if (evilBulldog.posTag >= 5)
                evilBulldog.draw(window);
        }
        powerBar.drawBar(window);
//        draw texts
        tLives.draw(window);
        tPower.draw(window);
        tScore.draw(window);
        if (pausedFlag)
        {
            if (gameOverFlag)
                tOver.draw(window);
            else
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
    return 0;
}
