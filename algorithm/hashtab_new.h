//
// Created by Administrator on 2018/11/25.
//

#ifndef ALGORITHM_HASHTAB_H
#define ALGORITHM_HASHTAB_H

typedef unsigned int uint;
// functions used by hash table
typedef uint(*hash_key_func)(uint size, const char *key); //hash function
typedef int(*key_cmp_func)(const char *key1, const char *key2); //hash compare function

#define ERROR_INSERT_NULLPTR 1
#define ERROR_INSERT_EQUAL 2
#define ERROR_INSERT_MALLOC 3
#define ERROR_INSERT_ARRAY_FILL 4

/**
 * hash table that used linked list to solve conflict
 * store type: string
 */
// node of linked list
typedef struct _hashtab_node
{
	char *key;
	char *data;
	struct _hashtab_node *next;
}hashtab_node;

typedef struct _hashtab_list
{
	hashtab_node **htables; //hash bucket, each bucket is a point to hashtab_node, htable is a list of bucket
	uint size; //count of hash bucket
	uint nel; //count of total node
	uint(*hash_value)(uint size, const char *key); //hash function
	int(*key_cmp)(const char *key1, const char *key2); //hash compare function
}hashtab_list;

hashtab_list *hashtab_list_create(uint size, hash_key_func hash_value, key_cmp_func key_cmp);
void hashtab_list_destroy(hashtab_list *h);
int hashtab_list_insert(hashtab_list *h, char *key, char *data);
hashtab_node *hashtab_list_delete(hashtab_list *h, char *key);
char * hashtab_list_search(hashtab_list *h, char *key);
void hashtab_list_dump(hashtab_list *h);
void hashtab_node_free(hashtab_node *node);

/**
 * hash table that used linear probing to solve conflict
 * support dynamic expansion
 * store type: string
 */

#define FILL_FACTOR 0.75
#define EXPANSION_SCALAR 2

// element of linked list
typedef struct _hashtab_element
{
    char *key;
    char *data;
}hashtab_element;

typedef struct _hashtab_array
{
    hashtab_element *htables; //hash table array that store element
    bool *deleted; //flag array that represent delete state
    uint size; //total space
    uint used; //used space
    uint(*hash_value)(uint size, const char *key); //hash function
    int(*key_cmp)(const char *key1, const char *key2); //hash compare function
}hashtab_array;

// array list support dynamic expansion and shrinkage
typedef struct _hashtab_array_list
{
    hashtab_array *large_harray; //hashtable array used to expansion
    hashtab_array *small_harray; //hashtable array that store small scale
}hashtab_array_list;

// functions used by hashtab_array
hashtab_array *hashtab_array_create(uint size, hash_key_func hash_value, key_cmp_func key_cmp);
void hashtab_array_destroy(hashtab_array *h);
int hashtab_array_insert(hashtab_array *h, char *key, char *data);
hashtab_element * hashtab_array_delete(hashtab_array *h, char *key);
hashtab_element *hashtab_array_move(hashtab_array *h);
char * hashtab_array_search(hashtab_array *h, char *key);
void hashtab_array_dump(hashtab_array *h);
void hashtab_element_free(hashtab_element *element);

// functions used by hashtab_array_list
hashtab_array_list *hashtab_array_list_create(uint initial_size, hash_key_func hash_value, key_cmp_func key_cmp);
void hashtab_array_list_destroy(hashtab_array_list *h);
int hashtab_array_list_expansion(hashtab_array_list *h);
//int hashtab_array_list_shrinkage(hashtab_array_list *h);
void hashtab_array_list_dump(hashtab_array_list *h);

// functions used by hashtab_array dynamic operation
int hashtab_array_dynamic_insert(hashtab_array_list *h, char *key, char *data);
hashtab_element * hashtab_array_dynamic_delete(hashtab_array_list *h, char *key);
char * hashtab_array_dynamic_search(hashtab_array_list *h, char *key);

#endif //ALGORITHM_HASHTAB_H
