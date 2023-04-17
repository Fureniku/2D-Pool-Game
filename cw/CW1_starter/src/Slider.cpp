#include "Slider.h"

Slider::Slider(b2World *world, sf::Vector2f& position, const sf::Vector2f &anchorPos, const sf::Vector2f &size, sf::Color col, int rot, float range) {
	this->isDynamic = true;
	this->startPos = position.x;
	this->range = range;

	bodyDef.position.Set(position.x, position.y);
	bodyDef.angle = rot * DEG2RAD;

	body = world->CreateBody(&bodyDef);
	bodyMoving = world->CreateBody(&defineBody(position, 0));

	anchorShape.SetAsBox(0.005f, 0.005f);
	anchorShape.m_radius = 0.f;

	movingShape.SetAsBox(size.x * 0.5f, size.y * 0.5f);
	movingShape.m_radius = 0.f;

	body->CreateFixture(&createFixture(&anchorShape, ANCHOR, 0));
	bodyMoving->CreateFixture(&createFixture(&movingShape, SLIDER_PLATFORM, BALL));

	//Joints
	jointDef.bodyA = body;
	jointDef.bodyB = bodyMoving;
	jointDef.collideConnected = false;
	jointDef.localAxisA.Normalize();
	jointDef.localAxisA.Set(1, 0);

	jointDef.bodyB->SetLinearDamping(10.f);

	jointDef.enableMotor = true;
	jointDef.maxMotorForce = 100;
	jointDef.motorSpeed = speed;
	
	joint = (b2PrismaticJoint*)world->CreateJoint(&jointDef);

	//SFML shape definitions
	setPosition(position);
	setSize(size);
	setOrigin(size * 0.5f);
	setRotation(0);
	setFillColor(col);
}

void Slider::update() {
	b2Vec2 pos = bodyMoving->GetPosition();
	setPosition(pos.x, pos.y);

	if (cd > 0) {
		cd--;
	}
	//Invert the direction when the block reaches the edge of the screen
	if (cd == 0) {
		if (pos.x > (startPos + range)) {
			speed = abs(speed) * -1;
			cd = 60;
		}
		else if (pos.x < (startPos - range)) {
			speed = abs(speed);
			cd = 60;
		}
	}

	joint->SetMotorSpeed(speed);

	float angle = bodyMoving->GetAngle() * RAD2DEG;
	setRotation(angle);
}

void Slider::move(float amt) {
	bodyMoving->ApplyForce(b2Vec2(amt, 0), bodyMoving->GetWorldCenter(), true);
}

void Slider::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(static_cast<sf::RectangleShape>(*this));
}