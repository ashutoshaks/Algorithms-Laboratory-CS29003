// Ashutosh Kumar Singh
// 19CS30008

#include <iostream>
#include <cstdlib>
#include <string.h>
using namespace std;

/*
EXPLANATION : 
The subproblem chosen is that we compute what will be the maximum probablity 
if upto some i'th reaction we have used atmost j amount of catalyst.
An optimal solution to the entire problem will have this optimal substructure.

Now, dp[i][j] represents the maximum probablity if upto the i'th reaction 
we have used atmost j amount of catalyst.

So, for calculating dp[i][j], we loop over all k from 1 to j - (i - 1), as every
reaction uses atleast 1 catalyst, and see which amount to take, i. e. which k
to take for the i'th reaction.
So, the dp formulation can be written as : 
dp[i][j] = max(e[i][k] * dp[i - 1][j - k]) over all k from 1 to j - (i - 1).

For the base case we take dp[1][j] = e[1][j] for all j because considering upto 1 reaction, 
we have only one possibility for each amount of catalyst.
*/
 
int main()
{
	FILE* inp = fopen("input.txt", "r");
	FILE* out = fopen("output.txt", "w");

	int n, c, i, j, k, left, curr;

	// reading n and c
	fscanf(inp, "%d", &n);
	fscanf(inp, "%d", &c);

	if(c < n)
	{
		cout << "ERROR! c should be >= n.\n";
		return 0;
	}

	double e[n + 1][c + 1];
	memset(e, 0, sizeof(e));

	// reading the individual probabilities
	for(i = 1; i <= n; i++)
	{
		for(j = 1; j <= c; j++)
		{
			fscanf(inp, "%lf", &e[i][j]);
		}
	}

	// the dp array stores the solutions to the various subproblems
	double dp[n + 1][c + 1];
	memset(dp, 0, sizeof(dp));

	// the quantity array stores the best choices for all the subproblems
	int quantity[n + 1][c + 1];
	memset(quantity, 0, sizeof(quantity));

	// initializing the base cases
	for(j = 1; j <= c; j++)
	{
		dp[1][j] = e[1][j];
		quantity[1][j] = j;
	}

	// the main dp formulation
	for(i = 2; i <= n; i++)
	{
		for(j = 2; j <= c; j++)
		{
			for(k = 1; k <= j - i + 1; k++)
			{
				if(dp[i][j] < e[i][k] * dp[i - 1][j - k])
				{
					dp[i][j] = e[i][k] * dp[i - 1][j - k];
					quantity[i][j] = k;
				}
			}
		}
	}

	fprintf(out, "%lf\n", dp[n][c]);

	// calculating the amount of catalyst used in different reactions
	// left denotes the amount of catalyst left after making optimal choices from the end
	left = c;

	// reaction[] stores the amount of catalyst to be used in different reactions
	int reaction[n + 1];
	reaction[n] = quantity[n][c];
	for(i = n - 1; i > 0; i--)
	{
		left -= reaction[i + 1];
		reaction[i] = quantity[i][left];
	}

	for(i = 1; i <= n; i++)
		fprintf(out, "reaction %d : %d\n", i, reaction[i]);

	fclose(inp);
	fclose(out);
}