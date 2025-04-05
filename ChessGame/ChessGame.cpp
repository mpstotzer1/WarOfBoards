// ChessGame.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Initializer.h"

using namespace std;
using namespace sf;


int main()
{
    //Game variables
    GameVariables gv = initGameVariables();
    //Create the window
    int xres = 1920;
    int yres = 1080;
    VideoMode vm(xres, yres);
    RenderWindow window(vm, "War of Boards");
    //Create time
    Clock clock;
    float dt = clock.getElapsedTime().asMilliseconds();

    //Game loop
    bool running = true;
    while(running){
        //update time
        dt = clock.getElapsedTime().asMilliseconds();

        //update logic
        //update all towers
        for (int i = 0; i < gv.getGameTowers().size(); i++) {
            gv.getGameTowers()[i].update();
        }
        //update all shots
        for (int i = 0; i < gv.getGameShots().size(); i++) {
            gv.getGameShots()[i].update(dt);
        }
        //update all warriors
        for (int i = 0; i < gv.getGameWarriors().size(); i++) {
            gv.getGameWarriors()[i].update(dt, &gv);
        }

        //update graphics
        window.clear(); //clear window
        //draw all towers
        for (int i = 0; i < gv.getGameTowers().size(); i++) {
            gv.getGameTowers()[i].draw(window);
        }
        //draw all shots
        for (int i = 0; i < gv.getGameShots().size(); i++) {
            gv.getGameShots()[i].draw(window);
        }
        //draw all warriors
        for (int i = 0; i < gv.getGameWarriors().size(); i++) {
            gv.getGameWarriors()[i].draw(window);
        }

        window.display();

    }
}
