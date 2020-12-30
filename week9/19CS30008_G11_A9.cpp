// Ashutosh Kumar Singh
// 19CS30008

#include <iostream>
#include <cstdlib>
#include <string.h>
using namespace std;

#define MAX_SIZE 100

typedef struct _job
{
	int jobId;
	int startTime;
	int jobLength;
	int remLength;
} job;

typedef struct _heap
{
	job list[MAX_SIZE];
	int numJobs;
} heap;

void initHeap(heap *H)
{
	H -> numJobs = 0;
}

bool compare(job i, job j)
{
	if(i.remLength != j.remLength)
		return (i.remLength < j.remLength);
	else
		return (i.jobId < j.jobId);
}

void insertJob(heap *H, job j)
{   
	int i, p;
    i = ++(H -> numJobs);
    p = i / 2;
    H -> list[H -> numJobs] = j;
    if(i == 1)
    	return;

    while(p > 0 && compare(j, H -> list[p]))
    {
        job t = H -> list[i];
        H -> list[i] = H -> list[p];
        H -> list[p] = t;
        i = i / 2;
        p = i / 2;
	}
}


int extractMinJob(heap* H, job* j)
{
	int i = 1, sind =  1;
    if(H -> numJobs == 0)
        return -1;
    *j = H -> list[1];
    H -> list[1] = H -> list[H -> numJobs];
    --(H -> numJobs);

    while(1)
    {   
    	int left = 2 * i, right = 2 * i + 1;
        if(left <= H -> numJobs && compare(H -> list[left], H -> list[sind]))
            sind = left;
        if(right <= H -> numJobs && compare(H -> list[right], H -> list[sind]))
            sind = right;

        if(sind == i)
        	break;
        else
        {
        	job t = H -> list[sind];
            H -> list[sind] = H -> list[i];
            H -> list[i] = t;
            i = sind; 	
        }
    }
    return 0;
}

void scheduler(job jobList[], int n)
{
	int i, ind = 0, time = 0, ok, left = n, turn = 0;
	bool done[n + 1], comp = 0;
	memset(done, 0, sizeof(done));
	int ans[1000];
	memset(ans, 0, sizeof(ans));
	job currjob;
	heap H;
	initHeap(&H);
	while(left)
	{
		for(i = 0; i < n; i++)
		{
			if(jobList[i].startTime == time)
				insertJob(&H, jobList[i]);
		}

		if(H.numJobs != 0)
		{
			if(H.list[1].remLength == H.list[1].jobLength)
				turn += time - H.list[1].startTime;
			--(H.list[1].remLength);
			ans[time] = H.list[1].jobId;
			if(H.list[1].remLength == 0)
			{
				extractMinJob(&H, &currjob);
				left--;
			}
		}
		else
		{
			ans[time] = -1;
		}
		time++;
	}
	cout << "Jobs scheduled at each timestep are:\n";
	for(i = 0; i < time; i++)
		cout << ans[i] << " ";
	cout << endl;
	cout << "Average Turnaround Time is ";
	cout << (1.0 * turn) / n << endl;
}

int main()
{
	int n, i, stmax = 0;
	cout << "Enter no. of jobs (n): ";
	cin >> n;
	job *jobList = (job*)malloc(n * sizeof(job));
	cout << "Enter the jobs:\n";
	for(i = 0; i < n; i++)
	{
		cin >> jobList[i].jobId >> jobList[i].startTime >> jobList[i].jobLength;
		jobList[i].remLength = jobList[i].jobLength;
		stmax = max(stmax, jobList[i].startTime);
	}

	// sort the jobs according to start time using counting sort
	int count[stmax + 1];
	memset(count, 0, sizeof(count));
	for(i = 0; i < n; i++)
	{
		count[jobList[i].startTime]++;
	}
	
	for(i = stmax; i > 0; i--)
	{
		count[i] = count[i - 1];
	}
	count[0] = 0;

	for(i = 1; i <= stmax; i++)
	{
		count[i] += count[i - 1];
	}

	job jobs[n];
	for(i = 0; i < n; i++)
	{
		jobs[count[jobList[i].startTime]] = jobList[i];
		count[jobList[i].startTime]++;
	}

	for(i = 0; i < n; i++)
		jobList[i] = jobs[i];

	scheduler(jobList, n);

}