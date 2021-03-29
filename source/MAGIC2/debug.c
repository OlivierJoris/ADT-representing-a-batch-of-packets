/**********************************************************
 * 
 * TEMPORARY -- debug file -- should NOT be submited
 *
 **********************************************************/

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
typedef struct OCCUPIED_NODE_t OCCUPIED_NODE;
struct OCCUPIED_NODE_t
{
    TSTNode* node;        // Node that contains an Item
    OCCUPIED_NODE *next; // Pointer to the next element of the list
};

/* MAGIC structure : Ternary Search Tries (TST)
 * that contains the mapping between the addresses
 * and the index in the auxiliary array.
 */
struct MAGIC
{
    OCCUPIED_NODE* occupied_beg;   // Beginning of list all the nodes in the TST that contain an Item
    OCCUPIED_NODE* occupied_end;   // End of list all the nodes in the TST that contain an Item
    int maxSize;                    // Maximum number of addresses
    int addrSize;                   // Size of one address (in bytes)
    int nbElements;                 // Current number of addresses being stored
    TSTNode* root;                  // Root of the TST
};

/*
 * Creates a node of a Ternary Search Trie (TST).
 *
 * @param digit Digit (explicit character) of the node.
 * @param item Item stored in the node.
 *
 * @return Pointer to the new node.
 */
static TSTNode* tst_create_node(char digit, Item item);

/*
 * Searches the item (index in auxiliary array) associated
 * with the given address (addr) inside the TST.
 *
 * @param m MAGIC structure.
 * @param node Current node inside the TST.
 * @param addr Address for which we want the associated index.
 * @param addrIndex Current index in the array of addr.
 *
 * @return Index in auxiliary array associated to addr. 
 */
static Item tst_search(MAGIC m, TSTNode* node, char* addr, size_t addrIndex);

/*
 * Inserts the address addr inside the TST.
 *
 * @param m MAGIC structure.
 * @param node Current node inside the TST.
 * @param addr Address that we want to add inside the TST.
 * @param keyIndex Current index in the array of addr.
 *
 * @return Node.
 */
static TSTNode* tst_insert(MAGIC m, TSTNode* node, char* addr, size_t keyIndex);

/*
 * Adds a node to the list of occupied nodes inside the TST.
 *
 * @param m MAGIC structure.
 * @param node Node to add in the list of occupied nodes.
 */
static void occupied_add_node(MAGIC m, TSTNode* node);

MAGIC MAGICinit(int maxSize, int addrSize){
    MAGIC m = malloc(sizeof(*m));
    if(!m)
        return NULL;
    
    m->occupied_beg = NULL;
    m->occupied_end = NULL;
    m->maxSize = maxSize;
    m->addrSize = addrSize;
    m->nbElements = 0;
    m->root = NULL;

    return m;
}

int MAGICindex(MAGIC m, char* dest){
    if(!m || !dest)
        return -1;

    // First, we check if the address is already in the TST
    Item item = tst_search(m, m->root, dest, 0);
    if(item >= 0)
        return item;
    else{
        // If not, we add the address inside the TST
        TSTNode* n = tst_insert(m, m->root, dest, 0);
        if(n == NULL){
            fprintf(stderr, "Error while inserting\n");
            return -1;
        }
        m->root = n;
        return m->nbElements-1;
    }
}

void MAGICreset(MAGIC m){
    if(!m || !m->occupied_beg)
        return;

    // Goes through the entire list, updates the item field,
    // and frees the memory
    OCCUPIED_NODE* s = m->occupied_beg;
    OCCUPIED_NODE* tmp = m->occupied_beg;
    while(s){
        if(s->node)
            s->node->item = -1;
        tmp = s->next;
        free(s);
        s = tmp;
    }
    m->occupied_beg = NULL;
    m->occupied_end = NULL;

    // Resets the number of items in the TST
    m->nbElements = 0;

    return;
}

static void occupied_add_node(MAGIC m, TSTNode* node){
    if(!m || !node)
        return;
    
    // If the list is empty
    if(!m->occupied_beg){
        m->occupied_beg = malloc(sizeof(OCCUPIED_NODE));
        if(!m->occupied_beg)
            return;
        m->occupied_end = m->occupied_beg;
        m->occupied_beg->node = node;
        m->occupied_beg->next = NULL;
    }else{
        // The list already contains some nodes
        m->occupied_end->next = malloc(sizeof(OCCUPIED_NODE));
        if(!m->occupied_end->next)
            return;
        m->occupied_end = m->occupied_end->next;
        m->occupied_end->node = node;
        m->occupied_end->next = NULL;
    }

    return;
}

/**********************************************
 *                                            *
 *   IMPLEMENTATION OF TERNARY SEARCH TRIE    *
 *                                            *
 **********************************************/

static TSTNode* tst_create_node(char digit, Item item){
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

static Item tst_search(MAGIC m, TSTNode* node, char* addr, size_t addrIndex){
    if(!m || !node)
        return -1;

    if(node->digit == NULLDigit && addrIndex >= (size_t)m->addrSize && node->item != -1)
        return node->item;
    if(addrIndex >= (size_t)m->addrSize)
        return -1;

    if(addr[addrIndex] < node->digit)
        return tst_search(m, node->left, addr, addrIndex);
    else
        if(addr[addrIndex] == node->digit)
            return tst_search(m, node->middle, addr, addrIndex + 1);
        else
            return tst_search(m, node->right, addr, addrIndex);
}

static TSTNode* tst_insert(MAGIC m, TSTNode* node, char* addr, size_t keyIndex){
    if(!m)
        return NULL;

    fprintf(stderr, "keyIndex = %lu || ", keyIndex);

    /*
     * If the node was already allocated by another batch,
     * we just update the item field
     */
    if(node && node->digit == NULLDigit && keyIndex == (size_t)m->addrSize){
        node->item = m->nbElements;
        occupied_add_node(m, node);
        m->nbElements++;
        return node;
    }

    if(node == NULL){
        if(keyIndex >= (size_t)m->addrSize){
            fprintf(stderr, "keyIndex >= (size_t)m->addrSize || m->nbElements = %d\n", m->nbElements);
            node = tst_create_node(NULLDigit, m->nbElements);
            occupied_add_node(m, node);
            m->nbElements++;
            return node;
        }else{
            fprintf(stderr, "else keyIndex >= (size_t)m->addrSize || addr[keyIndex] : %d || ", addr[keyIndex]);
            node = tst_create_node(addr[keyIndex], -1);
        }       
    }
    
    if(addr[keyIndex] < node->digit){
        fprintf(stderr, "%d < %d\n", addr[keyIndex], node->digit);
        node->left = tst_insert(m, node->left, addr, keyIndex);
    }
    if(addr[keyIndex] == node->digit){
        fprintf(stderr, "%d == %d\n", addr[keyIndex], node->digit);
        node->middle = tst_insert(m, node->middle, addr, keyIndex+1);
    }
    if(addr[keyIndex] > node->digit){
        fprintf(stderr, "%d > %d\n", addr[keyIndex], node->digit);
        node->right = tst_insert(m, node->right, addr, keyIndex);
    }

    return node;
}
