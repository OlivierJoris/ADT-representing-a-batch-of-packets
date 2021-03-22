#include "magic.h"

#include <stdlib.h>

typedef struct{
    char *key;
    size_t index;
}MAGIC_ELEMENT;

struct MAGIC{
    int maxSize;
    int addrSize;
    MAGIC_ELEMENT *hashTable;
};

static void MAGICfree(MAGIC m);

static void MAGICfree(MAGIC m)
{
    if(!m)
        return;
    
    if(!m->hashTable)
    {
        free(m);
        m = NULL;
        return;
    }

    if(!m->hashTable->key)
    {
        free(m->hashTable);
        m->hashTable = NULL;
        return;
    }

    free(m->hashTable->key);
    m->hashTable->key = NULL;
    return;
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

    m->hashTable = malloc(maxSize * sizeof(MAGIC_ELEMENT));
    if(!m->hashTable)
    {
        MAGICfree(m);
        return NULL;
    }

    m->hashTable->key = malloc(addrSize * sizeof(char));
    if(!m->hashTable->key)
    {
        MAGICfree(m);
        return NULL;
    }   
        
    return m;
}

int MAGICindex(MAGIC m, char* dest)
{

}

void MAGICreset(MAGIC m)
{
    
}
