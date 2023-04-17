#pragma once
#include "PhysicalObject.h"
#include <math.h>

class Slider : public sf::RectangleShape, public PhysicalObject {
protected:
	b2Body *bodyMoving = nullptr; //!< A second body in addition to the one in PhysicalObject; this is for the moving part.

	b2PolygonShape anchorShape; //!< The shape for the anchor (non-moving) point
	b2PolygonShape movingShape; //!< The shape for the moving part
	b2BodyDef bodyDef;

	b2PrismaticJoint *joint; //!< The joint itself
	b2PrismaticJointDef jointDef; //!< The joint definition
	float speed = 1.25f; //!< The blocks movement speed, if it has power. Used to set current speed to positive/negative as needed.
	float startPos = 0.f; //!< The position at which the slider starts
	float range = 0.f; //!< How far in each direction the slider can move
	int cd = 0; //!< A cooldown on how quickly the direction change can occur

public:
	Slider() = default;
	Slider(b2World *world, sf::Vector2f &position, const sf::Vector2f &anchorPos, const sf::Vector2f &size, sf::Color col, int rot, float range);
	void update(); //!< Update function called every frame
	void move(float amount); //!< Apply physics to a block; used for the controllable slider.
	void sensorEnter() override {} //!< Handle action when connected sensor is entered
	void sensorExit() override {} //!< Handle action when connected sensor is exited
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override; //!< Draw the SFML object
};
