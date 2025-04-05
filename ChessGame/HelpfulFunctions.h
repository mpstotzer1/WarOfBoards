#pragma once
#ifndef HELPFULFUNCTIONS_H
#define HELPFULFUNCITONS_H

#include <SFML/Graphics.hpp>
#include "Shot.h"
#include "Warrior.h"

using namespace std;
using namespace sf; //SFML namespace

bool overlap(Sprite sprite, Sprite other) {
    //Paste the overlap-checking code here
}

Warrior* findNearestEnemy(float radius) {
    //To be implemented later, possibly with spatial partitions
}
vector<Warrior*> warriorsInRange(Shot* shot){
    //find all warriors in a certain range
    //return vector of these warriors
}


#endif