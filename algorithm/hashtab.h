//
// Created by Administrator on 2018/11/25.
//

#ifndef ALGORITHM_HASHTAB_H
#define ALGORITHM_HASHTAB_H

// node of linked list
typedef struct _hashtab_node
{
	char *key;
	char *data;
	struct _hashtab_node *next;
}hashtab_node;

/*hash tab that used linked list to solve conflict*/
typedef struct _hashtab_list
{
	hashtab_node **htables; //hash bucket, each bucket is a point to hashtab_node, htable is a list of bucket
	int size; //count of hash bucket
	int nel; //count of total node
	int(*hash_value)(struct _hashtab_list *h, const char *key); //hash function
	int(*key_cmp)(struct _hashtab_list *h, const char *key1, const char *key2); //hash compare function
	void(*hash_node_free)(hashtab_node *node); //free node
}hashtab_list;

typedef int(*hash_key_func)(struct _hashtab_list *h, const char *key); //hash function
typedef int(*key_cmp_func)(struct _hashtab_list *h, const char *key1, const char *key2); //hash compare function
typedef void(*hash_node_free_func)(hashtab_node *node); //free node

hashtab_list *hashtab_create(int size, hash_key_func hash_value, key_cmp_func key_cmp, hash_node_free_func hash_node_free);
void hashtab_destory(hashtab_list *h);
int hashtab_insert(hashtab_list *h, char *key, char *data);
hashtab_node *hashtab_delete(hashtab_list *h, char *key);
void *hashtab_search(hashtab_list *h, char *key);

#endif //ALGORITHM_HASHTAB_H
