#pragma once
#ifndef TOWER_H
#define TOWER_H

#include "GameObject.h"
#include "Shot.h"

using namespace std;
using namespace sf;

/*
=====Tower=====
Represents the defense towers

Member Variables:
-A pointer to a Warrior to shoot at
-Tower's range
-Coordinate where shots will appear/be drawn from
-How much it costs
-How much money has been sunk into it
-List of its attacks

Member Functions:
-Default and parametrized constructor
-A copy constructor
-Updates all shots
-Getters and setters

Notes:
I still need to add selection menus. Also, it might be a lot better
and easier to let the tower level up with an enum (like ONE, TWO, BLACK, WHITE).
*/
class Tower: public GameObject {
private:
    Warrior* target_; //The tower's current target
    float range_; //The tower's firing radius
    Vector2f shotPoint_; //The point from which shots will appear
    int buyPrice_; //How much to build/upgrade-to this tower
    int totalPrice_; //The accumulation of all money spent buying/upgrading this tower
    vector<Shot> shots_;

public:
    //Normal constructor
    Tower(Sprite sprite, Side side, float range, Vector2f shotPoint, int buyPrice):
        GameObject(sprite, side, Ontology::DNE),
        target_(nullptr),
        range_(range),
        shotPoint_(shotPoint),
        buyPrice_(buyPrice),
        totalPrice_(0)
    {
        totalPrice_ = totalPrice_ + buyPrice_;
    }
    //Copy constructor (uses the address of another tower: why?)
    Tower(Tower* clone, Vector2f position):
        GameObject(clone->getSprite(), clone->getSide(), Ontology::DNE),
        target_(nullptr),
        range_(clone->getRange()),
        shotPoint_(clone->getShotPoint()),
        buyPrice_(clone->getBuyPrice()),
        totalPrice_(clone->getTotalPrice())
    {}

    //Other methods
    void update() {
        for (int i = 0; i < shots_.size(); i++) {
            shots_[i].fireIfReady();
        }
    }
    void draw(RenderWindow& window){
        window.draw(sprite_);
    }

    //Getters and setters
    float getRange() { return range_; }
    Vector2f getShotPoint() { return shotPoint_; }
    int getBuyPrice() { return buyPrice_; }
    int getTotalPrice() { return totalPrice_; }

};

//TOWERS ARE BEING REFACTORED. PLEASE BE PATIENT
/*
=====TowerDecorator=====
Standard decorator class for a Tower object

Member Variables:
-A tower pointer that it wraps (called the wrappee)

Member Functions:
-All Tower functions are redirected to the wrappee
-Getters and setters

Notes:
Changes to the Tower class methods require changes to the TowerDecorator, but
the compiler should flag these

class TowerDecorator: public Tower {
private:
    Tower* wrappee;

public:
    //Constructor
    TowerDecorator(Tower* wrappee) {
        this->wrappee = wrappee;
    }

    //Cover the Tower functions
    void updateShot() { wrappee->updateShot(); }
    float getRange() { return wrappee->getRange(); }
    int getBuyPrice() { return wrappee->getBuyPrice(); }
    int getTotalPrice() { return wrappee->getTotalPrice(); }

    //Getters and setters
    Tower* getWrappee() { return wrappee; }
    void setWrappee(Tower* wrappee) { this->wrappee = wrappee; }
};
*/
/*
class TroopTower : public TowerDecorator {
private:
    Shot _troopShot;
    vector<Warrior> _troops;
    Vector2f _rallyPoint;
    float _troopRange;

public:
    //Constructors
    TroopTower(Shot troopShot, Vector2f rallyPoint, float troopRange) {
        _troopShot = troopShot;
        _rallyPoint = rallyPoint;
        _troopRange = troopRange;
    }

    //Other Functions
    void updateShot() {
        wrappee.updateShot();

    }
};
*/

#endif