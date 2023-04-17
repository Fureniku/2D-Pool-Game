#pragma once
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <iostream>

#define DEG2RAD 0.017453f
#define RAD2DEG 57.29577f

class PhysicalObject {
protected:
	b2Body *body = nullptr; //!< The base body pointer for any given object
	float density = 1.0f; //!< Default density
	float friction = 0.4f; //!< Default friction
	float restitution = 0.6f; //!< Default restitution
	bool isDynamic = false; //!< Whether the object has physics applied or not
public:
	b2FixtureDef createFixture(b2Shape *shape, uint16 catBits, uint16 maskBits); //!< Handle common code for creating fixture definitions. \return A fixture
	b2FixtureDef createFullFixture(b2Shape *shape, bool isSensor, uint16 catBits, uint16 maskBits); //!< Handle common code for creating fixture definitions. \return A fixture
	b2BodyDef defineBody(sf::Vector2f position, float orientation); //!< Handle common code for creating body definitions. \return A bodyDef
	b2Body *getBody(); //!< Getter for the protected body pointer
	void setUserData(void* data) { body->SetUserData(data); }; //!< Save data to the object for use with collision listener
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const {}
	virtual void sensorEnter() {} //!< Polymorphic sensor handling for when a connected sensor is entered
	virtual void sensorExit() {} //!< Polymorphic sensor handling for when a connected sensor is exited
	virtual void update() {}
};

//! Enum for different bit types for the colliders. Kept in here for ease of access throughout the code; all physical objects need this information.
enum colliderCategory {
	BOUNDARY_WALL = 0x0001,
	BALL = 0x0002,
	SLIDER_PLATFORM = 0x0004,
	ANCHOR = 0x0008,
	TRAPDOOR = 0x0016,
};