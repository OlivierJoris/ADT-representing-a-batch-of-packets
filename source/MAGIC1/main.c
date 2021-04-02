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
    /*
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
    */

    static const int MAX_ELEMENTS = 10;
    static const int MAX_ADDRESS_SIZE = 16;

    MAGIC m = MAGICinit(MAX_ELEMENTS, MAX_ADDRESS_SIZE);

    char addresses1[10][16] = 
    {
        {127, 196, 69, 209, 162, 176, 116, 196, 55, 80, 70, 203, 189, 105, 141, 226},
        {193, 53, 103, 146, 127, 99, 92, 126, 161, 25, 236, 156, 221, 87, 80, 79},
        {236, 81, 29, 166, 111, 235, 95, 178, 84, 155, 163, 0, 135, 59, 71, 121},
        {100, 11, 143, 220, 47, 114, 246, 70, 217, 216, 121, 207, 136, 230, 239, 113},
        {115, 174, 162, 32, 18, 211, 142, 77, 186, 191, 104, 226, 161, 149, 211, 80},
        {101, 191, 240, 18, 129, 126, 158, 5, 38, 16, 129, 253, 213, 163, 224, 120},
        {161, 81, 112, 55, 60, 61, 11, 194, 211, 118, 69, 207, 182, 74, 35, 167},
        {83, 118, 212, 210, 14, 71, 38, 77, 116, 150, 134, 70, 14, 175, 32, 18},
        {187, 177, 113, 228, 108, 171, 72, 233, 232, 135, 241, 167, 18, 47, 208, 106},
        {24, 169, 223, 151, 173, 89, 70, 150, 151, 111, 106, 96, 227, 58, 53, 49}
    };

    char addresses2[10][16] =
    {
        {169, 76, 233, 133, 143, 247, 107, 83, 190, 175, 97, 236, 181, 224, 252, 107},
        {21, 15, 156, 191, 82, 55, 29, 236, 244, 139, 217, 194, 155, 87, 234, 44},
        {58, 239, 109, 159, 170, 102, 4, 120, 31, 187, 171, 150, 64, 246, 214, 143},
        {164, 56, 181, 17, 20, 223, 16, 223, 240, 223, 138, 49, 245, 178, 231, 156},
        {111, 170, 167, 161, 82, 22, 255, 61, 198, 185, 5, 142, 158, 92, 56, 56},
        {117, 81, 109, 192, 115, 68, 17, 0, 6, 149, 191, 19, 13, 227, 240, 250},
        {96, 229, 50, 251, 197, 62, 186, 72, 133, 157, 196, 217, 106, 57, 182, 199},
        {232, 26, 158, 191, 183, 161, 207, 164, 198, 19, 133, 251, 137, 199, 51, 249},
        {160, 170, 85, 202, 159, 165, 66, 104, 33, 235, 246, 45, 189, 81, 168, 185},
        {255, 199, 91, 109, 166, 225, 108, 43, 105, 138, 66, 237, 125, 72, 103, 87}
    };

    char addresses3[10][16] =
    {
        {176, 94, 12, 175, 25, 178, 80, 206, 225, 41, 218, 70, 255, 82, 50, 7},
        {232, 230, 121, 155, 47, 41, 48, 69, 104, 175, 199, 194, 146, 73, 244, 55},
        {28, 208, 214, 78 ,105, 7, 153 ,238, 255, 177 ,168,47 ,192 ,193 ,227, 151},
        {124, 50 ,35, 12 ,207 ,20, 86 ,245, 193 ,134, 142, 188 ,73 ,241, 11 ,134},
        {224, 73, 235, 22, 137, 134 ,169 ,63 ,126, 190, 163, 154, 118, 189, 123, 108},
        {143, 109, 48 ,216 ,234 ,155 ,167 ,93 ,239, 186 ,109, 255, 55, 249 ,35, 187},
        {42, 255, 1 ,17 ,255, 151, 70, 212, 46,50, 218, 50, 61, 27, 145, 0},
        {70, 255, 9 ,109, 8 ,38 ,49 ,46, 117, 184, 173, 74, 42, 113, 176, 37},
        {139, 236, 13 ,142, 139, 217, 44 ,30, 92, 207, 213 ,233 ,25 ,107, 10, 44},
        {89, 181 ,41 ,232 ,14 ,170 ,26 ,174 ,73, 200, 218, 232, 247 ,102 ,18 ,206}
    };

    size_t batch1[12] = {0, 1, 2, 3, 4, 3, 5, 6, 4, 7, 8, 9};
    size_t batch2[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    size_t batch3[11] = {0, 1, 2, 0, 3, 4, 5, 6, 7, 8, 9};

    printf("Batch 1: ");
    for(size_t i = 0; i < 12; ++i)
        printf("%d ", MAGICindex(m, addresses1[batch1[i]]));
    printf("\n");

    MAGICreset(m);

    printf("Batch 2: ");
    for(size_t i = 0; i < 10; ++i)
        printf("%d ", MAGICindex(m, addresses2[batch2[i]]));
    printf("\n");

    MAGICreset(m);

    printf("Batch 3: ");
    for(size_t i = 0; i < 11; ++i)
        printf("%d ", MAGICindex(m, addresses3[batch3[i]]));
    printf("\n");

   return 0;
}
