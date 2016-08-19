//
// Created by marcin on 16.08.16.
//


#include <stdlib.h>
#include "ms_response_list.h"

list_operation_result ms_response_list_create(ms_response_list_node* head)
{
    head = malloc(sizeof(ms_response_list_node));
    if(head == NULL)
    {
        return OPERATION_LIST_ERROR;
    }
    return OPERATION_LIST_OK;
}

void push(ms_response_list_node * head, response_t val) {
    ms_response_list_node * current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = malloc(sizeof(ms_response_list_node));
    current->next->val = val;
    current->next->next = NULL;
}

list_operation_result string_list_create(string_list_node* head)
{
    head = malloc(sizeof(char));
    if(head == NULL)
    {
        return OPERATION_LIST_ERROR;
    }
    return OPERATION_LIST_OK;
}

void push_string(string_list_node * head, char* val) {
    string_list_node * current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = malloc(sizeof(string_list_node));
    current->next->val = val;
    current->next->next = NULL;
}

void print_list(string_list_node * head)
{
    string_list_node * current = head;
    while (current->next != NULL) {
        printf(current->val);
        current = current->next;
    }
}