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

    double meanIndex, meanReset = 0;

    FILE *index = fopen("index.txt", "w");
	if(index == NULL)
		return -1;
    
    FILE *reset = fopen("reset.txt", "w");
	if(reset == NULL)
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

            clock_t t1 = clock();

            for(size_t i = 0; i < nbAdresses; ++i)
            {
                int index = MAGICindex(m, addresses[i]);
            }

            clock_t t2 = clock();

            meanIndex += ((double) (t2 - t1)) / CLOCKS_PER_SEC;

            clock_t t3 = clock();

            MAGICreset(m);

            clock_t t4 = clock();

            meanReset += ((double) (t4 - t3)) / CLOCKS_PER_SEC;
        }

        meanIndex /= NB_TESTS;

        meanReset /= NB_TESTS;

        fprintf(index, "%lf, ", meanIndex);
        fprintf(reset, "%lf, ", meanReset);

        meanIndex = 0;
        meanReset = 0;

        for(size_t i = 0; i < nbAdresses; ++i)
            free(addresses[i]);

        free(addresses);
    }

    fprintf(index, "\n");
    fprintf(reset, "\n");

    fclose(index);

    fclose(reset);

    return 0;
}
