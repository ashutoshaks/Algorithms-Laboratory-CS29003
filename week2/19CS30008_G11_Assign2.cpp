#include <iostream>
#include <cstdlib>
using namespace std;

typedef struct  {
	int left;
	int ht;
	int right;
} Box;

typedef struct  {
	int x;
	int y;
} Point;

Point* findOutLine(Box boxes[], int size, int *outputsize);
void prettyPrint(Point* points, int outputsize) {
	int i;
	printf("OutLine for given Boxes is \n[");
	for (i = 0; i < outputsize; i++) {

		printf(" (%d, %d), ", points[i].x, points[i].y);

	}
	printf("\b\b ]\n");
}
void process() {
	int n;
	scanf("%d", &n);
	Box boxes[n];
	int i;
	for (i = 0; i < n; i++) {
		scanf("%d %d %d", &boxes[i].left , &boxes[i].right , &boxes[i].ht);
	}
	int outputsize;
	Point* ptr = findOutLine(boxes, n, &outputsize);

	prettyPrint(ptr, outputsize);
}


typedef struct
{
	Point vals[2000];
	int len;
} ans;


void append(ans* res, int *ind, Point p)
{
	if(ind > 0 && res->vals[*(ind) - 1].y == p.y)
		return;
	if(ind > 0 && res->vals[*(ind) - 1].x == p.x)
	{
		res->vals[*(ind) - 1].y = max(res->vals[*(ind) - 1].y, p.y);
		return;
	}

	res->vals[*ind] = p;
	(*ind)++;
}

ans* merge(ans* leftans, ans* rightans)
{
	int i = 0, j = 0, hleft = 0, hright = 0, ind = 0, mxh = 0;
	ans* res;
	res = (ans*)malloc(sizeof(ans));

	while(i < (leftans->len) && j < (rightans->len))
	{
		if((leftans->vals)[i].x < (rightans->vals)[j].x || ((leftans->vals)[i].x == (rightans->vals)[j].x && (leftans->vals)[i].y > (rightans->vals)[j].y))
		{
			hleft = (leftans->vals)[i].y;
			mxh = max(hleft, hright);
			append(res, &ind, {(leftans->vals)[i].x, mxh});
			i++;
		}
		else
		{
			hright = (rightans->vals)[j].y;
			mxh = max(hleft, hright);
			append(res, &ind, {(rightans->vals)[j].x, mxh});
			j++;
		}
	}

	while(i < (leftans->len))
	{
		append(res, &ind, {(leftans->vals)[i].x, (leftans->vals)[i].y});
		i++;
	}

	while(j < (rightans->len))
	{
		append(res, &ind, {(rightans->vals)[j].x, (rightans->vals)[j].y});
		j++;
	}
	res->len = ind;

	return res;
}

ans* findans(Box boxes[], int l, int r)
{
	if(l == r)
	{
		int ind = 0;
		ans* res = (ans*)malloc(sizeof(ans));
		append(res, &ind, {boxes[l].left, boxes[l].ht});
		append(res, &ind, {boxes[l].right, 0});
		res->len = ind;
		return res;
	}

	int mid = (l + r) / 2;

	ans* leftans = (ans*)malloc(sizeof(ans));
	ans* rightans = (ans*)malloc(sizeof(ans));
	ans* result = (ans*)malloc(sizeof(ans));

	leftans = findans(boxes, l, mid);
	rightans = findans(boxes, mid + 1, r);

	result = merge(leftans, rightans);

	free(leftans);
	free(rightans);

	return result;
}

Point* findOutLine(Box boxes[], int size, int* outputsize)
{
	Point* point_array;
	Point ansarray[2000];

	ans* outline = (ans*)malloc(sizeof(ans));
	outline = findans(boxes, 0, size - 1);
	
	int i, ind = 0;
	ansarray[ind++] = (outline->vals)[0];
	for(i = 1; i < (outline->len); i++)
	{
		if((outline->vals)[i].y != ansarray[ind - 1].y)
			ansarray[ind++] = (outline->vals)[i];
	}
	point_array = (Point*)malloc(ind * sizeof(Point));
	for(i = 0; i < ind; i++)	
		point_array[i] = ansarray[i];
	(*outputsize) = ind;

	return point_array;
}

int main()
{
	process();
	return 0;
}
