#pragma once
#ifndef GAMEVARIABLES_H
#define GAMEVARIABLES_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "Shot.h"
#include "Warrior.h"
#include "Tower.h"

using namespace std;
using namespace sf; //SFML namespace


class GameVariables{
private:
	vector<Shot> gameShots_; //This vector holds all currently-existing shots in the game
	vector<Warrior> gameWarriors_; //This vector holds all currently-existing warriors in the game
	vector<Tower> gameTowers_; //This vector holds all currently-existing towers in the game
	int lives_; //Number of lives the player has
	
public:
	//A constructor
	GameVariables(vector<Shot> gameShots, vector<Warrior> gameWarriors, vector<Tower> gameTowers, int lives):
		gameShots_(gameShots),
		gameWarriors_(gameWarriors),
		gameTowers_(gameTowers),
		lives_(lives)
	{}

	//getters and setters
	vector<Shot> getGameShots() { return gameShots_; }
	vector<Warrior> getGameWarriors() { return gameWarriors_; }
	vector<Tower> getGameTowers() { return gameTowers_; }
	int getLives() { return lives_; }

	void setLives(int lives) { lives_ = lives; }
};

#endif
