#ifndef RBTREE_H_
#define RBTREE_H_

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

#include "figure.h"

typedef struct {
    int p_id;
    int p_sock;
    int p_figure;
    int p_debt;
    struct in_addr p_ip_addr;
} Player;

typedef enum { RED, BLACK } color_t;

typedef struct Node {
    Figure figura;
    Player player;
    color_t color;
    struct Node *parent;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct {
    Node *root;
    Node *nil;
} RBT;

RBT *get_rbt();
void allocate_rbt();
/* выделяет память под корень и nil*/
RBT *new_rbtree();
/* очистка памяти дерева */
void delete_rbtree(RBT *t);
/* очистка памяти нод */
void rb_postorder_delete(Node *root, RBT *t);

/* повороты дерева */
void left_rotate(RBT *t, Node *x);
void right_rotate(RBT *t, Node *x);

/* добавление фигуры в дерево */
Node *rbtree_insert(RBT *t, Figure figura);
/* добавление фигуры в дерево */
void add_player_in_figure(Player player, const int id);
/* балансировка после добавления */
Node *rb_insert_Fixup(RBT *t, Node *z);
/* поиск фигуры по её индексу фигуры*/
Node *rbtree_find_figure(const RBT *t, const int index);
/* поиск фигуры по её индексу игрока*/
Node *rbtree_find_player(const RBT *t, const int index);

/* обход дерева */
int rb_inorder(Node *root, int *res, const RBT *t, int i, const size_t n);

#endif // RBTREE_H_
