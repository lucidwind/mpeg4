#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mpeg4.h>



int32_t main()
{
        //printf("hello world");
        int fd = open("./test.mp4",O_RDWR);

        mpeg4 *mp4 = new mpeg4(fd,0);
        mp4->parse_chunk(0);
}

