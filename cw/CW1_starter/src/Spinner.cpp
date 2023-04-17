#include "Spinner.h"

Spinner::Spinner(b2World *world, sf::Vector2f& position, const sf::Vector2f &size, sf::Color col, int direction) {
	this->isDynamic = true;

	bodyDef.position.Set(position.x, position.y);
	bodyDef.angle = 0 * DEG2RAD;

	body = world->CreateBody(&bodyDef);
	bodyMoving = world->CreateBody(&defineBody(position, 0));
	bodyMovingB = world->CreateBody(&defineBody(position, 0));

	anchorShape.SetAsBox(0.005f, 0.005f);
	anchorShape.m_radius = 0.f;

	movingShape.SetAsBox(size.x * 0.5f, size.y * 0.5f);
	movingShape.m_radius = 0.f;
	movingShapeB.SetAsBox(size.y * 0.5f, size.x * 0.5f);
	movingShapeB.m_radius = 0.f;

	body->CreateFixture(&createFixture(&anchorShape, ANCHOR, 0));
	bodyMoving->CreateFixture(&createFixture(&movingShape, SLIDER_PLATFORM, BALL));
	bodyMovingB->CreateFixture(&createFixture(&movingShapeB, SLIDER_PLATFORM, BALL));

	jointDef.bodyA = body;
	jointDef.bodyB = bodyMoving;
	jointDef.collideConnected = false;
	jointDef.localAnchorA.Set(0, 0);
	jointDef.localAnchorB.Set(0, 0);

	jointDef.bodyB->SetAngularDamping(1.0f);

	jointDef.enableMotor = true;
	jointDef.maxMotorTorque = 1000;
	jointDef.motorSpeed = power * direction;

	joint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);

	weldJointDef.bodyA = bodyMoving;
	weldJointDef.bodyB = bodyMovingB;
	weldJointDef.collideConnected = false;
	weldJointDef.localAnchorA.Set(0, 0);
	weldJointDef.localAnchorB.Set(0, 0);

	weldJoint = (b2WeldJoint*)world->CreateJoint(&weldJointDef);

	//SFML shape definitions
	setPosition(position);
	setSize(size);
	setOrigin(size * 0.5f);
	setRotation(bodyMoving->GetAngle() * RAD2DEG);
	setFillColor(col);

	blade.setPosition(position);
	blade.setSize(size);
	blade.setOrigin(size * 0.5f);
	blade.setRotation((bodyMovingB->GetAngle()) * RAD2DEG);
	blade.setFillColor(col);
}

void Spinner::update() {
	b2Vec2 pos = bodyMoving->GetPosition();
	b2Vec2 posB = bodyMovingB->GetPosition();
	setPosition(pos.x, pos.y);
	blade.setPosition(posB.x, posB.y);

	setRotation(bodyMoving->GetAngle() * RAD2DEG);
	blade.setRotation((bodyMovingB->GetAngle() * RAD2DEG) + 90);
}

void Spinner::sensorEnter() {
	std::cout << "Invert direction" << std::endl;
	joint->SetMotorSpeed(joint->GetMotorSpeed() * -1);
}

void Spinner::sensorExit() {}

void Spinner::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(static_cast<sf::RectangleShape>(*this));
	target.draw(blade);
}