#pragma once
#include "Slider.h"

class Spinner : public Slider {
protected:
	b2RevoluteJoint *joint;
	b2RevoluteJointDef jointDef;
	int power = 2;
	sf::RectangleShape blade;

	b2Body *bodyMovingB = nullptr;
	b2PolygonShape movingShapeB;
	b2BodyDef bodyDefB;

	b2WeldJoint *weldJoint; //!< A joint to connect the pendulum arm to its weight
	b2WeldJointDef weldJointDef; //!< The weld joint definition

public:
	Spinner() = default;
	Spinner(b2World *world, sf::Vector2f &position, const sf::Vector2f &size, sf::Color col, int direction);
	void update() override; //!< Refresh the SFML drawn object and handle any other functionality that should be performed every game tick
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override; //!< Draw the SFML object
	virtual void sensorEnter() override; //!< Handle action when connected sensor is entered
	virtual void sensorExit() override; //!< Handle action when connected sensor is exited
};