//
// Created by Administrator on 2018/11/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hashtab.h"

#ifdef MEMORY_TEST
#include <mcheck.h>
#endif

/*create a linkedlist hash tab*/
hashtab_list * hashtab_create(int size, hash_key_func hash_value, key_cmp_func key_cmp, hash_node_free_func hash_node_free)
{
	hashtab_list *h = nullptr;
	int i = 0;
	if ((size < 0) || (hash_value == nullptr) || (key_cmp == nullptr))
	{
		return nullptr;
	}

	h = (hashtab_list *)malloc(sizeof(hashtab_list));

	if (h == nullptr)
	{
		return nullptr;
	}

	h->htables = (hashtab_node **)malloc(size * sizeof(hashtab_node*));

	if (h->htables == nullptr)
	{
		return nullptr;
	}

	h->size = size;
	h->nel = 0;
	h->hash_value = hash_value;
	h->key_cmp = key_cmp;
	h->hash_node_free = hash_node_free;

	for (i = 0; i < size; i++)
	{
		h->htables[i] = nullptr;
	}

	return h;
}

void hashtab_destory(hashtab_list * h)
{
	int i = 0;
	hashtab_node *cur = nullptr;
	hashtab_node *tmp = nullptr;

	if (h == nullptr)
	{
		return;
	}

	// free every bucket
	for (i = 0; i < h->size; i++)
	{
		cur = h->htables[i];
		while (cur != nullptr)
		{
			tmp = cur;
			cur = cur->next;
			h->hash_node_free(tmp);
		}
		h->htables[i] = nullptr;
	}

	free(h->htables);
	free(h);
}

int hashtab_insert(hashtab_list * h, char * key, char * data)
{
	int h_value = 0;
	hashtab_node *cur = nullptr;
	hashtab_node *prev = nullptr;
	hashtab_node *new_node = nullptr;

	if ((h == nullptr) || (key == nullptr) || (data == nullptr))
	{
		return 1;
	}

	h_value = h->hash_value(h, key);
	cur = h->htables[h_value];

	// find right position to insert, make bucket internal ordered
	while ((cur != nullptr) && (h->key_cmp(h, key, cur->key) > 0))
	{
		prev = cur;
		cur = cur->next;
	}

	// if equal, do not need to insert
	if ((cur != nullptr) && (h->key_cmp(h, key, cur->key) == 0))
	{
		return 2;
	}

	new_node = (hashtab_node *)malloc(sizeof(hashtab_node));
	if (new_node == nullptr)
	{
		return 3;
	}

	new_node->key = key;
	new_node->data = data;
	// insert into bucket
	if (prev == nullptr)
	{
		new_node->next = h->htables[h_value];
		h->htables[h_value] = new_node;
	}
	else
	{
		new_node->next = prev->next;
		prev->next = new_node;
	}

	h->nel++;

	return 0;
}

hashtab_node * hashtab_delete(hashtab_list * h, char * key)
{
	int hvalue = 0;
	hashtab_node *cur = nullptr;
	hashtab_node *prev = nullptr;

	if ((h == nullptr) || (key == nullptr))
	{
		return nullptr;
	}

	hvalue = h->hash_value(h, key);
	cur = h->htables[hvalue];

	// find the element and delete it
	while ((cur != nullptr) && (h->key_cmp(h, key, cur->key)) >= 0)
	{
		if (h->key_cmp(h, key, cur->key) == 0)
		{
			if (prev == nullptr)
			{
				h->htables[hvalue] = cur->next;
			}
			else
			{
				prev->next = cur->next;
			}
			return cur;
		}
		prev = cur;
		cur = cur->next;
	}

	return nullptr;
}

void * hashtab_search(hashtab_list * h, char * key)
{
	int hvalue = 0;
	hashtab_node *cur = nullptr;

	if ((h == nullptr) || (key == nullptr))
	{
		return nullptr;
	}

	hvalue = h->hash_value(h, key);
	cur = h->htables[hvalue];

	while ((cur != nullptr) && (h->key_cmp(h, key, cur->key) >= 0))
	{
		if (h->key_cmp(h, key, cur->key) == 0)
		{
			return cur->data;
		}
		cur = cur->next;
	}

	return nullptr;
}


void hashtab_dump(hashtab_list *h)
{
	int i = 0;
	hashtab_node *cur = nullptr;

	if (h == nullptr)
	{
		return;
	}

	printf("------start--size[%d], nel[%d]----------\r\n", h->size, h->nel);
	for (i = 0; i < h->size; i++)
	{
		printf("htables[%d]:", i);
		cur = h->htables[i];
		while ((cur != nullptr))
		{
			printf("{key[%s], data[%s]}\t", cur->key, cur->data);
			cur = cur->next;
		}
		printf("\r\n");
	}

	printf("\r\n------end----size[%d], nel[%d]----------\r\n", h->size, h->nel);
}

// in this example, key and data are string
struct test_node
{
	char key[80];
	char data[80];
};

unsigned int sample_hash(const char *str)
{
	unsigned int hash = 0;
	unsigned int seed = 131;

	while (*str)
	{
		hash = hash * seed + *str++;
	}

	return hash & (0x7FFFFFFF);
}

int hashtab_hvalue(hashtab_list *h, const char *key)
{
	return (sample_hash((char *)key) % h->size);
}

int hashtab_key_cmp(hashtab_list *h, const char *key1, const char *key2)
{
	return strcmp((char *)key1, (char *)key2);
}


void hashtab_node_free(hashtab_node *node)
{
	// delete the array(test_node) inside struct
	int offset = static_cast<int>((size_t) &(((test_node *) nullptr)->key));
	auto *ptmp = (test_node *)(node->key - offset);
	free(ptmp);
	free(node);
}

int main()
{
	int res = 0;
	char *pres = nullptr;
	hashtab_node *node = nullptr;
	struct test_node *p = nullptr;
	hashtab_list *h = nullptr;
#ifdef MEMORY_TEST
	setenv("MALLOC_TRACE", "1.txt", 1);
	mtrace();
#endif
	h = hashtab_create(5, hashtab_hvalue, hashtab_key_cmp, hashtab_node_free);
	assert(h != nullptr);
	while (true)
	{
		p = (struct test_node*)malloc(sizeof(struct test_node));
		assert(p != nullptr);
		printf("Enter key and value(enter \"quit\" to exit)\r\n");
		scanf("%s", p->key);
		if (strcmp(p->key, "quit") == 0)
		{
			free(p); // free test_node
			break;
		}
		scanf("%s", p->data);

		res = hashtab_insert(h, p->key, p->data);
		if (res != 0)
		{
			printf("key[%s], data[%s] insert failed %d\r\n", p->key, p->data, res);
			free(p); // free test_node
		}
		else
		{
			printf("key[%s], data[%s] insert success %d\r\n", p->key, p->data, res);
			//free(p); //Attention: if success, do not free this node
		}
	}

	hashtab_dump(h);

	while (true)
	{
		p = (struct test_node *)malloc(sizeof(struct test_node));
		assert(p != nullptr);
		printf("Enter key to search value(enter \"quit\" to exit)\r\n");
		scanf("%s", p->key);

		if (strcmp(p->key, "quit") == 0)
		{
			free(p);
			break;
		}

		pres = static_cast<char *>(hashtab_search(h, p->key));

		if (pres == nullptr)
		{
			printf("key[%s] search data failed\r\n", p->key);
		}
		else
		{
			printf("key[%s] search data[%s] success\r\n", p->key, pres);
		}
	}

	hashtab_dump(h);

	while (true)
	{
		p = (struct test_node *)malloc(sizeof(struct test_node));
		assert(p != nullptr);
		printf("Enter key to delete value(enter \"quit\" to exit)\r\n");
		scanf("%s", p->key);

		if (strcmp(p->key, "quit") == 0)
		{
			free(p);
			break;
		}

		node = hashtab_delete(h, p->key);

		if (node == nullptr)
		{
			printf("key[%s] delete node failed\r\n", p->key);
		}
		else
		{
			printf("key[%s] delete data[%s] success\r\n", node->key, node->data);
			h->hash_node_free(node); //free the delete node
		}
		free(p);

		hashtab_dump(h);
	}

	hashtab_destory(h);
#ifdef MEMORY_TEST
	muntrace();
#endif
	return 0;
}