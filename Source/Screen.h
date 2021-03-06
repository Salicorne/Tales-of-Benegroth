#include "Scene.h"

class AbstractScreen {
    protected:
        sfg::SFGUI& gui;
		sfg::Desktop desktop;
		int next;

    public:
        AbstractScreen(sfg::SFGUI& gui);
        virtual void setUp(MemoryManager& mgr, GameManager& gmgr, sf::RenderWindow& window) = 0;
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
        virtual void setUp(MemoryManager& mgr, GameManager& gmgr, sf::RenderWindow& window);
        virtual int run(MemoryManager& mgr, GameManager& gmgr, sf::RenderWindow& window);
		virtual void cleanup();
};

class GameScreen : public AbstractScreen {
    protected:
		sfg::Label::Ptr message;
		sfg::Button::Ptr closeMessageButton;
		sfg::Box::Ptr messageBox;
		sfg::ScrolledWindow::Ptr scrollwindow;
		sfg::Window::Ptr gwindow;
        sfg::Label::Ptr actionMessage;
        sfg::ProgressBar::Ptr lifeBar;
        sfg::Box::Ptr inventory;
        sfg::Window::Ptr inventoryHelper;
        sfg::Label::Ptr inventoryHelperText;
        sf::Shader shader;
        bool inventoryVisible;

    public:
        GameScreen(sfg::SFGUI& gui);
        virtual void setUp(MemoryManager& mgr, GameManager& gmgr, sf::RenderWindow& window);
        virtual int run(MemoryManager& mgr, GameManager& gmgr, sf::RenderWindow& window);
		virtual void cleanup();
		void showMessage(std::string sender, std::string message);
        void setActionMessage(std::string message);
        void buildInventory(GameManager& gmgr);
        void setInventoryHelperText(std::string text);
};