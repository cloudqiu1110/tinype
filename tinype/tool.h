#ifndef TINYPE_TOOL_H
#define TINYPE_TOOL_H

#include <stdlib.h>  
#include <time.h>

static unsigned long long myseed = 1;  // ֻ�ǹ������������ʹ��
void srand48(unsigned int i)
{
    myseed = (((long long int)i) << 16) | rand();
}

double drand48(void)
{
    srand48(time(NULL));

    myseed = (0x5DEECE66DLL * myseed + 0xB16) & 0xFFFFFFFFFFFFLL;
    unsigned int x = myseed >> 16;
    return  ((double)x / (double)0x100000000LL);
}



#endif