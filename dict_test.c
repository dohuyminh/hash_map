#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "dict.h"

#define NUM_BUCKETS 26

int hash(void* n) {
    return abs(*(int*)n) % 26;
}

int cmp(void* n1, void* n2) {
    return (*(int*)n1 - *(int*)n2);
}

void* destroy(void* n) {
    free((int*)n);
    return NULL;
}

void print_bucket(bucket_t* bucket) {
    if (!bucket) {
        printf("NULL\n");
        return;
    }
    
    node_t* tmp = bucket;
    while (tmp) {
        printf("%d -> ", *(int*)(tmp->_data));
        tmp = tmp->_next;
    }
    printf("NULL\n");
}

void print_set(dict_t* dict) {
    printf("The set currently looks like this:\n");
    for (int i = 0; i < dict->_num_buckets; ++i)
        print_bucket(dict->_set[i]);
    printf("-------------------------------------------\n");
    printf("Size: %ld\n", dict->_size);
}

void print_menu() {
    printf("\nOptions:\n");
    printf("\t1: Insert integer into HashSet.\n");
    printf("\t2: Find element in HashSet.\n");
    printf("\t3 or else: Delete HashSet and exit.\n");
}

void test() {
    printf("Welcome to the HashSet test:\n");
    
    dict_t* dict = new_dict(NUM_BUCKETS, hash, cmp);
    printf("HashSet created successfully.\n");
    print_menu();

    int option;
    while (scanf("%d", &option) == 1 && option != 3) {
        if (option == 1) {
            printf("What number do you want to insert? ");
            int* num = (int*)malloc(sizeof(int));
            if (scanf("%d", num) != 1) {
                printf("Invalid input.\n");
                num = (int*)destroy(num);
                print_menu();
                break;
            }

            if (find(dict, num)) {
                printf("Number already existed in set.\n");
                num = (int*)destroy(num);
                print_menu();
                break;
            }
            dict = insert(dict, num);
            printf("Successfully inserted %d into set.\n", *num);
            print_set(dict);  
            print_menu();
        }

        else if (option == 2) {
            printf("What number do you want to search? ");
            int* num = (int*)malloc(sizeof(int));
            if (scanf("%d", num) != 1) {
                printf("Invalid input.\n");
                num = (int*)destroy(num);
                print_menu();
                break;
            }
            if (find(dict, num))
                printf("%d exists in set.\n", *num);
            else printf("%d does not exist in set.\n", *num);
            print_menu();
        }
        
        else 
            break;
    }

    dict = delete_dict(dict, destroy);
    printf("Successfully deleted set.\n");

    return;
}

int main(int argc, char** argv) {

    test();

    return 0;
}