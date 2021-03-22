#include "magic.h"

#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define NUMBER_BYTE 4
#define NUMBER_ADDRESS 4


int check_equity(int tmp_index[], size_t tmp_r[])
{
    for (size_t i = 0; i < NUMBER_ADDRESS; ++i)
    {
        for (size_t j = 0; j < NUMBER_ADDRESS; ++j)
        {
            if (tmp_index[i] == tmp_index[j])
                if (tmp_r[i] != tmp_r[j])
                    return 0;
        }
    }

    return 1;
}


int same_array(char b1[], char b2[], size_t N) {
    for (size_t i = 0; i < N; ++i)
        if(b1[i] != b2[i])
            return 0;
    return 1;
}

void check_validity(int tmp_index[], char batch[NUMBER_ADDRESS][NUMBER_BYTE], size_t tmp_r[])
{
    if (check_equity(tmp_index, tmp_r) != 1)
    {
        // two same array
        for (size_t i = 0; i < 4; ++i)
        {
            for (size_t j = i + 1; j < 4; ++j)
            {
                // same array
                if (same_array(batch[i], batch[j], 4))
                    return;
            }
        }

        for (size_t i = 0; i < 4; ++i)
        {
            for (size_t e = 0; e < 4; ++e)
                printf("%d ", batch[i][e]);
            printf("\n");
        }

        assert(0);
    }
}

int main()
{
    time_t t;
    srand((unsigned)time(&t));
    int tmp_index[NUMBER_ADDRESS];
    size_t tmp_r[NUMBER_ADDRESS];
    MAGIC a = MAGICinit(NUMBER_ADDRESS, NUMBER_BYTE);
    char batch[NUMBER_ADDRESS][NUMBER_BYTE];
    size_t counter = 0;

    while (1)
    {
        printf("%lu\n", counter++);
        // make random batch
        for (size_t j = 0; j < NUMBER_ADDRESS; ++j)
        {
            int random_range = pow(2, 8);
            for (size_t i = 0; i < NUMBER_BYTE; ++i)
                batch[j][i] = rand() % random_range;
        }

        // call index
        for (size_t i = 0; i < NUMBER_ADDRESS; ++i)
        {
            size_t r = i;
            int index = MAGICindex(a, batch[r]);
            printf("index = %d r = %lu\n", index, r);
            tmp_index[i] = index;
            tmp_r[i] = r;
            assert(index < NUMBER_ADDRESS);
        }

        check_validity(tmp_index, batch, tmp_r);

        printf("-----------------\n\n");
        MAGICreset(a);
    }

    return 0;
}