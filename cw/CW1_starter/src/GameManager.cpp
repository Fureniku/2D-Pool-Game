#include "GameManager.h"

GameManager::GameManager() {
	std::cout << "Starting to create game manager" << std::endl;
	view.setCenter(0.f, 0.f);
	view.setSize(worldSize);

	world = new b2World(gravity);

	debugDraw.setWorld(world);

	std::cout << "Fonts" << std::endl;
	if (!font.loadFromFile("assets/cour.ttf")) { //Courier New font https://fontzone.net/download/courier-new
		std::cout << "Unable to load font" << std::endl;
	}

	if (!texture.loadFromFile("assets/spritesheet.png")) {
		std::cout << "Unable to load textures" << std::endl;
	}
	std::cout << "Fonts complete. Texts:" << std::endl;

	initText(counterText, -(worldSize.x / 2) + 0.1f, (worldSize.y / 2) - 0.3f, 1.f);
	initText(helperText, -(worldSize.x / 2) + 0.1f, (worldSize.y / 2) - 2.25f, 0.75f);
	initText(sizeText, -(worldSize.x / 2) + 5.f, (worldSize.y / 2) - 6.f, 0.5f);

	helperText.setString("WASD to move cube, Q/E to rotate cube \nLeft/Right Arrow Keys to aim ball \nHold space to charge shot, release to fire");
	std::cout << "Texts complete. start game world creation" << std::endl;
	createGameWorld();
}

GameManager::~GameManager() {
	// Clean up all pointers
	delete world;
	world = nullptr;
}

void GameManager::initText(sf::Text &text, float posX, float posY, float scale) {
	text.setFont(font);
	text.setCharacterSize(25);
	text.setPosition(posX, posY);
	text.setOrigin(0.f, 0.f);
	text.setScale(0.01f * scale, 0.01f * scale);
	text.setFillColor(sf::Color::Black);
}

void GameManager::createGameWorld() {
	//The slider
	magentaSlider = Slider(world, sf::Vector2f(3.5f, 0.5f), sf::Vector2f(0.f, 0.5f), sf::Vector2f(1.f, 0.125f), sf::Color::Magenta, -45, 0.8f);
	objects.push_back(&magentaSlider);

	//Spinners
	spinner1 = Spinner(world, sf::Vector2f(2.85f, -2.85f), sf::Vector2f(1.35f, 0.1f), sf::Color::Blue, 1);
	spinner2 = Spinner(world, sf::Vector2f(3.85f, -1.85f), sf::Vector2f(1.35f, 0.1f), sf::Color::Blue, -1);
	spinners.push_back(&spinner1);
	spinners.push_back(&spinner2);
	objects.push_back(&spinner1);
	objects.push_back(&spinner2);

	//Trapdoor
	leftTrapdoor  = Trapdoor(world, sf::Vector2f(-3.9f, -0.2f), sf::Vector2f(0.75f, 0.1f), sf::Color(255, 127, 0), 45, true);
	rightTrapdoor = Trapdoor(world, sf::Vector2f(-2.8f, 0.9f), sf::Vector2f(0.75f, 0.1f), sf::Color(255, 127, 0), 45, false);
	trapdoors.push_back(&leftTrapdoor);
	trapdoors.push_back(&rightTrapdoor);
	objects.push_back(&leftTrapdoor);
	objects.push_back(&rightTrapdoor);

	std::cout << "done slider, spinners, trapdoor" << std::endl;

	//Controlled ball & ray it casts
	whiteBall = WhiteBall(world, sf::Vector2f(-2.75f, -1.f), 0.15f, texture);
	whiteBall.setUserData(new std::pair<std::string, void*>(typeid(decltype(whiteBall)).name(), &whiteBall));
	ray = Ray(world, sf::Color::Red);
	objects.push_back(&whiteBall);

	//Sensors
	trapdoorSensor = Sensor(world, sf::Vector2f(0.f, -2.5f), sf::Vector2f(0.25f, 0.25f), sf::Color(255, 127, 0), trapdoors);
	spinnerSensor = Sensor(world, sf::Vector2f(0.f, -1.f), sf::Vector2f(0.5f, 0.25f), sf::Color::Blue, spinners);
	trapdoorSensor.setUserData(new std::pair<std::string, void*>(typeid(decltype(trapdoorSensor)).name(), &trapdoorSensor));
	spinnerSensor.setUserData(new std::pair<std::string, void*>(typeid(decltype(spinnerSensor)).name(), &spinnerSensor));
	objects.push_back(&trapdoorSensor);
	objects.push_back(&spinnerSensor);

	std::cout << "done whiteball, sensors" << std::endl;

	//Pockets
	tlPocket = Sensor(world, sf::Vector2f(-3.75f, -2.75f), sf::Vector2f(0.25f, 0.25f), sf::Color(255, 255, 0));
	trPocket = Sensor(world, sf::Vector2f( 3.75f, -2.75f), sf::Vector2f(0.25f, 0.25f), sf::Color(255, 255, 0));
	blPocket = Sensor(world, sf::Vector2f(-3.75f,  0.75f), sf::Vector2f(0.25f, 0.25f), sf::Color(255, 255, 0));
	brPocket = Sensor(world, sf::Vector2f( 3.75f,  0.75f), sf::Vector2f(0.25f, 0.25f), sf::Color(255, 255, 0));

	tlPocket.setUserData(new std::pair<std::string, void*>(typeid(decltype(tlPocket)).name(), &tlPocket));
	trPocket.setUserData(new std::pair<std::string, void*>(typeid(decltype(trPocket)).name(), &trPocket));
	blPocket.setUserData(new std::pair<std::string, void*>(typeid(decltype(blPocket)).name(), &blPocket));
	brPocket.setUserData(new std::pair<std::string, void*>(typeid(decltype(brPocket)).name(), &brPocket));

	objects.push_back(&tlPocket);
	objects.push_back(&trPocket);
	objects.push_back(&blPocket);
	objects.push_back(&brPocket);

	std::cout << "done pockets" << std::endl;

	//Controlled block
	controlledBlock = DynamicPolygon(world, sf::Vector2f(0.f, 0.f), 0.25f, 4, texture, 0, 896, 128, 128, false, -1);
	controlledBlock.setUserData(new std::pair<std::string, void*>(typeid(decltype(controlledBlock)).name(), &controlledBlock));
	objects.push_back(&controlledBlock);

	/*
	*   GAME WORLD
	*/

	//Create the game world borders
	staticBlocks.reserve(4); //Pre-allocation. Not sure what the demo code meant about loading from a file, don't think we've ever covered that...?
	staticBlocks.push_back(Block(world, sf::Vector2f(-4.f, -1.f), sf::Vector2f(0.25f, 4.25f), texture, 0, 0, 30, 528)); //left side
	staticBlocks.push_back(Block(world, sf::Vector2f(4.f, -1.f), sf::Vector2f(0.25f, 4.25f), texture, 994, 0, 30, 528)); //right side
	staticBlocks.push_back(Block(world, sf::Vector2f(0.f, 1.f), sf::Vector2f(8.25f, 0.25f), texture, 0, 498, 1024, 30)); //bottom
	staticBlocks.push_back(Block(world, sf::Vector2f(0.f, -3.f), sf::Vector2f(8.25f, 0.25f), texture, 0, 0, 1024, 30)); //top
	
	//Set custom data for the static blocks
	for (Block &block : staticBlocks) block.setUserData(new std::pair<std::string, void*>(typeid(decltype(block)).name(), &block));

	sf::RectangleShape maskTop;
	sf::RectangleShape maskLeft;
	sf::RectangleShape maskRight;
	sf::RectangleShape maskBottom;

	background.setSize(sf::Vector2f(8.25f, 4.25f));
	background.setPosition(sf::Vector2f(0.f, -1.f));
	background.setOrigin(background.getSize().x / 2.f, background.getSize().y / 2.f);
	background.setTexture(&texture);
	background.setTextureRect(sf::IntRect(29, 30, 965, 468));

	chargeBg.setSize(sf::Vector2f(1.28f, 0.64f));
	chargeBg.setPosition(sf::Vector2f(2.8f, 1.2f));
	//chargeBg.setOrigin(chargeBg.getSize().x / 2.f, chargeBg.getSize().y / 2.f);
	chargeBg.setTexture(&texture);
	chargeBg.setTextureRect(sf::IntRect(512, 896, 256, 128));

	chargeBar.setSize(sf::Vector2f(1.28f, 0.64f));
	chargeBar.setPosition(sf::Vector2f(2.8f, 1.2f));
	//chargeBar.setOrigin(chargeBar.getSize().x / 2.f, chargeBar.getSize().y / 2.f);
	chargeBar.setTexture(&texture);
	chargeBar.setTextureRect(sf::IntRect(768, 896, 256, 128));
	
	//Create the mask which covers screen edges to hide moving parts that are "off screen"
	maskTop.setSize(sf::Vector2f(worldSize.x, 2.f));
	maskLeft.setSize(sf::Vector2f(1.f, worldSize.y));
	maskRight.setSize(sf::Vector2f(1.f, worldSize.y));
	maskBottom.setSize(sf::Vector2f(worldSize.x, 3.f));

	maskTop.setPosition(sf::Vector2f(0.f, -4.f));
	maskLeft.setPosition(sf::Vector2f(-worldSize.x/2.f, 0.f));
	maskRight.setPosition(sf::Vector2f(worldSize.x/2.f, 0.f));
	maskBottom.setPosition(sf::Vector2f(0.f, 2.5f));

	masks.push_back(maskTop);
	masks.push_back(maskLeft);
	masks.push_back(maskRight);
	masks.push_back(maskBottom);

	for (int i = 0; i < masks.size(); i++) {
		masks[i].setOrigin(sf::Vector2f(masks[i].getSize().x / 2.f, masks[i].getSize().y / 2.f));
		masks[i].setFillColor(maskCol);
	}

	std::cout << "done gameworld and masks" << std::endl;

	/*
	*   BALLS
	*/
	//Parameters for creating balls easily
	float start = 1.5f;
	float diam = 0.25f;
	float space = 0.025f;
	//Create the balls
	balls.push_back(DynamicPolygon(world, sf::Vector2f(start, 0.f - 1.f), diam / 2, -1, texture, 0, 656, 128, 128, true, 9)); //9/yellow stripe
	
	balls.push_back(DynamicPolygon(world, sf::Vector2f(start + diam + space, (diam / 2) + space - 1.f), diam / 2, -1, texture, 768, 528, 128, 128, true, 7)); //7/maroon
	balls.push_back(DynamicPolygon(world, sf::Vector2f(start + diam + space, -(diam / 2) - space - 1.f), diam / 2, -1, texture, 384, 656, 128, 128, true, 12)); //12/violet stripe
	
	balls.push_back(DynamicPolygon(world, sf::Vector2f(start + diam*2 + space*2, diam + space - 1.f), diam / 2, -1, texture, 768, 656, 128, 128, true, 15)); //15/maroon stripe
	balls.push_back(DynamicPolygon(world, sf::Vector2f(start + diam*2 + space*2, 0.f - 1.f), diam / 2, -1, texture, 896, 528, 128, 128, true, 8)); //8/black
	balls.push_back(DynamicPolygon(world, sf::Vector2f(start + diam*2 + space*2, -diam - space - 1.f), diam / 2, -1, texture, 0, 528, 128, 128, true, 1)); //1/yellow
	
	balls.push_back(DynamicPolygon(world, sf::Vector2f(start + diam*3 + space*3, diam*1.6 + space - 1.f), diam / 2, -1, texture, 640, 528, 128, 128, true, 6)); //6/green
	balls.push_back(DynamicPolygon(world, sf::Vector2f(start + diam*3 + space*3, diam*0.5 + space - 1.f), diam / 2, -1, texture, 128, 656, 128, 128, true, 10)); //10/blue stripe
	balls.push_back(DynamicPolygon(world, sf::Vector2f(start + diam*3 + space*3, -diam*0.5 - space - 1.f), diam / 2, -1, texture, 256, 528, 128, 128, true, 3)); //3/red
	balls.push_back(DynamicPolygon(world, sf::Vector2f(start + diam*3 + space*3, -diam*1.6 - space - 1.f), diam / 2, -1, texture, 640, 656, 128, 128, true, 14)); //14/green stripe

	balls.push_back(DynamicPolygon(world, sf::Vector2f(start + diam*4 + space*4, diam*2 + space*2 - 1.f), diam / 2, -1, texture, 256, 656, 128, 128, true, 11)); //11/red stripe
	balls.push_back(DynamicPolygon(world, sf::Vector2f(start + diam*4 + space*4, diam*1 + space - 1.f), diam / 2, -1, texture, 128, 528, 128, 128, true, 2)); //2/blue
	balls.push_back(DynamicPolygon(world, sf::Vector2f(start + diam*4 + space*4, 0              - 1.f), diam / 2, -1, texture, 512, 656, 128, 128, true, 13)); //13/orange stripe
	balls.push_back(DynamicPolygon(world, sf::Vector2f(start + diam*4 + space*4, -diam*1 - space - 1.f), diam / 2, -1, texture, 384, 528, 128, 128, true, 4)); //4/violet
	balls.push_back(DynamicPolygon(world, sf::Vector2f(start + diam*4 + space*4, -diam*2 - space*2 - 1.f), diam / 2, -1, texture, 512, 528, 128, 128, true, 5)); //5/orange

	//Set custom data for the balls
	for (DynamicPolygon &ball : balls) ball.setUserData(new std::pair<std::string, void*>(typeid(decltype(ball)).name(), &ball));

	world->SetContactListener(&listener);

	std::cout << "done balls, gameworld created" << std::endl;
}



void GameManager::tick(float timestep) {
	// Update the world
	world->Step(timestep, velIterations, velIterations);

	int unpocketed = 0;
	for (int i = 0; i < balls.size(); i++) {
		DynamicPolygon &ball = balls.at(i);
		if (!ball.isPocketed()) {
			ball.update();
			unpocketed++;
		}
	}

	pocketed = balls.size() - unpocketed;

	for (int i = 0; i < objects.size(); i++) {
		objects[i]->update();
	}

	if (whiteBall.isStationary()) {
		ray.setDirection(whiteBall.getDirection());
		ray.cast(whiteBall.getPosition());
	}

	//Set the text objects
	counterText.setString("Balls pocketed: " + std::to_string(pocketed) + "/" + std::to_string(balls.size()));

	// Delete debug shapes
	if (debug) debugDraw.clear();
}

void GameManager::mouseClickHandler(sf::Vector2i mousePosIn, sf::RenderWindow &window, bool leftClick) {
	sf::Vector2f mousePos = window.mapPixelToCoords(mousePosIn);
}

void GameManager::keyHandler(sf::Vector2f moveAmount, float rotation, float wbDir) {
	controlledBlock.move(moveAmount, rotation);
	whiteBall.updateDirection(wbDir);
}

void GameManager::chargeWhiteBall(bool keyDown) {
	if (whiteBall.isStationary()) {
		if (keyDown) {
			if (power < 1.f) {
				power += 0.01f;
				std::cout << "Charging... " << power << std::endl;
			}
			if (power >= 1.f) {
				power = 1.f;
				std::cout << "Charged! " << power << std::endl;
			}
		}
		else {
			if (power > 0.f) {
				whiteBall.simulateCueImpact(power);
			}
			power = 0.f;
		}
	}
	chargeBar.setSize(sf::Vector2f(1.28f * power, 0.64f));
	chargeBar.setTextureRect(sf::IntRect(768, 896, ceil(256 * power), 128));
}

void GameManager::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	// Set the view
	target.setView(view);
	target.draw(background);

	
	// Draw everything
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->draw(target, states);
	}

	for (sf::RectangleShape rect : masks) target.draw(rect);
	for (Block block : staticBlocks) target.draw(block);
	for (DynamicPolygon ball : balls) {
		if (!ball.isPocketed()) {
			ball.draw(target, states);
		}
	}

	if (whiteBall.isStationary()) target.draw(ray);

	target.draw(chargeBg);
	target.draw(chargeBar);

	target.draw(counterText);
	target.draw(helperText);
	target.draw(sizeText);

	// Debug Draw
	if (debug) target.draw(debugDraw);
}