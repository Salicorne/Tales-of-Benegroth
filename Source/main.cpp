#include "Screen.h"

void animate(sf::RenderWindow& window, MemoryManager& mgr, GameManager& gmgr) {
	sf::Clock clock;
	while (window.isOpen()) {
		sf::Time elapsed = clock.restart();
		if(Game::currentScreen == Game::Screen::Game) {
			mgr.animateSprites(elapsed);
			mgr.updateBlur(gmgr.getPlayer()->getSpriteSet()->getFeetPos());
		}
		sf::sleep(sf::milliseconds(50));
	}
}

void action(sf::RenderWindow& window, MemoryManager& mgr, GameManager& gmgr) {
	sf::Clock clock;
	while (window.isOpen()) {
		sf::Time elapsed = clock.restart();
		if(Game::currentScreen == Game::Screen::Game) {
			gmgr.playActors(elapsed);
		}
		sf::sleep(sf::milliseconds(20));
	}
}

void updateActionMessage(sf::RenderWindow& window, GameManager& gmgr) {
	while (window.isOpen()) {
		if(Game::currentScreen == Game::Screen::Game) {
			gmgr.updateActionMessage();
		}
		sf::sleep(sf::milliseconds(300));
	}
}


int main()
{
	std::cout << "Tales of Benegroth" << std::endl;
	sfg::SFGUI gui;
	Game::Window::Width = 853;
	Game::Window::Height = 480;
	Game::Window::widthScrollLimit = 0.1f;
	Game::Window::heightScrollLimit = 0.1f;

	sf::Font font;
    if (!font.loadFromFile("Assets/Onciale PhF01.ttf"))
        std::cout << "Could not load font." << std::endl;

	srand(time(NULL));

	sf::RenderWindow window(sf::VideoMode(Game::Window::Width, Game::Window::Height), "SFML works!", sf::Style::Titlebar | sf::Style::Close);
	window.setActive(false);
	window.setKeyRepeatEnabled(false);
	window.setMouseCursorVisible(true);

	Game::Window::Width = window.getSize().x;
	Game::Window::Height = window.getSize().y;

	MemoryManager mgr;
	GameManager gmgr(mgr, window);

	Game::joystick = -1;
	for (unsigned int i = 1; i< sf::Joystick::Count; ++i)
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
	Game::currentScreen = 0;

	if(!Scene::loadScene("save.dat", mgr, gmgr)) {
		std::cerr << "Error loading main scene, aborting. " << std::endl;
		return -1;
	}

	std::thread animateThread(animate, std::ref(window), std::ref(mgr), std::ref(gmgr));
	std::thread actionThread(action, std::ref(window), std::ref(mgr), std::ref(gmgr));
	std::thread actionMessageThread(updateActionMessage, std::ref(window), std::ref(gmgr));

	sf::Clock clock;
	vec2f playerMovement(0, 0);
	while(Game::currentScreen >= 0) {
		screens[Game::currentScreen]->setUp(mgr, gmgr, window);
		Game::currentScreen = screens[Game::currentScreen]->run(mgr, gmgr, window);
	}

	window.close();
	actionMessageThread.join();
	animateThread.join();
	actionThread.join();

	return 0;
}
