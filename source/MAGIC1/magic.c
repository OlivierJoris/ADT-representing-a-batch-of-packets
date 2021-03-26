#include "magic.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/* Implementation of the MAGIC_ELEMENT structure : one element of 
   the hash table */
typedef struct MAGIC_ELEMENT_t MAGIC_ELEMENT;
struct MAGIC_ELEMENT_t
{
    char *address;       /* Stored address */
    int index;           /* Stored index */
    MAGIC_ELEMENT *next; /* Pointer to next element to handle collisions 
                            (chaining) */
};

/* Implementation of the SLOTS_OCCUPIED structure : Link list storing 
   all the slot that are actually occupied in the hash table */
typedef struct SLOTS_OCCUPIED_t SLOTS_OCCUPIED;
struct SLOTS_OCCUPIED_t
{
    unsigned long slot;   /* Index of the used slot */
    SLOTS_OCCUPIED *next; /* Pointer to the next element of the list */
};

/* Implementation of the MAGIC structure : Hash table matching 
   addresses and indexes of the auxiliary array */
struct MAGIC
{
    SLOTS_OCCUPIED *slots;    /* Pointer to all the 
                                 actually occupied slots of the hash table */
    int maxSize;              /* Size of the hash table */
    int addrSize;             /* Size of one address stored in the hash table */
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
 * 
 * @return The hash of the given address.
 */
static unsigned long hash(const char *address, const int maxSize);

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
static bool sameAdresses(const char *address1, const char *address2,
                         const size_t addrSize);

/*
 * Sets a couple address/index in the m structure.
 * 
 * @param m The magic structure
 * @param address The address.
 * @param index The index.
 */
static void MAGICSetValue(MAGIC m, const char *address, const int index);

/*
 * Creates a pointer to MAGIC_ELEMENT with a given address and index.
 * 
 * @param address The address.
 * @param index The index.
 * @param m The magic structure
 * 
 * @return A pointer to the newly created element.
 */
static MAGIC_ELEMENT *MAGICAddPair(const char *address, const int index, 
                                   MAGIC m);

/*
 * Gets the index of a given address. If no match, add this address with the
 * next available element.
 * 
 * @param m The magic structure
 * @param address The address.
 * 
 * @return The index matching the given address.
 */
static int MAGICGetValue(MAGIC m, const char *address);

/*
 * Appends to the list storing the occupied slot a new given one.
 * 
 * @param m The magic structure
 * @param slot The slot.
 */
static void MAGICAppendOccupiedSlot(MAGIC m, const unsigned long slot);

/*
 * Frees the memory allocated by the structure m.
 * 
 * @param m The magic structure
 */
static void MAGICfree(MAGIC m);

static unsigned long hash(const char *address, const int maxSize)
{
    unsigned long hash = 5381;
    char c;

    while((c = *address++))
        hash = ((hash << 5) + hash) + c;

    return hash % maxSize;
}

static bool sameAdresses(const char *address1, const char *address2, 
                         const size_t addrSize)
{
    for(size_t i = 0; i < addrSize; ++i)
    {
        if(address1[i] != address2[i])
            return false;
    }

    return true;
}

static void MAGICSetValue(MAGIC m, const char *address, const int index) 
{
    unsigned long i = hash(address, m->maxSize);

    MAGIC_ELEMENT *current = m->elements[i];

    if(current == NULL) 
    {
        m->elements[i] = MAGICAddPair(address, index, m);
        MAGICAppendOccupiedSlot(m, i);
        return;
    }

    MAGIC_ELEMENT *prev;

    while(current) 
    {
        prev = current;
        current = prev->next;
    }

    // End of the list
    prev->next = MAGICAddPair(address, index, m);

    return;
}

static MAGIC_ELEMENT *MAGICAddPair(const char *address, const int index, 
                                   MAGIC m)
{
    MAGIC_ELEMENT *me = malloc(sizeof(MAGIC_ELEMENT));

    me->address = malloc(sizeof(char) * m->addrSize);

    for(size_t i = 0; i < (unsigned long) m->addrSize; ++i)
        me->address[i] = address[i];

    me->index = index;
    me->next = NULL;

    ++m->nbElements;

    return me;
}

static int MAGICGetValue(MAGIC m, const char *address) 
{
    unsigned long i = hash(address, m->maxSize);

    MAGIC_ELEMENT *current = m->elements[i];

    while(current) 
    { 
        if(sameAdresses(current->address, address, m->addrSize))
            return current->index;

        current = current->next;
    }

    MAGICSetValue(m, address, m->nbElements);

    return m->nbElements - 1;
}

static void MAGICAppendOccupiedSlot(MAGIC m, const unsigned long slot)
{
    SLOTS_OCCUPIED *tmp = malloc(sizeof(SLOTS_OCCUPIED));
    if(!tmp)
        return;

    tmp->slot = slot;
    tmp->next = m->slots;

    m->slots = tmp;

    return;
}

void MAGICfree(MAGIC m)
{
    if(m)
    {
        if(m->elements)
        {
            for(size_t i = 0; i < (unsigned long) m->maxSize; ++i)
            {
                if(m->elements[i])
                {  
                    MAGIC_ELEMENT *current = m->elements[i];

                    MAGIC_ELEMENT *prev;

                    while(current)
                    {
                        prev = current;
                        current = prev->next;

                        if(prev->address)
                        {
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

        while(m->slots)
        {
            SLOTS_OCCUPIED *tmp = m->slots;
            m->slots = m->slots->next;

            if(tmp)
            {
                free(tmp);
                tmp = NULL;
            }
        }

        free(m);
        m = NULL;
    }
     
    return;
}

MAGIC MAGICinit(int maxSize, int addrSize)
{
    MAGIC m;

    m = malloc(sizeof(*m));

    if(!m)
        return NULL;
    
    if(maxSize <= 0 || addrSize <= 0)
    {
        MAGICfree(m);
        return NULL;
    }

    m->maxSize = maxSize;
    m->addrSize = addrSize;
    m->nbElements = 0;
    m->slots = NULL;

    m->elements = malloc(sizeof(MAGIC_ELEMENT *) * maxSize);
    if(!m->elements)
    {
        MAGICfree(m);
        return NULL;
    }

    for(size_t i = 0; i < (unsigned long) maxSize; ++i)
        m->elements[i] = NULL;
        
    return m;
}

int MAGICindex(MAGIC m, char *dest)
{
    return MAGICGetValue(m, dest);
}

void MAGICreset(MAGIC m)
{
    while(m->slots)
    {
        MAGIC_ELEMENT *current = m->elements[m->slots->slot];

        MAGIC_ELEMENT *prev;

        while(current)
        {
            prev = current;
            current = prev->next;

            if(prev->address)
            {
                free(prev->address);
                prev->address = NULL;
            }

            free(prev);
            prev = NULL;
        }

        m->elements[m->slots->slot] = NULL;
        
        SLOTS_OCCUPIED *tmp = m->slots;
        m->slots = m->slots->next;

        if(tmp)
        {
            free(tmp);

            tmp = NULL;
        }
    }

    m->nbElements = 0;

    return;
}
