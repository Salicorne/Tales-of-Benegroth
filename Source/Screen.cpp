#include "Screen.h"

AbstractScreen::AbstractScreen(sfg::SFGUI& gui) : gui(gui) {
	
}


MenuScreen::MenuScreen(sfg::SFGUI& gui) : AbstractScreen(gui) {
	next = -2;
}

void MenuScreen::setUp() {
	next = -2;
	title = sfg::Label::Create("Tales of Benegroth v0.1");
	beginButton = sfg::Button::Create("Commencer");
	quitButton = sfg::Button::Create("Quitter");
	box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 50.0f);
	box->Pack(title);
	box->Pack(beginButton);
	box->Pack(quitButton);
	beginButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] {this->next = 1; });
	quitButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] {this->next = -1; });

	gwindow = sfg::Window::Create(sfg::Window::Style::BACKGROUND);
	gwindow->SetTitle("");
	gwindow->Add(box);
	desktop.Add(gwindow);
	desktop.LoadThemeFromFile("Assets/example.theme");
}

int MenuScreen::run(MemoryManager& mgr, GameManager& gmgr, sf::RenderWindow& window) {
    sf::Clock clock;
    window.resetGLStates();
	gwindow->SetAllocation(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
	while (window.isOpen())
	{
		if(next != -2) { 
			cleanup();
			return next; 
		}
		sf::Event event;
		sf::Time elapsed = clock.restart();
		while (window.pollEvent(event))
		{
			desktop.HandleEvent(event);
			if (event.type == sf::Event::Closed) {
				cleanup();
                return -1;  // Close
            }

			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					next = -1;
				}
			}
		}
		desktop.Update(elapsed.asSeconds());

		window.clear();
        gui.Display(window);
		window.display();

		sf::sleep(sf::milliseconds(10));
	}
	cleanup();	//Just in case
	return -1;
}

void MenuScreen::cleanup() {
	gwindow->Show(false);
}

/************ GameScreen ************/


GameScreen::GameScreen(sfg::SFGUI& gui) : AbstractScreen(gui) {
}

void GameScreen::setUp() {
	message = sfg::Label::Create(L"label");
	closeMessageButton = sfg::Button::Create("Fermer");
	messageBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.0f);
	messageBox->Pack(message);
	messageBox->Pack(closeMessageButton, false);

	gwindow = sfg::Window::Create();
	gwindow->SetTitle("");
	gwindow->Add(messageBox);
	gwindow->Show(false);
	closeMessageButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] {cleanup(); });
	desktop.Add(gwindow);
	desktop.LoadThemeFromFile("Assets/example.theme");
	next = -2;
}

int GameScreen::run(MemoryManager& mgr, GameManager& gmgr, sf::RenderWindow& window) {
    sf::Clock clock;
	vec2f playerMovement(0, 0);
    while (window.isOpen())
	{
		if (next != -2) {
			cleanup();
			return next;
		}
		sf::Event event;
		sf::Time elapsed = clock.restart();
		while (window.pollEvent(event))
		{
			desktop.HandleEvent(event);

			if (event.type == sf::Event::Closed) {
				cleanup();
				return -1;
			}

			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					next = 0;
				}
				if (event.key.code == sf::Keyboard::P) {
					showMessage("Magicien", "Bienvenue, jeune homme...");
				}
			}
		}
		desktop.Update(elapsed.asSeconds());


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
		gui.Display(window);
		window.display();

		sf::sleep(sf::milliseconds(10));
	}
	cleanup();	//Just in case
	return -1;
}

void GameScreen::cleanup() {
	gwindow->Show(false);
}

void GameScreen::showMessage(std::string sender, std::string message) {
	this->message->SetText(message);
	// this->gwindow->SetPosition(vec2f(0, window.getSize().y/2));
	this->gwindow->SetPosition(vec2f(0,0));
	this->gwindow->SetTitle(sender);
	this->gwindow->Show(true);
}