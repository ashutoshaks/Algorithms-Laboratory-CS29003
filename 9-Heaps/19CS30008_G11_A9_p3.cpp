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
	int numDependencies;
	int dependencies[10];
} job;

typedef struct _jobPair
{
	int jobid_from;
	int jobid_to;
} jobpair;

typedef struct _heap
{
	job list[MAX_SIZE];
	int numJobs;
	int index[MAX_SIZE];
} newheap;

void initHeap(newheap *H)
{
	H -> numJobs = 0;
	memset(H -> index, 0, sizeof(H -> index));
}

bool compare(job i, job j)
{
	if(i.remLength != j.remLength)
		return (i.remLength < j.remLength);
	else
		return (i.jobId < j.jobId);
}

void insertJob(newheap *H, job j)
{   
	int i, p;
    i = ++(H -> numJobs);
    p = i / 2;
    H -> list[H -> numJobs] = j;
    H -> index[j.jobId] = H -> numJobs;
    if(i == 1)
    	return;

    while(p > 0 && compare(j, H -> list[p]))
    {
    	H -> index[H -> list[i].jobId] = p;
    	H -> index[H -> list[p].jobId] = i;
        job t = H -> list[i];
        H -> list[i] = H -> list[p];
        H -> list[p] = t;
        i = i / 2;
        p = i / 2;
	}
}


int extractMinJob(newheap* H, job* j)
{
	int i = 1, sind =  1;
    if(H -> numJobs == 0)
        return -1;
    *j = H -> list[1];
    H -> index[H -> list[1].jobId] = 0;
    H -> list[1] = H -> list[H -> numJobs];
    H -> index[H -> list[H -> numJobs].jobId] = 1;
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
        	H -> index[H -> list[i].jobId] = sind;
    		H -> index[H -> list[sind].jobId] = i;
        	job t = H -> list[sind];
            H -> list[sind] = H -> list[i];
            H -> list[i] = t;
            i = sind; 	
        }
    }
    return 0;
}

void decreaseKey(newheap *H, int jid)
{
	int i = H -> index[jid], p, curr;
	if(H -> list[i].remLength != H -> list[i].jobLength)
		return;
	H -> list[i].remLength /= 2;
	H -> list[i].jobLength /= 2;
	curr = i;
	p = i / 2;
	while(p > 0 && compare(H -> list[curr], H -> list[p]))
	{
		H -> index[H -> list[curr].jobId] = p;
		H -> index[H -> list[p].jobId] = curr;
		job t = H -> list[curr];
		H -> list[curr] = H -> list[p];
		H -> list[p] = t;
		curr /= 2;
		p = curr / 2;
	}
	
}


void newScheduler(job jobList[], int n, jobpair pairList[], int m, int jobListIndex[])
{
	int i, ind = 0, time = 0, ok, left = n, turn = 0, j, id;
	bool comp = 0, added[n + 1];
	memset(added, 0, sizeof(added));
	int ans[1000];
	memset(ans, 0, sizeof(ans));
	job currjob;
	newheap H;
	initHeap(&H);
	while(left)
	{
		for(i = 0; i < n; i++)
		{
			if(jobList[i].startTime == time)
			{
				insertJob(&H, jobList[i]);
				added[jobList[i].jobId] = 1;
			}
		}

		if(H.numJobs != 0)
		{
			if(H.list[1].remLength == H.list[1].jobLength)
				turn += time - H.list[1].startTime;
			--(H.list[1].remLength);
			ans[time] = H.list[1].jobId;
			if(H.list[1].remLength == 0)
			{
				for(i = 0; i < H.list[1].numDependencies; i++)
				{
					id = H.list[1].dependencies[i];
					if(added[id])
						decreaseKey(&H, id);
					else
					{
						jobList[jobListIndex[id]].jobLength /= 2;
						jobList[jobListIndex[id]].remLength /= 2;
					}
				}
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
	int n, i, stmax = 0, m;
	cout << "Enter no. of jobs (n): ";
	cin >> n;
	job *jobList = (job*)malloc(n * sizeof(job));
	int jobListIndex[MAX_SIZE];
	memset(jobListIndex, -1, sizeof(jobListIndex));
	cout << "Enter the jobs:\n";
	for(i = 0; i < n; i++)
	{
		cin >> jobList[i].jobId >> jobList[i].startTime >> jobList[i].jobLength;
		jobList[i].remLength = jobList[i].jobLength;
		jobList[i].numDependencies = 0;
		memset(jobList[i].dependencies, 0, sizeof(jobList[i].dependencies));
		jobListIndex[jobList[i].jobId] = i;
		stmax = max(stmax, jobList[i].startTime);
	}
	cout << "Enter the number of dependency pairs:\n";
	cin >> m;
	jobpair pairList[m];
	cout << "Enter the dependency pairs:\n";
	for(i = 0; i < m; i++)
	{
		cin >> pairList[i].jobid_from >> pairList[i].jobid_to;
		jobList[jobListIndex[pairList[i].jobid_from]].dependencies[jobList[jobListIndex[pairList[i].jobid_from]].numDependencies++] = pairList[i].jobid_to;
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

	newScheduler(jobList, n, pairList, m, jobListIndex);

}