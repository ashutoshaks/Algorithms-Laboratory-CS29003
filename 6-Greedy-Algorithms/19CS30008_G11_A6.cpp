// Ashutosh Kumar Singh
// 19CS30008

#include <iostream>
#include <cstdlib>
#include <string.h>
using namespace std;

typedef struct
{
	int ff, ss;
} Interval;

bool verify(int *index, int mid, Interval times[], int n, int *ans)
{
	int i, curr = times[index[0]].ff;
	for(i = 1; i < n; i++)
	{
		if(curr + mid <= times[index[i]].ss)
			curr = max(curr + mid, times[index[i]].ff);
		else 
			return 0;
	}
	for(i = 0; i < n; i++)
		ans[i] = index[i];
	return 1;
}

bool check(int *index, int mid, Interval times[], int n, int *ans, int size)
{
	if(size == 1)
	{
		bool found = verify(index, mid, times, n, ans);
		if(found)
			return 1;
	}
	else
	{
		for(int i = 0; i < size; i++)
		{
			if(check(index, mid, times, n, ans, size - 1) == 1)
				return 1;
			if(size & 1)
				swap(index[0], index[size - 1]);
			else
				swap(index[i], index[size - 1]);
		}
	}
	return 0;

}

void schedule_visits(FILE *out, Interval times[], int n)
{
	int low = 0, high = 1e8, mid, i, min, sec;
	int *best = (int*)malloc(n * sizeof(int));
	int *ans = (int*)malloc(n * sizeof(int));
	int *index = (int*)malloc(n * sizeof(int));
	for(i = 0; i < n; i++)
		index[i] = i;
	bool ok;
	while(low <= high)
	{
		mid = (low + high) / 2;
		ok = check(index, mid, times, n, ans, n);
		if(ok)
		{
			for(i = 0; i < n; i++)
				best[i] = ans[i];
			low = mid + 1;
		}
		else
			high = mid - 1;
	}

	min = high / 60;
	sec = high % 60;
	fprintf(out, "%d:", high / 60);
	if(sec < 10)
		fprintf(out, "0%d\n", sec);
	else
		fprintf(out, "%d\n", sec);
	for(i = 0; i < n; i++)
		fprintf(out, "%d ", best[i]);
	fprintf(out, "\n");
}


int main()
{
	FILE* inp = fopen("input1.txt", "r");
	FILE* out = fopen("output1.txt", "w");

	int n, i;
	fscanf(inp, "%d", &n);
	Interval times[n];

	for(i = 0; i < n; i++)
	{
		fscanf(inp, "%d", &times[i].ff);
		fscanf(inp, "%d", &times[i].ss);
		times[i].ff *= 60;
		times[i].ss *= 60;

	}
	schedule_visits(out, times, n);
}