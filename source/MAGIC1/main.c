#include "magic.h"

#include <stdio.h>
#include <stdlib.h>

#define ADDRESS_SIZE 4
#define NB_ADDRESSES 6

void printAddresses(unsigned char *address)
{
    printf("Address : ");

    for(size_t i = 0; i < ADDRESS_SIZE; ++i)
    {
        printf("%d ",  address[i]);
    }

    printf("\n");

    return;
}

void printIndexes(int index)
{
    printf("Index : %d\n", index);

    printf("\n");

    return;
}

int main()
{
    MAGIC m = MAGICinit(NB_ADDRESSES, ADDRESS_SIZE);

    unsigned char addresses[NB_ADDRESSES][ADDRESS_SIZE] = 
    {
        {192, 168, 0, 1},
        {192, 168, 0, 2},
        {192, 168, 0, 3},
        {192, 168, 0, 2},
        {192, 168, 0, 1},
        {192, 168, 0, 1},
    };

    for(size_t i = 0; i < NB_ADDRESSES; ++i)
    {
        printAddresses(addresses[i]);
        printIndexes(MAGICindex(m, addresses[i]));
    }

    return 0;
}
