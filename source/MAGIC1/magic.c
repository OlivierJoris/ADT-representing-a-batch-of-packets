/*
 * Module implementing the magic interface using a hash table.
 * 
 * @author Maxime Goffart (180521) & Olivier Joris (182113)
 */

#include "magic.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/* 
 * Implementation of the MAGIC_ELEMENT structure: one element of 
 * the hash table.
 */
typedef struct MAGIC_ELEMENT_t MAGIC_ELEMENT;
struct MAGIC_ELEMENT_t{
    char *address;       /* Stored address */
    int index;           /* Stored index */
    MAGIC_ELEMENT *next; /* Pointer to next element to handle collisions 
                            (chaining) */
};

/*
 * Implementation of the SLOTS_OCCUPIED structure: Linked list storing 
 * all the slots that are actually occupied in the hash table.
 */
typedef struct SLOTS_OCCUPIED_t SLOTS_OCCUPIED;
struct SLOTS_OCCUPIED_t{
    unsigned long slot;   /* Index of the used slot */
    SLOTS_OCCUPIED *next; /* Pointer to the next element of the list */
};

/*
 * Implementation of the MAGIC structure: Hash table matching 
 * addresses and indexes of the auxiliary array.
 */
struct MAGIC{
    SLOTS_OCCUPIED *slots;    /* Pointer to all the 
                                 actually occupied slots of the hash table */
    int maxSize;              /* Size of the hash table */
    int addrSize;             /* Size of one address (in bytes) stored in the hash table */
    int nbElements;           /* Current number of elements stored in the hash 
                                 table */
    MAGIC_ELEMENT **elements; /* Hash table storing matches of indexes and 
                                 addresses */
};

/*
 * Hash function of the hash table storing matches between addresses (key)
 * and indexes.
 * This hash function has been taken from
 * https://theartincode.stanis.me/008-djb2/#:~:text=The%20simple%20C
 * %20function%20starts,the%20current%20character%20to%20it.
 * 
 * @param address Address to hash (key).
 * @param maxSize Size of the hash table.
 * @param addrSize Size of the address.
 * 
 * @return The hash of the given address.
 */
static unsigned long hash(const char *address, const int maxSize,
                            const int addrSize);

/*
 * Checks if two addresses are equals.
 * 
 * @param address1 First address.
 * @param address2 Second address.
 * @param addrSize Size of the addresses we are storing.
 * 
 * @return True if the addresses are equal.
 *         False otherwise.
 */
static bool same_adresses(const char *address1, const char *address2,
                          const size_t addrSize);

/*
 * Sets a couple address/index in the m structure.
 * 
 * @param m The magic structure
 * @param address The address.
 * @param index The index.
 */
static void ht_set_value(MAGIC m, const char *address, const int index);

/*
 * Creates a pointer to MAGIC_ELEMENT with a given address and index.
 * 
 * @param address The address.
 * @param index The index.
 * @param m The magic structure
 * 
 * @return A pointer to the newly created element.
 */
static MAGIC_ELEMENT *ht_add_pair(const char *address, const int index, 
                                  MAGIC m);

/*
 * Gets the index of a given address. If no match, adds this address with the
 * next available element.
 * 
 * @param m The magic structure
 * @param address The address.
 * 
 * @return The index matching the given address.
 */
static int ht_get_value(MAGIC m, const char *address);

/*
 * Appends to the list storing the occupied slot a new given one.
 * 
 * @param m The magic structure
 * @param slot The slot.
 */
static void ht_append_occupied_slot(MAGIC m, const unsigned long slot);

/*
 * Frees the memory allocated by the structure m.
 * 
 * @param m The magic structure
 */
static void ht_free(MAGIC m);

static unsigned long hash(const char *address, const int maxSize,
    const int addrSize){
    if(!address)
        return 0;

    unsigned long hash = 5381;
    int i = 0;
    char c;

    while(i < addrSize){
        c = address[i];
        hash = ((hash << 5) + hash) + c;
        i++;
    }
    return hash % maxSize;
}

static bool same_adresses(const char *address1, const char *address2, 
                          const size_t addrSize){
    if(!address1 || !address2)
        return false;

    for(size_t i = 0; i < addrSize; ++i){
        if(address1[i] != address2[i])
            return false;
    }

    return true;
}

static void ht_set_value(MAGIC m, const char *address, const int index){
    if(!m || !address){
        fprintf(stderr, "ht_set_value: invalid parameters!\n");
        exit(1);
    }

    unsigned long hashValue = hash(address, m->maxSize, m->addrSize);

    MAGIC_ELEMENT *current = m->elements[hashValue];

    // If element not already set in the hash table
    if(!current){
        m->elements[hashValue] = ht_add_pair(address, index, m);
        if(!m->elements[hashValue]){
            fprintf(stderr, "ht_set_value: memory allocation issue!\n");
            ht_free(m);
            exit(1);
        }
        ht_append_occupied_slot(m, hashValue);
        return;
    }

    // If collision, chaining
    MAGIC_ELEMENT *prev;

    while(current){
        prev = current;
        current = prev->next;
    }

    // End of the list
    prev->next = ht_add_pair(address, index, m);
    if(!prev->next){
        fprintf(stderr, "ht_set_value: memory allocation issue!\n");
        ht_free(m);
        exit(1);
    }

    return;
}

static MAGIC_ELEMENT *ht_add_pair(const char *address, const int index, 
                                   MAGIC m){
    if(!m || !address)
        return NULL;

    MAGIC_ELEMENT *me = malloc(sizeof(MAGIC_ELEMENT));
    if(!me)
        return NULL;

    me->address = malloc(sizeof(char) * m->addrSize);
    if(!me->address){
        free(me);
        return NULL;
    }

    for(size_t i = 0; i < (unsigned long) m->addrSize; ++i)
        me->address[i] = address[i];

    me->index = index;
    me->next = NULL;

    ++m->nbElements;

    return me;
}

static int ht_get_value(MAGIC m, const char *address){
    if(!m || !address){
        fprintf(stderr, "MAGICGetValue: invalid parameters!\n");
        exit(1);
    }

    unsigned long hashValue = hash(address, m->maxSize, m->addrSize);

    MAGIC_ELEMENT *current = m->elements[hashValue];

    while(current){ 
        if(same_adresses(current->address, address, m->addrSize))
            return current->index;

        current = current->next;
    }

    ht_set_value(m, address, m->nbElements);

    return m->nbElements - 1;
}

static void ht_append_occupied_slot(MAGIC m, const unsigned long slot){
    if(!m)
        return;

    SLOTS_OCCUPIED *tmp = malloc(sizeof(SLOTS_OCCUPIED));
    if(!tmp)
        return;

    tmp->slot = slot;
    tmp->next = m->slots;

    m->slots = tmp;

    return;
}

void ht_free(MAGIC m){
    if(m){
        if(m->elements){
            for(size_t i = 0; i < (unsigned long) m->maxSize; ++i){
                if(m->elements[i]){  
                    MAGIC_ELEMENT *current = m->elements[i];

                    MAGIC_ELEMENT *prev;

                    while(current){
                        prev = current;
                        current = prev->next;

                        if(prev->address){
                            free(prev->address);
                            prev->address = NULL;
                        }

                        free(prev);
                        prev = NULL;
                    }
                }
            }

            free(m->elements);
            m->elements = NULL;
        }

        while(m->slots){
            SLOTS_OCCUPIED *tmp = m->slots;
            m->slots = m->slots->next;

            if(tmp){
                free(tmp);
                tmp = NULL;
            }
        }

        free(m);
        m = NULL;
    }
     
    return;
}

MAGIC MAGICinit(int maxSize, int addrSize){
    MAGIC m;

    m = malloc(sizeof(*m));

    if(!m)
        return NULL;
    
    if(maxSize <= 0 || addrSize <= 0){
        ht_free(m);
        return NULL;
    }

    m->maxSize = maxSize;
    m->addrSize = addrSize;
    m->nbElements = 0;
    m->slots = NULL;

    m->elements = malloc(sizeof(MAGIC_ELEMENT *) * maxSize);
    if(!m->elements){
        ht_free(m);
        return NULL;
    }

    for(size_t i = 0; i < (unsigned long) maxSize; ++i)
        m->elements[i] = NULL;
        
    return m;
}

int MAGICindex(MAGIC m, char *dest){
    if(!m || !dest){
        fprintf(stderr, "MAGICindex: invalid parameters!\n");
        exit(1);
    }
    return ht_get_value(m, dest);
}

void MAGICreset(MAGIC m){
    if(!m)
        return;

    while(m->slots){
        MAGIC_ELEMENT *current = m->elements[m->slots->slot];

        MAGIC_ELEMENT *prev;

        while(current){
            prev = current;
            current = prev->next;

            if(prev->address){
                free(prev->address);
                prev->address = NULL;
            }

            free(prev);
            prev = NULL;
        }

        m->elements[m->slots->slot] = NULL;
        
        SLOTS_OCCUPIED *tmp = m->slots;
        m->slots = m->slots->next;

        if(tmp){
            free(tmp);

            tmp = NULL;
        }
    }

    m->nbElements = 0;

    return;
}
