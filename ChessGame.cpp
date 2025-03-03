// ChessGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//Because I have literally no idea how to manage classes in C++ (one file per class doesn't work here)
//I am just including them all in this one, monstrously long document and praying that dropdown arrows
//can make it readable. Please send help.

#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

/*
An enum detailing the possible alliegence of a GameObject.
This is used primarily for finding enemies and avoiding friendly fire.
*/
enum class Side {TOWER, CREEP, NEUTRAL};

/*
An enum of whether an object should exist any more.
This is used for checking, in a way, if an "object != NULL".
Note that ontology is a branch of metaphysics concerning existence and being.
DNE stands for "Does Not Exist"
*/
enum class Ontology { EXIST, DNE };

/*
=====GameObject=====
Defines a GameObject class with some basic usefulness:

Member Variables:
-Texture
-Sprite
-An num for side
-An enum for whether it should exist or not

Functions:
-Default and paramter constructors
    -Automatically sets the orgin to the "root point" of the sprite (aka the middle bottom)
    -Coordinates of the sprite can be accessed through the sprite itself
-A method to set the object's Ontology to DNE
-A method to tell if a given GameObject is an enemy
-A method to quickly calculate the center of the sprite
-Getters and setters

NOTES:
-Apparantly, an object is destroyed and the memory is freed once it exits the scope
-This means I can safely create GameObjects and trust that they will be deleted
-However, if I create a GameObject and put it in a vector, destroying the original won't affect the vector's copy.
-Resultantly, the logic running through the vectors will be responsible ONLY for removing the GameObject
 from the vector. It will NOT have to destroy the GameObject itself--the vector scope handles it.
-If you don't mess with pointers or malloc/calloc/new, C++ will handle memory fine?
*/
class GameObject {
private:
    Texture texture;
    Sprite sprite;
    Side side; //Which side it's on
    Ontology ontology; //If it exists

public:
    //Some constructors
    GameObject() {
        //Set the texture to a "null" image and stuff it into the sprite
        texture.loadFromFile("Graphics/null.png");
        sprite.setTexture(texture);
        sprite.setPosition(0, 0);

        //Set origin to middle bottom of sprite
        FloatRect spriteBounds = sprite.getGlobalBounds();
        float middle = spriteBounds.left + (spriteBounds.width / 2);
        float bottom = spriteBounds.top + (spriteBounds.height);
        sprite.setOrigin(middle, bottom);

        //Default side is NEUTRAL
        side = Side::NEUTRAL;
        ontology = Ontology::EXIST;
    }
    GameObject(Vector2f position, Texture texture, Sprite sprite, Side side) {
        this->texture = texture;
        this->sprite = sprite;
        this->sprite.setPosition(position);

        //Set origin to middle bottom of sprite
        FloatRect spriteBounds = sprite.getGlobalBounds();
        float middle = spriteBounds.left + (spriteBounds.width / 2);
        float bottom = spriteBounds.top + (spriteBounds.height);
        sprite.setOrigin(middle, bottom);

        this->side = side;
        this->ontology = Ontology::EXIST;
    }

    //Various other methods
    void destroy() { //A sanitized substitute for and Ontology setter
        ontology = Ontology::DNE;
    }

    bool isEnemy(GameObject other) { //Returns true if passed GameObject is an enemy and false if friendly or NEUTRAL
        if (other.getSide() == this->side) {
            return false;
        }
        else if(other.getSide() == Side::NEUTRAL) {
            return false;
        }
        
        return true;
    }

    Vector2f getCenter() {
        FloatRect spriteBounds = sprite.getGlobalBounds();
        Vector2f center(spriteBounds.left + (spriteBounds.width / 2),
                        spriteBounds.top + (spriteBounds.height / 2));
        return center;
    }


    //Getters and setters
    Texture getTexture() { return texture; }
    Sprite getSprite() { return sprite; }
    Side getSide() { return side; }
    Ontology getOntology() { return ontology; }

    void setTexture(Texture texture) { this->texture = texture; }
    void setSprite(Sprite sprite) { this->sprite = sprite; }
};

int main()
{
    GameObject GO;

    Clock clock;

    Time dt = clock.getElapsedTime();

    std::cout << dt.asSeconds();
}
