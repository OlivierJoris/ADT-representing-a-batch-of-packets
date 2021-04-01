#include "magic.h"

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

#define ADDRESS_SIZE 4
#define NB_ADDRESSES_MAX 10000
#define NB_TESTS 1000

int main()
{
    srand(817);

    unsigned long memo = 0;

    FILE *mem = fopen("mem.txt", "w");
	if(mem == NULL)
		return -1;

    for(unsigned int nbAdresses = 0; nbAdresses < NB_ADDRESSES_MAX; nbAdresses += 500)
    {
        MAGIC m = MAGICinit(NB_ADDRESSES_MAX, ADDRESS_SIZE);

        if(!m)
            return -1;

        char **addresses = malloc(sizeof(char **) * nbAdresses);
        for(size_t i = 0; i < nbAdresses; ++i)
        {
            addresses[i] = malloc(sizeof(char *) * ADDRESS_SIZE);
        }

        for(unsigned int nbTests = 0; nbTests < NB_TESTS; ++nbTests)
        {
            for(size_t j = 0; j < nbAdresses; ++j)
            {
                for(size_t i = 0; i < ADDRESS_SIZE; ++i)
                {
                    addresses[j][i] = rand() % 127; 
                }
            }
            
            for(size_t i = 0; i < nbAdresses; ++i)
            {
                int index = MAGICindex(m, addresses[i]);
            }

            memo += getMemory(m);

            MAGICreset(m);
        }

        memo /= NB_TESTS;

        fprintf(mem, "%ld, ", memo);

        for(size_t i = 0; i < nbAdresses; ++i)
            free(addresses[i]);

        free(addresses);
    }

    fprintf(mem, "\n");

    fclose(mem);

    return 0;
}
