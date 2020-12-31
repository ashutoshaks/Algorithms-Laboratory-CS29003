#include <iostream>
#include <cstdlib>
using namespace std;

struct node
{
	int data;
	struct node *add;
};
typedef struct node node;

// a function used to calculate xor of addresses and retrieving them back as needed
node* findxor(node *prev, node *next)
{
	return (node*)((unsigned long long)prev ^ (unsigned long long)next);
}

// to print the linked list from the front to the end
void traverse_from_front_to_end(struct node *head)
{
	node *p = head, *prevptr = NULL, *nextptr;
	while(p != NULL)
	{
		cout << p->data << " ";
		nextptr = findxor(prevptr, p->add);
		prevptr = p;
		p = nextptr;
	}
	cout << endl;
}

// to print the linked list from the end to the front
void traverse_from_end_to_front(struct node *tail)
{
	node *p = tail, *prevptr = NULL, *nextptr = NULL;
	while(p != NULL)
	{
		cout << p->data << " ";
		nextptr = findxor(prevptr, p->add);
		prevptr = p;
		p = nextptr;
	}
	cout << endl;
}

// function to reverse the linked list
// because of the xor method of storing addresses we only need to swap the head and tail references
void reverse(struct node **head, struct node **tail)
{
	node *temp = *head;
	*head = *tail;
	*tail = temp;
}

void alternate(struct node **head, struct node **tail)
{
	node *p = *head, *prevptr = NULL, *nextptr = NULL;

	// p refers to the current element
	// prevptr refers to the previos element
	// nextptr refers to the next element
	while(p != NULL)
	{
		nextptr = findxor(prevptr, p->add);
		if(nextptr == NULL) // this means p is the last element
			break;
		
		// p2 refers to the node after nextptr
		node *p2 = findxor(nextptr->add, p);
		if(p2 == NULL) // this means nextptr is the last element
			break;

		// here we are breaking of the link from p to nextptr
		nextptr->add = findxor(NULL, p2);

		// now we reverse the list from nextptr to the tail
		reverse(&nextptr, tail);
		
		//now we join the links that we had broken

		// p2 here refers to the node that is now to the right of nextptr
		p2 = findxor(nextptr->add, NULL);
		if(p2 == NULL)
			break;

		// fixing the add value for nextptr
		nextptr->add = findxor(p, p2);

		// fixing the add value for p
		p->add = findxor(prevptr, nextptr);

		prevptr = p;
		p = nextptr;
	}
}

int main()
{
	int n, i, x;
	cout << "Enter the value of n :\nn = ";
	cin >> n;
	cout << "Enter the " << n << " integers between -100 and 100 : ";
	node *head, *tail, *prevptr, *newnode, *nextptr;
	head = prevptr = NULL;
	
	for(i = 0; i < n; i++)
	{
		cin >> x;

		// newnode stores everything for the current node
		// prevnode referes to the previous node
		// nextnode refers to the next node

		if(i == 0)
			newnode = (node*)malloc(sizeof(node));
		if(head == NULL)
			head = newnode; // initializing head as the pointer to the first element
		newnode->data = x;
		
		if(i != n - 1)
			nextptr = (node*)malloc(sizeof(node));
		else
			nextptr = NULL; // for the last element the next pointer will be NULL
		newnode->add = findxor(prevptr, nextptr);
		prevptr = newnode;
		if(i != n - 1)
			newnode = nextptr;
		else
			tail = newnode; // initializing tail as the pointer to the last element
	} 
	
	cout << "Doubly linked list traversed from front to end : ";
	traverse_from_front_to_end(head);
	cout << "Doubly linked list traversed from end to front : ";
	traverse_from_end_to_front(tail);
	reverse(&head, &tail);
	cout << "Reversed doubly linked list traversed from front to end : ";
	traverse_from_front_to_end(head);
	alternate(&head, &tail);
	cout << "Alternated doubly linked list traversed from front to end : ";
	traverse_from_front_to_end(head);

}