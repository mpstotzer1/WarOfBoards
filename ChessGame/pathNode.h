#pragma once
#ifndef MISC_H
#define MISC_H

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

/*
A node in the path. Creeps follow these
*/
class pathNode {
private:
    Vector2f coord_; //Position of node
    pathNode* nextNode_; //next node in path

public:
    pathNode(Vector2f coord, pathNode* nextNode) :
        coord_(coord),
        nextNode_(nextNode)
    {}

    //getters and setters
    Vector2f getCoord() { return coord_; }
    pathNode* getNextNode() { return nextNode_; }
};


#endif