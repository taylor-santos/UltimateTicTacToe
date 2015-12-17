#include <Windows.h>
#include <SFML/Graphics.hpp>
using namespace sf;

Color color1 = Color::Red;
Color color2 = Color::Blue;
Color playableColor = Color(255, 153, 0);
int alpha = 150;

struct box {
	int owner; //0 = unowned, 1 = p1, 2 = p2
	box()
	{
		owner = 0;
	}
};

struct board {
	box* boxes[3][3][3][3];	//Grid X, Grid Y, Box X, Box Y.
	
	int gridOwner(int x, int y);
	int playableGrid;
	int boardOwner();

	int value(int player);

	bool playMove(int gX, int gY, int bX, int bY, int player);

	board()
	{
		playableGrid = -1;
		for (int gY = 0; gY < 3; ++gY)
		{
			for (int gX = 0; gX < 3; ++gX)
			{
				for (int bY = 0; bY < 3; ++bY)
				{
					for (int bX = 0; bX < 3; ++bX)
					{
						boxes[gX][gY][bX][bY] = new box();
					}
				}
			}
		}
	}
};

struct move {
	move* next;
	move* result;
	int val;
	int player; //0 = p1, 1 = p2.


	move(int p)
	{
		next = NULL;
		result = NULL;
		val = 0;
		player = p;
	}
	~move()
	{
		delete next;
		delete result;
	}
};

move* getMoves(int player, board* b);

int main()
{
	srand((int)time(NULL));
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
	line[index++] = Vertex(Vector2f(12.5f*boxSize.x, 9.0f*boxSize.y), lineColor);

	Font font;

	//font.loadFromFile("Roboto-Light.ttf");
	
	Text text;
	text.setFont(font);
	text.setString("PLAYER 1's turn");
	text.setCharacterSize(36);
	text.setColor(color1);

	int currPlayer = 0;
	bool mouseDown = false;

	board* b = new board();

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

//AI
		if (currPlayer == 1)
		{
			move* root = getMoves(currPlayer, b);
			currPlayer = !currPlayer;
		}
//PLAYER
		else if (currPlayer == 0)
		{
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				if (!mouseDown)
				{
					mouseDown = true;
					Vector2i mousePos = Mouse::getPosition(window);
					for (int gridY = 0; gridY < 3; ++gridY)
					{
						for (int gridX = 0; gridX < 3; ++gridX)
						{
							if (b->gridOwner(gridX,gridY) == 0 && (b->playableGrid == 3* gridY + gridX || b->playableGrid == -1))
							{
								Vector2f corner = Vector2f(boxSize.x + 4 * gridX * boxSize.x, boxSize.y * 1.5f + 4 * gridY * boxSize.y);
								if (mousePos.x > corner.x && mousePos.x < corner.x + 3 * boxSize.x && mousePos.y > corner.y && mousePos.y < corner.y + 3 * boxSize.y)
								{
									int x = (int)floor((mousePos.x - corner.x) / (boxSize.x));
									int y = (int)floor((mousePos.y - corner.y) / (boxSize.y));
									
									if (b->boxes[gridX][gridY][x][y]->owner == 0)
									{
										b->playMove(gridX, gridY, x, y, currPlayer);
										currPlayer = !currPlayer;
									}
								}
							}
						}
					}
				}
			}
			else {
				mouseDown = false;
			}
		}

		FloatRect textRect = text.getLocalBounds();
		text.setOrigin(textRect.left + (int)(textRect.width / 2),textRect.top);
		text.setPosition(Vector2f((screenSize.x/2), (boxSize.y/4)));
		
		window.clear(Color::White);

		window.draw(line, lineCount * 2, Lines);

		window.draw(text);

		window.display();
	}

	return 0;
}

move* getMoves(int player, board* b)
{
	move* newMove = new move(player);
	newMove->val = b->value(player);

	return newMove;
}
int board::value(int player)
{
	if (boardOwner() == player + 1)
	{
		return INT_MAX;
	}
	if (boardOwner() == (!player) + 1)
	{
		return 0;
	}
	int val = 0;
	for (int x = 0; x < 3; ++x)
	{
		for (int y = 0; y < 3; ++y)
		{
			if (gridOwner(x, y) == player + 1)
			{
				val += 100;
			}
			if (gridOwner(x, y) == (!player) + 1)
			{
				val -= 50;
			}
		}
	}
}
bool board::playMove(int gX, int gY, int bX, int bY, int player)
{
	if (playableGrid != 3 * gY + gX && playableGrid != -1)
	{
		return false;
	}
	if (gridOwner(gX, gY) != 0)
	{
		return false;
	}
	if (boxes[gX][gY][bX][bY]->owner != 0)
	{
		return false;
	}
	else {
		boxes[gX][gY][bX][bY]->owner = player + 1;
		if (gridOwner(bX, bY) != 0)
		{
			playableGrid = -1;
		}
		else {
			playableGrid = 3 * bY + bX;
		}
		return true;
	}
}
int board::gridOwner(int x, int y)
{
	int owners[3][3];
	for (int col = 0; col < 3; ++col)
	{
		for (int row = 0; row < 3; ++row)
		{
			owners[col][row] = boxes[x][y][col][row]->owner;
		}
	}
	if (owners[0][0] == owners[1][1] && owners[0][0] == owners[2][2] && owners[0][0] != 0)	//Diagonal
	{
		return owners[0][0];
	}
	if (owners[2][0] == owners[1][1] && owners[2][0] == owners[0][2] && owners[2][0] != 0)	//Anti-diagonal
	{
		return owners[2][0];
	}
	for (int row = 0; row < 3; ++row)
	{
		if (owners[0][row] == owners[1][row] && owners[0][row] == owners[2][row] && owners[0][row] != 0) //Horizontal
		{
			return owners[0][row];
		}
	}
	for (int col = 0; col < 3; ++col)
	{
		if (owners[col][0] == owners[col][1] && owners[col][0] == owners[col][2] && owners[col][0] != 0) //Vertical
		{
			return owners[col][0];
		}
	}
	return 0;
}
int board::boardOwner()
{
	int owners[3][3];
	for (int col = 0; col < 3; ++col)
	{
		for (int row = 0; row < 3; ++row)
		{
			owners[col][row] = gridOwner(col, row);
		}
	}
	if (owners[0][0] == owners[1][1] && owners[0][0] == owners[2][2] && owners[0][0] != 0)	//Diagonal
	{
		return owners[0][0];
	}
	if (owners[2][0] == owners[1][1] && owners[2][0] == owners[0][2] && owners[2][0] != 0)	//Anti-diagonal
	{
		return owners[2][0];
	}
	for (int row = 0; row < 3; ++row)
	{
		if (owners[0][row] == owners[1][row] && owners[0][row] == owners[2][row] && owners[0][row] != 0) //Horizontal
		{
			return owners[0][row];
		}
	}
	for (int col = 0; col < 3; ++col)
	{
		if (owners[col][0] == owners[col][1] && owners[col][0] == owners[col][2] && owners[col][0] != 0) //Vertical
		{
			return owners[col][0];
		}
	}
	return 0;
}