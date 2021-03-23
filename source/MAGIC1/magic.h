#ifndef __MAGIC__
#define __MAGIC__

typedef struct MAGIC *MAGIC;

MAGIC MAGICinit(int maxSize, int addrSize);
int MAGICindex(MAGIC m, char* dest);
void MAGICreset(MAGIC m);

#endif
