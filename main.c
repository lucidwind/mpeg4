#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



int32_t main()
{
        //printf("hello world");
        int fd = open("./test.mp4",O_RDWR);
        parse_chunk(fd,0);
        return 0;
}

