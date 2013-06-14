#ifndef __FUNC_H
#define __FUNC_H

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>

#define make4cc(c1, c2, c3, c4)\
        (c1<<24 | c2<<16 | c3<<8 |c4)



void make4cc_string(uint32_t x,char *s);
uint64_t ntoh64(uint64_t x);
uint64_t hton64(uint64_t x);

#endif
