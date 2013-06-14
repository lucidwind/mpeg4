#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>

#define make4cc(c1, c2, c3, c4)\
        (c1<<24 | c2<<16 | c3<<8 |c4)



void make4cc_string(uint32_t x,char *s)
{
        s[0] = x >> 24;
        s[1] = (x >> 16) & 0xff;
        s[2] = (x >> 8) & 0xff;
        s[3] = x & 0xff;
        s[4] = 0;
}

uint64_t ntoh64(uint64_t x)
{
        return ((uint64_t)ntohl(x & 0xffffffff) << 32) | ntohl(x >> 32);
}

uint64_t hton64(uint64_t x)
{
        return ((uint64_t)htonl(x & 0xffffffff) << 32) | htonl(x >> 32);
}

