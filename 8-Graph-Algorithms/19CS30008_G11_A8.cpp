// Ashutosh Kumar Singh
// 19CS30008

#include <iostream>
#include <cstdlib>
#include <string.h>
using namespace std;

// The approach is to binary search over the time taken.
// For a specific time value, we check whether it is possile or not using BFS
// Then after we have the answer, we follow parent pointers to print the path

// Direction vectors to facilitate traversal between neighbouring cells
int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, 1, 0, -1};

const int mxn = 65;
int n;

typedef struct 
{
	int x, y;
} POINT;

// Implementation of queue data structure

typedef struct
{
	POINT Arr[mxn * mxn];
	int queue_size, start_id, end_id;
} QUEUE;

void init(QUEUE *qP)
{
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
	POINT p = qP -> Arr[qP -> start_id];
	qP -> start_id++;
	qP -> queue_size--;
	return p;
}

// This function checks whether a given point is within the limits of the grid or not
bool isvalid(POINT p)
{
	return (p.x >= 0 && p.x < n && p.y >= 0 && p.y < n);
}

// This function checks whether a specific value of time is possible or not for fulfilling our criteria.
// It uses a BFS approach to do so.
bool checkbfs(int grid[mxn][mxn], int ans, POINT s, POINT f)
{
	int ngrid[n][n], i, j;

	// In the ngrid array, we store the values of the grid at time t = ans.
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
		{
			ngrid[i][j] = (grid[i][j] < ans ? ans : grid[i][j]);
		}
	}

	if(ngrid[s.x][s.y] != ans)
		return 0;

	// Basic BFS to check if we can each the destination from the source or not
	bool vis[n][n];
	memset(vis, 0, sizeof(vis));
	QUEUE q;
	init(&q);
	enqueue(&q, s);
	vis[s.x][s.y] = 1;
	while(!isempty(q))
	{
		POINT curr = dequeue(&q);
		if(curr.x == f.x && curr.y == f.y)
			return 1;
		for(int i = 0; i < 4; i++)
		{
			POINT p = {curr.x + dx[i], curr.y + dy[i]};
			if(isvalid(p) && ngrid[p.x][p.y] == ans && !vis[p.x][p.y])
			{
				vis[p.x][p.y] = 1;
				enqueue(&q, p);
			}
		}
	}
	return 0;
}

// After we have found the answer, this function finds a path from the source to the destination
void findpath(int grid[mxn][mxn], int ans, POINT s, POINT f)
{
	int ngrid[n][n], i, j;
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
		{
			ngrid[i][j] = (grid[i][j] < ans ? ans : grid[i][j]);
		}
	}

	// the par array stores the parent of each cell in the BFS tree
	POINT par[n][n];
	bool vis[n][n];
	memset(vis, 0, sizeof(vis));
	QUEUE q;
	init(&q);
	enqueue(&q, s);
	par[s.x][s.y] = {-1, -1};
	while(!isempty(q))
	{
		POINT curr = dequeue(&q);
		vis[curr.x][curr.y] = 1;
		if(curr.x == f.x && curr.y == f.y)
			break;
		for(int i = 0; i < 4; i++)
		{
			POINT p = {curr.x + dx[i], curr.y + dy[i]};
			if(isvalid(p) && ngrid[p.x][p.y] == ans && !vis[p.x][p.y])
			{
				par[p.x][p.y] = curr;
				enqueue(&q, p);
			}
		}
	}


	POINT curr = f, path[mxn * mxn];
	int ind = 0;

	// traversing the par array to get the path
	while(1)
	{
		path[ind++] = {curr.x, curr.y};
		if(curr.x == s.x && curr.y == s.y)
			break;
		curr = par[curr.x][curr.y];
	}

	for(i = ind - 1; i > 0; i--)
		cout << "(" << path[i].x << ", " << path[i].y << "), ";
	cout << "(" << path[0].x << ", " << path[0].y << ")\n";

	cout << "The Number of Blocks traversed are : " << ind << endl;
}

int main()
{
	int i, j, mx = 0;
	POINT s, f;
	cin >> n;
	if(n > mxn)
	{
		cout << "n is not within the given range. 2 <= n <= 50.\n";
		return 0;
	}
	int grid[mxn][mxn];
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
		{
			cin >> grid[i][j];
			mx = max(mx, grid[i][j]);
		}
	}
	cin >> s.x >> s.y >> f.x >> f.y;

	int low = 0, high = mx, mid;

	// Binary search over the time taken
	while(low <= high)
	{
		mid = (low + high) / 2;
		if(checkbfs(grid, mid, s, f))
			high = mid - 1;
		else
			low = mid + 1;
	}

	cout << "Minimum time taken is : " << low << endl;
	cout << "The path to reach from (" << s.x << ", " << s.y << ") to (" << f.x << ", " << f.y << ") is :\n"; 
	findpath(grid, low, s, f);
}