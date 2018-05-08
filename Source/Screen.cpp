#include "Screen.h"

AbstractScreen::AbstractScreen(sfg::SFGUI& gui) : gui(gui) {
	
}


MenuScreen::MenuScreen(sfg::SFGUI& gui) : AbstractScreen(gui) {
	next = -2;
}

void MenuScreen::setUp(MemoryManager& mgr, GameManager& gmgr, sf::RenderWindow& window) {
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
	gwindow->SetAllocation(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));

	gmgr.setShowMessageFunction([](std::string, std::string){});
}

int MenuScreen::run(MemoryManager& mgr, GameManager& gmgr, sf::RenderWindow& window) {
    sf::Clock clock;
    window.resetGLStates();
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

void GameScreen::setUp(MemoryManager& mgr, GameManager& gmgr, sf::RenderWindow& window) {
	if(!shader.loadFromFile("Assets/shader.frag", sf::Shader::Fragment)) {
		std::cerr << "Error : could not load shader at Assets/shader.frag" << std::endl;
		Game::canUseShaders = false;
	}
	message = sfg::Label::Create(L"label");
	message->SetRequisition(vec2f(window.getSize().x-120, 0));
	message->SetLineWrap(true);
	actionMessage = sfg::Label::Create("");
	closeMessageButton = sfg::Button::Create("Fermer");
	messageBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 50.0f);
	messageBox->Pack(message);
	messageBox->Pack(closeMessageButton);
	lifeBar = sfg::ProgressBar::Create(sfg::ProgressBar::Orientation::HORIZONTAL);
	lifeBar->SetRequisition(vec2f(300, 20));
	lifeBar->SetPosition(vec2f(30, 30));

	buildInventory(gmgr);

	scrollwindow = sfg::ScrolledWindow::Create();
	scrollwindow->SetScrollbarPolicy(sfg::ScrolledWindow::VERTICAL_AUTOMATIC);
	scrollwindow->AddWithViewport(messageBox);
	gwindow = sfg::Window::Create();
	gwindow->SetTitle("");
	gwindow->Add(scrollwindow);
	gwindow->SetAllocation(sf::FloatRect(20, window.getSize().y/2+20, window.getSize().x-40, window.getSize().y/2-40));
	gwindow->Show(false);
	closeMessageButton->GetSignal(sfg::Widget::OnLeftClick).Connect([this] {gwindow->Show(false); actionMessage->Show(true); });

	actionMessage->SetAllocation(sf::FloatRect(0, window.getSize().y * 0.8f, window.getSize().x, actionMessage->GetAllocation().height));

	desktop.Add(gwindow);
	desktop.Add(actionMessage);
	desktop.Add(lifeBar);
	desktop.LoadThemeFromFile("Assets/example.theme");
	next = -2;

	gmgr.setShowMessageFunction([this](std::string a, std::string b){ this->showMessage(a, b); });
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
				if (event.key.code == sf::Keyboard::E) {
					gmgr.interact();
				}
				if (event.key.code == sf::Keyboard::O) {
					Scene::loadScene("save2.dat", mgr, gmgr);
				}
			}
		}
		setActionMessage(gmgr.getActionMessage());
		lifeBar->SetFraction(gmgr.getPlayerLifeRatio());
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

		shader.setUniform("time", gmgr.getTimeOfDay());

		window.clear();
		mgr.draw(window, &shader);
		gui.Display(window);
		window.display();

		sf::sleep(sf::milliseconds(10));
	}
	cleanup();	//Just in case
	return -1;
}

void GameScreen::cleanup() {
	gwindow->Show(false);
	actionMessage->Show(false);
	lifeBar->Show(false);
	inventory->Show(false);
}

void GameScreen::showMessage(std::string sender, std::string message) {
	this->message->SetText(message);
	this->gwindow->SetTitle(sender);
	this->gwindow->Show(true);
	this->actionMessage->Show(false);
}

void GameScreen::setActionMessage(std::string message) {
	this->actionMessage->SetText(message);
}

void GameScreen::buildInventory(GameManager& gmgr) {
	desktop.Remove(inventory);
	inventory = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	Inventory& ivt = gmgr.getInventory();
	auto lst = ivt.getItems();
	int inventoryRows = 5, inventoryCols = 5;
	for(int i = 0; i<inventoryRows; i++) {
		auto b = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
		for(int j = 0; j<inventoryCols; j++) {
			sf::Image icon;
			if(i * inventoryRows + j < lst.size()) {
				Item* currentItem = lst.at(i * inventoryRows + j).second;
				auto img = sfg::Image::Create(currentItem->getImage());
				auto btn = sfg::Button::Create();
				btn->SetImage(img);
				btn->GetSignal(sfg::Widget::OnLeftClick).Connect([currentItem, &ivt, &gmgr, this](){ivt.useItem(currentItem->getId(), &gmgr); this->buildInventory(gmgr);});
				b->Pack(btn, false, false);
			}
			else {
				if(icon.loadFromFile("Assets/orb.png")) {
					auto img = sfg::Image::Create(icon);
					auto btn = sfg::Button::Create();
					btn->SetImage(img);
					b->Pack(btn, false, false);
				}
			}
		}
		inventory->Pack(b, false);
	}
	inventory->SetPosition(vec2f(Game::Window::Width - inventory->GetAllocation().width - 20, Game::Window::Height/2.0));
	desktop.Add(inventory);
}