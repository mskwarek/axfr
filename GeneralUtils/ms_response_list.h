//
// Created by marcin on 16.08.16.
//

#ifndef MYDIG_MS_RESPONSE_LIST_H
#define MYDIG_MS_RESPONSE_LIST_H

#include <dig_parser.h>

typedef struct list_node
{
    response_t val;
    struct list_node* next;
} ms_response_list_node;

typedef struct string_node
{
    char* val;
    struct string_node* next;
} string_list_node;

typedef enum
{
    OPERATION_LIST_ERROR,
    OPERATION_LIST_OK
} list_operation_result;

list_operation_result string_list_create(string_list_node* head);
void push_string(string_list_node * head, char* val);
void print_list(string_list_node * head);


#endif //MYDIG_MS_RESPONSE_LIST_H
