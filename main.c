#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mpeg4.h>
#include <track.h>



int32_t main()
{
        //printf("hello world");
        int fd = open("./test.mp4",O_RDWR);

        mpeg4 *mp4 = new mpeg4(fd,0);
        mp4->parse_chunk(0);
        track * t_track = mp4->get_track(2);

        t_track->parse_track();

        printf("track num %d\n",mp4->count_track());
        //printf("track 1 offset %lld size %lld\n",t_track->data_offset,t_track->data_size);
        return 0;
}

