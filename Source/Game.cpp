#include "Game.h"


GameManager::GameManager(MemoryManager& mgr, sfg::SFGUI& gui, sf::Window& window) : mgr(mgr), gui(gui), window(window) {
	player = nullptr;

	// Init GUI
	message = sfg::Label::Create( L"label" );
	closeMessageButton = sfg::Button::Create( "Fermer" );
	messageBox = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.0f );
	messageBox->Pack( message );
	messageBox->Pack( closeMessageButton, false);

	gwindow = sfg::Window::Create();
	gwindow->SetTitle( "" );
	gwindow->Add( messageBox );
	gwindow->Show(false);
	closeMessageButton->GetSignal( sfg::Widget::OnLeftClick ).Connect( [this] {clearWindow();});
	desktop.Add( gwindow );
	desktop.LoadThemeFromFile("Assets/example.theme");
}

GameManager::~GameManager() {
	for (const Actor* a : actors) {
		delete a;
	}
}

Actor * GameManager::getPlayer() {
	return player;
}

Actor* GameManager::addActor(game_id sprite, vec2f pos, float speed, game_id id) {
	Actor* a = new Actor(static_cast<SpriteSet*>(&mgr.getSprite(sprite)), pos, speed, id);
	actors.push_back(a);
	return a;
}

NPC * GameManager::addNPC(game_id sprite, vec2f pos, float speed, game_id id) {
	NPC* a = new NPC(static_cast<SpriteSet*>(&mgr.getSprite(sprite)), pos, speed, id);
	actors.push_back(a);
	return a;
}

void GameManager::setPlayer(Actor * a) {
	player = a;
}

void GameManager::clearWindow() {
	this->gwindow->Show(false);
}

sfg::Desktop& GameManager::getDesktop() {
	return desktop;
}

void GameManager::movePlayer(vec2f delta, sf::Time elapsed) {
	if (player != nullptr) {
		player->move(delta, elapsed);
		vec2f feet = player->getSpriteSet()->getFeetPos() - mgr.getWindowPos();
		vec2f head = player->getSpriteSet()->getHeadPos() - mgr.getWindowPos();
		vec2f windowMovement(0, 0);
		if (head.x < Game::Window::Width * Game::Window::widthScrollLimit) { windowMovement.x += head.x - Game::Window::Width * Game::Window::widthScrollLimit; }
		if (head.x > Game::Window::Width * (1 - Game::Window::widthScrollLimit)) { windowMovement.x += head.x - Game::Window::Width * (1 - Game::Window::widthScrollLimit); }
		if (head.y < Game::Window::Height * Game::Window::heightScrollLimit) { windowMovement.y += head.y - Game::Window::Height * Game::Window::heightScrollLimit; }
		if (feet.y > Game::Window::Height * (1 - Game::Window::heightScrollLimit)) { windowMovement.y += feet.y - Game::Window::Height * (1 - Game::Window::heightScrollLimit); }
		mgr.moveWindow(windowMovement);
	}
}

void GameManager::showMessage(std::string sender, std::string message) {
	this->message->SetText(message);
	this->gwindow->SetPosition(vec2f(0, window.getSize().y/2));
	this->gwindow->SetTitle(sender);
	this->gwindow->Show(true);
}

void GameManager::playActors(sf::Time elapsed) {
	for (Actor* a : actors) {
		a->action(elapsed);
	}
}

void GameManager::drawGUI(sf::RenderWindow& window) {
	gui.Display(window);
}

void AssetCreator::createTreeSprite(MemoryManager& mgr, game_id trunk, game_id leaves, int minOffset, int maxOffset, vec2f posInWorld, game_id id, float feetOffset) {
	mgr.addTreeSprite(trunk, leaves, minOffset + (rand() % (maxOffset - minOffset + 1)), posInWorld, id, feetOffset);
}

void AssetCreator::createSpriteSet(MemoryManager& mgr, game_id baseTexture, std::vector<std::pair<game_id, sf::Color>> additions, sf::IntRect rect, vec2f posInWorld, sf::Time animationTime, game_id id, float feetOffset) {
	for (auto& i : additions) {
		game_id uid = mgr.getFreeSpriteId();
		mgr.addAdditionalSprite(i.first, rect, animationTime, uid);
		mgr.getSprite(uid).setColor(i.second);
		i.first = uid;
	}
	SpriteSet* s = mgr.addSpriteSet(baseTexture, rect, animationTime, posInWorld, id, feetOffset);
	for (const auto i : additions) {
		s->addAdditionalSprite(static_cast<AdditionalSprite&>(mgr.getSprite(i.first)));
	}
}
