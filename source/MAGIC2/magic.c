#include "magic.h"

#include <stdio.h>
#include <stdlib.h>

// End of a key.
#define NULLDigit '\0'

/* 
 * Item inside a node of a TST.
 */
typedef int Item;

/*
 * Node of a Ternary Search Trie (TST).
 */
typedef struct TSTNode_t TSTNode;
struct TSTNode_t{
    char digit;       // Digit (explicit character)
    Item item;        // Item stored in node
    TSTNode* left;    // To key with smaller character
    TSTNode* middle;  // To key with same character
    TSTNode* right;   // To key with bigger character
};

/*
 * Linked list storing all the nodes inside the TST
 * that contain an Item (index in auxiliary array).
*/
typedef struct SLOTS_OCCUPIED_t SLOTS_OCCUPIED;
struct SLOTS_OCCUPIED_t
{
    TSTNode* node;        // Node that contains an Item
    SLOTS_OCCUPIED *next; // Pointer to the next element of the list
};

/* MAGIC structure : Ternary Search Tries (TST)
 * that contains the mapping between the addresses
 * and the index in the auxiliary array.
 */
struct MAGIC
{
    SLOTS_OCCUPIED *slots;  // Pointer to all the nodes in the TST that contain an Item
    int maxSize;            // Maximum number of addresses
    int addrSize;           // Size of one address (in bytes)
    int nbElements;         // Current number of addresses being stored
    TSTNode* root;          // Root of the TST
};

TSTNode* tst_create_node(char digit, Item item);

Item tst_search(MAGIC m, TSTNode* node, char* addr, size_t index);

TSTNode* tst_insert(MAGIC m, TSTNode* node, char* addr, size_t keyIndex);

MAGIC MAGICinit(int maxSize, int addrSize){
    MAGIC m = malloc(sizeof(*m));
    if(!m)
        return NULL;
    
    m->slots = NULL;
    m->maxSize = maxSize;
    m->addrSize = addrSize;
    m->nbElements = 0;
    m->root = NULL;

    return m;
}

int MAGICindex(MAGIC m, char* dest){
    if(!m || !dest)
        return -1;
    TSTNode* n = tst_insert(m, m->root, dest, 0);
    if(n == NULL){
        fprintf(stderr, "Error while inserting\n");
        return -1;
    }
    m->root = n;

    Item item = tst_search(m, m->root, dest, 0);
    if(item == -1){
        fprintf(stderr, "Error while searching\n");
    }
    return item;
}

void MAGICreset(MAGIC m){
    return;
}

/**********************************************
 *                                            *
 *   IMPLEMENTATION OF TERNARY SEARCH TRIE    *
 *                                            *
 **********************************************/

TSTNode* tst_create_node(char digit, Item item){
    TSTNode* node = malloc(sizeof(TSTNode));
    if(!node)
        return NULL;

    node->digit = digit;
    node->item = item;
    node->left = NULL;
    node->middle = NULL;
    node->right = NULL;

    return node;
}

Item tst_search(MAGIC m, TSTNode* node, char* addr, size_t index){
    if(!m)
        return -1;

    if(node->digit == NULLDigit && index >= (size_t)m->addrSize)
        return node->item;
    if(index >= (size_t)m->addrSize)
        return -1;

    if(addr[index] < node->digit)
        return tst_search(m, node->left, addr, index);
    if(addr[index] == node->digit)
        return tst_search(m, node->middle, addr, index + 1);
    if(addr[index] > node->digit)
        return tst_search(m, node->right, addr, index);
}

TSTNode* tst_insert(MAGIC m, TSTNode* node, char* addr, size_t keyIndex){
    if(!m)
        return NULL;

    if(node == NULL){
        if(keyIndex >= (size_t)m->addrSize){
            node = tst_create_node(NULLDigit, m->nbElements);
            // TBA: Add node to SLOTS_OCCUPIED
            m->nbElements++;
            return node;
        }else
            node = tst_create_node(addr[keyIndex], 0);
    }
    
    if(addr[keyIndex] < node->digit)
        node->left = tst_insert(m, node->left, addr, keyIndex);
    if(addr[keyIndex] == node->digit)
        node->middle = tst_insert(m, node->middle, addr, keyIndex+1);
    if(addr[keyIndex] > node->digit)
        node->right = tst_insert(m, node->right, addr, keyIndex);

    return node;
}
