#include "../include/binary_tree.h"

#include <stdlib.h>

typedef struct container {
    void *node;
    struct container *left;
    struct container *right;
} container;

container* binary_tree_create() {
    return calloc(sizeof(container), 1);
}

void* binary_tree_add(container *c, void *node, int (*node_handler)(void*, void*)) {
    
    if (c->node == NULL) {
        c->node = node;
        return NULL;
    }

    if (node_handler(c->node, node) > 0) {
        if (c->left == NULL) c->left = binary_tree_create();
        return binary_tree_add(c->left, node, node_handler);
    } else {
        if (c->right == NULL) c->right = binary_tree_create();
        return binary_tree_add(c->right, node, node_handler);
    }
}

void* binary_tree_get(container *c, void *key, int (*node_handler)(void*, void*)) {
    
    if (c->node == NULL) return NULL;

    int cmp = node_handler(c->node, key);

    if (cmp == 0) return c->node;
    else if (cmp > 0) return c->left == NULL ? NULL : binary_tree_get(c->left, key, node_handler);
    else return c->right == NULL ? NULL : binary_tree_get(c->right, key, node_handler);
}

void* binary_tree_remove(container *c, void *key, int (*node_handler)(void*, void*)) {
    return NULL;
}

void binary_tree_iterator(container *c, iterator **it) {

    if (c->node == NULL) return;

    if (c->left != NULL) binary_tree_iterator(c->left, it);

    (*it)->node = c->node;
    (*it)->next = calloc(sizeof(iterator), 1);
    *it = (*it)->next;

    if (c->right != NULL) binary_tree_iterator(c->right, it);
}

void* binary_tree_free(container *c, void (*node_free)(void*)) {

    if (node_free != NULL) node_free(c->node);

    if (c->left != NULL) binary_tree_free(c->left, node_free);
    if (c->right != NULL) binary_tree_free(c->right, node_free); 

    free(c);

    return NULL;
}

void* binary_tree_handler(void *container, void *p, int (*node_handler)(void*, void*), int op) {

    switch (op) {
        case CONTAINER_CREATE:
            return binary_tree_create();
        case CONTAINER_ADD:
            return binary_tree_add(container, p, node_handler);
        case CONTAINER_GET:
            return binary_tree_get(container, p, node_handler);
        case CONTAINER_REMOVE:
            return binary_tree_remove(container, p, node_handler);
        case CONTAINER_ITERATOR:;
            iterator *it = calloc(sizeof(iterator), 1), *it_c = it;
            binary_tree_iterator(container, &it_c);
            return it;
        case CONTAINER_FREE:
            return binary_tree_free(container, p);
        default:
            return NULL;
    }
}