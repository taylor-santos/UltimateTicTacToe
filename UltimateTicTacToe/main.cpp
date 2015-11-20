#include <SFML/Graphics.hpp>
using namespace sf;
int main()
{
	Vector2f screenSize = Vector2f(640, 640);
	RenderWindow window(VideoMode(screenSize.x, screenSize.y), "SFML works!");
	Vertex line[] =
	{
		Vertex(Vector2f(screenSize.x/8, screenSize.y/8)),
		Vertex(Vector2f(screenSize.x/8, screenSize.y/8*7))
	};

	

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(line, 2, Lines);
		window.display();
	}

	return 0;
}