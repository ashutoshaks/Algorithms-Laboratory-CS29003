// Ashutosh Kumar Singh
// 19CS30008

#include <iostream>
#include <cstdlib>
#include <string.h>
using namespace std;

typedef struct _node
{
	int rank;
	struct _node *parent;
} node;

// to initialize all the cells as singleton sets
void makeset(int m, int n, node **C)
{
	int i, j;
	for(i = 0; i < m; i++)
	{
		for(j = 0; j < n; j++)
		{
			C[i][j].rank = 0;
			C[i][j].parent = &C[i][j];
		}
	}
}

// finding in which set an element belongs along with path compression
node* findset(node **C, int x, int y)
{
	node* ptr = &C[x][y];
	while(ptr -> parent != ptr)
		ptr = ptr -> parent;

	node* z = &C[x][y];
	node* w;
	while(z -> parent != z)
	{
		w = z;
		z = z -> parent;
		w -> parent = ptr;
	}
	return ptr;
}

// merging two sets using union by rank
void mergeset(node* root1, node* root2)
{
	if(root1 -> rank == root2 -> rank)
	{
		root2 -> parent = root1;
		(root1 -> rank)++;
	}
	else if(root1 -> rank < root2 -> rank)
		root1 -> parent = root2;
	else if(root2 -> rank < root1 -> rank)
		root2 -> parent = root1;
	
}

void findtreasure(int m, int n, node **C, bool **H, bool **V)
{
	int i, j, x, broken;
	node *root1, *root2;
	for(broken = 0, x = 0; broken < m * n - 1; x++)
	{
		// we alternately try to take horizontal and even walls depending on whether x is even or odd

		// remove vertical wall
		// the m == 1 and n!= 1 are to check for boundary conditions
		if(((x & 1) || m == 1) && n != 1)
		{
			i = rand() % m;
			j = rand() % (n - 1);
			if(!V[i][j])
			{
				root1 = findset(C, i, j);
				root2 = findset(C, i, j + 1);

				// check that they alrady do not belong to the same set
				if(root1 != root2)
				{
					mergeset(root1, root2);
					V[i][j] = 1;
					broken++;
				}
			}
		}

		// removing horizontal wall
		else if((!(x & 1) || n == 1) && m != 1)
		{
			i = rand() % (m - 1);
			j = rand() % n;
			if(!H[i][j])
			{
				root1 = findset(C, i, j);
				root2 = findset(C, i + 1, j);

				// check that they alrady do not belong to the same set
				if(root1 != root2)
				{
					mergeset(root1, root2);
					H[i][j] = 1;
					broken++;
				}
			}
		}
	}

	// checking whether the first and final chambers are connected
	if(findset(C, 0, 0) == findset(C, m - 1, n - 1))
		cout << "The final chamber can be reached from the start chamber.\n\n";
	else
		cout << "Cannot go to the final chamber from the start chamber\n\n";
}


void printgrid(int m, int n, bool **H, bool **V)
{
	int i, j;
	cout << "+   +";
	for(i = 1; i < n; i++)
		cout << "---+";
	cout << endl;
	for(i = 0; i < m; i++)
	{
		cout << "|";
		for(j = 0; j < n - 1; j++)
		{
			if(!V[i][j])
				cout << "   |";
			else
				cout << "    ";
		}
		if(i != m - 1)
			cout << "   |" << endl;
		else
			cout << "    " << endl;
		if(i == m - 1)
			continue;

		for(j = 0; j < n; j++)
		{
			if(!H[i][j])
				cout << "+---";
			else
				cout << "+   ";
		}
		cout << "+" << endl;
	}
	cout << "+";
	for(j = 1; j <= n; j++)
		cout << "---+";
	// cout << "   +" << endl;
}


int main(int argc, char *argv[])
{
	srand((unsigned int)time(NULL));
	int m, n, i, j;

	// input from command line
	if(argc < 3)
		m = n = 10;
	else
	{
		m = atoi(argv[1]);
		n = atoi(argv[2]);
	}

	// declaring C to store all the cells
	node **C;
	C = (node**)malloc(m * sizeof(node*));
	for(i = 0; i < m; i++)
		C[i] = (node*)malloc(n * sizeof(node));

	makeset(m, n, C);

	// declaring H and V to store details about the horizontal and vertical walls
	bool **H, **V;
	H = (bool**)malloc((m - 1) * sizeof(bool*));
	V = (bool**)malloc(m * sizeof(bool*));

	for(i = 0; i < m - 1; i++)
		H[i] = (bool*)malloc(n * sizeof(bool));

	for(i = 0; i < m; i++)
		V[i] = (bool*)malloc((n - 1) * sizeof(bool));

	// 0 means closed, 1 means open
	for(i = 0; i < m - 1; i++)
	{
		for(j = 0; j < n; j++)
			H[i][j] = 0;
	}

	for(i = 0; i < m; i++)
	{
		for(j = 0; j < n - 1; j++)
			V[i][j] = 0;
	}

	cout << "\nInitial Grid\n\n";
	printgrid(m, n, H, V);
	cout << endl;

	findtreasure(m, n, C, H, V);

	cout << "Final Grid\n\n";
	printgrid(m, n, H, V);
	cout << endl;
}