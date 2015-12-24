#include <Windows.h>
#include <SFML/Graphics.hpp>
using namespace sf;

Color color1 = Color::Red;
Color color2 = Color::Blue;
Color playableColor = Color(255, 153, 0);
int alpha = 150;

int maxValue = INT_MIN;
int minValue = INT_MAX;

struct box {
	int owner; //0 = unowned, 1 = p1, 2 = p2
	box()
	{
		owner = 0;
	}
	~box()
	{

	}
};

struct board {
	box* boxes[3][3][3][3];	//Grid X, Grid Y, Box X, Box Y.
	
	int gridOwner(int x, int y);
	int playableGrid;
	int boardOwner();

	int value(int player);

	bool playMove(int gX, int gY, int bX, int bY, int player);
	bool gridIsWinnable(int gX, int gY, int player);
	bool playerCanWinGridInOneMove(int gX, int gY, int player);
	bool playerWillWinGameWithGrid(int gX, int gY, int player);
	bool gridDoesNotHelpPlayerToWin(int gX, int gY, int player);

	board* copy();

	void printBoard();

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
	~board()
	{
		for (int gY = 0; gY < 3; ++gY)
		{
			for (int gX = 0; gX < 3; ++gX)
			{
				for (int bY = 0; bY < 3; ++bY)
				{
					for (int bX = 0; bX < 3; ++bX)
					{
						delete (this->boxes[gX][gY][bX][bY]);
					}
				}
			}
		}
	}
};

struct move {
	move* next;
	move* result;
	board* board;
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
		delete board;
	}
};

move* getMoves(int player, board* b, int depth, int valuePlayer, bool allowFullGrid);

int alphabeta(move* node, int a, int b, bool maximizing);

void printTree(move* n, int depth, bool max);

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
		if (currPlayer == 1 && b->boardOwner() == 0)
		{
			move* root = getMoves(currPlayer, b, 3, currPlayer, false);
			//printTree(root, 0);
			move* moveWalker = root;
			int index = 0;
			int chosenIndex = 0;
			int min = INT_MAX;
			int max = INT_MIN;
			//printTree(root, 0, true);
			while (moveWalker != NULL)
			{
				int val;
				val = alphabeta(moveWalker, INT_MIN, INT_MAX, true);
				//moveWalker->board->printBoard();
				//printf("Value: %d\n\n", val);
				printf("%d, ", val);
				if (val > max)
				{
					max = val;
					chosenIndex = index;
				}
				if (val < min)
				{
					min = val;
				}
				index++;
				moveWalker = moveWalker->next;
			}
			printf("\n");
			printf("Chosen: %d\nIndex: %d/%d\n", max, chosenIndex+1, index);
			
			//printf("Max: %d\nMin: %d\nChosen: %d\nValue: %d\n", max,min,max,b->value(currPlayer));
			index = 0;
			for (int gY = 0; gY < 3; ++gY)
			{
				for (int gX = 0; gX < 3; ++gX)
				{
					if ((b->playableGrid == 3 * gY + gX || b->playableGrid == -1) && b->gridOwner(gX, gY) == 0)
					{
						for (int bY = 0; bY < 3; ++bY)
						{
							for (int bX = 0; bX < 3; ++bX)
							{
								if (b->boxes[gX][gY][bX][bY]->owner == 0)
								{
									if (index == chosenIndex)
									{
										b->playMove(gX, gY, bX, bY, currPlayer);
										currPlayer = !currPlayer;
									}
									index++;
								}
							}
						}
					}
				}
			}
			delete root;
		}
//PLAYER
		else if (currPlayer == 0 && b->boardOwner() == 0)
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

		for (int gridY = 0; gridY < 3; ++gridY)
		{
			for (int gridX = 0; gridX < 3; ++gridX)
			{
				Vector2f gridCorner = Vector2f(boxSize.x + 4 * gridX * boxSize.x, boxSize.y * 1.5f + 4 * gridY * boxSize.y);
				for (int y = 0; y < 3; ++y)
				{
					for (int x = 0; x < 3; ++x)
					{
						if (b->boxes[gridX][gridY][x][y]->owner != 0 || (b->playableGrid == -1 && b->gridOwner(gridX,gridY) == 0 && b->boardOwner() == 0) || (b->playableGrid == 3 * gridY + gridX && b->boardOwner() == 0))
						{
							RectangleShape boxRect;
							Vector2f boxCorner = Vector2f(boxSize.x*(x + 0.05f) + gridCorner.x, boxSize.y*(y + 0.05f) + gridCorner.y);
							boxRect.setSize(Vector2f(boxSize.x * 0.9f, boxSize.y*0.9f));
							boxRect.setPosition(boxCorner);
							if (b->boxes[gridX][gridY][x][y]->owner == 1)
							{
								boxRect.setFillColor(color1);
							}
							else if (b->boxes[gridX][gridY][x][y]->owner == 2) {
								boxRect.setFillColor(color2);
							}
							else {
								boxRect.setFillColor(playableColor);
							}
							window.draw(boxRect);
						}
					}
				}
				if (b->gridOwner(gridX,gridY) != 0)
				{
					RectangleShape gridRect;
					gridRect.setSize(Vector2f(boxSize.x*3.5f, boxSize.y*3.5f));
					gridRect.setPosition(Vector2f(gridCorner.x - boxSize.x*0.25f, gridCorner.y - boxSize.y*0.25f));
					if (b->gridOwner(gridX, gridY) == 1)
					{
						gridRect.setFillColor(Color(color1.r, color1.g, color1.b, alpha));
					}
					else if (b->gridOwner(gridX, gridY) == 2)
					{
						gridRect.setFillColor(Color(color2.r, color2.g, color2.b, alpha));
					}
					else {
						gridRect.setFillColor(Color(0, 0, 0, 255 - (255 - alpha) / 2));
					}
					window.draw(gridRect);
				}

			}
		}

		window.display();
	}

	return 0;
}

void printTree(move* n, int depth, bool max)
{
	if (n == NULL)
		return;
	move* nodeWalker = n;
	while (nodeWalker != NULL)
	{
		for (int i = 0; i < depth; ++i)
			printf("  ");
		printf("%d\n", nodeWalker->val);
		printTree(nodeWalker->result, depth + 1, !max);
		nodeWalker = nodeWalker->next;
	}
}

void board::printBoard()
{
	printf("\n");
	for (int gY = 0; gY < 3; ++gY)
	{
		for (int bY = 0; bY < 3; bY++)
		{
			for (int gX = 0; gX < 3; ++gX)
			{
				for (int bX = 0; bX < 3; bX++)
				{
					if (boxes[gX][gY][bX][bY]->owner == 1)
					{
						printf("X");
					}else if (boxes[gX][gY][bX][bY]->owner == 2)
					{
						printf("O");
					}
					else {
						printf(" ");
					}
				}
				if (gX < 2)
					printf("|");
			}
			printf("\n");
		}
		if (gY < 2)
			printf("---+---+---\n");
	}
}

bool board::gridIsWinnable(int gX, int gY, int player)
{
	if (this->gridOwner(gX, gY) == player + 1)
	{
		return true;
	}
	else if (this->gridOwner(gX, gY) == !player + 1)
	{
		return false;
	}
	else {
		int owners[3][3];
		for (int col = 0; col < 3; ++col)
		{
			for (int row = 0; row < 3; ++row)
			{
				owners[col][row] = boxes[gX][gY][col][row]->owner;
			}
		}
		if (owners[0][0] != !player + 1 && owners[1][1] != !player + 1 && owners[2][2] != !player + 1)	//Diagonal
		{
			return true;
		}
		if (owners[2][0] != !player + 1 && owners[1][1] != !player + 1 && owners[0][2] != !player + 1)	//Anti-diagonal
		{
			return true;
		}
		for (int row = 0; row < 3; ++row)
		{
			if (owners[0][row] != !player + 1 && owners[1][row] != !player + 1 && owners[2][row] != !player + 1)	//Horizontal
			{
				return true;
			}
		}
		for (int col = 0; col < 3; ++col)
		{
			if (owners[col][0] != !player + 1 && owners[col][1] != !player + 1 && owners[col][2] != !player + 1)	//Vertical
			{
				return true;
			}
		}
		return false;
	}
}
bool board::playerCanWinGridInOneMove(int gX, int gY, int player) {
	if (this->gridOwner(gX, gY) == player + 1)
	{
		return true;
	}
	else if (this->gridOwner(gX, gY) == !player + 1)
	{
		return false;
	}
	else {
		int owners[3][3];
		for (int col = 0; col < 3; ++col)
		{
			for (int row = 0; row < 3; ++row)
			{
				owners[col][row] = boxes[gX][gY][col][row]->owner;
			}
		}
		if ((owners[0][0] == player + 1 && owners[1][1] == player + 1 && owners[2][2] == 0) ||
			(owners[0][0] == player + 1 && owners[1][1] == 0 && owners[2][2] == player + 1) ||
			(owners[0][0] == 0 && owners[1][1] == player + 1 && owners[2][2] == player + 1))	//Diagonal
		{
			return true;
		}
		if ((owners[2][0] == player + 1 && owners[1][1] == player + 1 && owners[0][2] == 0) ||
			(owners[2][0] == player + 1 && owners[1][1] == 0 && owners[0][2] == player + 1) ||
			(owners[2][0] == 0 && owners[1][1] == player + 1 && owners[0][2] == player + 1))	//Anti-diagonal
		{
			return true;
		}
		for (int row = 0; row < 3; ++row)
		{
			if ((owners[0][row] == player + 1 && owners[1][row] == player + 1 && owners[2][row] == 0) ||
				(owners[0][row] == player + 1 && owners[1][row] == 0 && owners[2][row] == player + 1) ||
				(owners[0][row] == 0 && owners[1][row] == player + 1 && owners[2][row] == player + 1))	//Horizontal
			{
				return true;
			}
		}
		for (int col = 0; col < 3; ++col)
		{
			if ((owners[col][0] == player + 1 && owners[col][1] == player + 1 && owners[col][2] == 0) ||
				(owners[col][0] == player + 1 && owners[col][1] == 0 && owners[col][2] == player + 1) ||
				(owners[col][0] == 0 && owners[col][1] == player + 1 && owners[col][2] == player + 1))	//Vertical
			{
				return true;
			}
		}
		return false;
	}
}
bool board::playerWillWinGameWithGrid(int gX, int gY, int player)
{
	//DOES NOT IMPLY GRID IS WINNABLE!
	int gX2 = (gX + 1) % 3;
	int gX3 = (gX + 2) % 3;
	int gY2 = (gY + 1) % 3;
	int gY3 = (gY + 2) % 3;
	if (gX == gY && gridOwner(gX2, gY2) == player + 1 && gridOwner(gX3, gY3) == player + 1) //Diagonal
	{
		return true;
	}
	if (gX + gY == 2 && gridOwner(gX2, gY3) == player + 1 && gridOwner(gX3, gY2) == player + 1) //Anti-Diagonal
	{
		return true;
	}
	if (gridOwner(gX2, gY) == player + 1 && gridOwner(gX3, gY) == player + 1) //Horizontal
	{
		return true;
	}
	if (gridOwner(gX, gY2) == player + 1 && gridOwner(gX, gY3) == player + 1) //Vertical
	{
		return true;
	}
	return false;
}
bool board::gridDoesNotHelpPlayerToWin(int gX, int gY, int player) {
	if (gridIsWinnable(gX, gY, player) == false)
		return true;
	else {
		int gX2 = (gX + 1) % 3;
		int gX3 = (gX + 2) % 3;
		int gY2 = (gY + 1) % 3;
		int gY3 = (gY + 2) % 3;
		if (!(gX == gY && (gridOwner(gX2, gY2) == !player + 1 || gridOwner(gX3, gY3) == !player + 1))) //Diagonal
		{
			return false;
		}
		if (!(gX + gY == 2 && (gridOwner(gX2, gY3) == !player + 1 || gridOwner(gX3, gY2) == !player + 1))) //Anti-Diagonal
		{
			return false;
		}
		if (!(gridOwner(gX2, gY) == !player + 1 || gridOwner(gX3, gY) == !player + 1)) //Horizontal
		{
			return false;
		}
		if (!(gridOwner(gX, gY2) == !player + 1 || gridOwner(gX, gY3) == !player + 1)) //Vertical
		{
			return false;
		}
		return true;
	}
}

int alphabeta(move* node, int a, int b, bool maximizing)
{
	int bestValue = 0;
	if (maximizing)
		bestValue = INT_MIN;
	else
		bestValue = INT_MAX;
	move* moveWalker = node;
	while (moveWalker != NULL)
	{
		int val = moveWalker->val;
		if (moveWalker->result != NULL)
			val = alphabeta(moveWalker->result, a, b, !maximizing);
		if (maximizing)
			bestValue = max(bestValue, val);
		else
			bestValue = min(bestValue, val);
		moveWalker = moveWalker->next;
	}
	return bestValue;
	/*
	int val;
	if (maximizing)
	{
		val = a;
		move* nodeWalker = node;
		while (nodeWalker != NULL)
		{
			int childValue = nodeWalker->val;
			if (nodeWalker->result != NULL)
				childValue = alphabeta(nodeWalker->result, val, b, false);
			val = max(val, childValue);
			if (b <= val)
			{
				break;
			}
			nodeWalker = nodeWalker->next;
		}
	}
	else {
		val = b;
		move* nodeWalker = node;
		while (nodeWalker != NULL)
		{
			int childValue = nodeWalker->val;
			if (nodeWalker->result != NULL)
				childValue = alphabeta(nodeWalker->result, a, val, true);
			val = min(val, childValue);
			if (val <= a)
			{
				break;
			}
			nodeWalker = nodeWalker->next;
		}
	}
	return val;
	*/
}

move* getMoves(int player, board* b, int depth, int valuePlayer, bool allowFullGrid)
{
	if (depth <= 0)
		return NULL;
	if (b->boardOwner() != 0)
		return NULL;
	move* root = NULL;
	move* moveWalker = NULL;

	for (int gY = 0; gY < 3; ++gY)
	{
		for (int gX = 0; gX < 3; ++gX)
		{
			if ((b->playableGrid == 3 * gY + gX || b->playableGrid == -1) && b->gridOwner(gX,gY) == 0)
			{
				for (int bY = 0; bY < 3; ++bY)
				{
					for (int bX = 0; bX < 3; ++bX)
					{
						if (b->boxes[gX][gY][bX][bY]->owner == 0 && (b->gridOwner(bX,bY) == 0 || allowFullGrid == true))
						{
							int depthSub = 1;
							if (b->playableGrid == -1)
								depthSub++;
							if (moveWalker == NULL)
							{
								moveWalker = new move(player);
								root = moveWalker;
							}
							else {
								move* newMove = new move(player);
								moveWalker->next = newMove;
								moveWalker = moveWalker->next;
							}
							board* newBoard = b->copy();
							newBoard->playMove(gX, gY, bX, bY, player);
							moveWalker->val = newBoard->value(valuePlayer);
							moveWalker->board = newBoard;
							if (newBoard->boardOwner() == 0)
								moveWalker->result = getMoves(!player, newBoard, depth - depthSub, valuePlayer, false);
							//delete newBoard;
						}
					}
				}
			}
		}
	}
	return root;
}

board* board::copy()
{
	board* newBoard = new board();
	newBoard->playableGrid = playableGrid;
	for (int gY = 0; gY < 3; ++gY)
	{
		for (int gX = 0; gX < 3; ++gX)
		{
			for (int bY = 0; bY < 3; ++bY)
			{
				for (int bX = 0; bX < 3; ++bX)
				{
					newBoard->boxes[gY][gX][bY][bX]->owner = boxes[gY][gX][bY][bX]->owner;
				}
			}
		}
	}
	return newBoard;
}

int board::value(int player)
{
	if (boardOwner() == player + 1)
	{
		return INT_MAX;
	}
	else if (boardOwner() == !player + 1)
	{
		return INT_MIN;
	}
	else {
		int val = 0;
		for (int gY = 0; gY < 3; ++gY)
		{
			for (int gX = 0; gX < 3; ++gX)
			{
				if (gridOwner(gX, gY) == player + 1)
				{
					val += 100;
				}
				else if (gridOwner(gX, gY) == !player + 1)
				{
					val -= 100;
				}
				else if (gridOwner(gX, gY) == 0)
				{
					if (playerCanWinGridInOneMove(gX, gY, player))
					{
						val += 20;
					}
					else if (playerCanWinGridInOneMove(gX, gY, !player))
					{
						val -= 20;
					}
				}
			}
		}
		return val;
	}
	/*
	if (boardOwner() == player + 1)
	{
		return INT_MAX;
	}
	int val = 0;
	for (int gY = 0; gY < 3; ++gY)
	{
		for (int gX = 0; gX < 3; ++gX)
		{
			if (gridOwner(gX, gY) == player + 1)
			{
				val += 100;
			}
			else if (gridOwner(gX, gY) == 0)
			{
				int boxCount = 0;
				for (int bY = 0; bY < 3; ++bY)
				{
					for (int bX = 0; bX < 3; ++bX)
					{
						if (boxes[gX][gY][bX][bY]->owner == player + 1)
						{
							boxCount++;
						}
					}
				}
				if (boxCount > 1)
				{
					val += (boxCount-1)*10;
				}
			}
		}
	}
	return val;
	*/
}
/*
int board::value(int player)
{
	int val = 0;
	if (boardOwner() == player + 1)
	{
		return INT_MAX;
	}
	for (int gY = 0; gY < 3; ++gY)
	{
		for (int gX = 0; gX < 3; ++gX)
		{
			if (gridOwner(gX, gY) == player + 1)
			{
				val += 100000;
			}
			if (gridOwner(gX, gY) != !player + 1 && gridOwner(gX, gY) != -1)
			{
				if (playerWillWinGameWithGrid(gX, gY, player))
				{
					val += 1000;
					if (playerCanWinGridInOneMove(gX, gY, player))
					{
						val += 100;
					}
				}

				if (gridOwner(gX, gY) == !player + 1 && gridDoesNotHelpPlayerToWin(gX, gY, !player) == true)
				{
					val += 50;
				}

				if (gridDoesNotHelpPlayerToWin(gX, gY, player) == false)	//Winning grid helps towards winning game. Also implies grid can be won
				{
					if (gridOwner(gX, gY) == player + 1)
					{
						val += 200;
					}
					if (playerCanWinGridInOneMove(gX, gY, player) == true)
					{
						val += 150;
					}
					else {
						val += 100;
					}
				}
				else {
					//Grid does not help win game
					if (playerWillWinGameWithGrid(gX, gY, !player) && gridOwner(gX, gY) == player)
					{
						val += 50;
					}
				}
			}
			/*
			for (int bX = 0; bX < 3; ++bX)
			{
				for (int bY = 0; bY < 3; ++bY)
				{

				}
			}
			*/
/*
		}
	}

	
	return val;
}

*/
bool board::playMove(int gX, int gY, int bX, int bY, int player)
{
	if (playableGrid != 3 * gY + gX && playableGrid != -1)
	{
		return false;
	}
	if (gridOwner(gX, gY) > 0)
	{
		return false;
	}
	if (boxes[gX][gY][bX][bY]->owner != 0)
	{
		return false;
	}
	else {
		boxes[gX][gY][bX][bY]->owner = player + 1;
		if (gridOwner(bX, bY) > 0)
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
	bool tie = true;
	for (int col = 0; col < 3; ++col)
	{
		for (int row = 0; row < 3; ++row)
		{
			owners[col][row] = boxes[x][y][col][row]->owner;
			if (owners[col][row] == 0)
				tie = false;
		}
	}
	if (tie)
		return -1;
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