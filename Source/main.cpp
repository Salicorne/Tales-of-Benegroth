#include "Game.h"

void render(sf::RenderWindow& window, MemoryManager& mgr) {
	while (window.isOpen()) {
		window.clear();
		mgr.draw(window);
		window.display();
		sf::sleep(sf::milliseconds(10));
	}
}

void animate(sf::RenderWindow& window, MemoryManager& mgr) {
	sf::Clock clock;
	while (window.isOpen()) {
		mgr.animateSprites(clock.restart());
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
	Game::Window::Height = 400;
	Game::Window::Width = Game::Window::Height*16/9;
	Game::Window::widthScrollLimit = 0.1f;
	Game::Window::heightScrollLimit = 0.1f;

	std::cout << "Creating a window of " << Game::Window::Width << "x" << Game::Window::Height << std::endl;
	sf::RenderWindow window(sf::VideoMode(Game::Window::Width, Game::Window::Height), "SFML works!");
	window.setActive(false);
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
	mgr.addTexture("Assets/treetrunk.png", 106);
	mgr.addTexture("Assets/treetop.png", 107);

	mgr.addTreeSprite(106, 107, 50, vec2f(150, 200), 250, 0.3);

	// player
	mgr.addAdditionalSprite(103, sf::IntRect(0, 0, 64, 64), sf::milliseconds(100), 206);
	mgr.addAdditionalSprite(104, sf::IntRect(0, 0, 64, 64), sf::milliseconds(100), 207);
	mgr.addAdditionalSprite(105, sf::IntRect(0, 0, 64, 64), sf::milliseconds(100), 208);
	SpriteSet* s = mgr.addSpriteSet(102, sf::IntRect(0, 0, 64, 64), sf::milliseconds(100), vec2f(100, 100), 205);
	s->addAdditionalSprite((AdditionalSprite&)mgr.getSprite(206));
	s->addAdditionalSprite((AdditionalSprite&)mgr.getSprite(207));
	s->addAdditionalSprite((AdditionalSprite&)mgr.getSprite(208));

	// npc
	mgr.addAdditionalSprite(103, sf::IntRect(0, 0, 64, 64), sf::milliseconds(100), 210);
	mgr.addAdditionalSprite(104, sf::IntRect(0, 0, 64, 64), sf::milliseconds(100), 211);
	mgr.addAdditionalSprite(105, sf::IntRect(0, 0, 64, 64), sf::milliseconds(100), 212);
	SpriteSet* npc = mgr.addSpriteSet(102, sf::IntRect(0, 0, 64, 64), sf::milliseconds(100), vec2f(0, 0), 213);
	npc->addAdditionalSprite((AdditionalSprite&)mgr.getSprite(210));
	npc->addAdditionalSprite((AdditionalSprite&)mgr.getSprite(211));
	npc->addAdditionalSprite((AdditionalSprite&)mgr.getSprite(212));
	gmgr.addNPC(213, vec2f(300, 350), 0.5, 301);


	gmgr.setPlayer(gmgr.addActor(205, vec2f(10, 100), 1.2f, 300));

	mgr.addRepeatedSprite(101, 500, 500, vec2f(-100, -100), 203);
	mgr.getSprite(203).setLayer(-1);

	std::thread renderThread(render, std::ref(window), std::ref(mgr));
	std::thread animateThread(animate, std::ref(window), std::ref(mgr));
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
				if (event.key.code == sf::Keyboard::P) {
					npc->blur(true);
				}
				if (event.key.code == sf::Keyboard::O) {
					npc->blur(false);
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
			for (int i = 0; i < sf::Joystick::getButtonCount(2); i++) {
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