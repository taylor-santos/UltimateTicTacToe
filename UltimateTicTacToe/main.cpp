
#include <SFML/Graphics.hpp>
using namespace sf;

Color color1 = Color::Red;
Color color2 = Color::Blue;
Color playableColor = Color(255, 153, 0);
int alpha = 150;

struct box {
	RectangleShape rectangle;
	struct grid* parentGrid;
	int owner;
	Vector2f corner;

	box(){
		owner = 0;
		rectangle.setFillColor(Color(255, 255, 255, 0));
	}
};

struct grid {
	RectangleShape rectangle;
	bool complete = false;
	bool playable;
	int owner = 0;
	box* boxes[3][3];
	Vector2f corner;
	int checkComplete()
	{
		if (complete == true)
			return owner;
		for (int y = 0; y < 3; ++y)
		{
			if (boxes[0][y]->owner == boxes[1][y]->owner && boxes[1][y]->owner == boxes[2][y]->owner && boxes[2][y]->owner != 0)
			{
				complete = true;
				owner = boxes[0][y]->owner;
				if (owner == 1)
				{
					rectangle.setFillColor(Color(color1.r,color1.g,color1.b, alpha));
				}
				else {
					rectangle.setFillColor(Color(color2.r, color2.g, color2.b, alpha));
				}
				return owner;
			}
		}
		for (int x = 0; x < 3; ++x)
		{
			if (boxes[x][0]->owner == boxes[x][1]->owner && boxes[x][1]->owner == boxes[x][2]->owner && boxes[x][2]->owner != 0)
			{
				complete = true;
				owner = boxes[x][0]->owner;
				if (owner == 1)
				{
					rectangle.setFillColor(Color(color1.r, color1.g, color1.b, alpha));
				}
				else {
					rectangle.setFillColor(Color(color2.r, color2.g, color2.b, alpha));
				}
				return owner;
			}
		}
		if (boxes[0][0]->owner == boxes[1][1]->owner && boxes[1][1]->owner == boxes[2][2]->owner && boxes[2][2]->owner != 0)
		{
			complete = true;
			owner = boxes[0][0]->owner;
			if (owner == 1)
			{
				rectangle.setFillColor(Color(color1.r, color1.g, color1.b, alpha));
			}
			else {
				rectangle.setFillColor(Color(color2.r, color2.g, color2.b, alpha));
			}
			return owner;
		}
		if (boxes[2][0]->owner == boxes[1][1]->owner && boxes[1][1]->owner == boxes[0][2]->owner && boxes[0][2]->owner != 0)
		{
			complete = true;
			owner = boxes[2][0]->owner;
			if (owner == 1)
			{
				rectangle.setFillColor(Color(color1.r, color1.g, color1.b, alpha));
			}
			else {
				rectangle.setFillColor(Color(color2.r, color2.g, color2.b, alpha));
			}
			return owner;
		}
		return 0;
	}
	grid()
	{
		playable = true;
		rectangle.setFillColor(Color(255, 255, 255, 0));
	}

};

struct board {
	grid* grids[3][3];
};

int main()
{
	ContextSettings settings;
	settings.antialiasingLevel = 32;
	Vector2f screenSize = Vector2f(800,800);
	RenderWindow window(VideoMode((int)screenSize.x, (int)screenSize.y), "SFML works!");
	const int lineCount = 40;
	Vertex line[lineCount*2];

	Color lineColor = Color::Black;

	Vector2f boxSize = Vector2f((int)(screenSize.x / 13), (int)(screenSize.y / 13));
	int index = 0;
	for (int row = 0; row < 3; ++row)
	{
		for (int col = 0; col < 3; ++col)
		{
			
			Vector2f corner = Vector2f(boxSize.x + 4 * col * boxSize.x, boxSize.y + 4 * row * boxSize.y);
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
	line[index++] = Vertex(Vector2f(4.5f*boxSize.x, 0.5f*boxSize.y), lineColor);
	line[index++] = Vertex(Vector2f(4.5f*boxSize.x, 12.5f*boxSize.y), lineColor);
	line[index++] = Vertex(Vector2f(8.5f*boxSize.x, 0.5f*boxSize.y), lineColor);
	line[index++] = Vertex(Vector2f(8.5f*boxSize.x, 12.5f*boxSize.y), lineColor);
	line[index++] = Vertex(Vector2f(0.5f*boxSize.x, 4.5f*boxSize.y), lineColor);;
	line[index++] = Vertex(Vector2f(12.5f*boxSize.x, 4.5f*boxSize.y), lineColor);
	line[index++] = Vertex(Vector2f(0.5f*boxSize.x, 8.5f*boxSize.y), lineColor);
	line[index++] = Vertex(Vector2f(12.5f*boxSize.x, 8.5f*boxSize.y) , lineColor);

	board *board = (struct board*)malloc(sizeof (struct board));

	box* boxes[81];
	int boxIndex = 0;

	for (int gridY = 0; gridY < 3; ++gridY)
	{
		for (int gridX = 0; gridX < 3; ++gridX)
		{
			board->grids[gridX][gridY] = new grid();
			board->grids[gridX][gridY]->corner = Vector2f(boxSize.x + 4 * gridX * boxSize.x, boxSize.y + 4 * gridY * boxSize.y);
			board->grids[gridX][gridY]->rectangle.setSize(boxSize*3.5f);
			board->grids[gridX][gridY]->rectangle.setPosition(board->grids[gridX][gridY]->corner - boxSize*0.25f);
			for (int boxX = 0; boxX < 3; ++boxX)
			{
				for (int boxY = 0; boxY < 3; ++boxY)
				{
					board->grids[gridX][gridY]->boxes[boxX][boxY] = new box();
					board->grids[gridX][gridY]->boxes[boxX][boxY]->parentGrid = board->grids[gridX][gridY];
					board->grids[gridX][gridY]->boxes[boxX][boxY]->corner = board->grids[gridX][gridY]->corner + Vector2f(boxSize.x*boxX, boxSize.y*boxY);
					board->grids[gridX][gridY]->boxes[boxX][boxY]->rectangle.setSize(boxSize * 0.9f);
					board->grids[gridX][gridY]->boxes[boxX][boxY]->rectangle.setPosition(board->grids[gridX][gridY]->boxes[boxX][boxY]->corner + boxSize*0.05f);
					boxes[boxIndex++] = board->grids[gridX][gridY]->boxes[boxX][boxY];
				}
			}
		}
	}

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

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (!mouseDown)
			{
				mouseDown = true;
				Vector2i mousePos = Mouse::getPosition(window);
				for (int y = 0; y < 3; ++y)
				{
					for (int x = 0; x < 3; ++x)
					{
						if (board->grids[x][y]->complete == false)
						{
							Vector2f corner = board->grids[x][y]->corner;
							if (mousePos.x > corner.x && mousePos.x < corner.x + 3 * boxSize.x && mousePos.y > corner.y && mousePos.y < corner.y + 3 * boxSize.y)
							{
								int row = floor((mousePos.x - corner.x) / (boxSize.x));
								int col = floor((mousePos.y - corner.y) / (boxSize.y));
								if (board->grids[x][y]->boxes[row][col]->owner == 0)
								{
									board->grids[x][y]->boxes[row][col]->owner = currPlayer + 1;
									board->grids[x][y]->checkComplete();
									currPlayer = !currPlayer;
								}
							}
						}
					}
				}
			}
		} else {
			mouseDown = false;
		}
		

		window.clear(Color::White);
		
		window.draw(line, lineCount * 2, Lines);

		for (int i = 0; i < 81; ++i)
		{
			if (boxes[i]->owner == 1)
			{
				boxes[i]->rectangle.setFillColor(color1);
			}else if (boxes[i]->owner == 2)
			{
				boxes[i]->rectangle.setFillColor(color2);
			}
			else if (boxes[i]->parentGrid->playable == true)
			{
				boxes[i]->rectangle.setFillColor(playableColor);
			}
			else {
				boxes[i]->rectangle.setFillColor(Color(255, 255, 255, 0));
			}
			window.draw(boxes[i]->rectangle);
		}

		for (int y = 0; y < 3; ++y)
		{
			for (int x = 0; x < 3; ++x)
			{
				board->grids[x][y]->checkComplete();
				window.draw(board->grids[x][y]->rectangle);
			}
		}

		

		window.display();
	}

	return 0;
}