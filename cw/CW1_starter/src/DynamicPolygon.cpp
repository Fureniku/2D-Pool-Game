#include "DynamicPolygon.h"

DynamicPolygon::DynamicPolygon(b2World *world, sf::Vector2f position, float radius, int sideCount, sf::Texture &texture, int texX, int texY, int sizeX, int sizeY, bool pocketable, int id) {
	this->isDynamic = true;
	this->canBePocketed = pocketable;
	this->startPos = position;
	this->ballId = id;

	body = world->CreateBody(&defineBody(position, 0));

	//Ensure a valid shape - if not, it'll default to a circle.
	if (sideCount > 2) {
		this->setPointCount(sideCount);
	}

	//SFML shape definitions
	setTexture(&texture);
	setTextureRect(sf::IntRect(texX, texY, sizeX, sizeY));
	setPosition(position);
	setRadius(radius);
	setOrigin(radius, radius);

	body->SetLinearDamping(0.5f);
	body->SetAngularDamping(0.5f);

	//If we have a custom point count, create a Box2d shape based off the SFML shape
	if (sideCount > 2) {
		b2PolygonShape shape;

		b2Vec2 *verts = new b2Vec2[sideCount];

		//Iterate through the SFML sides, creating a b2d collider
		for (int i = 0; i < sideCount; i++) {
			verts[i] = b2Vec2(this->getPoint(i).x - radius, this->getPoint(i).y - radius);
		}

		shape.Set(verts, sideCount);
		body->CreateFixture(&createFixture(&shape, BALL, BALL | BOUNDARY_WALL | SLIDER_PLATFORM | TRAPDOOR));

		//Clean up memory used earlier
		delete[] verts;
	}
	else {
		//Default to a circle
		b2CircleShape shape;
		shape.m_radius = radius;
		body->CreateFixture(&createFixture(&shape, BALL, BALL | BOUNDARY_WALL | SLIDER_PLATFORM | TRAPDOOR));
	}

	if (!font.loadFromFile("assets/cour.ttf")) { //Courier New font https://fontzone.net/download/courier-new
		std::cout << "Unable to load font for circle" << std::endl;
	}
}

void DynamicPolygon::pocket() {
	if (canBePocketed) {
		std::cout << "pocketed " << ballId << std::endl;
		pocketed = true;

		b2Filter filter;
		filter.categoryBits = 0;
		filter.maskBits = 0;

		body->GetFixtureList()->SetFilterData(filter);
	}
	else {
		std::cout << "Respawn?" << std::endl;
		respawn = true;
	}
}

void DynamicPolygon::update() {
	if (respawn) {
		body->SetTransform(b2Vec2(startPos.x, startPos.y), 0);
		body->SetAngularVelocity(0);
		body->SetLinearVelocity(b2Vec2(0, 0));
		respawn = false;
	}
	b2Vec2 pos = body->GetPosition();
	setPosition(pos.x, pos.y);
	float angle = body->GetAngle() * RAD2DEG;
	setRotation(angle);

	if (lifeTimer > 0 && !expired) {
		expireTimer++;
		if (expireTimer > lifeTimer) {
			expired = true;
		}
	}
}

void DynamicPolygon::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw((sf::CircleShape)*this); // Draw the Circle Shape
}

void DynamicPolygon::move(sf::Vector2f direction, float rotation) {
	body->SetLinearVelocity(b2Vec2(direction.x, direction.y));
	body->SetAngularVelocity(rotation);
}