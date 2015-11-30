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
	//struct grid* grid;
	int posX;
	int posY;

	box()
	{
		owner = 0;
	}

	~box()
	{
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
				//boxes[x][y]->grid = this;
				boxes[x][y]->posX = x;
				boxes[x][y]->posY = y;
			}
		}
	}
	~grid()
	{
		delete[9] boxes;
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
	~board()
	{
		delete[9] grids;
	}
};



struct move {
	move* next;
	struct node* result;

	move()
	{
		next = NULL;
		result = NULL;
	}
};

struct node {
	move* first;
	int player;
	int value;

	node() {
		first = NULL;
		value = 0;
	}
};

int alphabeta(node* node, int depth, int a, int b, bool maximizing);

void getNextMoves(node* node, board* currBoard, int depth);

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

	/*
	board->grids[0][0]->boxes[0][0]->owner = 2;
	board->grids[0][0]->boxes[1][0]->owner = 2;
	board->grids[0][0]->boxes[2][0]->owner = 2;
	board->grids[1][0]->boxes[0][0]->owner = 2;
	board->grids[1][0]->boxes[1][0]->owner = 2;
	board->grids[1][0]->boxes[2][0]->owner = 2;
	board->grids[2][0]->boxes[0][0]->owner = 2;
	board->grids[2][0]->boxes[1][0]->owner = 2;
	board->updateBoard();
	*/


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
			//struct board* newBoard = new struct board();
			//newBoard = board->copy();
			node* root = new node();
			root->player = currPlayer + 1;

			nodeCount = 0;
			getNextMoves(root, board, 2);

			move* moveWalker = root->first;
			int max = INT_MIN;
			move* chosenMove = moveWalker;
			int index = 0;
			int chosenIndex;
			while (moveWalker != NULL)
			{
				int val = alphabeta(moveWalker->result, 3, INT_MIN, INT_MAX, false);
				if (val > max)
				{
					max = val;
					chosenMove = moveWalker;
					chosenIndex = index;
				}

				index++;
				moveWalker = moveWalker->next;
			}
			printf("%d\n", max);
			index = 0;
			struct box* chosenBox;
			bool found = false;
			for (int gridY = 0; gridY < 3 && found == false; ++gridY)
			{
				for (int gridX = 0; gridX < 3 && found == false; ++gridX)
				{
					if (board->grids[gridX][gridY]->owner == 0 && (board->playableGrid == 3*gridY+gridX || board->playableGrid == -1) && found == false)
					{
						for (int boxY = 0; boxY < 3 && found == false; ++boxY)
						{
							for (int boxX = 0; boxX < 3 && found == false; ++boxX)
							{
								if (board->grids[gridX][gridY]->boxes[boxX][boxY]->owner == 0 && found == false)
								{
									if (index == chosenIndex && found == false)
									{
										board->grids[gridX][gridY]->boxes[boxX][boxY]->owner = currPlayer+1;
										board->playableGrid = 3 * boxY + boxX;
										if (board->grids[boxX][boxY]->owner != 0)
											board->playableGrid = -1;
										found = true;
									}
									index++;
								}
							}
						}
					}
				}
			}
			printf("%d\n", nodeCount);
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

void getNextMoves(node* node, board* currBoard, int depth)
{
	//node->moveCount = 0;
	move* moveWalker = NULL;
	nodeCount++;
	for (int gridY = 0; gridY < 3; ++gridY)
	{
		for (int gridX = 0; gridX < 3; ++gridX)
		{
			if (currBoard->grids[gridX][gridY]->owner == 0)
			{
				if (currBoard->playableGrid == 3 * gridY + gridX || currBoard->playableGrid == -1)
				{
					for (int boxY = 0; boxY < 3; boxY++)
					{
						for (int boxX = 0; boxX < 3; boxX++)
						{
							if (currBoard->grids[gridX][gridY]->boxes[boxX][boxY]->owner == 0)
							{
								move* newMove = new move();
								newMove->next = NULL;
								//newMove->parentNode = node;
								if (moveWalker == NULL)
								{
									moveWalker = newMove;
									node->first = newMove;
									//moveWalker->prev = NULL;
								}
								else {
									//newMove->prev = moveWalker;
									moveWalker->next = newMove;
									moveWalker = moveWalker->next;
								}
								struct board* newBoard = new struct board();
								newBoard = currBoard->copy();
								newBoard->grids[gridX][gridY]->boxes[boxX][boxY]->owner = node->player;
								newBoard->updateBoard();

								newBoard->playableGrid = 3 * boxY + boxX;
								if (newBoard->grids[boxX][boxY]->owner != 0)
								{
									newBoard->playableGrid = -1;
								}
								struct node* newNode = new struct node();
								newNode->player = !(node->player - 1) + 1;
								moveWalker->result = newNode;

								if (newBoard->owner == newNode->player)
								{
									newNode->value = INT_MAX;
								}
								else if (newBoard->owner != 0){
									newNode->value = 0;
								} else {

									if (newBoard->hasTwoInARow(newNode->player))
									{
										newNode->value += 50;
									}
									if (newBoard->hasTwoInARow(!(newNode->player-1)+1))
									{
										newNode->value -= 25;
									}
									for (int y = 0; y < 3; ++y)
									{
										for (int x = 0; x < 3; ++x)
										{
											if (newBoard->grids[x][y]->owner == newNode->player)
											{
												newNode->value += 50;
											}else if (newBoard->grids[x][y]->owner == !(newNode->player-1)+1)
											{
												newNode->value -= 25;
											}
											if (newBoard->grids[x][y]->hasTwoInARow(newNode->player))
												newNode->value += 10;
											bool possible1 = newBoard->grids[x][y]->possibleToWin(1);
											if (newBoard->grids[x][y]->possibleToWin(newNode->player))
											{
												for (int boxY = 0; boxY < 3; ++boxY)
												{
													for (int boxX = 0; boxX < 3; ++boxX)
													{
														if (newBoard->grids[x][y]->boxes[boxX][boxY]->owner == newNode->player)
															newNode->value += 1;
													}
												}
											}
										}
									}
								}

								if (newBoard->owner == 0 && newBoard->playableGrid != -1 && depth > 0)
								{
									getNextMoves(newNode, currBoard, depth-1);
								}

								//delete newMove;
								/*
								for (int gX = 0; gX < 3; ++gX)
								{
									for (int gY = 0; gY < 3; ++gY)
									{
										for (int x = 0; x < 3; ++x)
										{
											for (int y = 0; y < 3; ++y)
											{
												delete newBoard->grids[gX][gY]->boxes[x][y];
											}
										}
										delete newBoard->grids[gX][gY];
									}
								}
								*/
								//delete newBoard;
								//delete newNode;
							}
						}
					}
				}
			}
		}
	}
}

int alphabeta(node* node, int depth, int a, int b, bool maximizing)
{
	if (node == NULL)
	{
		if (maximizing)
			return INT_MIN;
		return INT_MAX;
	}
	int value = node->value;
	if (maximizing == false)
		value = -value;
	if (depth == 0 || node == NULL)
		return value;
	if (maximizing == true)
	{
		int v = INT_MIN;
		move* moveWalker = node->first;
		while (moveWalker != NULL)
		{
			v = max(v, alphabeta(moveWalker->result, depth - 1, a, b, false));
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
		while (moveWalker != NULL)
		{
			v = min(v, alphabeta(moveWalker->result, depth - 1, a, b, true));
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
	board* newBoard = new board();

	newBoard->playableGrid = playableGrid;
	newBoard->owner = owner;

	for (int gridY = 0; gridY < 3; ++gridY)
	{
		for (int gridX = 0; gridX < 3; ++gridX)
		{
			newBoard->grids[gridX][gridY] = new grid();
			newBoard->grids[gridX][gridY]->board = this;
			newBoard->grids[gridX][gridY]->posX = grids[gridX][gridY]->posX;
			newBoard->grids[gridX][gridY]->posY = grids[gridX][gridY]->posY;
			newBoard->grids[gridX][gridY]->owner = grids[gridX][gridY]->owner;
			for (int boxY = 0; boxY < 3; ++boxY)
			{
				for (int boxX = 0; boxX < 3; ++boxX)
				{
					newBoard->grids[gridX][gridY]->boxes[boxX][boxY] = new box();
					//newBoard->grids[gridX][gridY]->boxes[boxX][boxY]->grid = newBoard->grids[gridX][gridY];
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

