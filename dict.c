#include "dict.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

dict_t* new_dict(size_t num_buckets, int hashfunc(void*), int cmpfunc(void*, void*)) {
    
    //Just how lmao
    assert(num_buckets > 0);
    
    // To make sure malloc is successful
    dict_t* dict = (dict_t*)malloc(sizeof(*dict));
    assert(dict);
    
    // Initialize size at 0
    dict->_size = 0;

    // Give out the number of buckets specified 
    dict->_num_buckets = num_buckets;
    dict->_set = (bucket_t**)malloc(sizeof(bucket_t*) * num_buckets);
    for (int i = 0; i < num_buckets; ++i)
        dict->_set[i] = NULL;
    
    // Fill in function ptr
    dict->_cmpfunc = cmpfunc;
    dict->_hashfunc = hashfunc;

    return dict;
}

static void* search_bucket(bucket_t* bucket, void* data, int cmp(void*, void*)) {
    if (!bucket)
        return NULL;

    // Just normal linked list traversal
    node_t* tmp = bucket;
    while (tmp) {
        if ((*cmp)(data, tmp->_data) == 0)
            return tmp->_data;
        tmp = tmp->_next;
    }
    return tmp; // Can return NULL, adjustments can be done at data.c
}

void* find(dict_t* dict, void* data) {
    int pos = (*dict->_hashfunc)(data);
    
    // search linked list at index of specified pos
    void* search = search_bucket(dict->_set[pos], data, dict->_cmpfunc);
    return search;
}

static bucket_t* insert_bucket(void* data, bucket_t* bucket) {
    
    // Create new node, assure successful malloc
    node_t* new = (node_t*)malloc(sizeof(*new));
    assert(new);

    // Assign data
    new->_data = data;

    // Reconnecting nodes, so the most recent one is the head
    new->_next = bucket;
    bucket = new;
    
    return bucket;
}

dict_t* insert(dict_t* dict, void* data) {
    
    // search first, if it's not there, insert into bucket
    void* search = find(dict, data);
    if (!search) {
        int pos = (*dict->_hashfunc)(data);
        dict->_set[pos] = insert_bucket(data, dict->_set[pos]); 
        ++(dict->_size);
    }

    return dict;
} 

static bucket_t* clear_bucket(bucket_t* bucket, void* delete_func(void*)) {
    node_t* tmp = bucket;
    
    // Iterate through tmp node, free along the way
    while (tmp) {
        node_t* done = tmp;
        tmp = tmp->_next;
        done->_data = delete_func(done->_data);
        free(done);
    }
    return NULL;
}

dict_t* delete_dict(dict_t* dict, void* delete_func(void*)) {
    for (int i = 0; i < dict->_num_buckets; ++i)
        dict->_set[i] = clear_bucket(dict->_set[i], delete_func);

    free(dict->_set);
    free(dict);
    return NULL;
}
