#include <stdlib.h>
#include "badlinkedlist.h"

typedef struct node {
    struct node *prev;
    int data;
    struct node *next;
}node_s;

node_p create_list(void) { return NULL; }

node_p last_node(node_p list) {
    if (list->next != NULL) {
        return last_node(list->next);
    }
    return list;
}

void push(node_p *listptr, int data) {
    if (*listptr == NULL) {
        *listptr = malloc(sizeof(node_s));
        node_p list = *listptr;
        list->prev = NULL;
        list->data = data;
        list->next = NULL;
    } else {
        node_p node = last_node(*listptr);
        node_p next_node = malloc(sizeof(node_s));
        next_node->prev = node;
        next_node->data = data;
        next_node->next = NULL;
        node->next = next_node;
    }
}

int pop(node_p *listptr) {
    if (*listptr == NULL) {
        abort();
        return 0;
    }
    node_p lastnode = last_node(*listptr);
    if (lastnode != *listptr) {
        node_p prevnode = lastnode->prev;
        prevnode->next = NULL;
    } else {
        *listptr = NULL;
    }
    int data = lastnode->data;
    free(lastnode);
    return data;
}
