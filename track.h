#ifndef __TRACK_H
#define __TRACK_H

#include <stdint.h>
#include <netinet/in.h>
#include <func.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <func.h>
#include <netinet/in.h>



class track
{
public:
        track(int fd, uint64_t offset, uint64_t size);
        int32_t read_time();
        int32_t read_at();
        int32_t parse_track(uint64_t file_offset = 0);
        int32_t parse_track_head();

public:
        track *next;
        int file_fd;
        uint64_t track_offset;
        uint64_t track_size;
        uint64_t data_offset;
        uint64_t file_offset;



private:
        ~track();
};

#endif
