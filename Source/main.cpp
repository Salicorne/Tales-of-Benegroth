#include "Screen.h"


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
	std::cout << "Tales of Benegroth" << std::endl;
	sfg::SFGUI gui;
	Game::Window::Width = 711;
	Game::Window::Height = 400;
	Game::Window::widthScrollLimit = 0.1f;
	Game::Window::heightScrollLimit = 0.1f;

	sf::Font font;
    if (!font.loadFromFile("Assets/Onciale PhF01.ttf"))
        std::cout << "Could not load font." << std::endl;

	srand(time(NULL));

	sf::RenderWindow window(sf::VideoMode(Game::Window::Width, Game::Window::Height), "SFML works!");
	//sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML works!", sf::Style::Fullscreen);
	window.setActive(false);
	window.setKeyRepeatEnabled(false);
	window.setMouseCursorVisible(true);

	Game::Window::Width = window.getSize().x;
	Game::Window::Height = window.getSize().y;

	MemoryManager mgr;
	GameManager gmgr(mgr, window);

	Game::joystick = -1;
	for (unsigned int i = 0; i< sf::Joystick::Count; ++i)
	{
		if (sf::Joystick::isConnected(i)) {
			std::cout << "Joystick " << i << " is connected !" << std::endl;
			Game::joystick = i;
		}
	}

	std::vector<AbstractScreen*> screens;
	MenuScreen menuScreen(gui);
	screens.push_back(&menuScreen);
	GameScreen gameScreen(gui);
	screens.push_back(&gameScreen);
	int screen = 0;

	if(!Scene::loadScene("save.dat", mgr, gmgr)) {
		std::cerr << "Error loading main scene, aborting. " << std::endl;
		return -1;
	}

	//std::thread renderThread(render, std::ref(window), std::ref(mgr));
	std::thread animateThread(animate, std::ref(window), std::ref(mgr), std::ref(gmgr));
	std::thread actionThread(action, std::ref(window), std::ref(mgr), std::ref(gmgr));

	//sf::Thread renderThread(std::bind(render, std::ref(window), std::ref(mgr)));
	//renderThread.launch();

	sf::Clock clock;
	vec2f playerMovement(0, 0);
	while(screen >= 0) {
		screens[screen]->setUp();
		screen = screens[screen]->run(mgr, gmgr, window);
	}
	/*while (window.isOpen())
	{
		sf::Event event;
		sf::Time elapsed = clock.restart();
		while (window.pollEvent(event))
		{
			gmgr.getDesktop().HandleEvent(event);

			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
				if (event.key.code == sf::Keyboard::P) {
					gmgr.showMessage("Magicien", "Bienvenue, jeune homme...");
				}
			}
		}
		gmgr.getDesktop().Update(elapsed.asSeconds());


		playerMovement = vec2f(0, 0);

		sf::Joystick::update();

		if (Game::joystick != -1 && sf::Joystick::isConnected(Game::joystick)) {
			playerMovement.x = sf::Joystick::getAxisPosition(Game::joystick, sf::Joystick::Axis::X)/100;
			playerMovement.y = sf::Joystick::getAxisPosition(Game::joystick, sf::Joystick::Axis::Y)/100;
		}
		else {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) { playerMovement.y--; }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) { playerMovement.x--; }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { playerMovement.y++; }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { playerMovement.x++; }
		}


		gmgr.movePlayer(playerMovement, elapsed);

		window.clear();
		mgr.draw(window);
		gmgr.drawGUI(window);
		window.display();

		sf::sleep(sf::milliseconds(10));
	}*/

	window.close();
	///renderThread.join();
	animateThread.join();
	actionThread.join();

	//renderThread.wait();

	return 0;
}
