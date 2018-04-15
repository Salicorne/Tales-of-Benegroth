#include "Scene.h"

class AbstractScreen {
    protected:
        sfg::SFGUI& gui;
		sfg::Desktop desktop;
		int next;

    public:
        AbstractScreen(sfg::SFGUI& gui);
        virtual void setUp() = 0;
        virtual int run(MemoryManager& mgr, GameManager& gmgr, sf::RenderWindow& window) = 0;
		virtual void cleanup() = 0;
};

class MenuScreen : public AbstractScreen {
    protected:
		sfg::Label::Ptr title;
		sfg::Button::Ptr beginButton;
		sfg::Button::Ptr quitButton;
		sfg::Box::Ptr box;
		sfg::Window::Ptr gwindow;

    public:
        MenuScreen(sfg::SFGUI& gui);
        virtual void setUp();
        virtual int run(MemoryManager& mgr, GameManager& gmgr, sf::RenderWindow& window);
		virtual void cleanup();
};

class GameScreen : public AbstractScreen {
    protected:
		sfg::Label::Ptr message;
		sfg::Button::Ptr closeMessageButton;
		sfg::Box::Ptr messageBox;
		sfg::Window::Ptr gwindow;

    public:
        GameScreen(sfg::SFGUI& gui);
        virtual void setUp();
        virtual int run(MemoryManager& mgr, GameManager& gmgr, sf::RenderWindow& window);
		virtual void cleanup();
		void showMessage(std::string sender, std::string message);
};