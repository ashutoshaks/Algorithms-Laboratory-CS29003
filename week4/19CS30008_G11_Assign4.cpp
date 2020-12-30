// Ashutosh Kumar Singh
// 19CS30008

#include <iostream>
#include <cstdlib>
#include <string.h>
using namespace std;

const int mxn = 36;
int precnt = 0;
char** Huffman; // stores the huffman codes for all characters

/*
	a - z are in 0 - 25
	0 - 9 are in 26 - 35
*/

/*
	Part 1, 2, 3 and 4 all are working.
*/

typedef struct _Node
{
	char *symbol;
	int frequency;
	struct _Node *next, *left, *right;
} Node;

Node* createnewnode(char c, int cnt)
{
	Node* newnode = (Node*)malloc(sizeof(Node));
	newnode -> left = newnode -> right = newnode -> next = NULL;
	newnode -> frequency = cnt;
	newnode -> symbol = (char*)malloc(1000 * sizeof(char));
	newnode -> symbol[0] = c;
	newnode -> symbol[1] = '\0';
	return newnode;
}

bool smaller(Node* x, Node* y)
{
	if(x -> frequency != y -> frequency)
		return x -> frequency < y -> frequency;
	else
	{
		if(x -> symbol[0] == 'N' && y -> symbol[0] != 'N')
			return 1;
		else if(x -> symbol[0] != 'N' && y -> symbol[0] == 'N')
			return 0;
		else if(x -> symbol[0] == 'N' && y -> symbol[0] == 'N')
			return (strcmp(x -> symbol, y -> symbol) < 0);
		else
		{
			if(x -> symbol[0] >= 'a' && x -> symbol[0] <= 'z')
			{
				if(y -> symbol[0] >= 'a' && y -> symbol[0] <= 'z')
					return (strcmp(x -> symbol, y -> symbol) < 0);
				else 
					return 1;
			}
			else
			{
				if(y -> symbol[0] >= '0' && y -> symbol[0] <= '9')
					return (strcmp(x -> symbol, y -> symbol) < 0);
				else 
					return 0;
			}
		}
	}

}

void insert(Node** head, Node* curr)
{
	if(*head == NULL || smaller(curr, *head))
	{
		curr -> next = *head;
		*head = curr;
	}
	else
	{
		Node* p = *head;
		while(p -> next && smaller(p -> next, curr))
			p = p -> next;
		curr -> next = p -> next;
		p -> next = curr;
	}
}

void preorder(FILE* out, Node* x)
{
	if(x == NULL)
		return;
	precnt++;
	// precnt ensures that a comma is not printed after the last character
	if(precnt != 71)
		fprintf(out, "%s(%d), ", x -> symbol, x -> frequency);
	else
		fprintf(out, "%s(%d)", x -> symbol, x -> frequency);
	preorder(out, x -> left);
	preorder(out, x -> right);
}

// finds the huffman codes for all characters
void findHuffman(FILE* out, Node* x, char** Huffman, char* code, int ind)
{
	if(x -> left != NULL || x -> right != NULL)
	{
		code[ind] = '0';
		code[ind + 1] = '\0';
		findHuffman(out, x -> left, Huffman, code, ind + 1);

		code[ind] = '1';
		code[ind + 1] = '\0';
		findHuffman(out, x -> right, Huffman, code, ind + 1);
	}
	else
	{
		char sym = x -> symbol[0];
		if(sym >= 'a' && sym <= 'z')
			strcpy(Huffman[sym - 'a'], code);
		else
			strcpy(Huffman[sym - '0' + 26], code);
		
		return;
	}
}

// prints the huffman codes for all characters
void printHuffman(FILE* out, Node* head)
{
	char* code = (char*)malloc(100 * sizeof(char));
	Huffman = (char**)malloc(36 * sizeof(char*));
	int i;
	for(i = 0; i < 36; i++)
		Huffman[i] = (char*)malloc(100 * sizeof(char));
	findHuffman(out, head, Huffman, code, 0);

	for(i = 0; i < 26; i++)
		fprintf(out, "%c %s\n", (char)('a' + i), Huffman[i]);
	for(i = 0; i < 10; i++)
		fprintf(out, "%c %s\n", (char)('0' + i), Huffman[i + 26]);
}

// encodes a given text
void encodetext(FILE* inpenc, FILE* out)
{
	int n, i, j;
	fscanf(inpenc, "%d", &n);
	fgetc(inpenc);

	char text[1000];
	for(i = 0; i < n; i++)
	{
		fgets(text, 1000, inpenc);
		char encoding[1000];
		encoding[0] = '\0';
		for(j = 0; j < strlen(text); j++)
		{
			if(text[j] >= '0' && text[j] <= '9')
				strcat(encoding, Huffman[text[j] - '0' + 26]);
			else if(text[j] >= 'a' && text[j] <= 'z')
				strcat(encoding, Huffman[text[j] - 'a']);
		}
		fprintf(out, "%s\n", encoding);
	}
}


char* substr(char* s, int start, int len)
{
	int i, c, ind = 0;
	char* word = (char*)malloc(1000 * sizeof(char));
	for(i = start, c = 0; c < len; c++, i++)
	{
		word[ind++] = s[i];
	}
	word[ind] = '\0';
	return word;
}

// decodes a given text
void decodetext(FILE* inpdec, FILE* out)
{
	int n, i, j, k;
	fscanf(inpdec, "%d", &n);
	fgetc(inpdec);

	char text[1000];
	for(i = 0; i < n; i++)
	{
		fgets(text, 1000, inpdec);
		char decoding[1000];
		int ind = 0;
		for(j = 0; j < strlen(text); j++)
		{
			for(k = 0; k < 36; k++)
			{
				if(strcmp(Huffman[k], substr(text, j, strlen(Huffman[k]))) == 0)
				{	
					if(k >= 26)
						decoding[ind++] = (char)('0' + k - 26);
					else
						decoding[ind++] = (char)('a' + k);
					j += strlen(Huffman[k]) - 1;
					break;
				}
			}

		}
		decoding[ind] = '\0';
		fprintf(out, "%s\n", decoding);
	}
}

int main()
{
	int n, i, j;
	int count[36];
	memset(count, 0, sizeof(count));
	FILE* inp = fopen("log.txt", "r");
	FILE* out = fopen("output.txt", "w");

	if(inp == NULL || out == NULL)
	{
		printf("Error in opening file.\n");
		return 0;
	}

	fscanf(inp, "%d", &n);
	fgetc(inp);

	char word[1000];
	for(i = 0; i < n; i++)
	{
		fgets(word, 1000, inp);
		for(j = 0; j < strlen(word); j++)
		{
			if((word[j] >= 'a' && word[j] <= 'z') || (word[j] >= 'A' && word[j] <= 'Z'))
				count[tolower(word[j]) - 'a']++;
			else if(word[j] >= '0' && word[j] <= '9')
				count[word[j] - '0' + 26]++;
		}
	}

	for(i = 0; i < 26; i++)
		fprintf(out, "%c=%d, ", (char)('a' + i), count[i]);
	for(i = 26; i < 35; i++)
		fprintf(out, "%c=%d, ", (char)('0' + i - 26), count[i]);
	fprintf(out, "%c=%d\n", '9', count[35]);

	Node* head = NULL;

	for(i = 0; i < 26; i++)
	{
		Node* curr = createnewnode('a' + i, count[i]);
		insert(&head, curr);
	}
	for(i = 0; i < 10; i++)
	{
		Node* curr = createnewnode('0' + i, count[26 + i]);
		insert(&head, curr);
	}

	int cntnodes = 36;
	char ch = '1';
	for(i = 1; i <= 35; i++)
	{
		Node* curr = head;
		Node* supernode = (Node*)malloc(sizeof(Node));
		supernode -> symbol = (char*)malloc(1000 * sizeof(char));
		supernode -> symbol[0] = 'N';
		if(i < 10)
		{
			supernode -> symbol[1] = (char)('0' + i);
			supernode -> symbol[2] = '\0';
		}
		else
		{
			int ones = i % 10, tens = i / 10;
			supernode -> symbol[1] = (char)('0' + tens);
			supernode -> symbol[2] = (char)('0' + ones);
			supernode -> symbol[3] = '\0';
		}
		supernode -> frequency = (curr -> frequency) + (curr -> next -> frequency);
		supernode -> left = curr;
		supernode -> right = curr -> next;
		supernode -> next = NULL;
		
		head = head -> next;
		head = head -> next;
		insert(&head, supernode);
	}

	preorder(out, head);
	fprintf(out, "\n");

	printHuffman(out, head);

	fclose(inp);

	FILE* inpenc = fopen("encode.txt", "r");
	if(inpenc == NULL)
	{
		printf("Error in opening file.\n");
		return 0;
	}
	encodetext(inpenc, out);

	fclose(inpenc);

	FILE* inpdec = fopen("decode.txt", "r");
	if(inpdec == NULL)
	{
		printf("Error in opening file.\n");
		return 0;
	}
	decodetext(inpdec, out);

	fclose(inpdec);

	fclose(out);
}