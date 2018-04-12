#include "Game.h"

void render(sf::RenderWindow& window, MemoryManager& mgr) {
	while (window.isOpen()) {
		window.clear();
		mgr.draw(window);
		window.display();
		sf::sleep(sf::milliseconds(10));
	}
}

void animate(sf::RenderWindow& window, MemoryManager& mgr, GameManager& gmgr) {
	sf::Clock clock;
	while (window.isOpen()) {
		mgr.animateSprites(clock.restart());
		mgr.updateBlur(gmgr.getPlayer()->getSpriteSet()->getFeetPos());
		sf::sleep(sf::milliseconds(50));
	}
}

void action(sf::RenderWindow& window, MemoryManager& mgr, GameManager& gmgr) {
	sf::Clock clock;
	while (window.isOpen()) {
		gmgr.playActors(clock.restart());
		sf::sleep(sf::milliseconds(20));
	}
}


int main()
{
	Game::Window::Width = 711;
	Game::Window::Height = 400;
	Game::Window::widthScrollLimit = 0.1f;
	Game::Window::heightScrollLimit = 0.1f;

	srand(time(NULL));

	sf::RenderWindow window(sf::VideoMode(Game::Window::Width, Game::Window::Height), "SFML works!");
	//sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML works!", sf::Style::Fullscreen);
	window.setActive(false);
	window.setKeyRepeatEnabled(false);
	window.setMouseCursorVisible(false);
	

	Game::Window::Width = window.getSize().x;
	Game::Window::Height = window.getSize().y;

	MemoryManager mgr;
	GameManager gmgr(mgr);

	for (unsigned int i = 0; i< sf::Joystick::Count; ++i)
	{
		if (sf::Joystick::isConnected(i))
			std::cout << "Joystick " << i << " is connected!" << std::endl;
	}

	mgr.addTexture("Assets/Image.bmp", 100);
	mgr.addTexture("Assets/grass_2.png", 101);
	mgr.addTexture("Assets/male_patched.png", 102);
	mgr.addTexture("Assets/pants_patched.png", 103);
	mgr.addTexture("Assets/gloves_patched.png", 104);
	mgr.addTexture("Assets/torso_patched.png", 105);
	mgr.addTexture("Assets/male_hair_patched.png", 108);
	mgr.addTexture("Assets/treetrunk.png", 106);
	mgr.addTexture("Assets/treetop.png", 107);

	for (int i = 0; i < 10; i++) {
		AssetCreator::createTreeSprite(mgr, 106, 107, 30, 65, vec2f(i*100, -300), 250 + i, 0.2);
	}

	// player
	AssetCreator::createSpriteSet(mgr, 102, { { 103, sf::Color::White },{ 104, sf::Color::White },{ 105, sf::Color::White },{ 108, sf::Color::Cyan } }, sf::IntRect(0, 0, 64, 64), vec2f(100, 300), sf::milliseconds(100), 270);
	gmgr.setPlayer(gmgr.addActor(270, vec2f(10, 100), 3.f, 300));

	// npc
	AssetCreator::createSpriteSet(mgr, 102, { { 103, sf::Color::Red },{ 104, sf::Color::White },{ 105, sf::Color::White },{ 108, sf::Color::Green } }, sf::IntRect(0, 0, 64, 64), vec2f(100, 300), sf::milliseconds(100), 213);
	gmgr.addNPC(213, vec2f(300, 350), 0.5, 301);


	mgr.addRepeatedSprite(101, 5000, 5000, vec2f(-1000, -1000), 203);
	mgr.getSprite(203).setLayer(-1);

	std::thread renderThread(render, std::ref(window), std::ref(mgr));
	std::thread animateThread(animate, std::ref(window), std::ref(mgr), std::ref(gmgr));
	std::thread actionThread(action, std::ref(window), std::ref(mgr), std::ref(gmgr));

	sf::Clock clock;
	vec2f playerMovement(0, 0);
	while (window.isOpen())
	{
		sf::Event event;
		sf::Time elapsed = clock.restart();
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
			}
		}
		playerMovement = vec2f(0, 0);

		sf::Joystick::update();

		if (sf::Joystick::isConnected(2)) {
			playerMovement.x = sf::Joystick::getAxisPosition(2, sf::Joystick::Axis::X)/100;
			playerMovement.y = sf::Joystick::getAxisPosition(2, sf::Joystick::Axis::Y)/100;
			/*for (int i = 0; i < sf::Joystick::getButtonCount(2); i++) {
				std::cout << sf::Joystick::isButtonPressed(2, i) << " - ";
			}
			std::cout << std::endl;*/
			for (unsigned int i = 0; i < sf::Joystick::getButtonCount(2); i++) {
				if (sf::Joystick::isButtonPressed(2, i)) { std::cout << "Button " << i << " pressed !" << std::endl; }
			}
		}
		else {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) { playerMovement.y--; }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) { playerMovement.x--; }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { playerMovement.y++; }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { playerMovement.x++; }
		}


		gmgr.movePlayer(playerMovement, elapsed);

		sf::sleep(sf::milliseconds(10));
	}

	renderThread.join();
	animateThread.join();
	actionThread.join();

	return 0;
}