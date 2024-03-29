#include "magic.h"

#include <stdio.h>
#include <stdlib.h>

#define ADDRESS_SIZE 4
#define NB_ADDRESSES 6

static void printAddresses(char *address);

static void printIndexes(int index);

static void printAddresses(char *address)
{
    printf("Address : ");

    for(size_t i = 0; i < ADDRESS_SIZE; ++i)
    {
        printf("%d ",  address[i]);
    }

    printf("\n");

    return;
}

static void printIndexes(int index)
{
    printf("Index : %d\n", index);

    printf("\n");

    return;
}

int main()
{
    MAGIC m = MAGICinit(NB_ADDRESSES, ADDRESS_SIZE);

    if(!m)
        return -1;

    char addresses[NB_ADDRESSES][ADDRESS_SIZE] = 
    {
        {10, 1, 0, 1},
        {10, 1, 0, 2},
        {10, 1, 0, 3},
        {10, 1, 0, 2},
        {10, 1, 0, 1},
        {10, 1, 0, 1},
    };

    for(size_t i = 0; i < NB_ADDRESSES; ++i)
    {
        printAddresses(addresses[i]);
        printIndexes(MAGICindex(m, addresses[i]));
    }

    printf("** Reset **\n\n");

    MAGICreset(m);

    addresses[0][3] = 4;

    for(size_t i = 0; i < NB_ADDRESSES; ++i)
    {
        printAddresses(addresses[i]);
        printIndexes(MAGICindex(m, addresses[i]));
    }

    printf("** Reset **\n\n");

    MAGICreset(m);

    addresses[0][3] = 5;
    addresses[2][3] = 5;
    addresses[5][3] = 10;

    for(size_t i = 0; i < NB_ADDRESSES; ++i)
    {
        printAddresses(addresses[i]);
        printIndexes(MAGICindex(m, addresses[i]));
    }

    return 0;
}
