#include "magic.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct MAGIC_ELEMENT_t MAGIC_ELEMENT;
struct MAGIC_ELEMENT_t
{
    char *address;
    int index;
    MAGIC_ELEMENT *next;
};

typedef struct SLOTS_OCCUPIED_t SLOTS_OCCUPIED;
struct SLOTS_OCCUPIED_t
{
    unsigned long slot;
    SLOTS_OCCUPIED *next;
};

struct MAGIC
{
    SLOTS_OCCUPIED *slots;
    int maxSize;
    int addrSize;
    int nbElements;
    MAGIC_ELEMENT **elements;
};

static unsigned long hash(const char *address, const int maxSize);

static bool sameAdresses(const char *address1, const char *address2,
                         const size_t addrSize);

static void MAGICSetValue(MAGIC m, const char *address, const int index);

static MAGIC_ELEMENT *MAGICAddPair(const char *address, const int index, 
                                   MAGIC m);

static int MAGICGetValue(MAGIC m, const char *address);

static void MAGICAppendOccupiedSlot(MAGIC m, unsigned long slot);

static void MAGICfree(MAGIC m);

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

static MAGIC_ELEMENT *MAGICAddPair(const char *address, const int index, 
                                   MAGIC m)
{
    MAGIC_ELEMENT *me = malloc(sizeof(MAGIC_ELEMENT));

    me->address = malloc(sizeof(char) * m->addrSize);

    for(size_t i = 0; i < m->addrSize; ++i)
        me->address[i] = address[i];

    me->index = index;
    me->next = NULL;

    ++m->nbElements;

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

static void MAGICAppendOccupiedSlot(MAGIC m, unsigned long slot)
{
    SLOTS_OCCUPIED *tmp = malloc(sizeof(SLOTS_OCCUPIED));
    if(!tmp)
        return;

    tmp->slot = slot;
    tmp->next = m->slots;

    m->slots = tmp;

    return;
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
        if(sameAdresses(address, current->address, m->addrSize)) 
            return;

        prev = current;
        current = prev->next;
    }

    // No match while being at the end of the list
    prev->next = MAGICAddPair(address, index, m);

    return;
}

void MAGICfree(MAGIC m)
{
    if(m)
    {
        if(m->elements)
        {
            for(size_t i = 0; i < m->maxSize; ++i)
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

static unsigned long hash(const char *address, const int maxSize)
{
    unsigned long hash = 5381;
    char c;

    while(c = *address++)
        hash = ((hash << 5) + hash) + c;

    return hash % maxSize;
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

    for(size_t i = 0; i < maxSize; ++i)
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
