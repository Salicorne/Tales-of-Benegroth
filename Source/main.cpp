#include "Scene.h"
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>


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
	sfg::SFGUI gui;
	auto label = sfg::Label::Create( L"Bienvenue à Kergal !" );
	auto button = sfg::Button::Create( "Fermer" );
	auto box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.0f );
	box->Pack( label );
	box->Pack( button, false);
	auto gwindow = sfg::Window::Create();
	gwindow->SetTitle( "Marchand" );
	gwindow->Add( box );
	button->GetSignal( sfg::Widget::OnLeftClick ).Connect( [gwindow] { gwindow->Show(false); } );
	sfg::Desktop desktop;
	desktop.Add( gwindow );
	desktop.LoadThemeFromFile("Assets/example.theme");


	std::cout << "Tales of Benegroth" << std::endl;
	Game::Window::Width = 711;
	Game::Window::Height = 400;
	Game::Window::widthScrollLimit = 0.1f;
	Game::Window::heightScrollLimit = 0.1f;

	srand(time(NULL));

	sf::RenderWindow window(sf::VideoMode(Game::Window::Width, Game::Window::Height), "SFML works!");
	//sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML works!", sf::Style::Fullscreen);
	window.setActive(false);
	window.setKeyRepeatEnabled(false);
	window.setMouseCursorVisible(true);

	Game::Window::Width = window.getSize().x;
	Game::Window::Height = window.getSize().y;

	MemoryManager mgr;
	GameManager gmgr(mgr);

	int joystick = -1;
	for (unsigned int i = 0; i< sf::Joystick::Count; ++i)
	{
		if (sf::Joystick::isConnected(i)) {
			std::cout << "Joystick " << i << " is connected !" << std::endl;
			joystick = i;
		}
	}

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
	while (window.isOpen())
	{
		sf::Event event;
		sf::Time elapsed = clock.restart();
		while (window.pollEvent(event))
		{
			desktop.HandleEvent(event);

			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
			}
		}
		desktop.Update(elapsed.asSeconds());


		playerMovement = vec2f(0, 0);

		sf::Joystick::update();

		if (joystick != -1 && sf::Joystick::isConnected(joystick)) {
			playerMovement.x = sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::X)/100;
			playerMovement.y = sf::Joystick::getAxisPosition(joystick, sf::Joystick::Axis::Y)/100;
			/*for (int i = 0; i < sf::Joystick::getButtonCount(joystick); i++) {
				std::cout << sf::Joystick::isButtonPressed(joystick, i) << " - ";
			}*/
			//std::cout << std::endl;
			/*for (unsigned int i = 0; i < sf::Joystick::getButtonCount(2); i++) {
				if (sf::Joystick::isButtonPressed(2, i)) { std::cout << "Button " << i << " pressed !" << std::endl; }
			}*/
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
		gui.Display(window);
		window.display();

		sf::sleep(sf::milliseconds(10));
	}

	///renderThread.join();
	animateThread.join();
	actionThread.join();

	//renderThread.wait();

	return 0;
}
