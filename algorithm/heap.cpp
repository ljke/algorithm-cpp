#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>


/* Implement heap*/
#define MAX_HEAP_SIZE (1 << 8)

struct element
{
	int data;
};

struct heap
{
	union
	{
		unsigned long elements; //element count
		element *elem[MAX_HEAP_SIZE]; //array that store elements
	};
};

void init_heap(heap *heap)
{
	int i;
	for (i = 0; i < MAX_HEAP_SIZE; i++)
	{
		heap->elem[i] = NULL;
	}
}

/**
* Linux kernel function
* fls - find last (most-significant) bit set
* @x: the word to search
*
* This is defined the same way as ffs.
* Note fls(0) = 0, fls(1) = 1, fls(0x80000000) = 32.
*/
static int fls(int x)
{
	int r = 32;

	if (!x)
		return 0;
	if (!(x & 0xffff0000u)) {
		x <<= 16;
		r -= 16;
	}
	if (!(x & 0xff000000u)) {
		x <<= 8;
		r -= 8;
	}
	if (!(x & 0xf0000000u)) {
		x <<= 4;
		r -= 4;
	}
	if (!(x & 0xc0000000u)) {
		x <<= 2;
		r -= 2;
	}
	if (!(x & 0x80000000u)) {
		x <<= 1;
		r -= 1;
	}
	return r;
}

void dump_heap(heap *h, int index)
{
	element *elem;
	int level;
	if (index > h->elements)
	{
		return;
	}
	elem = h->elem[index];
	level = fls(index);

	dump_heap(h, index * 2 + 1);

	if (!(index % 2) && index != 1)
		printf("%*s\n", level * 3, "|");

	printf("%*s - %05d\n", level * 3, " ", elem->data);

	if (index % 2 && index != 1)
		printf("%*s\n", level * 3, "|");

	dump_heap(h, index * 2);
}

void dump(heap *h, int elements)
{
	for (int i = 1; i <= elements; i++)
		printf("[%02d]: %4d\n", i, h->elem[i]->data);

}

element *create_element(int data)
{
	element *elem;
	elem = (element *)malloc(sizeof(element));
	if (elem)
	{
		elem->data = data;
	}
	return elem;
}

void fake_a_heap(heap *h)
{
	/*data is in ordered*/
	int data[10] = { 7, 4, 9, 2, 6, 8, 10, 1, 3, 5 };
	init_heap(h);
	/*root start at 1*/
	for (int i = 0; i < 10; i++)
	{
		h->elem[i + 1] = create_element(data[i]);
	}
	h->elements = 10;
}

//************************************
// Method:       swap
// Description:  swap i-th node and j-th node
// Access:       public 
// Returns:      void
// Qualifier:   
// Parameter:    heap * h
// Parameter:    int i
// Parameter:    int j
//************************************
void swap(heap *h, int i, int j)
{
	element *tmp;
	tmp = h->elem[j];
	h->elem[j] = h->elem[i];
	h->elem[i] = tmp;
}

//************************************
// Method:       heapify
// Description:  heapify from top to bottom
// Access:       public 
// Returns:      void
// Qualifier:   
// Parameter:    heap * h
// Parameter:    int parent, parent node index
//************************************
void heapify(heap *h, int parent)
{
	element **elem = h->elem;
	int elements = h->elements;
	int left, right, max;

	while (true)
	{
		left = parent * 2;
		right = left + 1;
		//compare with children and find max
		max = parent;
		if ((left <= elements) && (elem[max]->data < elem[left]->data)) //care for boundary 
		{
			max = left;
		}
		if ((right <= elements) && (elem[max]->data < elem[right]->data))
		{
			max = right;
		}
		if (max == parent) //heapify finish
		{
			break;
		}

		swap(h, max, parent);
		parent = max;
	}
}

void build_heap(heap *h)
{
	for (int i = h->elements / 2; i >= 1; i--)
	{
		heapify(h, i);
	}
}

//************************************
// Method:       heap_sort
// Description:  sort heap node, from top to bottom
// Access:       public 
// Returns:      int, sorted node count
// Qualifier:   
// Parameter:    heap * h
//************************************
int heap_sort(heap *h)
{
	int elements = h->elements;
	while (h->elements)
	{
		swap(h, 1, h->elements);
		h->elements--;
		heapify(h, 1); //heapify method use h->elements to control boundary
	}
	return elements;
}

int main()
{
	struct heap h;
	int elements;

	fake_a_heap(&h);
	dump_heap(&h, 1);

	printf("After Heapify:\n");
	build_heap(&h);
	dump_heap(&h, 1);

	printf("After Heap Sort:\n");
	elements = heap_sort(&h);
	dump(&h, elements);
	return 0;
}