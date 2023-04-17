#pragma once
#include "PhysicalObject.h"
#include <iostream>
#include <math.h>
#include <string>

class DynamicPolygon : public sf::CircleShape, public PhysicalObject {
private:
	int expireTimer = 0; //!< How many frames it has existed for
	int lifeTimer = 0; //!< How many frames it can exist for, set during construction
	bool expired = false; //!< Whether the circle should be removed on the next frame or not
	sf::Font font; //!< A font object for the frame timer
	sf::Texture texture; //!< The texture used for the sprite
	bool useSprite = false; //!< Whether the sprite is used. Set to false if not loaded.
	bool canBePocketed = false; //!< Whether this object should be removed or have its position reset when touching a pocket.
	bool pocketed = false; //!< Whether this object has been pocketed (ready to be hidden on the next game tick)
	sf::Vector2f startPos; //!< The starting/original position, to reset to if needed.
	bool respawn = false; //!< Whether to move back to starting position on next game tick.
	int ballId; //!< The number shown on the object, if applicable. Mainly for debugging.

public:
	DynamicPolygon(b2World *world, sf::Vector2f position, float radius, int sideCount, sf::Texture &texture, int texX, int texY, int sizeX, int sizeY, bool pocketable, int id);
	DynamicPolygon() = default;

	void update(); //!< Called every frame to perform updates on the object.
	void pocket(); //!< Handle what should be done when the ball touches a pocket
	bool isPocketed() { return pocketed; } //!< Check if the ball has been flagged as pocketed.
	int getId() { return ballId; } //!< Return the balls ID for debug purposes
	void draw(sf::RenderTarget &target, sf::RenderStates states) const; //!< Draw the object
	void move(sf::Vector2f direction, float rotation); //!< handle manually controlled motion for the oibject
};