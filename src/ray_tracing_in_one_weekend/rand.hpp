#ifndef DRAND48_H  
#define DRAND48_H  
  
#include <stdlib.h>  
  
#define m_rand 0x100000000LL  
#define c_rand 0xB16  
#define a_rand 0x5DEECE66DLL  
  
  
inline double drand48(void)  
{  
	static unsigned long long seed = 1;  
    seed = (a_rand * seed + c_rand) & 0xFFFFFFFFFFFFLL;  
    unsigned int x = seed >> 16;  
    return  ((double)x / (double)m_rand);  
      
}  
  
inline void srand48(unsigned int i)  
{  
	static unsigned long long seed = 1;  
    seed  = (((long long int)i) << 16) | rand();  
}  
  
#endif  