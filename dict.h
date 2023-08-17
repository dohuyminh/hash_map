#ifndef DICT_H_
#define DICT_H_

#include <stdlib.h>

typedef struct node node_t;

struct node {
    void* _data;
    node_t* _next;
};

typedef node_t bucket_t;

typedef struct {
    size_t _size, _num_buckets; 
    bucket_t** _set;
    int (*_hashfunc)(void*);
    int (*_cmpfunc)(void*, void*);
} dict_t;

dict_t* new_dict(size_t num_buckets, int hashfunc(void*), int cmpfunc(void*, void*));

dict_t* insert(dict_t* dict, void* data);

void* find(dict_t* dict, void* data);

dict_t* delete_dict(dict_t* dict, void* delete_func(void*));

#endif