//
// Created by Administrator on 2018/11/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hashtab_new.h"

// in this example, key and data are string
struct test_node {
    char key[80];
    char data[80];
};

/**
 * linked_list hash table
 */

/*create a linked_list hash tab*/
hashtab_list *hashtab_list_create(uint size, hash_key_func hash_value, key_cmp_func key_cmp) {
    hashtab_list *h = nullptr;
    int i = 0;
    if ((size < 0) || (hash_value == nullptr) || (key_cmp == nullptr)) {
        return nullptr;
    }

    h = (hashtab_list *) malloc(sizeof(hashtab_list));

    if (h == nullptr) {
        return nullptr;
    }

    h->htables = (hashtab_node **) malloc(size * sizeof(hashtab_node *));

    if (h->htables == nullptr) {
        return nullptr;
    }

    h->size = size;
    h->nel = 0;
    h->hash_value = hash_value;
    h->key_cmp = key_cmp;

    for (i = 0; i < size; i++) {
        h->htables[i] = nullptr;
    }

    return h;
}

void hashtab_list_destroy(hashtab_list *h) {
    int i = 0;
    hashtab_node *cur = nullptr;
    hashtab_node *tmp = nullptr;

    if (h == nullptr) {
        return;
    }

    // free every bucket
    for (i = 0; i < h->size; i++) {
        cur = h->htables[i];
        while (cur != nullptr) {
            tmp = cur;
            cur = cur->next;
            hashtab_node_free(tmp);
        }
        h->htables[i] = nullptr;
    }

    free(h->htables);
    h->htables = nullptr;
    free(h);
}

int hashtab_list_insert(hashtab_list *h, char *key, char *data) {
    uint h_value = 0;
    hashtab_node *cur = nullptr;
    hashtab_node *prev = nullptr; // insert operation need prev point
    hashtab_node *new_node = nullptr;

    if ((h == nullptr) || (key == nullptr) || (data == nullptr)) {
        return ERROR_INSERT_NULLPTR;
    }

    h_value = h->hash_value(h->size, key);
    cur = h->htables[h_value];

    // find right position to insert, make bucket internal ordered
    while ((cur != nullptr) && (h->key_cmp(key, cur->key) > 0)) {
        prev = cur;
        cur = cur->next;
    }

    // if equal, do not need to insert
    if ((cur != nullptr) && (h->key_cmp(key, cur->key) == 0)) {
        return ERROR_INSERT_EQUAL;
    }

    new_node = (hashtab_node *) malloc(sizeof(hashtab_node));
    if (new_node == nullptr) {
        return ERROR_INSERT_MALLOC;
    }
    new_node->key = key;
    new_node->data = data;

    // insert into bucket
    if (prev == nullptr) { // insert node is the first one in bucket
        new_node->next = nullptr;
        h->htables[h_value] = new_node;
    } else {
        new_node->next = prev->next;
        prev->next = new_node;
    }

    h->nel++;

    return 0;
}

hashtab_node *hashtab_list_delete(hashtab_list *h, char *key) {
    uint hvalue = 0;
    hashtab_node *cur = nullptr;
    hashtab_node *prev = nullptr; // delete operation need prev point

    if ((h == nullptr) || (key == nullptr)) {
        return nullptr;
    }

    hvalue = h->hash_value(h->size, key);
    cur = h->htables[hvalue];

    // find the element and remove it from bucket
    while ((cur != nullptr) && (h->key_cmp(key, cur->key)) >= 0) {
        if (h->key_cmp(key, cur->key) == 0) {
            if (prev == nullptr) { // delete node is the first one in bucket
                h->htables[hvalue] = nullptr;
            } else {
                prev->next = cur->next;
            }
            return cur;
        }
        prev = cur;
        cur = cur->next;
    }
    return nullptr;
}

char *hashtab_list_search(hashtab_list *h, char *key) {
    uint hvalue = 0;
    hashtab_node *cur = nullptr;

    if ((h == nullptr) || (key == nullptr)) {
        return nullptr;
    }

    hvalue = h->hash_value(h->size, key);
    cur = h->htables[hvalue];

    while ((cur != nullptr) && (h->key_cmp(key, cur->key) >= 0)) {
        if (h->key_cmp(key, cur->key) == 0) { // still need to compare key
            return cur->data;
        }
        cur = cur->next;
    }

    return nullptr;
}


void hashtab_list_dump(hashtab_list *h) {
    int i = 0;
    hashtab_node *cur = nullptr;

    if (h == nullptr) {
        return;
    }

    printf("------start--size[%d], nel[%d]----------\r\n", h->size, h->nel);
    for (i = 0; i < h->size; i++) {
        printf("htables[%d]:", i);
        cur = h->htables[i];
        while ((cur != nullptr)) {
            printf("{key[%s], data[%s]}\t", cur->key, cur->data);
            cur = cur->next;
        }
        printf("\r\n");
    }
    printf("------end----size[%d], nel[%d]----------\r\n", h->size, h->nel);
}

// a sample hash function that produce hash value
unsigned int sample_hash(const char *str) {
    unsigned int hash = 0;
    unsigned int seed = 131;

    while (*str) {
        hash = hash * seed + *str++;
    }

    return hash & (0x7FFFFFFF);
}

uint hashtab_hvalue(uint size, const char *key) {
    return (sample_hash((char *) key) % size);
}

int hashtab_key_cmp(const char *key1, const char *key2) {
    return strcmp((char *) key1, (char *) key2);
}


void hashtab_node_free(hashtab_node *node) {
    if (node != nullptr) {
        // struct only contain reference pointing to test_node outside, test_node should be deleted, too
        int offset = static_cast<int>((size_t) &(((test_node *) nullptr)->key));
        auto *ptmp = (test_node *) (node->key - offset);
        free(ptmp);
        free(node);
    }
}

int test_linkedlist_hashtable() {
    int res = 0;
    char *pres = nullptr;
    hashtab_node *node = nullptr;
    struct test_node *p = nullptr;
    hashtab_list *h = nullptr;
    h = hashtab_list_create(5, hashtab_hvalue, hashtab_key_cmp);
    assert(h != nullptr);
    while (true) {
        p = (struct test_node *) malloc(sizeof(struct test_node));
        assert(p != nullptr);
        printf("Enter key and value to insert node(enter \"quit\" to exit)\r\n");
        scanf("%s", p->key);
        if (strcmp(p->key, "quit") == 0) {
            free(p); // free test_node
            break;
        }
        scanf("%s", p->data);

        res = hashtab_list_insert(h, p->key, p->data);
        if (res != 0) {
            printf("key[%s], data[%s] insert failed %d\r\n", p->key, p->data, res);
            free(p); // free failed inserted node
        } else {
            printf("key[%s], data[%s] insert success %d\r\n", p->key, p->data, res);
            //free(p); //Attention: if success, do not free this node
        }
    }

    hashtab_list_dump(h);

    while (true) {
        p = (struct test_node *) malloc(sizeof(struct test_node));
        assert(p != nullptr);
        printf("Enter key to search value(enter \"quit\" to exit)\r\n");
        scanf("%s", p->key);
        if (strcmp(p->key, "quit") == 0) {
            free(p);
            break;
        }

        pres = hashtab_list_search(h, p->key);
        if (pres == nullptr) {
            printf("key[%s] search data failed\r\n", p->key);
        } else {
            printf("key[%s] search data[%s] success\r\n", p->key, pres);
        }
        free(p);
    }

    hashtab_list_dump(h);

    while (true) {
        p = (struct test_node *) malloc(sizeof(struct test_node));
        assert(p != nullptr);
        printf("Enter key to delete value(enter \"quit\" to exit)\r\n");
        scanf("%s", p->key);
        if (strcmp(p->key, "quit") == 0) {
            free(p);
            break;
        }

        node = hashtab_list_delete(h, p->key);
        if (node == nullptr) {
            printf("key[%s] delete node failed\r\n", p->key);
        } else {
            printf("key[%s] delete data[%s] success\r\n", node->key, node->data);
            hashtab_node_free(node); //free the delete node
        }
        free(p);

        hashtab_list_dump(h);
    }

    hashtab_list_destroy(h);
    return 0;
}

/**
 * array hash table, using linear probing
 * value is used as key
*/
hashtab_array *hashtab_array_create(uint size, hash_key_func hash_value, key_cmp_func key_cmp) {
    hashtab_array *h = nullptr;
    int i = 0;
    if ((size < 0) || (hash_value == nullptr) || (key_cmp == nullptr)) {
        return nullptr;
    }

    h = (hashtab_array *) malloc(sizeof(hashtab_array));

    if (h == nullptr) {
        return nullptr;
    }

    h->htables = (hashtab_element *) malloc(size * sizeof(hashtab_element));
    h->deleted = (bool *) malloc(size * sizeof(bool));

    if ((h->htables == nullptr) || (h->deleted == nullptr)) {
        return nullptr;
    }

    h->size = size;
    h->used = 0;
    h->hash_value = hash_value;
    h->key_cmp = key_cmp;

    for (i = 0; i < size; i++) {
        memset(&(h->htables[i]), 0, sizeof(hashtab_element)); // h->htables is array of hashtab_element
        h->deleted[i] = false;
    }

    return h;
}

void hashtab_array_destroy(hashtab_array *h) {
    int i = 0;

    if (h == nullptr) {
        return;
    }

    // free every element and corresponding memory space
    for (i = 0; i < h->size; i++) {
        hashtab_element_free(&(h->htables[i]));
    }

    free(h->htables);
    h->htables = nullptr;
    free(h->deleted);
    h->deleted = nullptr;
    free(h);
}

int hashtab_array_insert(hashtab_array *h, char *key, char *data) {
    uint h_value = 0;

    if ((h == nullptr) || (key == nullptr) || (data == nullptr)) {
        return ERROR_INSERT_NULLPTR;
    }

    h_value = h->hash_value(h->size, key); //origin h_value

    while (true) { // key is nullptr or element has been deleted
        if (h->htables[h_value].key != nullptr) {
            if (h->key_cmp(key, h->htables[h_value].key) == 0) {
                return ERROR_INSERT_EQUAL; // if equal, do not need to insert
            }
        }else if(h->deleted[h_value]){
            //deleted element do not need to compare key
            //do not insert into deleted element, ensure dynamic expansion succeed
        }else{
            break;
        }
        // use linear probing
        h_value = (h_value + 1) % h->size;
    }

    // after search equal
    // if fill, do not insert
    if (((float) h->used / h->size) > FILL_FACTOR) {
        return ERROR_INSERT_ARRAY_FILL;
    }

    // h_value now represent insert position
    h->htables[h_value].key = key;
    h->htables[h_value].data = data;
    if (h->deleted[h_value]) { // reuse deleted space
        h->deleted[h_value] = false;
    }else{
        h->used++;
    }
    return 0;
}

hashtab_element *hashtab_array_delete(hashtab_array *h, char *key) {
    uint h_value = 0;
    if ((h == nullptr) || (key == nullptr)) {
        return nullptr;
    }

    h_value = h->hash_value(h->size, key); //origin h_value

    // determine by key pointer
    while (true) { // key is nullptr or element has been deleted
        if (h->htables[h_value].key != nullptr) {
            if (h->key_cmp(key, h->htables[h_value].key) == 0) {
                h->deleted[h_value] = true;
                return &(h->htables[h_value]);
            }
        }else if(h->deleted[h_value]){
            // delete element is a placeholder
        }else{
            break;
        }
        // use linear probing
        h_value = (h_value + 1) % h->size; // h_value now represent insert position
    }

    return nullptr;
}

hashtab_element *hashtab_array_move(hashtab_array *h) {
    if (h == nullptr) {
        return nullptr;
    }
    int i;
    for (i = 0; (i < h->size) && (h->htables[i].key == nullptr); ++i); //search for non-null element

    if (h->htables[i].key != nullptr) {
        auto *move_element = static_cast<hashtab_element *>(malloc(sizeof(hashtab_element)));
        move_element->key = h->htables[i].key;
        move_element->data = h->htables[i].data;
        //delete from small array
        h->htables[i].key = nullptr;
        h->htables[i].data = nullptr;
        h->deleted[i] = true;
        return move_element;
    } else { //entire table is empty
        return nullptr;
    }

}

char *hashtab_array_search(hashtab_array *h, char *key) {
    uint h_value = 0;

    if ((h == nullptr) || (key == nullptr)) {
        return nullptr;
    }

    h_value = h->hash_value(h->size, key); //origin h_value

    // determine by key pointer
    while (true) { // key is nullptr or element has been deleted
        if (h->htables[h_value].key != nullptr) {
            if (h->key_cmp(key, h->htables[h_value].key) == 0) {
                return h->htables[h_value].data;
            }
        }else if(h->deleted[h_value]){ //deleted element do not need to compare key
            // delete element is a placeholder
        }else{
            break;
        }
        // use linear probing
        h_value = (h_value + 1) % h->size; // h_value now represent insert position
    }

    return nullptr;
}

void hashtab_array_dump(hashtab_array *h) {
    int i = 0;

    if (h == nullptr) {
        printf("------array is empty------\r\n");
        return;
    }

    printf("------start--size[%d], used[%d]----------\r\n", h->size, h->used);
    for (i = 0; i < h->size; i++) {
        if (h->htables[i].key == nullptr) {
            printf("h_table[%d]: {null, deleted:%d}\n", i, h->deleted[i]);
        } else {
            printf("h_table[%d]: {key[%s], data[%s]}\n", i, h->htables[i].key, h->htables[i].data);
        }

    }
    printf("------end--------------------------------\r\n");
}

void hashtab_element_free(hashtab_element *element) {
    if (element != nullptr) {
        int offset = static_cast<int>((size_t) &(((test_node *) nullptr)->key));
        auto *ptmp = (test_node *) (element->key - offset);
        free(ptmp);
        // element will not be free and used for subsequent judgment
        element->key = nullptr;
        element->data = nullptr;
    }
}

hashtab_array_list *hashtab_array_list_create(uint initial_size, hash_key_func hash_value, key_cmp_func key_cmp) {
    hashtab_array_list *h = nullptr;

    h = static_cast<hashtab_array_list *>(malloc(sizeof(hashtab_array_list)));

    if (h == nullptr) {
        return nullptr;
    }

    h->large_harray = nullptr;
    h->small_harray = hashtab_array_create(initial_size, hash_value, key_cmp); //initial a small hashtab_array

    return h;
}

void hashtab_array_list_destroy(hashtab_array_list *h) {
    if (h == nullptr) {
        return;
    }

    if (h->large_harray != nullptr) {
        hashtab_array_destroy(h->large_harray);
    }

    if (h->small_harray != nullptr) {
        hashtab_array_destroy(h->small_harray);
    }

    free(h);
}

int hashtab_array_list_expansion(hashtab_array_list *h) {
    if (h == nullptr) {
        return 1;
    }

    if (h->large_harray != nullptr) {
        return 0;
    }

    printf("array expansion\r\n");
    h->large_harray = hashtab_array_create(h->small_harray->size * EXPANSION_SCALAR, h->small_harray->hash_value,
                                           h->small_harray->key_cmp);

    if (h->large_harray == nullptr) {
        return 2;
    } else {
        return 0;
    }

}

//int hashtab_array_list_shrinkage(hashtab_array_list *h) {
//    if (h == nullptr) {
//        return 1;
//    }
//
//    if (h->small_harray != nullptr) {
//        return 0;
//    }
//
//    h->small_harray = hashtab_array_create(h->large_harray->size / 2, h->large_harray->hash_value,
//                                           h->large_harray->key_cmp);
//
//    if (h->small_harray == nullptr) {
//        return 2;
//    } else {
//        return 0;
//    }
//
//}

void hashtab_array_list_dump(hashtab_array_list *h) {
    if (h == nullptr) {
        return;
    }

    if (h->small_harray != nullptr) {
        printf("------small array-------------------------\r\n");
        hashtab_array_dump(h->small_harray);
        printf("------end array---------------------------\r\n\r\n");
    }

    if (h->large_harray != nullptr) {
        printf("------large array--size[%d], used[%d]-----\r\n", h->large_harray->size, h->large_harray->used);
        hashtab_array_dump(h->large_harray);
        printf("------end array---------------------------\r\n\r\n");
    }
}

int hashtab_array_dynamic_insert(hashtab_array_list *h, char *key, char *data) {
    if ((h == nullptr) || (key == nullptr) || (data == nullptr)) {
        return ERROR_INSERT_NULLPTR;
    }

    int res;
    hashtab_element *move_element;
    // First, try to insert into small hash array
    res = hashtab_array_insert(h->small_harray, key, data);
    // Second, if small array filled, try to insert into large array
    if (res == ERROR_INSERT_ARRAY_FILL) {
        res = hashtab_array_list_expansion(h);
        if (res == 0) {
            res = hashtab_array_insert(h->large_harray, key, data);
        }
    }else{
        return res;
    }

    if (res != 0) {
        return res;
    }

    //Third, if large array exist, gradually move element
    if (h->large_harray != nullptr) {
        move_element = hashtab_array_move(h->small_harray);
        if (move_element != nullptr) {
            printf("move element: %s\r\n", move_element->key);
            res = hashtab_array_insert(h->large_harray, move_element->key, move_element->data);
        } else {
            // if small array is empty, destroy and switch large array to small array
            printf("move finish, destroy small array\r\n");
            hashtab_array_destroy(h->small_harray);
            h->small_harray = h->large_harray;
            h->large_harray = nullptr;
        }
    }

    return res;
}

hashtab_element *hashtab_array_dynamic_delete(hashtab_array_list *h, char *key) {
    if (h == nullptr) {
        return nullptr;
    }

    hashtab_element *deleted_element = nullptr;
    // First, try to delete from small array
    if (h->small_harray != nullptr) {
        deleted_element = hashtab_array_delete(h->small_harray, key);
    }
    // Second, try to delete from large array
    if ((deleted_element == nullptr) && (h->large_harray != nullptr)) {
        deleted_element = hashtab_array_delete(h->large_harray, key);
    }

    return deleted_element;
}

char * hashtab_array_dynamic_search(hashtab_array_list *h, char *key) {
    if (h == nullptr) {
        return nullptr;
    }

    char *result = nullptr;

    if (h->small_harray != nullptr) {
        result = hashtab_array_search(h->small_harray, key);
    }

    if ((result == nullptr) && (h->large_harray != nullptr)) {
        result = hashtab_array_search(h->large_harray, key);
    }

    return result;
}

int test_array_hashtable() {
    int res = 0;
    char *pres = nullptr;
    hashtab_element *node = nullptr;
    struct test_node *p = nullptr;
    hashtab_array *h = nullptr;
    int c;
    h = hashtab_array_create(5, hashtab_hvalue, hashtab_key_cmp);
    assert(h != nullptr);
    while (true) {
        printf("Enter mode:('i'-insert;'s'-search;'d'-delete;'q'-quit)\r\n");
        c = getchar();
        if (c == 'q') {
            break;
        }
        switch (c) {
            case 'i':
                while (true) {
                    p = (struct test_node *) malloc(sizeof(struct test_node));
                    assert(p != nullptr);
                    printf("Enter key and value to insert node(enter \"quit\" to exit)\r\n");
                    scanf("%s", p->key);
                    if (strcmp(p->key, "quit") == 0) {
                        free(p); // free test_node
                        break;
                    }
                    scanf("%s", p->data);

                    res = hashtab_array_insert(h, p->key, p->data);
                    if (res != 0) {
                        printf("key[%s], data[%s] insert failed %d\r\n", p->key, p->data, res);
                        free(p); // free failed inserted node
                    } else {
                        printf("key[%s], data[%s] insert success %d\r\n", p->key, p->data, res);
                        //free(p); //Attention: if success, do not free this node
                    }
                }
                hashtab_array_dump(h);
                break;
            case 's':
                while (true) {
                    p = (struct test_node *) malloc(sizeof(struct test_node));
                    assert(p != nullptr);
                    printf("Enter key to search value(enter \"quit\" to exit)\r\n");
                    scanf("%s", p->key);
                    if (strcmp(p->key, "quit") == 0) {
                        free(p);
                        break;
                    }

                    pres = hashtab_array_search(h, p->key);
                    if (pres == nullptr) {
                        printf("key[%s] search data failed\r\n", p->key);
                    } else {
                        printf("key[%s] search data[%s] success\r\n", p->key, pres);
                    }
                    free(p);
                }
                hashtab_array_dump(h);
                break;
            case 'd':
                while (true) {
                    p = (struct test_node *) malloc(sizeof(struct test_node));
                    assert(p != nullptr);
                    printf("Enter key to delete value(enter \"quit\" to exit)\r\n");
                    scanf("%s", p->key);
                    if (strcmp(p->key, "quit") == 0) {
                        free(p);
                        break;
                    }

                    node = hashtab_array_delete(h, p->key);
                    if (node == nullptr) {
                        printf("key[%s] delete node failed\r\n", p->key);
                    } else {
                        printf("key[%s] delete data[%s] success\r\n", node->key, node->data);
                        hashtab_element_free(node); //free the delete node
                    }
                    free(p);

                    hashtab_array_dump(h);
                }
                break;
            default:
                printf("Wrong command!!!\r\n");
        }
        while(getchar() != '\n') continue;
    }

    hashtab_array_destroy(h);
    return 0;
}

int test_array_list_hashtable() {
    int res = 0;
    char *pres = nullptr;
    hashtab_element *node = nullptr;
    struct test_node *p = nullptr;
    hashtab_array_list *h = nullptr;
    int c;
    h = hashtab_array_list_create(5, hashtab_hvalue, hashtab_key_cmp);
    assert(h != nullptr);
    while (true) {
        printf("Enter mode:('i'-insert;'s'-search;'d'-delete;'q'-quit)\r\n");
        c = getchar();
        if (c == 'q') {
            break;
        }
        switch (c) {
            case 'i':
                while (true) {
                    p = (struct test_node *) malloc(sizeof(struct test_node));
                    assert(p != nullptr);
                    printf("Enter key and value to insert node(enter \"quit\" to exit; enter \"show\" to show current)\r\n");
                    scanf("%s", p->key);
                    if (strcmp(p->key, "quit") == 0) {
                        free(p); // free test_node
                        break;
                    } else if (strcmp(p->key, "show") == 0) {
                        hashtab_array_list_dump(h);
                        free(p); // free test_node
                        continue;
                    }
                    scanf("%s", p->data);

                    res = hashtab_array_dynamic_insert(h, p->key, p->data);
                    if (res != 0) {
                        printf("key[%s], data[%s] insert failed %d\r\n", p->key, p->data, res);
                        free(p); // free failed inserted node
                    } else {
                        printf("key[%s], data[%s] insert success %d\r\n", p->key, p->data, res);
                        //free(p); //Attention: if success, do not free this node
                    }
                }
                hashtab_array_list_dump(h);
                break;
            case 's':
                while (true) {
                    p = (struct test_node *) malloc(sizeof(struct test_node));
                    assert(p != nullptr);
                    printf("Enter key to search value(enter \"quit\" to exit)\r\n");
                    scanf("%s", p->key);
                    if (strcmp(p->key, "quit") == 0) {
                        free(p);
                        break;
                    }

                    pres = hashtab_array_dynamic_search(h, p->key);
                    if (pres == nullptr) {
                        printf("key[%s] search data failed\r\n", p->key);
                    } else {
                        printf("key[%s] search data[%s] success\r\n", p->key, pres);
                    }
                    free(p);
                }
                hashtab_array_list_dump(h);
                break;
            case 'd':
                while (true) {
                    p = (struct test_node *) malloc(sizeof(struct test_node));
                    assert(p != nullptr);
                    printf("Enter key to delete value(enter \"quit\" to exit)\r\n");
                    scanf("%s", p->key);
                    if (strcmp(p->key, "quit") == 0) {
                        free(p);
                        break;
                    }

                    node = hashtab_array_dynamic_delete(h, p->key);
                    if (node == nullptr) {
                        printf("key[%s] delete node failed\r\n", p->key);
                    } else {
                        printf("key[%s] delete data[%s] success\r\n", node->key, node->data);
                        hashtab_element_free(node); //free the delete node
                    }
                    free(p);

                    hashtab_array_list_dump(h);
                }
                break;
            default:
                printf("Wrong command!!!\r\n");
        }
        while(getchar() != '\n') continue;
    }

    hashtab_array_list_destroy(h);
    return 0;
}

int main() {
    //test_linkedlist_hashtable();
    //test_array_hashtable();
    test_array_list_hashtable();
    return 0;
}
