#pragma once
#ifndef SHOT_H
#define SHOT_H

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "HelpfulFunctions.h"
#include "GameVariables.h"

using namespace std;
using namespace sf;

/*
Note: the shot class is unique (read: poorly designed) in that it plays two roles.
1) The "physical" shot stored in the vector of real things that moves accross the screen
2) The "abstract" shot that clones itself and is inside the Tower
Some functions of the shot class (like the move) function don't make any sense in the first context,
while others (like fireIfReady) don't make any sense in the second context.
To disambiguate, the "abstract" shots have Ontology::DNE and the "physical" shots have Ontology::EXIST
*/
class Shot : public GameObject {
protected:
    Shot* nextShot_;
    float damage_, rate_, cooldown_;
    Warrior* target_;

public:
    //Constructor
    Shot(Sprite sprite, Side side, Shot* nextShot, float damage, float rate, float cooldown, Warrior* target) :
        GameObject(sprite, side, Ontology::DNE), //Automatically starts out as not existing
        nextShot_(nullptr),
        damage_(damage),
        rate_(rate),
        cooldown_(cooldown),
        target_(nullptr)
    {}

    //Useful methods
    void fireIfReady() {
        //Don't fire if you have already been fired 
        if (ontology_ == Ontology::EXIST) { return; }
        //Don't fire if you don't have a target
        if (target_ == nullptr) { return; }

        //Check cooldown
        if (cooldown_ <= 0) {
            realize(); //Reify this shot
            cooldown_ = rate_; //Reset cooldown
        }
    }
    void realize(GameVariables* gv) {
        //Don't realize if you have already been realized
        if (ontology_ == Ontology::EXIST) { return; }

        Shot temp(sprite_, side_, nextShot_, damage_, rate_, cooldown_, target_); //temp (essentially a clone) will go out of scope and die after this function exits
        temp.ontology_ = Ontology::EXIST; //Set the new shot to EXIST
        gv->getGameShots().push_back(temp); //gameShots creates a copy of temp for its own uses (and the first temp dies quietly)
    }
    void remove(GameVariables* gv) {
        //Don't remove if you already don't exist
        if (ontology_ == Ontology::DNE) { return; }

        nextShot_->realize(gv); //Add the next shot to gameShots
        ontology_ = Ontology::DNE; //Make this not exist any more
    }
    //Subclasses must override
    virtual void update(float dt) { //This only applies to the "physical" shots.
        //Must be overriden by each subclass
        if (ontology_ == Ontology::DNE) {
            cerr << "this Shot object does not exist, but act() was called upon it" << endl;
        }
        cerr << "act() was called in the Shot class, not a subclass of Shot" << endl;
    }
    virtual void hit(float dt) { //This only applies to the "physical" shots.
        //Must be overriden by each subclass
        if (ontology_ == Ontology::DNE) {
            cerr << "this Shot object does not exist, but hit() was called upon it" << endl;
        }
        cerr << "hit() was called in the Shot class, not a subclass of Shot" << endl;
    }

    void draw(RenderWindow& window) {
        window.draw(sprite_);
    }
};

/*
The ProjectileShot class is a subclass of Shot, and move in a projectile fashion.
It overrides update() and hit() to simulate a projectile flying through the air and
then damaging the enemy it was aimed at (provided the enemy didn't move too far away in the interim)
*/
class ProjectileShot : public Shot {
private:
    Vector2f endpt_; //Where the projectile should land
public:
    ProjectileShot(Sprite sprite, Side side, Shot* nextShot, float damage, float rate, float cooldown, Warrior* target) :
        Shot(sprite, side, nextShot, damage, rate, cooldown, target),
        endpt_(target_->getCenter()) //endpt_ is the current target's coordinates
    {}
    void update(float dt) override {
        if (ontology_ == Ontology::DNE) { //In the tower
            cooldown_ -= dt; //Decrement cooldown
        }
        else if (ontology_ == Ontology::EXIST) { //Shot has been fired and is active in the game
            if (endpt_ == sprite_.getPosition()) { //If at endpt
                hit(dt); //Hit the target
            }
            else{
                moveParabolic(); //Keep moving
            }
        }

    }
    void moveParabolic(){
        //Move the shot in a parabola
    }
    void hit(float dt) override {
        //Check if target still exists
        if (target_ == nullptr) {
            miss();
        }
        else {
            bool collide = overlap(sprite_, target_->getSprite()); //Is it touching target_?
            if (collide) {
                target_->getHit(damage_);
            }
            else {
                miss();
            }
        }
        remove(); //After hitting, this object dies
    }
    void miss() {
        //draw a missed projectile
    }
};

/*
    The BeamShot class deals constant damage (in damage per second, or dps) to a target.
Really, it is continuous in that it deals constant dps to a target by creating
a BeamShot, hitting the target instantly (no projectile/other motion), and then removing
itself from the game.
    Uniquely, it has a cooldown of zero.
*/
class BeamShot : public Shot {
public:
    //A constructor (literally just a normal shot)
    BeamShot(Sprite sprite, Side side, Shot* nextShot, float damage, float rate, float cooldown, Warrior* target) :
        Shot(sprite, side, nextShot, damage, rate, cooldown, target)
    {}

    //Overriden methods
    void hit(float dt) override {
        if (target_ != nullptr) { //Only hit if target still in game
            float tempDmg = damage_ * dt; //Calculate laser damage (in damage per second)
            target_->getHit(tempDmg);
        }

        remove(); //Remove this shot
    }
    void update(float dt) override {
        //Beams have no cooldown to decrement: they deal constant damage

        if (ontology_ == Ontology::EXIST) { //Real in the game
            //Beams are fired once per tick (time interval), so upon hitting a target, it is instantly removed
            hit(dt);
        }
    }
};

/*
The AOEShot class (short for Area of Effect) creates a circular area that damages
any enemy inside of it. Like the BeamShot, it deals continuous damage over time measured
in dps. However, a single-shot AEO can still exist as long if the linger_ time is zero
*/
class AEOShot : public Shot {
private:
    float linger_; //How long the AEO lasts in the game
public:
    //A constructor
    AEOShot(Sprite sprite, Side side, Shot* nextShot, float damage, float rate, float cooldown, Warrior* target, float linger) :
        Shot(sprite, side, nextShot, damage, rate, cooldown, target),
        linger_(linger)
    {}

    //Overriden methods
    void update(float dt) override {
        if (linger_ >= 0) { //If time left to linger in the game is greater than zero
            hit(dt);
            linger_ -= dt;
        }
        else { //If out of time
            remove();
        }
    }
    void hit(float dt) override {
        //Find all enemies in range
        vector<Warrior*> enemies;
        enemies = warriorsInRange(this);
        for (int i = 0; i <= enemies.size(); i++) {//For each enemy in range
            if (enemies[i] != nullptr) { //If enemy is still around
                enemies[i]->getHit(damage_ * dt); //Deal continuous damage over time
            }
        }
    }

    void draw(RenderWindow& window) {
        window.draw(sprite_);
    }
};

/*
The TroopShot class creates a troop at a certain location. It holds a copy of a
troop in a database and clones it into the gameWarriors vector upon hitting.

class TroopShot: public Shot{
private:
    Warrior spawn_;
public:
    //A constructor
    TroopShot(Sprite sprite, Side side, Shot* nextShot, float damage, float rate, float cooldown, Warrior* target, Warrior spawn):
        Shot(sprite, side, nextShot, damage, rate, cooldown, target),
        spawn_(spawn)
    {}

    //Overriden methods
    void update(float dt) override {
        //It hits immidiately upon creation
        hit(dt);
    }
    void hit(float dt) override {
        Warrior temp;
        gameWarriors.push_back(temp);
        remove();
    }
};
*/


#endif