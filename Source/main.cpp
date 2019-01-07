#include "Sprite.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 640), "Demo app");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

	Sprite s(vec(0, 0), sf::Color::Blue, vec(200, 200));
	auto arm1 = s.getRoot()->addSubSprite(vec(175, 175), vec(25, 25), 	sf::Color::Red, 	vec(200, 50), -30);
	auto arm2 = 		arm1->addSubSprite(vec(175, 25), vec(25, 25), 	sf::Color::Magenta,	vec(200, 50), 80);
	auto arm3 = 		arm2->addSubSprite(vec(175, 25), vec(25, 25), 	sf::Color::Yellow, 	vec(200, 50), 0);
	auto arm4 = 		arm3->addSubSprite(vec(175, 25), vec(25, 25), 	sf::Color::Cyan, 	vec(200, 50), 0);

	HumanoidAnimation animation;
	HumanoidFrame frame1 = initHumanoidFrame(sf::milliseconds(500));
	frame1.image.arm1l = 40;
	frame1.image.arm2l = 50;
	HumanoidFrame frame2 = initHumanoidFrame(sf::milliseconds(500));
	frame2.image.arm1l = -20;
	frame2.image.arm2l = -30;
	animation.push(frame1);
	animation.push(frame2);

	Humanoid h(vec(0, 0));
	h.setAnimation(animation);

	sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed) {
                window.close();
			}
        }
		sf::Time elapsed = clock.restart();

		h.animate(elapsed);

        window.clear();
        window.draw(shape);
		//s.draw(&window, vec(100, 100));
		h.draw(&window, vec(300, 300));
        window.display();
    }

    return 0;
}

