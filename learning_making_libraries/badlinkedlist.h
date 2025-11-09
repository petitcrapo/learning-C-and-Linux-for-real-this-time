#ifndef BADLINKEDLIST_H
#define BADLINKEDLIST_H

typedef struct node node_s;
typedef node_s *node_p ;

node_p create_list(void);
node_p last_node(node_p);
void push(node_p *listpointer, int data);
int pop(node_p *listpointer);

#endif
