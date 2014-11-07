#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <queue>
#include <stack>
using namespace std;

const string INPUT = "die.in";
const string OUTPUT = "die.out";
const int BOARD_SIZE = 99;
const int AST = 7;

typedef array<array<int, 4>, 3 > DieType;
typedef array<array<int, BOARD_SIZE>, BOARD_SIZE> BoardType;

// Only one of these are used for the duration of the program
//BoardType Board;
//DieType Die;

#define Marker 'X'
#define Marked(ch) (ch == Marker)

// Utility Functions
void ClearDie(DieType &Die)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 4; j++)
			Die[i][j] = 0;
}
void ClearBoard(BoardType &Board)
{
	for (int i = 0; i < BOARD_SIZE; i++)
for (int j = 0; j < BOARD_SIZE; j++)
	Board[i][j] = 0;
}
void DrawBoard(BoardType &Board, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (Board[i][j] == 7)
				cout << '*';
			else if (Board[i][j] == 88)
				cout << 'X';
			else
				cout << Board[i][j];
		}
		cout << endl;
	}
	cout << endl;
}
void DrawDie(DieType &Die)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cout << Die[i][j];
		}
		cout << endl;
	}
	cout << endl;
}
int Mark(BoardType &Board, int row, int col) 
{
	int previousValue = Board[row][col];
	Board[row][col] = Marker; 
	return previousValue;
}
void UnMark(BoardType &Board, int previousValue, int row, int col)
{
	Board[row][col] = previousValue;
}

void ReadBoardFromFile(BoardType &Board, ifstream &fin, int size)
{
	string readLine;
	for (int i = 0; i < size; i++)
	{
		fin >> readLine;
		for (int j = 0; j < size; j++)
		{
			if (readLine[j] == '*')
				Board[i][j] = AST;
			else
				Board[i][j] = (int)readLine[j] - 48;
		}
	}
}
int FindOpposite(int sent)
{
	int opposite = 0;
	if (sent >= 1 && sent <= 6)
	{
		if (sent == 1) opposite = 6;
		else if (sent == 2) opposite = 5;
		else if (sent == 3) opposite = 4;
		else if (sent == 4) opposite = 3;
		else if (sent == 5) opposite = 2;
		else opposite = 1;
	}
	return opposite;
}
int FindAdjacentLeftOfTop(int top, int side)
{
	int adj;

	if ((top == 2 && side == 4) || (top == 3 && side == 2) ||
		(top == 4 && side == 5) || (top == 5 && side == 3))
	{
		adj = 1;
	}
	else if ((top == 1 && side == 3) || (top == 3 && side == 6) ||
			 (top == 4 && side == 1) || (top == 6 && side == 4))
	{
		adj = 2;
	}
	else if ((top == 1 && side == 5) || (top == 2 && side == 1) ||
			 (top == 5 && side == 6) || (top == 6 && side == 2))
	{
		adj = 3;
	}
	else if ((top == 1 && side == 2) || (top == 2 && side == 6) ||
			 (top == 5 && side == 1) || (top == 6 && side == 5))
	{
		adj = 4;
	}
	else if ((top == 1 && side == 4) || (top == 3 && side == 1) ||
			 (top == 4 && side == 6) || (top == 6 && side == 3))
	{
		adj = 5;
	}
	else
	{
		adj = 6;
	}

	return adj;
}
void MakeDie(DieType &Die, int top, int side)
{
	Die[1][1] = top;
	Die[1][3] = FindOpposite(top);
	Die[2][1] = side;
	Die[0][1] = FindOpposite(side);
	Die[1][0] = FindAdjacentLeftOfTop(top, side);
	Die[1][2] = FindOpposite(Die[1][0]);
}

DieType FlipDie(DieType &Die, int row, int col, int NewRow, int NewCol, char &direction)
{
	DieType NewDie;
	NewDie = Die;
	if (row > NewRow && col == NewCol)
	{
		// Up
		NewDie[0][1] = Die[1][1];
		NewDie[1][1] = Die[2][1];
		NewDie[1][3] = Die[0][1];
		NewDie[2][1] = Die[1][3];
		direction = 'U';
	}
	else if (row == NewRow && col < NewCol)
	{
		// Right
		NewDie[1][0] = Die[1][3];
		NewDie[1][1] = Die[1][0];
		NewDie[1][2] = Die[1][1];
		NewDie[1][3] = Die[1][2];
		direction = 'R';
	}
	else if (row < NewRow && col == NewCol)
	{
		// Down
		NewDie[0][1] = Die[1][3];
		NewDie[1][1] = Die[0][1];
		NewDie[1][3] = Die[2][1];
		NewDie[2][1] = Die[1][1];
		direction = 'D';
	}
	else if (row == NewCol && col > NewCol)
	{
		// Left
		NewDie[1][0] = Die[1][1];
		NewDie[1][1] = Die[1][2];
		NewDie[1][2] = Die[1][3];
		NewDie[1][3] = Die[1][0];
		direction = 'L';
	}
	return NewDie;
}

struct Node
{
	int row, col, top;
	array<array<int, 4>, 3> Dice;
	char direction;
};
/*
bool IsSolutionFound(int top, int size)
{
	bool foundSolution = false;
	int trackTop = top;
	int row = size / 2;
	int col = size / 2;
	int finishRow = row;
	int finishCol = col;
	int previousValue = 0;
	char direction = 'X';

	Node cell;
	queue<Node> cellQ;

	array<array<int, 4>, 3> NewDie;

	// put first cell into the queue
	//Mark(row, col);
	cell.row = row;
	cell.col = col;
	cell.top = trackTop;
	cell.Dice = Die;
	cellQ.push(cell);

	while (!cellQ.empty())
	{
		// dequeue front node from queue
		//cell = cellQ.front(); //cellQ.pop();
		cell = cellQ.back();
		row = cell.row;
		col = cell.col;
		trackTop = cell.top;
		NewDie = cell.Dice;

		// for all nodes adjacent to front node
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				int NewRow = row + i;
				int NewCol = col + j;

				// checks the bounds of the Board
				if (NewRow >= 0 && NewRow < size && NewCol >= 0 && NewCol < size)
				{
					// checks up, down, left right
					if (i == -1 && j == 0 || i == 1 && j == 0 ||
						i == 0 && j == -1 || i == 0 && j == 1)
					{
						if ((Board[NewRow][NewCol] == trackTop || Board[NewRow][NewCol] == AST) && (!Marked(Board[NewRow][NewCol])))
						{

							previousValue = Mark(row, col);
							DrawBoard(size);

							NewDie = FlipDie(NewDie, row, col, NewRow, NewCol, cell.direction);
							cout << cell.direction << endl;
							if (NewRow == finishRow && NewCol == finishCol)
							{ 
								foundSolution = true;
							}
							else
							{
								cell.row = NewRow;
								cell.col = NewCol;
								cell.top = NewDie[1][1];
								cell.Dice = NewDie;

								cellQ.push(cell);
							}
						}
					}
				}
			}
		}

		UnMark(previousValue, row, col);
		if (foundSolution)
		{
			for (int i = 0; i < cellQ.size(); i++)
			{
				cout << cellQ.back().direction;
				cellQ.pop();
			}
		}
	}
	
	return foundSolution;
}
*/

//void FindSolution(BoardType &Board, DieType &Die, stack<char> &Directions, int row, int col, int top, int size, char &direction, bool &foundEnd)
//{
//	if (direction != 'x')
//		Directions.push(direction);
//	int previousValue = Board[row][col];
//	Mark(Board, row, col);
//
//	// Finds adjacent accessibly nodes
//	for (int i = -1; i <= 1; i++)
//	{
//		for (int j = -1; j <= 1; j++)
//		{
//			if (!foundEnd)
//			{
//				int NewRow = row + i;
//				int NewCol = col + j;
//				if (NewRow >= 0 && NewRow < size && NewCol >= 0 && NewCol < size)
//				{
//					// checks up, down, left right
//					if (i == -1 && j == 0 || i == 1 && j == 0 ||
//						i == 0 && j == -1 || i == 0 && j == 1)
//					{
//						if ((Board[NewRow][NewCol] == top || Board[NewRow][NewCol] == AST) && !Marked(Board[NewRow][NewCol]))
//						{
//							if (NewRow == size / 2 && NewCol == size / 2)
//							{
//								foundEnd = true;
//								Die = FlipDie(Die, row, col, NewRow, NewCol, direction);
//								Directions.push(direction);
//							}
//							else
//							{
//								DrawBoard(Board, size);
//								UnMark(Board, previousValue, row, col);
//								Die = FlipDie(Die, row, col, NewRow, NewCol, direction);
//								top = Die[1][1];
//								FindSolution(Board, Die, Directions, NewRow, NewCol, top, size, direction, foundEnd);
//							}
//						}
//					}
//				}
//			}
//		}
//	}
//}
//
//void IsSolutionFound(BoardType &Board, DieType &Die, int top, int size)
//{
//	// gets starting point
//	int row = size / 2;
//	int col = size / 2;
//
//	stack<char> Directions;
//	DieType NewDie = Die;
//
//	char direction = 'x';
//	bool foundEnd = false;
//	FindSolution(Board, NewDie, Directions, row, col, top, size, direction, foundEnd);
//
//	DrawBoard(Board, size);
//	while (!Directions.empty())
//	{
//		cout << Directions.top() << endl;
//		Directions.pop();
//	}
//}

bool IsSolutionFound(BoardType &Board, DieType &Die, stack<char> &Directions, int row, int col, char &direction)
{
	if (direction != 'x')
		Directions.push(direction);

	int prevValue = Board[row][col];
	Board[row][col] = 'X';
	
	// checks adjacent cells
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			// checks up down left right
			if ((i == -1 && j == 0) || (i == 1 && j == 0) || (i == 0 && j == -1) || (i == 0 && j == 1))
			{
				int NewRow = row + i;
				int NewCol = col + j;
				// checks [NewRow][NewCol] if it matches the top of our die
				if (Board[NewRow][NewCol] == Die[1][1] || Board[NewRow][NewCol] == AST && Board[NewRow][NewCol] != prevValue)
				{
					Die = FlipDie(Die, row, col, NewRow, NewCol, direction);
					IsSolutionFound(Board, Die, Directions, NewRow, NewCol, direction);
				}
				else
				{
					
				}
			}
		}
	}



}

void main()
{
	ifstream fin(INPUT);
	if (fin)
	{
		//ofstream fout(OUTPUT);

		BoardType Board;
		DieType Die;

		int size, top, side;
		fin >> size >> top >> side;
		while (size != 0)
		{
			ClearBoard(Board);
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 4; j++)
					Die[i][j] = 0;

			ReadBoardFromFile(Board, fin, size);
			MakeDie(Die, top, side);

			//DrawBoard(Board, size);
			//DrawDie(Die);

			int row = size / 2;
			int col = size / 2;

			stack<char> Directions;
			char direction = 'x';

			IsSolutionFound(Board, Die, Directions, row, col, direction);

			fin >> size >> top >> side;
		}

		//fout.close();
		fin.close();
	}
	else
	{
		cout << "The input file could not be found." << endl;
	}
}