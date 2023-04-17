#include "Trapdoor.h"

Trapdoor::Trapdoor(b2World *world, sf::Vector2f& position, const sf::Vector2f &size, sf::Color col, int rot, bool isLeft) {
	this->isDynamic = true;
	this->isLeft = isLeft;

	bodyDef.position.Set(position.x, position.y);
	bodyDef.angle = 0 * DEG2RAD;

	body = world->CreateBody(&bodyDef);
	bodyMoving = world->CreateBody(&defineBody(position, 0));

	anchorShape.SetAsBox(0.01f, 0.01f);
	anchorShape.m_radius = 0.f;

	movingShape.SetAsBox(size.x * 0.5f, size.y * 0.5f);
	movingShape.m_radius = 0.f;

	body->CreateFixture(&createFixture(&anchorShape, ANCHOR, 0));
	bodyMoving->CreateFixture(&createFixture(&movingShape, TRAPDOOR, BALL));

	jointDef.bodyA = body;
	jointDef.bodyB = bodyMoving;
	jointDef.collideConnected = false;
	jointDef.localAnchorB.Set(isLeft ? -(size.x / 2) : size.x / 2, 0);

	jointDef.bodyB->SetAngularDamping(1.0f);

	jointDef.enableMotor = true;
	jointDef.maxMotorTorque = 100;
	jointDef.motorSpeed = isLeft ? -360 * DEG2RAD : 360 * DEG2RAD;

	jointDef.enableLimit = true;
	jointDef.upperAngle = isLeft ? (45 + rot) * DEG2RAD : rot * DEG2RAD;
	jointDef.lowerAngle = isLeft ? rot * DEG2RAD : (-45 + rot) * DEG2RAD;

	joint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);

	//SFML shape definitions
	setPosition(position);
	setSize(size);
	setOrigin(size * 0.5f);
	setRotation(body->GetAngle() * RAD2DEG);
	setFillColor(col);
}

void Trapdoor::update() {
	b2Vec2 pos = bodyMoving->GetPosition();
	setPosition(pos.x, pos.y);
	float angle = bodyMoving->GetAngle() * RAD2DEG;
	setRotation(angle);

	timer++;

	if (timer >= 5 && !closed) {
		joint->EnableMotor(true);
		timer = 0;
	}
}

//Hard set the numbers for motor speed instead of just inverting, in case multiple things hit the sensor.
//We don't want it getting stuck open, makes the game too easy!
void Trapdoor::sensorEnter() {
	joint->SetMotorSpeed(isLeft ? 360 * DEG2RAD : -360 * DEG2RAD);

	//50% chance of one joint getting stuck without this because box2d angle limits just seem buggy...
	joint->EnableMotor(false);
	closed = false;
	timer = 0;
}

void Trapdoor::sensorExit() {
	joint->SetMotorSpeed(isLeft ? -360 * DEG2RAD : 360 * DEG2RAD);

	closed = true;
}

void Trapdoor::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(static_cast<sf::RectangleShape>(*this));
}