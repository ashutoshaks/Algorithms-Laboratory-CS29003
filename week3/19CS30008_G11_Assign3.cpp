#include <iostream>
#include <cstdlib>
#include <string.h>
#include <cassert>
using namespace std;

const int mxn = 305;
bool vis[mxn][mxn];
int a, b;

// direction vectors to navigate to the adjacent cells
int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, 1, 0, -1};

typedef struct
{
	int x, y;
} POINT;

// QUEUE
typedef struct
{
	POINT *Arr;
	int queue_size, start_id, end_id;
} QUEUE;

void init(QUEUE *qP, int size)
{
	qP -> Arr = (POINT*)malloc(size * sizeof(POINT));
	qP -> queue_size = 0;
	qP -> start_id = 0;
	qP -> end_id = -1;
}

int isempty(QUEUE qP)
{
	return qP.queue_size == 0;
}

void enqueue(QUEUE *qP, POINT p)
{
	qP -> end_id++;
	qP -> Arr[qP -> end_id] = p;
	qP -> queue_size++;
}

POINT dequeue(QUEUE *qP)
{
	if(isempty(*qP))
	{
		cout << "Queue is empty.\n";
	}
	POINT p = qP -> Arr[qP -> start_id];
	qP -> start_id++;
	qP -> queue_size--;
	return p;
}

// STACK
struct STACK
{
	POINT head;
	struct STACK *next;	
};
typedef struct STACK STACK;

void init(STACK *s)
{
	s -> next = NULL;
}

int isempty(STACK s)
{
	return (s.next == NULL);
}

void push(STACK *s, POINT p)
{
	STACK *newnode;
	newnode = (STACK*)malloc(sizeof(STACK));
	newnode -> head = p;
	newnode -> next = s -> next;
	s -> next = newnode;

}

POINT pop(STACK *s)
{
	STACK *curr = s -> next;
	POINT p = curr -> head;
	s -> next = curr -> next;
	return p;
}

// to check if a point lies inside the grid or not
bool isvalid(POINT p)
{
	return (p.x >= 0 && p.x < a && p.y >= 0 && p.y < b);
}

// function to print the grid
void printgrid(char grid[mxn][mxn])
{
	int i, j;
	for(i = 0; i < a; i++)
	{
		for(j = 0; j < b; j++)
			cout << grid[i][j];
		cout << endl;
	}
}

// checking whether a path exists or not using a BFS based approach
int strategy1(char grid[mxn][mxn], POINT pj, POINT pf)
{
	memset(vis, 0, sizeof(vis));
	QUEUE q;
	init(&q, mxn * mxn);
	enqueue(&q, pj);
	vis[pj.x][pj.y] = 1;

	while(!isempty(q))
	{
		POINT curr = dequeue(&q);
		if(curr.x == pf.x && curr.y == pf.y)
			return 1;
		
		for(int i = 0; i < 4; i++)
		{
			POINT p = {curr.x + dx[i], curr.y + dy[i]};
			if(isvalid(p) && grid[p.x][p.y] == '*' && !vis[p.x][p.y])
			{
				vis[p.x][p.y] = 1;
				enqueue(&q, p);
			}
		}
	}
	return 0;
}

// printing the path using a DFS based approach
void strategy2(char grid[mxn][mxn], POINT pj, POINT pf)
{
	memset(vis, 0, sizeof(vis));
	STACK s;
	init(&s);
	push(&s, pj);
	bool ok;
	vis[pj.x][pj.y] = 1;
	int i, ind, j;

	// corner case when source and destination are same
	if(pj.x == pf.x && pj.y == pf.y)
	{
		cout << "(" << pf.x << ", " << pf.y << "), ";
		cout << "(" << pf.x << ", " << pf.y << ")\n";
		return;
	}

	while(!isempty(s))
	{
		POINT curr = pop(&s);
		push(&s, curr);
		ok = 0;
		for(i = 0; i < 4; i++)
		{
			POINT p = {curr.x + dx[i], curr.y + dy[i]};
			if(isvalid(p) && grid[p.x][p.y] == '*' && !vis[p.x][p.y])
			{
				ok = 1;
				push(&s, p);
				vis[p.x][p.y] = 1;
				if(p.x == pf.x && p.y == pf.y)
				{
					POINT anspath[mxn * mxn];
					ind = 0;
					while(!isempty(s))
					{
						POINT pt = pop(&s);
						anspath[ind++] = pt;
					}
					cout << "(" << anspath[ind - 1].x << ", " << anspath[ind - 1].y << ")";
					for(j = ind - 2; j >= 0; j--)
						cout << ", (" << anspath[j].x << ", " << anspath[j].y << ")";
					cout << endl;
					return;
				}
				break;
			}
		}
		if(!ok) // all neighbours are exhausted
			pop(&s);
	}

}


int main()
{
	// freopen("out.txt", "r", stdout);
	char grid[mxn][mxn];
	int n, i, j;
	cin >> a >> b >> n;
	int x[n], y[n];
	for(i = 0; i < n; i++)
		cin >> x[i];
	for(i = 0; i < n; i++)
		cin >> y[i];
	POINT pj, pf;
	cin >> pj.x >> pj.y >> pf.x >> pf.y;

	for(i = 0; i < a; i++)
	{
		for(j = 0; j < b; j++)
			grid[i][j] = '*';
	}
	for(i = 0; i < n; i++)
	{
		grid[x[i]][y[i]] = '-';
	}

	cout << "Grid of stones is :\n";
	printgrid(grid);

	
	int foundpath = strategy1(grid, pj, pf);
	
	if(foundpath)
	{
		cout << "Path Exists\n";
		strategy2(grid, pj, pf);
	}
	else
		cout << "No path exists\n";
}