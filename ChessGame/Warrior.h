#pragma once
#ifndef WARRIOR_H
#define WARRIOR_H

//#include <vector>
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "pathNode.h"
#include "GameVariables.h"

using namespace std;
using namespace sf; //SMFL namespace

/*
The Creep class are all your enemies. They have a speed, hp, bounty, life cost, and a node
in the path they are moving towards.
*/
class Warrior : public GameObject {
private:
    float speed_; //movement speed
    float hp_; //health
    int bounty_; //How much money it gives when it dies
    int lifeCost_; //How many lives you lose if the creep gets to the end of the path
    pathNode* goal_; //node in path its walking towards

public:
    //A constructor
    Warrior(Sprite sprite, Side side, float speed, float hp, int bounty, int lifeCost, pathNode* goal) :
        GameObject(sprite, side, Ontology::DNE), //Automatically starts out as not existing
        speed_(speed),
        hp_(hp),
        bounty_(bounty),
        lifeCost_(lifeCost),
        goal_(goal)
    {}

    //Useful methods
    void update(float dt, GameVariables* gv) {
        //check hp
        if (hp_ <= 0) {
            //Set to dead
            ontology_ = Ontology::DNE;
            return;
        }
        else {
            //alive and kicking
            move(dt);
            //If very close to goal, move to next node in path
            if (distBetween(sprite_.getPosition(), goal_->getCoord()) <= 5) {
                //Check if at end of path
                if (goal_ == nullptr) {
                    //At end of path
                    gv->setLives(gv->getLives() - lifeCost_); //The player loses lives
                    ontology_ = Ontology::DNE; //Remove creep from game
                }
                else {
                    //Start trecking towards the next node in the path
                    goal_ = goal_->getNextNode();
                }
            }
        }
    }
    void move(float dt) {
        float angle = atan(sprite_.getPosition().x / sprite_.getPosition().y); //Find angle to goal
        float dx = speed_ * cos(angle) * dt; //Calculate x step
        float dy = speed_ * sin(angle) * dt; //Calculate y step
        sprite_.move(dx, dy); //Use SFML's move() function to move sprite
    }
    float distBetween(Vector2f point, Vector2f other) {
        Vector2f diff = point - other; //x and y differences between two points
        //Use pythagorean theorem: a^2 + b^2 = c^2
        float dist = sqrt(pow(diff.x, 2) + pow(diff.y, 2));
    }
    void getHit(float damage) {
        hp_ -= damage;
    }

    void draw(RenderWindow& window) {
        window.draw(sprite_);
    }
};

#endif