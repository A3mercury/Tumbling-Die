#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <queue>
using namespace std;

const string INPUT = "die.in";
const string OUTPUT = "die.out";
const int BOARD_SIZE = 99;
const int AST = 7;

// Only one of these are used for the duration of the program
array<array<int, BOARD_SIZE>, BOARD_SIZE> Board;
array<array<int, 4>, 3> Die;

#define Marker 'X'
#define Marked(ch) (ch == Marker)

// Utility Functions
void ClearDie()
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 4; j++)
			Die[i][j] = 0;
}
void ClearBoard()
{
	for (int i = 0; i < BOARD_SIZE; i++)
for (int j = 0; j < BOARD_SIZE; j++)
	Board[i][j] = 0;
}
void DrawBoard(int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (Board[i][j] == 7)
				cout << '*';
			else
				cout << Board[i][j];
		}
		cout << endl;
	}
	cout << endl;
}
void DrawDie()
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
void Mark(int row, int col) { Board[row][col] = Marker; }

void ReadBoardFromFile(ifstream &fin, int size)
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
void MakeDie(int top, int side)
{
	Die[1][1] = top;
	Die[1][3] = FindOpposite(top);
	Die[2][1] = side;
	Die[0][1] = FindOpposite(side);
	Die[1][0] = FindAdjacentLeftOfTop(top, side);
	Die[1][2] = FindOpposite(Die[1][0]);
}
//
//void FlipDie()
//{
//	array<array<int, 4>, 3> NewDie;
//	NewDie = Die;
//	if (row > NewRow && col == NewCol)
//	{
//		// Up
//		NewDie[0][1] = Die[1][1];
//		NewDie[1][1] = Die[2][1];
//		NewDie[1][3] = Die[0][1];
//		NewDie[2][1] = Die[1][3];
//
//	}
//	else if (row == NewRow && col < NewCol)
//	{
//		// Right
//		NewDie[1][0] = Die[1][3];
//		NewDie[1][1] = Die[1][0];
//		NewDie[1][2] = Die[1][1];
//		NewDie[1][3] = Die[1][2];
//	}
//	else if (row < NewRow && col == NewCol)
//	{
//		// Down
//		NewDie[0][1] = Die[1][3];
//		NewDie[1][1] = Die[0][1];
//		NewDie[1][3] = Die[2][1];
//		NewDie[2][1] = Die[1][1];
//	}
//	else if (row == NewCol && col > NewCol)
//	{
//		// Left
//		NewDie[1][0] = Die[1][1];
//		NewDie[1][1] = Die[1][2];
//		NewDie[1][2] = Die[1][3];
//		NewDie[1][3] = Die[1][0];
//	}
//}

void TraverseBoard(int size, int top)
{
	int trackTop = top;
	int row = size / 2;
	int col = size / 2;
	int finishRow = row;
	int finishCol = col;

	int MaxQueueSize = 0;
	struct Node { int first, second, top; array<array<int, 4>, 3> Dice; };
	Node cell;
	queue<Node> cellQ;

	array<array<int, 4>, 3> NewDie;

	Mark(row, col);
	cell.first = row; 
	cell.second = col;
	cell.top = trackTop;
	cell.Dice = Die;
	cellQ.push(cell);
	while (!cellQ.empty())
	{
		// dequeue front node from queue
		cell = cellQ.front(); cellQ.pop();
		row = cell.first; 
		col = cell.second;
		trackTop = cell.top;
		NewDie = cell.Dice;

		// for all nodes adjacent to front node
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				int NewRow = row + i;
				int NewCol = col + j;
				if ((Board[NewRow][NewCol] == trackTop) && (!Marked(Board[NewRow][NewCol])))
				{
					Mark(NewRow, NewCol);

					cell.first = NewRow; cell.second = NewCol; 
					cell.top = trackTop; cell.Dice = Die;

					cellQ.push(cell);
					if (cellQ.size() > MaxQueueSize)
						MaxQueueSize = cellQ.size();
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

		int size, top, side;
		fin >> size >> top >> side;
		while (size != 0)
		{
			ClearBoard();
			ClearDie();

			ReadBoardFromFile(fin, size);
			MakeDie(top, side);

			DrawBoard(size);
			DrawDie();

			TraverseBoard(size, top);

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