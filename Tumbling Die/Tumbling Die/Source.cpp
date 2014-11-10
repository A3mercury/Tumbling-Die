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

struct Coordinate
{
	int currentX;
	int currentY;
};

struct NodeType
{
	Coordinate c;
	int top;
	DieType nodeDie;
};

struct Ast
{
	int pos;
	bool prevPos;
};

void FlipDie(DieType &Die, char direction)
{
	DieType NewDie = Die;
	if (direction == 'U')
	{
		// Up
		NewDie[0][1] = Die[1][1];
		NewDie[1][1] = Die[2][1];
		NewDie[1][3] = Die[0][1];
		NewDie[2][1] = Die[1][3];
	}
	else if (direction == 'R')
	{
		// Right
		NewDie[1][0] = Die[1][3];
		NewDie[1][1] = Die[1][0];
		NewDie[1][2] = Die[1][1];
		NewDie[1][3] = Die[1][2];
	}
	else if (direction == 'D')
	{
		// Down
		NewDie[0][1] = Die[1][3];
		NewDie[1][1] = Die[0][1];
		NewDie[1][3] = Die[2][1];
		NewDie[2][1] = Die[1][1];
	}
	else if (direction == 'L')
	{
		// Left
		NewDie[1][0] = Die[1][1];
		NewDie[1][1] = Die[1][2];
		NewDie[1][2] = Die[1][3];
		NewDie[1][3] = Die[1][0];
	}
	Die = NewDie;
}

struct Coord
{
	int x, y;
	char direction;
};

void Test(BoardType &Board, DieType &Die, int size)
{
	DieType NewDie = Die;
	int row = size / 2;
	int col = size / 2;

	stack<Coord> Directions;
	Coord Start;
	Start.x = row; Start.y = col;
	Directions.push(Start);

	Start.direction = ' ';

	while (!Directions.empty())
	{
		Coord CheckValid;
		CheckValid = Directions.top();
		Directions.pop();
		FlipDie(NewDie, CheckValid.direction);

		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (i == -1 && j == 0 || i == 1 && j == 0 || i == 0 && j == -1 || i == 0 && j == 1 || i == 1 && j == 1)
				{
					int NewRow = CheckValid.x + i; 
					int NewCol = CheckValid.y + j;					

					if (NewRow >= 0 && NewRow < size && NewCol >= 0 && NewCol < size)
					{
						if (Board[NewRow][NewCol] == NewDie[1][1] || Board[NewRow][NewCol] == AST)
						{
							Coord NewValid;
							NewValid.x = NewRow;
							NewValid.y = NewCol;							

							if (NewRow < CheckValid.x && NewCol == CheckValid.y) NewValid.direction = 'U';
							else if (NewRow > CheckValid.x && NewCol == CheckValid.y) NewValid.direction = 'D';
							else if (NewCol < CheckValid.y && NewRow == CheckValid.x) NewValid.direction = 'L';
							else NewValid.direction = 'R';

							Directions.push(NewValid);
							cout << NewRow << " " << NewCol << " " << NewValid.direction << endl;
						}
					}
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
			// Clear Die
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 4; j++)
					Die[i][j] = 0;

			ReadBoardFromFile(Board, fin, size);
			MakeDie(Die, top, side);

			int row = size / 2;
			int col = size / 2;

			stack<char> Directions;
			char direction = 'x';

			//Solver(Board, Die, size);

			Test(Board, Die, size);

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