#include "../header/rbtree.h"

static RBT *rbt;

RBT *get_rbt() { return rbt; }

RBT *new_rbtree()
{
    RBT *p = (RBT *)calloc(1, sizeof(RBT));
    Node *NIL = (Node *)calloc(1, sizeof(Node));
    p->nil = NIL;
    p->root = NIL;
    NIL->color = BLACK;
    return p;
}

void allocate_rbt() { rbt = new_rbtree(); }

void rb_postorder_delete(Node *root, RBT *t)
{
    if (root != t->nil) {
        rb_postorder_delete(root->left, t);
        rb_postorder_delete(root->right, t);
        free(root->figura.point);
        free(root);
    }
}

void delete_rbtree(RBT *t)
{
    if (t != NULL) {
        rb_postorder_delete(t->root, t);
        free(t->nil);
        free(t);
    }
}

void left_rotate(RBT *t, Node *x)
{
    Node *y;
    y = x->right;
    x->right = y->left;
    if (y->left != t->nil) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == t->nil) {
        t->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
    return;
}

void right_rotate(RBT *t, Node *x)
{
    Node *y;
    y = x->left;
    x->left = y->right;
    if (y->right != t->nil) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == t->nil) {
        t->root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
    return;
}

Node *rb_insert_Fixup(RBT *t, Node *z)
{
    Node *uncle;
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            uncle = z->parent->parent->right;
            if (uncle->color == RED) {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotate(t, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(t, z->parent->parent);
            }
        } else {
            uncle = z->parent->parent->left;
            if (uncle->color == RED) {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(t, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(t, z->parent->parent);
            }
        }
    }
    t->root->color = BLACK;
    return t->root;
}

Node *rbtree_insert(RBT *t, Figure figura)
{
    Node *parent = t->nil;
    Node *p = t->root;

    while (p != t->nil) {
        parent = p;
        if (figura.index_figure < p->figura.index_figure) {
            p = p->left;
        } else {
            p = p->right;
        }
    }
    Node *new_node = (Node *)calloc(1, sizeof(Node));
    new_node->figura.point =
        (Figure_point *)calloc(figura.count_point, sizeof(Figure_point));
    copy_figure(&figura, &new_node->figura);
    new_node->parent = parent;

    if (parent == t->nil) {
        t->root = new_node;
        new_node->color = BLACK;
    } else if (figura.index_figure < parent->figura.index_figure) {
        parent->left = new_node;
        new_node->color = RED;
    } else {
        parent->right = new_node;
        new_node->color = RED;
    }
    new_node->figura.index_figure = figura.index_figure;
    new_node->left = t->nil;
    new_node->right = t->nil;

    rb_insert_Fixup(t, new_node);
    return t->root;
}

void add_player_in_figure(Player player, const int id)
{
    Node *fig = rbtree_find_figure(get_rbt(), id);
    fig->player.p_debt = player.p_debt;
    fig->player.p_figure = player.p_figure;
    fig->player.p_id = player.p_id;
    fig->player.p_ip_addr = player.p_ip_addr;
    fig->player.p_sock = player.p_sock;
}

Node *rbtree_find_figure(const RBT *t, const int index)
{
    Node *r = t->root;
    while (r != t->nil) {
        if (r->figura.index_figure == index) {
            return r;
        } else if (r->figura.index_figure < index) {
            r = r->right;
        } else {
            r = r->left;
        }
    }
    return NULL;
}

Node *rbtree_find_player(const RBT *t, const int index)
{
    Node *r = t->root;
    while (r != t->nil) {
        if (r->player.p_id == index) {
            return r;
        } else if (r->player.p_id < index) {
            r = r->right;
        } else {
            r = r->left;
        }
    }
    return NULL;
}

int rb_inorder(Node *root, int *res, const RBT *t, int i, const size_t n)
{
    if (root == t->nil || i >= (int)n) {
        return i;
    }

    i = rb_inorder(root->left, res, t, i, n);
    res[i] = root->figura.index_figure;
    i += 1;
    i = rb_inorder(root->right, res, t, i, n);
    return i;
}
