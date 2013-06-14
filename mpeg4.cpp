#include "mpeg4.h"
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <mpeg4.h>
#include <func.h>
#include <netinet/in.h>


mpeg4::mpeg4(int fd,uint64_t offset)
{
        file_fd = fd;
        file_offset = offset;
        track_num = 0;
        first_track = NULL;
        last_track = NULL;
}

mpeg4::~mpeg4()
{
}

int32_t mpeg4::get_track(int index)
{

}

int32_t mpeg4::count_track()
{

}

int32_t mpeg4::parse_chunk(uint64_t file_offset)
{
        uint64_t chunk_offset;
        uint64_t data_offset;
        uint32_t result;
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
         //       printf("   here file io error\n");
                return -1;
        }
        uint64_t chunk_size = ntohl(hdr[0]);
        uint32_t chunk_type = ntohl(hdr[1]);
        data_offset = file_offset + 8;
        if(chunk_size == 1)
        {
                if(read(file_fd, &chunk_size, 8) < 8)     //chunk_size = 1 64size to chunk size
                {
          //              printf("   here file io error\n");
                        return -1;
                }
                chunk_size = ntoh64(chunk_size);
                data_offset = file_offset + 8;
        }

        char chunk_name[5];
        make4cc_string(chunk_type,chunk_name);
        //printf("   current chunk_name [%s] and chunk size %lld\n",chunk_name,chunk_size);

        switch (chunk_type)
        {
        case make4cc('m', 'o', 'o', 'v'):
        case make4cc('t', 'r', 'a', 'k'):
        case make4cc('e', 'd', 't', 's'):
        case make4cc('m', 'd', 'i', 'a'):
        case make4cc('m', 'i', 'n', 'f'):
        case make4cc('d', 'i', 'n', 'f'):
        case make4cc('s', 't', 'b', 'l'):
        case make4cc('m', 'p', '4', 'a'): //contain atom
        {
                if(chunk_type == make4cc('t','r','a','k'))
                {
                        track_num ++;

                        printf("Current track_index------------- %d-----------------offset %lld\n",track_num,data_offset);
                }
 //               printf("here into ----------\n");
                result = parse_chunk(data_offset);
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
        //printf("file_offset + = chunk size\n");
        //file_offset += chunk_size;
        result =  parse_chunk(file_offset);
        if(result)
                return result;
        return 0;
}

int32_t mpeg4::parse_trackhead()
{

}
