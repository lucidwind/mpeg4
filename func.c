#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include "box.h"

#define make4cc(c1, c2, c3, c4)\
        (c1<<24 | c2<<16 | c3<<8 |c4)


#define INIT_TRACK(NUM,FD,OFFSET) \
        struct __box track_##NUM;\
        track_##NUM.file_fd = FD;\
        track_##NUM.track_offset = OFFSET;\
        printf("Current num is %d",NUM);

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

/*
File-
file_offset
        Chunk_offset
                data_offset
*/


int32_t parse_chunk(int32_t file_fd,uint64_t file_offset)
{
        uint64_t chunk_offset;
        uint64_t data_offset;
        uint32_t result;
        uint32_t track_num = 0;
        result = lseek(file_fd,file_offset,SEEK_SET);
        if(result == -1)
        {
                printf("   lseek error\n");
                return -1;
        }
    //    printf("   into %s file_fd %d file_offset %lld\n",__func__,file_fd,data_offset);
//        usleep(100000);
        uint32_t hdr[2];                      //Contain type and size

        if(read(file_fd, hdr, 8) < 8)
        {
                printf("   here file io error\n");
                return -1;
        }
        uint64_t chunk_size = ntohl(hdr[0]);
        uint32_t chunk_type = ntohl(hdr[1]);
        data_offset = file_offset + 8;
        if(chunk_size == 1)
        {
                if(read(file_fd, &chunk_size, 8) < 8)     //chunk_size = 1 64size to chunk size
                {
                        printf("   here file io error\n");
                        return -1;
                }
                chunk_size = ntoh64(chunk_size);
                data_offset = file_offset + 8;
        }

        char chunk_name[5];
        make4cc_string(chunk_type,chunk_name);
        printf("   current chunk_name [%s] and chunk size %lld\n",chunk_name,chunk_size);

        switch (chunk_type)
        {
        case make4cc('m', 'o', 'o', 'v'):
        case make4cc('t', 'r', 'a', 'k'):
        case make4cc('e', 'd', 't', 's'):
        case make4cc('m', 'd', 'i', 'a'):
        case make4cc('m', 'i', 'n', 'f'):
        case make4cc('d', 'i', 'n', 'f'):
        case make4cc('s', 't', 'b', 'l'):
        case make4cc('m', 'p', '4', 'a'):
        {
                if(chunk_type == make4cc('t','r','a','k'))
                {
                        track_num ++;
                        INIT_TRACK(track_num,file_fd,file_offset);                        
                }
 //               printf("here into ----------\n");
                result = parse_chunk(file_fd, data_offset);
                if(result)
                        return result;
                break;
        }
        case make4cc('s', 't', 's', 'd'):
        {
                file_offset += chunk_size; 
                break;
        }
        case make4cc('d', 'r', 'e', 'f'):
        {
                file_offset += chunk_size; 
                break;
        }
        case make4cc('s', 't', 'c', 'o'):
        {
                file_offset += chunk_size; 
                break;
        }
        case make4cc('s', 't', 's', 'z'):
        {
                file_offset += chunk_size; 
                break;
        }
        case make4cc('s', 't', 's', 'c'):
        {
                file_offset += chunk_size; 
                break;
        }
        case make4cc('s', 't', 't', 's'):
        {
                file_offset += chunk_size; 
                break;
        }
        case make4cc('e', 's', 'd', 's'):
        {
                file_offset += chunk_size; 
                break;
        }
        case make4cc('0', 'u', 'r', 'l'):
        {
                file_offset += chunk_size; 
                break;
        }
        case make4cc('s', 'm', 'h', 'd'):
        {
                file_offset += chunk_size; 
                break;
        }
        case make4cc('h', 'd', 'l', 'r'):
        {
                file_offset += chunk_size; 
                break;
        }
        case make4cc('m', 'd', 'h', 'd'):
        {
                file_offset += chunk_size; 
                break;
        }
        case make4cc('t', 'k', 'h', 'd'):
        {
                file_offset += chunk_size; 
                break;
        }
        case make4cc('e', 'l', 's', 't'):
        {
                file_offset += chunk_size; 
                break;
        }
        case make4cc('m', 'v', 'h', 'd'):
        {
                file_offset += chunk_size; 
                break;
        }

        default:
        {
                file_offset += chunk_size; 
                break;
        }
        }
        printf("file_offset + = chunk size\n");
        //file_offset += chunk_size;
        result =  parse_chunk(file_fd,file_offset);
        if(result)
                return result;
        return 0;
}

