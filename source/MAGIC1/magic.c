#include "magic.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct MAGIC_ELEMENT_t MAGIC_ELEMENT;

struct MAGIC_ELEMENT_t{
    char *address;
    int index;
    MAGIC_ELEMENT *next;
};

struct MAGIC{
    int maxSize;
    int addrSize;
    int currentSize;
    MAGIC_ELEMENT **elements;
};

static void MAGICfree(MAGIC m);

static unsigned int hash(const char *address, const int maxSize);

static bool sameAdresses(const char *address1, const char *address2,
                         const size_t addrSize);

static void MAGICSetValue(MAGIC m, const char *address, const int index);

static MAGIC_ELEMENT *MAGICAddPair(const char *address, const int index, 
                                   const size_t addrSize);

static int MAGICGetValue(MAGIC m, const char *address);

static int MAGICGetValue(MAGIC m, const char *address) 
{
    unsigned int i = hash(address, m->maxSize);

    MAGIC_ELEMENT *current = m->elements[i];

    while(current != NULL) 
    {
        if(sameAdresses(current->address, address, m->addrSize))
            return current->index;

        current = current->next;
    }

    MAGICSetValue(m, address, m->currentSize);
    ++m->currentSize;

    return m->currentSize - 1;
}

static MAGIC_ELEMENT *MAGICAddPair(const char *address, const int index, 
                                   const size_t addrSize)
{
    MAGIC_ELEMENT *me = malloc(sizeof(MAGIC_ELEMENT));

    me->address = malloc(sizeof(char) * addrSize);

    for(size_t i = 0; i < addrSize; ++i)
        me->address[i] = address[i];

    me->index = index;
    me->next = NULL;

    return me;
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
    unsigned int i = hash(address, m->maxSize);

    MAGIC_ELEMENT *current = m->elements[i];

    if(current == NULL) 
    {
        m->elements[i] = MAGICAddPair(address, index, m->addrSize);
        return;
    }

    MAGIC_ELEMENT *prev;

    while(current != NULL) 
    {
        if(sameAdresses(address, current->address, m->addrSize)) 
            return;

        prev = current;
        current = prev->next;
    }

    // No match while being at the end of the list
    prev->next = MAGICAddPair(address, index, m->addrSize);

    return;
}

static void MAGICfree(MAGIC m)
{
    if(!m)
        return;
    
    if(!m->elements)
    {
        free(m);
        m = NULL;
        return;
    }

    for(size_t i = 0; i < m->maxSize; ++i)
    {
        if(m->elements[i])
        {   
            if(m->elements[i]->address)
            {
                free(m->elements[i]->address);
                m->elements[i]->address = NULL;
            }
            
            free(m->elements[i]);
            m->elements[i] = NULL;
        }
    }

    free(m->elements);
    m->elements = NULL;

    return;
}

static unsigned int hash(const char *address, const int maxSize)
{
    unsigned int hash = 5381;
    unsigned int c;

    while (c = *address++)
        hash = ((hash << 5) + hash) + c;

    return hash % maxSize;
}

MAGIC MAGICinit(int maxSize, int addrSize)
{
    MAGIC m = malloc(sizeof(MAGIC));

    if(!m)
        return NULL;
    
    if(maxSize <= 0 || addrSize <= 0)
    {
        MAGICfree(m);
        return NULL;
    }

    m->maxSize = maxSize;
    m->addrSize = addrSize;
    m->currentSize = 0;

    m->elements = malloc(sizeof(MAGIC_ELEMENT *) * maxSize);
    if(!m->elements)
    {
        MAGICfree(m);
        return NULL;
    }

    for(size_t i = 0; i < maxSize; ++i)
        m->elements[i] = NULL;
        
    return m;
}

int MAGICindex(MAGIC m, char *dest)
{
    return MAGICGetValue(m, dest);
}

// void MAGICreset(MAGIC m)
// {
    
// }
