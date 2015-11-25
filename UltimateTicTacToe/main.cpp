#include <Windows.h>
#include <SFML/Graphics.hpp>
using namespace sf;

Color color1 = Color::Red;
Color color2 = Color::Blue;
Color playableColor = Color(255, 153, 0);
int alpha = 150;

int nodeCount = 0;

struct box {
	int owner;
	struct grid* grid;
	int posX;
	int posY;

	box()
	{
		owner = 0;
	}
};

struct grid {
	box* boxes[3][3];
	struct board* board;
	int owner;
	int posX;
	int posY;

	int winner();

	bool hasTwoInARow(int player);
	bool possibleToWin(int player);

	grid()
	{
		owner = 0;
		for (int y = 0; y < 3; ++y)
		{
			for (int x = 0; x < 3; ++x)
			{
				boxes[x][y] = new box();
				boxes[x][y]->grid = this;
				boxes[x][y]->posX = x;
				boxes[x][y]->posY = y;
			}
		}
	}
};

struct board {
	grid* grids[3][3];
	int playableGrid;
	int owner;

	int winner();

	void drawBoard(Vector2f boxSize, RenderWindow* window);
	void updateBoard();

	bool hasTwoInARow(int player);

	board* copy();

	board()
	{
		owner = 0;
		playableGrid = -1;
		for (int y = 0; y < 3; ++y)
		{
			for (int x = 0; x < 3; ++x)
			{
				grids[x][y] = new grid();
				grids[x][y]->board = this;
				grids[x][y]->posX = x;
				grids[x][y]->posY = y;
			}
		}
	}
};



struct move {
	box* box;
	move* next;
	move* prev;
	int player;
	struct node* result;
	struct node* parentNode;

	node* getNextMoves(int depth);

	move()
	{
		next = NULL;
		result = NULL;
		box = NULL;
		player = 0;
	}
};

struct node {
	move* first;
	board* board;
	int moveCount;
	int P1Value;
	int P2Value;

	node(struct board* newBoard) {
		first = NULL;
		board = newBoard;
		moveCount = 0;
		P1Value = 0;
		P2Value = 0;
		if (board->owner == 1)
		{
			P1Value = INT_MAX;
			P2Value = 0;
		}
		else if (board->owner == 2)
		{
			P1Value = 0;
			P2Value = INT_MAX;
		}
		else {
			
			if (board->hasTwoInARow(1))
				P1Value += 50;
			if (board->hasTwoInARow(2))
				P2Value += 50;
			for (int y = 0; y < 3; ++y)
			{
				for (int x = 0; x < 3; ++x)
				{
					if (board->grids[x][y]->owner == 1)
						P1Value += 50;
					if (board->grids[x][y]->owner == 2)
						P2Value += 50;
					if (board->grids[x][y]->hasTwoInARow(1))
						P1Value += 10;
					if (board->grids[x][y]->hasTwoInARow(2))
						P2Value += 10;
					bool possible1 = board->grids[x][y]->possibleToWin(1);
					bool possible2 = board->grids[x][y]->possibleToWin(2);
					if (possible1 || possible2)
					{
						for (int boxY = 0; boxY < 3; ++boxY)
						{
							for (int boxX = 0; boxX < 3; ++boxX)
							{
								if (board->grids[x][y]->boxes[boxX][boxY]->owner == 1 && possible1)
									P1Value += 1;
								if (board->grids[x][y]->boxes[boxX][boxY]->owner == 2 && possible2)
									P2Value += 1;
							}
						}
					}
				}
			}
		}
	}
};

int alphabeta(node* node, int depth, int a, int b, int player, bool maximizing);

int main()
{
	Clock clock;
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
	line[index++] = Vertex(Vector2f(12.5f*boxSize.x, 9.0f*boxSize.y) , lineColor);

	Font font;

	//font.loadFromFile("Roboto-Light.ttf");
	
	Text text;
	text.setFont(font);
	text.setString("PLAYER 1's turn");
	text.setCharacterSize(36);
	text.setColor(color1);

	board* board = new struct board();

	int currPlayer = 0;
	bool mouseDown = false;

	while (window.isOpen())
	{
		board->updateBoard();
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

//AI
		if (currPlayer == 1 && board->owner == 0)
		{
			struct board* newBoard = new struct board();
			newBoard = board->copy();
			node* root = new node(newBoard);
			move* currMove = (move*)malloc(sizeof(move));
			currMove = NULL;
			for (int gridY = 0; gridY < 3; ++gridY)
			{
				for (int gridX = 0; gridX < 3; ++gridX)
				{
					if (root->board->playableGrid == -1 || root->board->playableGrid == 3 * gridY + gridX)
					{
						for (int boxY = 0; boxY < 3; ++boxY)
						{
							for (int boxX = 0; boxX < 3; ++boxX)
							{
								if (root->board->grids[gridX][gridY]->boxes[boxX][boxY]->owner == 0)
								{
									move* newMove = new move();
									newMove->box = root->board->grids[gridX][gridY]->boxes[boxX][boxY];
									newMove->player = currPlayer + 1;
									newMove->next = NULL;
									newMove->parentNode = root;
									if (currMove == NULL)
									{
										currMove = newMove;
										root->first = newMove;
										currMove->prev = NULL;
									}
									else {
										newMove->prev = currMove;
										currMove->next = newMove;
										currMove = currMove->next;
									}
									root->moveCount++;
								}
							}
						}
					}
				}
			}
			nodeCount = 0;
			int values[81];
			if (root->board->owner == 0)
			{
				move* moveWalker = root->first;
				for (int i = 0; i < root->moveCount; ++i)
				{
					moveWalker->result = moveWalker->getNextMoves(3);
					values[i] = alphabeta(moveWalker->result, 3, INT_MIN, INT_MAX, !currPlayer+1, true);
					moveWalker = moveWalker->next;
				}
			}
			int max = INT_MIN;
			move* moves[81];
			int moveCounter = 0;
			move* moveWalker = root->first;
			for (int i = 0; i < root->moveCount; ++i)
			{
				if (values[i] > max)
				{
					max = values[i];
					moves[moveCounter++] = moveWalker;
				}
				moveWalker = moveWalker->next;
			}

			printf("%d\n", max);
			
			int randMove = rand() % moveCounter;

			moveWalker = root->first;
			for (int i = 0; i < randMove; ++i)
			{
				moveWalker = moveWalker->next;
			}

			board->grids[moveWalker->box->grid->posX][moveWalker->box->grid->posY]->boxes[moveWalker->box->posX][moveWalker->box->posY]->owner = currPlayer + 1;
			board->playableGrid = 3 * moveWalker->box->posY + moveWalker->box->posX;
			if (board->grids[moveWalker->box->posX][moveWalker->box->posY]->owner != 0)
				board->playableGrid = -1;

			currPlayer = !currPlayer;
		}
//PLAYER
		if (currPlayer == 0 && board->owner == 0)
		{
			if (Mouse::isButtonPressed(Mouse::Left) && board->winner() == 0)
			{
				if (!mouseDown)
				{
					clock.restart();
					mouseDown = true;
					Vector2i mousePos = Mouse::getPosition(window);
					for (int gridY = 0; gridY < 3; ++gridY)
					{
						for (int gridX = 0; gridX < 3; ++gridX)
						{
							if (board->grids[gridX][gridY]->owner == 0 && (board->playableGrid == 3* gridY + gridX || board->playableGrid == -1))
							{
								Vector2f corner = Vector2f(boxSize.x + 4 * gridX * boxSize.x, boxSize.y * 1.5f + 4 * gridY * boxSize.y);
								if (mousePos.x > corner.x && mousePos.x < corner.x + 3 * boxSize.x && mousePos.y > corner.y && mousePos.y < corner.y + 3 * boxSize.y)
								{
									int x = (int)floor((mousePos.x - corner.x) / (boxSize.x));
									int y = (int)floor((mousePos.y - corner.y) / (boxSize.y));
									if (board->grids[gridX][gridY]->boxes[x][y]->owner == 0)
									{
										board->grids[gridX][gridY]->boxes[x][y]->owner = currPlayer + 1;
										board->updateBoard();
										board->playableGrid = 3 * y + x;
										if (board->grids[x][y]->owner != 0)
										{
											board->playableGrid = -1;
										}
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

		board->drawBoard(boxSize, &window);

		window.draw(text);

		window.display();
	}

	return 0;
}

int alphabeta(node* node, int depth, int a, int b, int player, bool maximizing)
{
	if (node == NULL)
		return INT_MIN;
	int value = node->P1Value;
	if (player == 2)
		value = node->P2Value;
	if (depth == 0 || node == NULL)
		return value;
	if (maximizing == true)
	{
		int v = INT_MIN;
		move* moveWalker = node->first;
		for (int i = 0; i < node->moveCount; ++i)
		{
			v = max(v, alphabeta(moveWalker->result, depth - 1, a, b, player, false));
			moveWalker = moveWalker->next;
			a = max(a, v);
			if (b <= a)
				break;
		}
		return v;
	}
	else {
		int v = INT_MAX;
		move* moveWalker = node->first;
		for (int i = 0; i < node->moveCount; ++i)
		{
			v = min(v, alphabeta(moveWalker->result, depth - 1, a, b, player, true));
			moveWalker = moveWalker->next;
			b = min(b, v);
			if (b <= a)
				break;
		}
		return v;
	}
}

int board::winner()
{
	for (int y = 0; y < 3; ++y)
	{
		if (grids[0][y]->owner == grids[1][y]->owner && grids[1][y]->owner == grids[2][y]->owner && grids[2][y]->owner != 0)
			return grids[0][y]->owner;
	}
	for (int x = 0; x < 3; ++x)
	{
		if (grids[x][0]->owner == grids[x][1]->owner && grids[x][1]->owner == grids[x][2]->owner && grids[x][2]->owner != 0)
			return grids[x][0]->owner;
	}
	if (grids[0][0]->owner == grids[1][1]->owner && grids[1][1]->owner == grids[2][2]->owner && grids[2][2]->owner != 0)
		return grids[0][0]->owner;
	if (grids[2][0]->owner == grids[1][1]->owner && grids[1][1]->owner == grids[0][2]->owner && grids[0][2]->owner != 0)
		return grids[2][0]->owner;
	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			if (grids[x][y]->owner == 0)
				return 0;
		}
	}
	return -1;
}

void board::drawBoard(Vector2f boxSize, RenderWindow* window)
{
	for (int gridY = 0; gridY < 3; ++gridY)
	{
		for (int gridX = 0; gridX < 3; ++gridX)
		{
			Vector2f gridCorner = Vector2f(boxSize.x + 4 * gridX * boxSize.x, boxSize.y * 1.5f + 4 * gridY * boxSize.y);
			for (int y = 0; y < 3; ++y)
			{
				for (int x = 0; x < 3; ++x)
				{
					if (grids[gridX][gridY]->boxes[x][y]->owner != 0 || (playableGrid == -1 && grids[gridX][gridY]->owner == 0 && owner == 0) || (playableGrid == 3 * gridY + gridX && owner == 0))
					{
						RectangleShape boxRect;
						Vector2f boxCorner = Vector2f(boxSize.x*(x + 0.05f) + gridCorner.x, boxSize.y*(y + 0.05f) + gridCorner.y);
						boxRect.setSize(Vector2f(boxSize.x * 0.9f, boxSize.y*0.9f));
						boxRect.setPosition(boxCorner);
						if (grids[gridX][gridY]->boxes[x][y]->owner == 1)
						{
							boxRect.setFillColor(color1);
						}
						else if (grids[gridX][gridY]->boxes[x][y]->owner == 2) {
							boxRect.setFillColor(color2);
						}
						else {
							boxRect.setFillColor(playableColor);
						}
						window->draw(boxRect);
					}
				}
			}
			if (grids[gridX][gridY]->owner != 0)
			{
				RectangleShape gridRect;
				gridRect.setSize(Vector2f(boxSize.x*3.5f, boxSize.y*3.5f));
				gridRect.setPosition(Vector2f(gridCorner.x - boxSize.x*0.25f, gridCorner.y - boxSize.y*0.25f));
				if (grids[gridX][gridY]->owner == 1)
				{
					gridRect.setFillColor(Color(color1.r, color1.g, color1.b, alpha));
				}
				else if (grids[gridX][gridY]->owner == 2)
				{
					gridRect.setFillColor(Color(color2.r, color2.g, color2.b, alpha));
				}
				else {
					gridRect.setFillColor(Color(0, 0, 0, 255 - (255 - alpha) / 2));
				}
				window->draw(gridRect);
			}

		}
	}

}

void board::updateBoard()
{
	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			grids[x][y]->owner = grids[x][y]->winner();
		}
	}
	owner = winner();
}

board* board::copy() {
	board* newBoard = (board*)malloc(sizeof(board));

	newBoard->playableGrid = playableGrid;
	newBoard->owner = owner;

	for (int gridY = 0; gridY < 3; ++gridY)
	{
		for (int gridX = 0; gridX < 3; ++gridX)
		{
			newBoard->grids[gridX][gridY] = (grid*)malloc(sizeof(grid));
			newBoard->grids[gridX][gridY]->board = this;
			newBoard->grids[gridX][gridY]->posX = grids[gridX][gridY]->posX;
			newBoard->grids[gridX][gridY]->posY = grids[gridX][gridY]->posY;
			newBoard->grids[gridX][gridY]->owner = grids[gridX][gridY]->owner;
			for (int boxY = 0; boxY < 3; ++boxY)
			{
				for (int boxX = 0; boxX < 3; ++boxX)
				{
					newBoard->grids[gridX][gridY]->boxes[boxX][boxY] = (box*)malloc(sizeof(box));
					newBoard->grids[gridX][gridY]->boxes[boxX][boxY]->grid = newBoard->grids[gridX][gridY];
					newBoard->grids[gridX][gridY]->boxes[boxX][boxY]->owner = grids[gridX][gridY]->boxes[boxX][boxY]->owner;
					newBoard->grids[gridX][gridY]->boxes[boxX][boxY]->posX = grids[gridX][gridY]->boxes[boxX][boxY]->posX;
					newBoard->grids[gridX][gridY]->boxes[boxX][boxY]->posY = grids[gridX][gridY]->boxes[boxX][boxY]->posY;
				}
			}
		}
	}
	return newBoard;
}

bool board::hasTwoInARow(int player)
{
	for (int y = 0; y < 3; ++y)
	{
		if ((grids[0][y]->owner == player && grids[1][y]->owner == player && grids[2][y]->possibleToWin(player)) ||
			(grids[0][y]->owner == player && grids[1][y]->possibleToWin(player) && grids[2][y]->owner == player) ||
			(grids[0][y]->possibleToWin(player) && grids[1][y]->owner == player && grids[2][y]->owner == player))
		{
			return true;
		}
	}
	for (int x = 0; x < 3; ++x)
	{
		if ((grids[x][0]->owner == player && grids[x][1]->owner == player && grids[x][2]->possibleToWin(player)) ||
			(grids[x][0]->owner == player && grids[x][1]->possibleToWin(player) && grids[x][2]->owner == player) ||
			(grids[x][0]->possibleToWin(player) && grids[x][1]->owner == player && grids[x][2]->owner == player))
		{
			return true;
		}
	}
	if ((grids[0][0]->owner == player && grids[1][1]->owner == player && grids[2][2]->possibleToWin(player)) ||
		(grids[0][0]->owner == player && grids[1][1]->possibleToWin(player) && grids[2][2]->owner == player) ||
		(grids[0][0]->possibleToWin(player) && grids[1][1]->owner == player && grids[2][2]->owner == player))
	{
		return true;
	}
	if ((grids[2][0]->owner == player && grids[1][1]->owner == player && grids[0][2]->possibleToWin(player)) ||
		(grids[2][0]->owner == player && grids[1][1]->possibleToWin(player) && grids[0][2]->owner == player) ||
		(grids[2][0]->possibleToWin(player) && grids[1][1]->owner == player && grids[0][2]->owner == player))
	{
		return true;
	}
	return false;
}

bool grid::possibleToWin(int player)
{
	if (owner == player)
	{
		return true;
	}
	if (hasTwoInARow(player) == true)
	{
		return true;
	}
	for (int y = 0; y < 3; ++y)
	{
		if ((boxes[0][y]->owner == player && boxes[1][y]->owner == 0 && boxes[2][y]->owner == 0) ||
			(boxes[0][y]->owner == 0 && boxes[1][y]->owner == player && boxes[2][y]->owner == 0) ||
			(boxes[0][y]->owner == 0 && boxes[1][y]->owner == 0 && boxes[2][y]->owner == player))
		{
			return true;
		}
	}
	for (int x = 0; x < 3; ++x)
	{
		if ((boxes[x][0]->owner == player && boxes[x][1]->owner == 0 && boxes[x][2]->owner == 0) ||
			(boxes[x][0]->owner == 0 && boxes[x][1]->owner == player && boxes[x][2]->owner == 0) ||
			(boxes[x][0]->owner == 0 && boxes[x][1]->owner == 0 && boxes[x][2]->owner == player))
		{
			return true;
		}
	}
	if ((boxes[0][0]->owner == player && boxes[1][1]->owner == 0 && boxes[2][2]->owner == 0) ||
		(boxes[0][0]->owner == 0 && boxes[1][1]->owner == player && boxes[2][2]->owner == 0) ||
		(boxes[0][0]->owner == 0 && boxes[1][1]->owner == 0 && boxes[2][2]->owner == player))
	{
		return true;
	}
	if ((boxes[2][0]->owner == player && boxes[1][1]->owner == 0 && boxes[0][2]->owner == 0) ||
		(boxes[2][0]->owner == 0 && boxes[1][1]->owner == player && boxes[0][2]->owner == 0) ||
		(boxes[2][0]->owner == 0 && boxes[1][1]->owner == 0 && boxes[0][2]->owner == player))
	{
		return true;
	}
	return false;
}

bool grid::hasTwoInARow(int player)
{
	for (int y = 0; y < 3; ++y)
	{
		if ((boxes[0][y]->owner == player && boxes[1][y]->owner == player && boxes[2][y]->owner == 0) ||
			(boxes[0][y]->owner == player && boxes[1][y]->owner == 0 && boxes[2][y]->owner == player) ||
			(boxes[0][y]->owner == 0 && boxes[1][y]->owner == player && boxes[2][y]->owner == player))
		{
			return true;
		}
	}
	for (int x = 0; x < 3; ++x)
	{
		if ((boxes[x][0]->owner == player && boxes[x][1]->owner == player && boxes[x][2]->owner == 0) ||
			(boxes[x][0]->owner == player && boxes[x][1]->owner == 0 && boxes[x][2]->owner == player) ||
			(boxes[x][0]->owner == 0 && boxes[x][1]->owner == player && boxes[x][2]->owner == player))
		{
			return true;
		}
	}
	if ((boxes[0][0]->owner == player && boxes[1][1]->owner == player && boxes[2][2]->owner == 0) ||
		(boxes[0][0]->owner == player && boxes[1][1]->owner == 0 && boxes[2][2]->owner == player) ||
		(boxes[0][0]->owner == 0 && boxes[1][1]->owner == player && boxes[2][2]->owner == player))
	{
		return true;
	}
	if ((boxes[2][0]->owner == player && boxes[1][1]->owner == player && boxes[0][2]->owner == 0) ||
		(boxes[2][0]->owner == player && boxes[1][1]->owner == 0 && boxes[0][2]->owner == player) ||
		(boxes[2][0]->owner == 0 && boxes[1][1]->owner == player && boxes[0][2]->owner == player))
	{
		return true;
	}
	return false;
}

node* move::getNextMoves(int depth)
{
	if (parentNode->board->playableGrid == -1)
		return NULL;
	nodeCount++;
	struct board* newBoard = new struct board();
	newBoard = parentNode->board->copy();
	newBoard->grids[box->grid->posX][box->grid->posY]->boxes[box->posX][box->posY]->owner = player;
	newBoard->updateBoard();
	node* newNode = new node(newBoard);
	move* currMove = (move*)malloc(sizeof(move));
	currMove = NULL;
	for (int gridY = 0; gridY < 3; ++gridY)
	{
		for (int gridX = 0; gridX < 3; ++gridX)
		{
			if (parentNode->board->playableGrid == -1 || parentNode->board->playableGrid == 3 * gridY + gridX)
			{
				for (int boxY = 0; boxY < 3; ++boxY)
				{
					for (int boxX = 0; boxX < 3; ++boxX)
					{
						if (parentNode->board->grids[gridX][gridY]->boxes[boxX][boxY]->owner == 0)
						{
							move* newMove = new move();
							newMove->box = newNode->board->grids[gridX][gridY]->boxes[boxX][boxY];
							newMove->player = !(player-1) + 1;
							newMove->next = NULL;
							newMove->parentNode = newNode;
							if (currMove == NULL)
							{
								currMove = newMove;
								newNode->first = newMove;
								currMove->prev = NULL;
							}
							else {
								newMove->prev = currMove;
								currMove->next = newMove;
								currMove = currMove->next;
							}
							newNode->moveCount++;
						}
					}
				}
			}
		}
	}
	if (depth > 0 && newNode->board->owner == 0 && nodeCount < 30000)
	{
		move* moveWalker = newNode->first;
		for (int i = 0; i < newNode->moveCount; ++i)
		{
			moveWalker->result = moveWalker->getNextMoves(depth-1);
			moveWalker = moveWalker->next;
		}
	}
	return newNode;
}

int grid::winner()
{
	for (int y = 0; y < 3; ++y)
	{
		if (boxes[0][y]->owner == boxes[1][y]->owner && boxes[1][y]->owner == boxes[2][y]->owner && boxes[2][y]->owner != 0)
			return boxes[0][y]->owner;
	}
	for (int x = 0; x < 3; ++x)
	{
		if (boxes[x][0]->owner == boxes[x][1]->owner && boxes[x][1]->owner == boxes[x][2]->owner && boxes[x][2]->owner != 0)
			return boxes[x][0]->owner;
	}
	if (boxes[0][0]->owner == boxes[1][1]->owner && boxes[1][1]->owner == boxes[2][2]->owner && boxes[2][2]->owner != 0)
		return boxes[0][0]->owner;
	if (boxes[2][0]->owner == boxes[1][1]->owner && boxes[1][1]->owner == boxes[0][2]->owner && boxes[0][2]->owner != 0)
		return boxes[2][0]->owner;
	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			if (boxes[x][y]->owner == 0)
				return 0;
		}
	}
	return -1;

}

