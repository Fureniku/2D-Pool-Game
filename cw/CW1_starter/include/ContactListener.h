#pragma once
#include <Box2D\Box2D.h>
#include "Block.h"
#include "Sensor.h"

class ContactListener : public b2ContactListener {
public:
	void BeginContact(b2Contact* contact); //!< Called when two fixtures begin to touch
	void EndContact(b2Contact* contact); //!< Called when two fixtures cease to touch
	void HandleContact(b2Contact* contact, sf::Color col, bool enter); //!< Handle any functionality that's shared between begin/end (eg change colour)
};