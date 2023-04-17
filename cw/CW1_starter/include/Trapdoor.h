#pragma once
#include "Slider.h"
class Trapdoor : public Slider {

protected:
	b2RevoluteJoint *joint;
	b2RevoluteJointDef jointDef;

	int timer = 0; //!< Timer for how long before the trapdoor changes state. 
	int timeMax = 180; //!< Target for timer to reach (default 3 seconds)
	bool closed = true; //!< True when closed, false when open. Used for box2d bug workaround.
	bool isLeft = false; //!< True for left, false for right

public:
	Trapdoor() = default;
	Trapdoor(b2World *world, sf::Vector2f &position, const sf::Vector2f &size, sf::Color col, int rot, bool isLeft);
	void update() override; //!< Update function called every frame
	virtual void sensorEnter() override; //!< Handle action when connected sensor is entered
	virtual void sensorExit() override; //!< Handle action when connected sensor is exited
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override; //!< Draw the SFML object
};
