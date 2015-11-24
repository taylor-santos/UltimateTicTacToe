#include <Windows.h>
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
	Vector2f pos;
	Vector2f corner;

	box(Vector2f boxPos){
		owner = 0;
		rectangle.setFillColor(Color(255, 255, 255, 0));
		pos = boxPos;
	}
	bool willWinGrid(int player);
	bool createsTwoInARow(int player);
};

struct grid {
	RectangleShape rectangle;
	struct board* parentBoard;
	bool playable;
	int owner = 0;
	box* boxes[3][3];
	Vector2f corner;
	Vector2f pos;

	grid(Vector2f gridPos)
	{
		playable = true;
		rectangle.setFillColor(Color(255, 255, 255, 0));
		pos = gridPos;
	}

	bool checkTwoInARow(int player)
	{
		for (int y = 0; y < 3; ++y)
		{
			if ((boxes[0][y]->owner == boxes[1][y]->owner && boxes[1][y]->owner == player && boxes[2][y]->owner == 0) ||
				(boxes[1][y]->owner == boxes[2][y]->owner && boxes[2][y]->owner == player && boxes[0][y]->owner == 0) ||
				(boxes[2][y]->owner == boxes[0][y]->owner && boxes[0][y]->owner == player && boxes[1][y]->owner == 0))
			{
				return true;
			}
		}
		for (int x = 0; x < 3; ++x)
		{
			if ((boxes[x][0]->owner == boxes[x][1]->owner && boxes[x][1]->owner == player && boxes[x][2]->owner == 0) ||
				(boxes[x][1]->owner == boxes[x][2]->owner && boxes[x][2]->owner == player && boxes[x][0]->owner == 0) ||
				(boxes[x][2]->owner == boxes[x][0]->owner && boxes[x][0]->owner == player && boxes[x][1]->owner == 0))
			{
				return true;
			}
		}
		if ((boxes[0][0]->owner == boxes[1][1]->owner && boxes[1][1]->owner == player && boxes[2][2]->owner == 0) ||
			(boxes[1][1]->owner == boxes[2][2]->owner && boxes[2][2]->owner == player && boxes[0][0]->owner == 0) ||
			(boxes[2][2]->owner == boxes[0][0]->owner && boxes[0][0]->owner == player && boxes[1][1]->owner == 0) ||
			(boxes[2][0]->owner == boxes[1][1]->owner && boxes[1][1]->owner == player && boxes[0][2]->owner == 0) ||
			(boxes[1][1]->owner == boxes[0][2]->owner && boxes[0][2]->owner == player && boxes[2][0]->owner == 0) ||
			(boxes[0][2]->owner == boxes[2][0]->owner && boxes[2][0]->owner == player && boxes[1][1]->owner == 0))
		{
			return true;
		}
		return false;
	}

	int checkComplete()
	{
		if (owner != 0)
			return owner;
		for (int y = 0; y < 3; ++y)
		{
			if (boxes[0][y]->owner == boxes[1][y]->owner && boxes[1][y]->owner == boxes[2][y]->owner && boxes[2][y]->owner != 0)
			{
				owner = boxes[0][y]->owner;
				if (owner == 1)
				{
					rectangle.setFillColor(Color(color1.r,color1.g,color1.b, alpha));
				}
				else if (owner == 2) {
					rectangle.setFillColor(Color(color2.r, color2.g, color2.b, alpha));
				}
				return owner;
			}
		}
		for (int x = 0; x < 3; ++x)
		{
			if (boxes[x][0]->owner == boxes[x][1]->owner && boxes[x][1]->owner == boxes[x][2]->owner && boxes[x][2]->owner != 0)
			{
				owner = boxes[x][0]->owner;
				if (owner == 1)
				{
					rectangle.setFillColor(Color(color1.r, color1.g, color1.b, alpha));
				}
				else if (owner == 2) {
					rectangle.setFillColor(Color(color2.r, color2.g, color2.b, alpha));
				}
				return owner;
			}
		}
		if (boxes[0][0]->owner == boxes[1][1]->owner && boxes[1][1]->owner == boxes[2][2]->owner && boxes[2][2]->owner != 0)
		{
			owner = boxes[0][0]->owner;
			if (owner == 1)
			{
				rectangle.setFillColor(Color(color1.r, color1.g, color1.b, alpha));
			}
			else if (owner == 2) {
				rectangle.setFillColor(Color(color2.r, color2.g, color2.b, alpha));
			}
			return owner;
		}
		if (boxes[2][0]->owner == boxes[1][1]->owner && boxes[1][1]->owner == boxes[0][2]->owner && boxes[0][2]->owner != 0)
		{
			owner = boxes[2][0]->owner;
			if (owner == 1)
			{
				rectangle.setFillColor(Color(color1.r, color1.g, color1.b, alpha));
			}
			else if (owner == 2) {
				rectangle.setFillColor(Color(color2.r, color2.g, color2.b, alpha));
			}
			return owner;
		}
		for (int x = 0; x < 3; ++x)
		{
			for (int y = 0; y < 3; ++y)
			{
				if (boxes[x][y]->owner == 0)
				{
					return 0;
				}
			}
		}
		owner = 3;
		rectangle.setFillColor(Color(0, 0, 0, 255 - (255-alpha)/2));
		return 3;
	}

	bool canWinGame(int player);

	bool cannotWinGame(int player);
};

struct board {
	grid* grids[3][3];
	int winner;
	board()
	{
		winner = 0;
	}
	int checkWinner()
	{
		if (winner != 0)
			return winner;
		for (int y = 0; y < 3; ++y)
		{
			if (grids[0][y]->owner == grids[1][y]->owner && grids[1][y]->owner == grids[2][y]->owner && grids[2][y]->owner != 0 && grids[2][y]->owner != 3)
			{
				winner = grids[0][y]->owner;
				return winner;
			}
		}
		for (int x = 0; x < 3; ++x)
		{
			if (grids[x][0]->owner == grids[x][1]->owner && grids[x][1]->owner == grids[x][2]->owner && grids[x][2]->owner != 0 && grids[x][2]->owner != 3)
			{
				winner = grids[x][0]->owner;
				return winner;
			}
		}
		if (grids[0][0]->owner == grids[1][1]->owner && grids[1][1]->owner == grids[2][2]->owner && grids[2][2]->owner != 0 && grids[2][2]->owner != 3)
		{
			winner = grids[0][0]->owner;
			return winner;
		}
		if (grids[2][0]->owner == grids[1][1]->owner && grids[1][1]->owner == grids[0][2]->owner && grids[0][2]->owner != 0 && grids[0][2]->owner != 3)
		{
			winner = grids[2][0]->owner;
			return winner;
		}
		for (int x = 0; x < 3; ++x)
		{
			for (int y = 0; y < 3; ++y)
			{
				if (grids[x][y]->owner == 0)
				{
					return 0;
				}
			}
		}
		winner = 3;
		return 3;
	}
};

struct move {
	box* box;
	bool willWinGrid;
	bool blocksEnemy;
	bool givesEnemyNextGrid;
	bool createsTwoInARow;
	bool allowsEnemyToBlock;
	bool opensBoardForEnemy;
	bool enemysNextMoveWinsGame;
	bool enemysNextMoveBlocksTwo;
	bool willWinGame;
	bool needGridToWin;
	bool enemyNeedsGridToWin;
	bool doNotNeedGrid;
	bool enemyDoesNotNeedGrid;
	bool enemyNextMoveIsGood;
	move()
	{
		willWinGrid = false;
		blocksEnemy = false;
		givesEnemyNextGrid = false;
		createsTwoInARow = false;
		allowsEnemyToBlock = false;
		opensBoardForEnemy = false;
	}
};

bool box::willWinGrid(int player)
{
	if (owner != 0)
	{
		return false;
	}
	int x = (int)pos.x;
	int x1 = (x + 1) % 3;
	int x2 = (x + 2) % 3;
	int y = (int)pos.y;
	if (parentGrid->boxes[x1][y]->owner == parentGrid->boxes[x2][y]->owner && parentGrid->boxes[x2][y]->owner == player)
	{
		return true;
	}
	int y1 = (y + 1) % 3;
	int y2 = (y + 2) % 3;
	if (parentGrid->boxes[x][y1]->owner == parentGrid->boxes[x][y2]->owner && parentGrid->boxes[x][y2]->owner == player)
	{
		return true;
	}
	if (x == 1 && y == 1)
	{
		if ((parentGrid->boxes[0][0]->owner == parentGrid->boxes[2][2]->owner && parentGrid->boxes[2][2]->owner == player) ||
			(parentGrid->boxes[0][2]->owner == parentGrid->boxes[2][0]->owner && parentGrid->boxes[2][0]->owner == player))
		{
			return true;
		}
	}
	if (x - y == 0)	//Part of top-left to bot-right diagonal
	{
		if (parentGrid->boxes[x1][y1]->owner == parentGrid->boxes[x2][y2]->owner && parentGrid->boxes[x2][y2]->owner == player)
		{
			return true;
		}
	}
	if (abs(x - y) == 2) //Part of top-right to bot-left diagonal
	{
		if (parentGrid->boxes[x2][y1]->owner == parentGrid->boxes[x1][y2]->owner && parentGrid->boxes[x1][y2]->owner == player)
		{
			return true;
		}
	}
	return false;
}

bool box::createsTwoInARow(int player)
{
	if (owner != 0)
	{
		return false;
	}
	int x = (int)pos.x;
	int x1 = (x + 1) % 3;
	int x2 = (x + 2) % 3;
	int y = (int)pos.y;
	if ((parentGrid->boxes[x1][y]->owner == player && parentGrid->boxes[x2][y]->owner == 0) ||
		(parentGrid->boxes[x2][y]->owner == player && parentGrid->boxes[x1][y]->owner == 0))
	{
		return true;
	}
	int y1 = (y + 1) % 3;
	int y2 = (y + 2) % 3;
	if ((parentGrid->boxes[x][y1]->owner == player && parentGrid->boxes[x][y2]->owner == 0) ||
		(parentGrid->boxes[x][y2]->owner == player && parentGrid->boxes[x][y1]->owner == 0))
	{
		return true;
	}
	if (x == 1 && y == 1)
	{
		if ((parentGrid->boxes[0][0]->owner == player && parentGrid->boxes[2][2]->owner == 0) ||
			(parentGrid->boxes[2][2]->owner == player && parentGrid->boxes[0][0]->owner == 0) ||
			(parentGrid->boxes[0][2]->owner == player && parentGrid->boxes[2][0]->owner == 0) ||
			(parentGrid->boxes[2][0]->owner == player && parentGrid->boxes[0][2]->owner == 0))
		{
			return true;
		}
	}
	if (x - y == 0)	//Part of top-left to bot-right diagonal
	{
		if ((parentGrid->boxes[x1][y1]->owner == player && parentGrid->boxes[x2][y2]->owner == 0) ||
			(parentGrid->boxes[x2][y2]->owner == player && parentGrid->boxes[x1][y1]->owner == 0))
		{
			return true;
		}
	}
	if (abs(x - y) == 2) //Part of top-right to bot-left diagonal
	{
		if ((parentGrid->boxes[x1][y2]->owner == player && parentGrid->boxes[x2][y1]->owner == 0) ||
			(parentGrid->boxes[x2][y1]->owner == player && parentGrid->boxes[x1][y2]->owner == 0))
		{
			return true;
		}
	}
	return false;
}

bool grid::canWinGame(int player)
{
	if (owner != 0)
	{
		for (int x = 0; x < 3; ++x)
		{
			for (int y = 0; y < 3; ++y)
			{
				if (parentBoard->grids[x][y]->owner == 0 && parentBoard->grids[x][y]->canWinGame(player))
				{
					return true;
				}
			}
		}
		return false;
	}
	int x = (int)pos.x;
	int x1 = (x + 1) % 3;
	int x2 = (x + 2) % 3;
	int y = (int)pos.y;
	if (parentBoard->grids[x1][y]->owner == parentBoard->grids[x2][y]->owner && parentBoard->grids[x2][y]->owner == player)
	{
		return true;
	}
	int y1 = (y + 1) % 3;
	int y2 = (y + 2) % 3;
	if (parentBoard->grids[x][y1]->owner == parentBoard->grids[x][y2]->owner && parentBoard->grids[x][y2]->owner == player)
	{
		return true;
	}
	if (x == 1 && y == 1)
	{
		if ((parentBoard->grids[0][0]->owner == parentBoard->grids[2][2]->owner && parentBoard->grids[2][2]->owner == player) ||
			(parentBoard->grids[0][2]->owner == parentBoard->grids[2][0]->owner && parentBoard->grids[2][0]->owner == player))
		{
			return true;
		}
	}
	if (x - y == 0)	//Part of top-left to bot-right diagonal
	{
		if (parentBoard->grids[x1][y1]->owner == parentBoard->grids[x2][y2]->owner && parentBoard->grids[x2][y2]->owner == player)
		{
			return true;
		}
	}
	if (abs(x - y) == 2) //Part of top-right to bot-left diagonal
	{
		if (parentBoard->grids[x2][y1]->owner == parentBoard->grids[x1][y2]->owner && parentBoard->grids[x1][y2]->owner == player)
		{
			return true;
		}
	}
	return false;
}

bool grid::cannotWinGame(int player)
{
	if (owner != 0)
	{
		return false;
	}
	int x = (int)pos.x;
	int x1 = (x + 1) % 3;
	int x2 = (x + 2) % 3;
	int y = (int)pos.y;
	if ((parentBoard->grids[x1][y]->owner == player && parentBoard->grids[x2][y]->owner == player) ||
		(parentBoard->grids[x1][y]->owner == 0 && parentBoard->grids[x2][y]->owner == player) ||
		(parentBoard->grids[x1][y]->owner == player && parentBoard->grids[x2][y]->owner == 0) ||
		(parentBoard->grids[x1][y]->owner == 0 && parentBoard->grids[x2][y]->owner == 0))
	{
		return false;
	}
	int y1 = (y + 1) % 3;
	int y2 = (y + 2) % 3;
	if ((parentBoard->grids[x][y1]->owner == player && parentBoard->grids[x][y2]->owner == player) ||
		(parentBoard->grids[x][y1]->owner == 0 && parentBoard->grids[x][y2]->owner == player) ||
		(parentBoard->grids[x][y1]->owner == player && parentBoard->grids[x][y2]->owner == 0) ||
		(parentBoard->grids[x][y1]->owner == 0 && parentBoard->grids[x][y2]->owner == 0))
	{
		return false;
	}
	if (x == 1 && y == 1)
	{
		if ((parentBoard->grids[0][0]->owner == player && parentBoard->grids[2][2]->owner == player) ||
			(parentBoard->grids[0][0]->owner == 0 && parentBoard->grids[2][2]->owner == player) ||
			(parentBoard->grids[0][0]->owner == player && parentBoard->grids[2][2]->owner == 0) ||
			(parentBoard->grids[0][0]->owner == 0 && parentBoard->grids[2][2]->owner == 0))
		{
			return false;
		}
		if ((parentBoard->grids[2][0]->owner == player && parentBoard->grids[0][2]->owner == player) ||
			(parentBoard->grids[2][0]->owner == 0 && parentBoard->grids[0][2]->owner == player) ||
			(parentBoard->grids[2][0]->owner == player && parentBoard->grids[0][2]->owner == 0) ||
			(parentBoard->grids[2][0]->owner == 0 && parentBoard->grids[0][2]->owner == 0))
		{
			return false;
		}
	}
	if (x - y == 0)	//Part of top-left to bot-right diagonal
	{
		if ((parentBoard->grids[x1][y1]->owner == player && parentBoard->grids[x2][y2]->owner == player) ||
			(parentBoard->grids[x1][y1]->owner == 0 && parentBoard->grids[x2][y2]->owner == player) ||
			(parentBoard->grids[x1][y1]->owner == player && parentBoard->grids[x2][y2]->owner == 0) ||
			(parentBoard->grids[x1][y1]->owner == 0 && parentBoard->grids[x2][y2]->owner == 0))
		{
			return false;
		}
	}
	if (abs(x - y) == 2) //Part of top-right to bot-left diagonal
	{
		if ((parentBoard->grids[x2][y1]->owner == player && parentBoard->grids[x1][y2]->owner == player) ||
			(parentBoard->grids[x2][y1]->owner == 0 && parentBoard->grids[x1][y2]->owner == player) ||
			(parentBoard->grids[x2][y1]->owner == player && parentBoard->grids[x1][y2]->owner == 0) ||
			(parentBoard->grids[x2][y1]->owner == 0 && parentBoard->grids[x1][y2]->owner == 0))
		{
			return false;
		}
	}
	return true;
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

	board *board = (struct board*)malloc(sizeof (struct board));
	board->winner = 0;

	box* boxes[81];
	int boxIndex = 0;

	
	Font font;

	//font.loadFromFile("Roboto-Light.ttf");
	
	Text text;
	text.setFont(font);
	text.setString("PLAYER 1's turn");
	text.setCharacterSize(36);
	text.setColor(color1);
	
	

	for (int gridY = 0; gridY < 3; ++gridY)
	{
		for (int gridX = 0; gridX < 3; ++gridX)
		{
			board->grids[gridX][gridY] = new grid(Vector2f(gridX,gridY));
			board->grids[gridX][gridY]->parentBoard = board;
			board->grids[gridX][gridY]->corner = Vector2f(boxSize.x + 4 * gridX * boxSize.x, boxSize.y * 1.5f + 4 * gridY * boxSize.y);
			board->grids[gridX][gridY]->rectangle.setSize(boxSize*3.5f);
			board->grids[gridX][gridY]->rectangle.setPosition(board->grids[gridX][gridY]->corner - boxSize*0.25f);
			for (int boxX = 0; boxX < 3; ++boxX)
			{
				for (int boxY = 0; boxY < 3; ++boxY)
				{
					board->grids[gridX][gridY]->boxes[boxX][boxY] = new box(Vector2f(boxX,boxY));
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
		if (currPlayer == 0 && clock.getElapsedTime() >= seconds(0.1f))
		{
			if (board->winner == 0)// && Mouse::isButtonPressed(Mouse::Left))
			{
				if (!mouseDown || true)
				{
					mouseDown = true;
					clock.restart();
					move* moves[81];
					int boxIndex = 0;
					for (int i = 0; i < 81; ++i)
					{
						if (boxes[i]->owner == 0 && boxes[i]->parentGrid->playable == true)
						{
							moves[boxIndex] = new move();
							moves[boxIndex]->box = boxes[i];
							moves[boxIndex]->willWinGrid = moves[boxIndex]->box->willWinGrid(currPlayer + 1);
							moves[boxIndex]->blocksEnemy = moves[boxIndex]->box->willWinGrid(!currPlayer + 1);
							moves[boxIndex]->willWinGame = moves[boxIndex]->box->parentGrid->canWinGame(currPlayer + 1);
							moves[boxIndex]->box->owner = currPlayer + 1;
							moves[boxIndex]->givesEnemyNextGrid = board->grids[(int)boxes[i]->pos.x][(int)boxes[i]->pos.y]->checkTwoInARow(!currPlayer + 1);
							moves[boxIndex]->allowsEnemyToBlock = board->grids[(int)boxes[i]->pos.x][(int)boxes[i]->pos.y]->checkTwoInARow(currPlayer + 1);
								moves[boxIndex]->enemysNextMoveWinsGame = board->grids[(int)boxes[i]->pos.x][(int)boxes[i]->pos.y]->canWinGame(!currPlayer + 1);
								moves[boxIndex]->enemysNextMoveBlocksTwo = board->grids[(int)boxes[i]->pos.x][(int)boxes[i]->pos.y]->canWinGame(currPlayer + 1);
							if (board->grids[(int)boxes[i]->pos.x][(int)boxes[i]->pos.y]->owner != 0)
								moves[boxIndex]->opensBoardForEnemy = true;
							moves[boxIndex]->needGridToWin = board->grids[(int)boxes[i]->pos.x][(int)boxes[i]->pos.y]->canWinGame(currPlayer + 1);
							moves[boxIndex]->enemyNeedsGridToWin = board->grids[(int)boxes[i]->pos.x][(int)boxes[i]->pos.y]->canWinGame(!currPlayer + 1);
							moves[boxIndex]->doNotNeedGrid = board->grids[(int)boxes[i]->pos.x][(int)boxes[i]->pos.y]->cannotWinGame(currPlayer + 1);
							moves[boxIndex]->enemyDoesNotNeedGrid = board->grids[(int)boxes[i]->pos.x][(int)boxes[i]->pos.y]->cannotWinGame(!currPlayer + 1);
							moves[boxIndex]->box->owner = 0;
							moves[boxIndex]->createsTwoInARow = moves[boxIndex]->box->createsTwoInARow(currPlayer + 1);
							
							
							boxIndex++;
						}
					}

					int boxToPlay = -1;

					int moveIndex = 0;

					
					for (int i = 0; i < boxIndex; ++i)	//Wins game.
					{
						if (moves[i]->willWinGame == true && moves[i]->willWinGrid == true && moves[i])
						{
							moves[moveIndex++] = moves[i];
						}
					}
					/*

					if (moveIndex == 0)
					{
						for (int i = 0; i < boxIndex; ++i)
						{
							if (moves[i]->willWinGrid == true &&
								moves[i]->enemyNeedsGridToWin == false &&
								moves[i]->needGridToWin == false &&
								moves[i]->givesEnemyNextGrid == false &&
								moves[i]->opensBoardForEnemy == false &&
								moves[i]->allowsEnemyToBlock == false)
							{
								moves[moveIndex++] = moves[i];
							}
						}
					}
					if (moveIndex == 0)
					{
						for (int i = 0; i < boxIndex; ++i)
						{
							if (moves[i]->willWinGrid == true &&
								moves[i]->enemyNeedsGridToWin == false &&
								moves[i]->needGridToWin == false &&
								moves[i]->givesEnemyNextGrid == false &&
								moves[i]->opensBoardForEnemy == false &&
								moves[i]->allowsEnemyToBlock == true)
							{
								moves[moveIndex++] = moves[i];
							}
						}
					}
					if (moveIndex == 0)
					{
						for (int i = 0; i < boxIndex; ++i)
						{
							if (moves[i]->willWinGrid == true &&
								moves[i]->needGridToWin == false &&
								moves[i]->givesEnemyNextGrid == true &&
								moves[i]->enemyDoesNotNeedGrid == true &&
								moves[i]->opensBoardForEnemy == false)
							{
								moves[moveIndex++] = moves[i];
							}
						}
					}

					if (moveIndex == 0)
					{
						for (int i = 0; i < boxIndex; ++i)
						{
							if (moves[i]->givesEnemyNextGrid == false &&
								moves[i]->opensBoardForEnemy == false &&
								moves[i]->createsTwoInARow == true)
							{
								moves[moveIndex++] = moves[i];
							}
						}
					}
					if (moveIndex == 0)
					{
						for (int i = 0; i < boxIndex; ++i)
						{
							if (moves[i]->givesEnemyNextGrid == false &&
								moves[i]->opensBoardForEnemy == false)
							{
								moves[moveIndex++] = moves[i];
							}
						}
					}
					if (moveIndex == 0)
					{
						for (int i = 0; i < boxIndex; ++i)
						{
							if (moves[i]->givesEnemyNextGrid == true &&
								moves[i]->enemyDoesNotNeedGrid == true)
							{
								moves[moveIndex++] = moves[i];
							}
						}
					}
					if (moveIndex == 0)
					{
						for (int i = 0; i < boxIndex; ++i)
						{
							if (moves[i]->givesEnemyNextGrid == true &&
								moves[i]->enemyNeedsGridToWin == false)
							{
								moves[moveIndex++] = moves[i];
							}
						}
					}

					if (moveIndex == 0)
					{
						moveIndex = boxIndex;
					}

					boxToPlay = rand() % moveIndex;
					*/
					/*
					if (moveIndex == 0)
					{
						for (int i = 0; i < boxIndex; ++i)	//Wins game.
						{
							if (moves[i]->willWinGrid == true)
							{
								moves[moveIndex++] = moves[i];
							}
						}
						if (moveIndex != 0)
						{
							for (int i = 0; i < boxIndex; ++i)
							{
								if (moves[i]->enemyNeedsGridToWin == false && moves[i]->doNotNeedGrid == true && moves[i]->opensBoardForEnemy == false)
								{
									moves[moveIndex++] = moves[i];
								}
							}
						}
					}
					if (moveIndex == 0)
					{
						for (int i = 0; i < boxIndex; ++i)
						{
							if (moves[i]->givesEnemyNextGrid == false && moves[i]->opensBoardForEnemy == false && moves[i]->allowsEnemyToBlock == false)
							{
								moves[moveIndex++] = moves[i];
							}
						}
					}
					if (moveIndex == 0)
					{
						for (int i = 0; i < boxIndex; ++i)
						{
							if (moves[i]->enemyNeedsGridToWin == false && moves[i]->opensBoardForEnemy == false && moves[i]->allowsEnemyToBlock == false)
							{
								moves[moveIndex++] = moves[i];
							}
						}
					}
					*/

					
					if (moveIndex == 0)	//Wins grid, doesn’t give enemy next grid, doesn’t allow enemy to block, and doesn’t lead to a claimed grid.
					{
						for (int i = 0; i < boxIndex; ++i)
						{
							if (moves[i]->willWinGrid == true && moves[i]->givesEnemyNextGrid == false && moves[i]->allowsEnemyToBlock == false && moves[i]->opensBoardForEnemy == false)
							{
								moves[moveIndex++] = moves[i];
							}
						}
					}
					if (moveIndex == 0) //Wins grid, doesn’t give enemy next grid, allows enemy to block, and doesn’t lead to a claimed grid.
					{
						for (int i = 0; i < boxIndex; ++i)
						{
							if (moves[i]->willWinGrid == true && moves[i]->givesEnemyNextGrid == false && moves[i]->allowsEnemyToBlock == true && moves[i]->opensBoardForEnemy == false)
							{
								moves[moveIndex++] = moves[i];
							}
						}
					}
					if (moveIndex == 0)	//Wins grid, doesn’t give enemy next grid, and doesn’t lead to a claimed grid.
					{
						for (int i = 0; i < boxIndex; ++i)
						{
							if (moves[i]->willWinGrid == true && moves[i]->givesEnemyNextGrid == false && moves[i]->opensBoardForEnemy == false)
							{
								moves[moveIndex++] = moves[i];
							}
						}
					}
					if (moveIndex == 0)	//Creates two-in-a-row, doesn’t give enemy next grid, doesn’t allow enemy to block, and doesn’t lead to a claimed grid.
					{
						for (int i = 0; i < boxIndex; ++i)
						{
							if (moves[i]->createsTwoInARow == true && moves[i]->givesEnemyNextGrid == false && moves[i]->allowsEnemyToBlock == false && moves[i]->opensBoardForEnemy == false)
							{
								moves[moveIndex++] = moves[i];
							}
						}
					}
					if (moveIndex == 0)	//Creates two-in-a-row, doesn’t give enemy next grid, allows enemy to block, and doesn’t lead to a claimed grid.
					{
						for (int i = 0; i < boxIndex; ++i)
						{
							if (moves[i]->createsTwoInARow == true && moves[i]->givesEnemyNextGrid == false && moves[i]->allowsEnemyToBlock == true && moves[i]->opensBoardForEnemy == false)
							{
								moves[moveIndex++] = moves[i];
							}
						}
					}
					if (moveIndex == 0)
					{
						for (int i = 0; i < boxIndex; ++i)
						{
							if (moves[i]->givesEnemyNextGrid == false && moves[i]->blocksEnemy == true && moves[i]->allowsEnemyToBlock == false && moves[i]->opensBoardForEnemy == false)
							{
								moves[moveIndex++] = moves[i];
							}
						}
					}
					if (moveIndex == 0)
					{
						for (int i = 0; i < boxIndex; ++i)
						{
							if (moves[i]->givesEnemyNextGrid == false && moves[i]->blocksEnemy == true && moves[i]->allowsEnemyToBlock == true && moves[i]->opensBoardForEnemy == false)
							{
								moves[moveIndex++] = moves[i];
							}
						}
					}
					if (moveIndex == 0)
					{
						for (int i = 0; i < boxIndex; ++i)
						{
							if (moves[i]->givesEnemyNextGrid == false && moves[i]->allowsEnemyToBlock == false && moves[i]->opensBoardForEnemy == false)
							{
								moves[moveIndex++] = moves[i];
							}
						}
					}
					if (moveIndex == 0)
					{
						for (int i = 0; i < boxIndex; ++i)
						{
							if (moves[i]->givesEnemyNextGrid == false && moves[i]->allowsEnemyToBlock == true && moves[i]->opensBoardForEnemy == false)
							{
								moves[moveIndex++] = moves[i];
							}
						}
					}
					if (moveIndex == 0)
					{
						for (int i = 0; i < boxIndex; ++i)
						{
							if (moves[i]->willWinGrid == true && moves[i]->opensBoardForEnemy == false && moves[i]->enemysNextMoveWinsGame == false)
							{
								moves[moveIndex++] = moves[i];
							}
						}
					}
					
					if (moveIndex == 0)
					{
						for (int i = 0; i < boxIndex; ++i)
						{
							if (moves[i]->createsTwoInARow == true && moves[i]->opensBoardForEnemy == false && moves[i]->enemysNextMoveWinsGame == false)
							{
								moves[moveIndex++] = moves[i];
							}
						}
					}
					if (moveIndex == 0)
					{
						for (int i = 0; i < boxIndex; ++i)
						{
							if (moves[i]->blocksEnemy == true && moves[i]->opensBoardForEnemy == false && moves[i]->enemysNextMoveWinsGame == false)
							{
								moves[moveIndex++] = moves[i];
							}
						}
					}

					if (moveIndex == 0)
					{
						for (int i = 0; i < boxIndex; ++i)
						{
							moves[moveIndex++] = moves[i];
						}
					}
					boxIndex = 0;
					boxToPlay = (int)(rand() % moveIndex);
					
					
					
					/*
					for (int i = 0; i < moveIndex; ++i)
					{
						if ((int)moves[i]->box->pos.x == 1 && (int)moves[i]->box->pos.y == 1)
						{
							moves[boxIndex++] = moves[i];
						}
					}
					if (boxIndex != 0)
					{
						boxToPlay = (int)(rand() % boxIndex);
					}else{
						for (int i = 0; i < moveIndex; ++i)
						{
							if ((int)abs(moves[i]->box->pos.x-moves[i]->box->pos.y) != 1)
							{
								moves[boxIndex++] = moves[i];
							}
						}
					}
					
					if (boxIndex != 0 && boxToPlay == -1)
					{
						boxToPlay = (int)(rand() % boxIndex);
					}
					if (boxToPlay == -1)
					{
						boxToPlay = (int)(rand() % moveIndex);
					}
					*/
					/*
					if (currPlayer == 0)
					{
						printf("\n\nP1: ");
					}
					else {
						printf("\n\nP2: ");
					}
					printf(" Move...\n");
					bool nothing = true;
					if (moves[boxToPlay]->willWinGame)
					{
						printf("	Wins game!\n");
						nothing = false;
					}
					else {
						if (moves[boxToPlay]->willWinGrid)
						{
							printf("	Wins grid.\n");
							nothing = false;
						}
						if (moves[boxToPlay]->blocksEnemy)
						{
							printf("	Blocks enemy's two-in-a-row.\n");
							nothing = false;
						}
						if (moves[boxToPlay]->createsTwoInARow)
						{
							printf("	Creates two-in-a-row.\n");
							nothing = false;
						}
						if (nothing)
						{
							printf("	Is randomly chosen.\n");
						}
						nothing = true;
						printf("BUT...\n");
						if (moves[boxToPlay]->givesEnemyNextGrid)
						{
							printf("	Gives enemy next grid.\n");
							nothing = false;
						}
						if (moves[boxToPlay]->allowsEnemyToBlock)
						{
							printf("	Allows enemy to block two-in-a-row.\n");
							nothing = false;
						}
						if (moves[boxToPlay]->opensBoardForEnemy)
						{
							printf("	Opens board for enemy.\n");
							nothing = false;
						}
						if (moves[boxToPlay]->enemysNextMoveBlocksTwo)
						{
							printf("	Enemy blocks two-in-a-row grid win.\n");
							nothing = false;
						}
						if (moves[boxToPlay]->enemysNextMoveWinsGame)
						{
							printf("	Loses game.\n");
							nothing = false;
						}
						if (nothing)
						{
							printf("	Has no downside.\n");
						}
					}
					*/
						
					int row = moves[boxToPlay]->box->pos.x;
					int col = moves[boxToPlay]->box->pos.y;
					moves[boxToPlay]->box->owner = currPlayer + 1;
					moves[boxToPlay]->box->parentGrid->checkComplete();
					currPlayer = !currPlayer;
					board->checkWinner();
					if (board->winner == 0)
					{

						if (currPlayer == 0)
						{
							text.setString("PLAYER 1's turn");
							text.setColor(color1);
						}
						else {
							text.setString("PLAYER 2's turn");
							text.setColor(color2);
						}


						bool allPlayable = false;
						if (board->grids[row][col]->owner != 0)
						{
							allPlayable = true;
						}
						for (int i = 0; i < 3; ++i)
						{
							for (int j = 0; j < 3; ++j)
							{
								if (allPlayable)
								{
									if (board->grids[j][i]->owner == 0)
										board->grids[j][i]->playable = true;
									else
										board->grids[j][i]->playable = false;
								}
								else if (j == row && i == col)
								{
									board->grids[j][i]->playable = true;
								}
								else {
									board->grids[j][i]->playable = false;
								}
							}
						}
					}
					else {
						for (int i = 0; i < 3; ++i)
						{
							for (int j = 0; j < 3; ++j)
							{
								board->grids[j][i]->playable = false;
							}
						}
					}
				}
			}else {
				mouseDown = false;
			}
		}
		if (currPlayer == 1)
		{
			if (Mouse::isButtonPressed(Mouse::Left) && board->winner == 0)
			{
				if (!mouseDown)
				{
					clock.restart();
					mouseDown = true;
					Vector2i mousePos = Mouse::getPosition(window);
					for (int y = 0; y < 3; ++y)
					{
						for (int x = 0; x < 3; ++x)
						{
							if (board->grids[x][y]->owner == 0 && board->grids[x][y]->playable == true)
							{
								Vector2f corner = board->grids[x][y]->corner;
								if (mousePos.x > corner.x && mousePos.x < corner.x + 3 * boxSize.x && mousePos.y > corner.y && mousePos.y < corner.y + 3 * boxSize.y)
								{
									int row = (int)floor((mousePos.x - corner.x) / (boxSize.x));
									int col = (int)floor((mousePos.y - corner.y) / (boxSize.y));
									if (board->grids[x][y]->boxes[row][col]->owner == 0)
									{
										board->grids[x][y]->boxes[row][col]->owner = currPlayer + 1;
										board->grids[x][y]->checkComplete();
										currPlayer = !currPlayer;

										board->checkWinner();

										if (board->winner == 0)
										{

											if (currPlayer == 0)
											{
												text.setString("PLAYER 1's turn");
												text.setColor(color1);
											}
											else {
												text.setString("PLAYER 2's turn");
												text.setColor(color2);
											}


											bool allPlayable = false;
											if (board->grids[row][col]->owner != 0)
											{
												allPlayable = true;
											}
											for (int i = 0; i < 3; ++i)
											{
												for (int j = 0; j < 3; ++j)
												{
													if (allPlayable)
													{
														if (board->grids[j][i]->owner == 0)
															board->grids[j][i]->playable = true;
														else
															board->grids[j][i]->playable = false;
													}
													else if (j == row && i == col)
													{
														board->grids[j][i]->playable = true;
													}
													else {
														board->grids[j][i]->playable = false;
													}
												}
											}
										}
										else {
											for (int i = 0; i < 3; ++i)
											{
												for (int j = 0; j < 3; ++j)
												{
													board->grids[j][i]->playable = false;
												}
											}
										}
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

		if (board->winner != 0)
		{
			if (board->winner == 1)
			{
				text.setString("PLAYER 1 WINS");
				text.setColor(color1);
			}
			else if (board->winner == 2){
				text.setString("PLAYER 2 WINS");
				text.setColor(color2);
			}
			else {
				text.setString("TIE");
				text.setColor(Color::Black);
			}
		}

		FloatRect textRect = text.getLocalBounds();
		text.setOrigin((int)(textRect.left + textRect.width / 2.0f),
			textRect.top);
		text.setPosition(Vector2f((int)(screenSize.x / 2.0f), (int)(boxSize.y/4)));
		
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

		window.draw(text);

		window.display();
	}

	return 0;
}