#pragma once
#ifndef INITIALIZER_H
#define INITIALIZER_H

#include "GameVariables.h"


//function to initialize a GameVariables object
static GameVariables initGameVariables(){
	vector<Shot> gameShots;
	vector<Warrior> gameWarriors;
	vector<Tower> gameTowers;
	int lives;

	GameVariables temp(gameShots, gameWarriors, gameTowers, lives);
	return temp;
}



#endif