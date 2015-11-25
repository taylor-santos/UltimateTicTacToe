#include <Windows.h>
#include <SFML/Graphics.hpp>
using namespace sf;

Color color1 = Color::Red;
Color color2 = Color::Blue;
Color playableColor = Color(255, 153, 0);
int alpha = 150;

const int SIZE = 4;

struct board{
	grid* grids[SIZE][SIZE];
};

struct grid {
	box* boxes[SIZE][SIZE];
	board* board;
	int winner();
};

struct box {
	int owner;
	grid* grid;
};

int grid::winner()
{
	for (int y = 0; y < SIZE; ++y)
	{
		if (boxes[0][y]->owner != 0)
		{
			bool found = true;
			int owner = boxes[0][y]->owner;
			for (int x = 1; x < SIZE && found == true; ++x)
			{
				if (boxes[x][y]->owner != owner)
					found = false;
			}
			if (found == true)
			{
				return owner;
			}
		}
	}
	for (int x = 0; x < SIZE; ++x)
	{
		if (boxes[x][0]->owner != 0)
		{
			bool found = true;
			int owner = boxes[x][0]->owner;
			for (int y = 1; y < SIZE && found == true; ++y)
			{
				if (boxes[x][y]->owner != owner)
					found = false;
			}
			if (found == true)
				return owner;
		}
	}
	if (boxes[0][0]->owner != 0)
	{
		bool found = true;
		int owner = boxes[0][0]->owner;
		for (int xy = 0; xy < SIZE && found == true; ++xy)
		{
			if (boxes[xy][xy]->owner != owner)
				found = false;
		}
		if (found == true)
			return owner;
	}
	if (boxes[SIZE-1][0]->owner != 0)
	{
		bool found = true;
		int owner = boxes[SIZE-1][0]->owner;
		for (int xy = 0; xy < SIZE && found == true; ++xy)
		{
			if (boxes[SIZE-1-xy][xy]->owner != owner)
				found = false;
		}
		if (found == true)
			return owner;
	}
	return 0;
}

int main()
{
	Clock clock;
	srand(time(NULL));
	Vector2f screenSize = Vector2f(800,800);
	RenderWindow window(VideoMode((int)screenSize.x, (int)screenSize.y), "Ultimate Tic-Tac-Toe");
	const int lineCount = 40;
	Vertex line[lineCount*2];

	Color lineColor = Color::Black;

	Vector2f boxSize = Vector2f((screenSize.x / 13), (screenSize.y / 13));
	int index = 0;
	for (int row = 0; row < 3; ++row)
	{
		for (int col = 0; col < 3; ++col)
		{
			
			Vector2f corner = Vector2f(boxSize.x + 4 * col * boxSize.x, boxSize.y * 1.5f + 4 * row * boxSize.y);
			line[index++] = Vertex(corner + Vector2f(boxSize.x, 0),lineColor);
			line[index++] = Vertex(corner + Vector2f(boxSize.x, 3 * boxSize.y), lineColor);
			line[index++] = Vertex(corner + Vector2f(boxSize.x*2, 0), lineColor);
			line[index++] = Vertex(corner + Vector2f(boxSize.x*2, 3 * boxSize.y), lineColor);
			
			line[index++] = Vertex(corner + Vector2f(0, boxSize.y), lineColor);
			line[index++] = Vertex(corner + Vector2f(3 * boxSize.x, boxSize.y), lineColor);
			line[index++] = Vertex(corner + Vector2f(0, boxSize.y*2), lineColor);
			line[index++] = Vertex(corner + Vector2f(3 * boxSize.x, boxSize.y*2), lineColor);
			
		}
	}
	line[index++] = Vertex(Vector2f(4.5f*boxSize.x, 1.0f*boxSize.y), lineColor);
	line[index++] = Vertex(Vector2f(4.5f*boxSize.x, 13.0f*boxSize.y), lineColor);
	line[index++] = Vertex(Vector2f(8.5f*boxSize.x, 1.0f*boxSize.y), lineColor);
	line[index++] = Vertex(Vector2f(8.5f*boxSize.x, 13.0f*boxSize.y), lineColor);
	line[index++] = Vertex(Vector2f(0.5f*boxSize.x, 5.0f*boxSize.y), lineColor);;
	line[index++] = Vertex(Vector2f(12.5f*boxSize.x, 5.0f*boxSize.y), lineColor);
	line[index++] = Vertex(Vector2f(0.5f*boxSize.x, 9.0f*boxSize.y), lineColor);
	line[index++] = Vertex(Vector2f(12.5f*boxSize.x, 9.0f*boxSize.y) , lineColor);
	
	Font font;

	//font.loadFromFile("Roboto-Light.ttf");
	
	Text text;
	text.setFont(font);
	text.setString("PLAYER 1's turn");
	text.setCharacterSize(36);
	text.setColor(color1);
	
	
	int currPlayer = 0;
	bool mouseDown = false;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		FloatRect textRect = text.getLocalBounds();
		text.setOrigin((int)(textRect.left + textRect.width / 2.0f),
			textRect.top);
		text.setPosition(Vector2f((int)(screenSize.x / 2.0f), (int)(boxSize.y/4)));
		
		window.clear(Color::White);

		window.draw(line, lineCount * 2, Lines);

		window.draw(text);

		window.display();
	}

	return 0;
}

