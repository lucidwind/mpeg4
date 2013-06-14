#ifndef __TRACK_H
#define __TRACK_H

#include <stdint.h>

class track
{
public:
        track();
        int32_t read();
        int32_t read_time();
        int32_t read_at();

private:
        ~track();
};

#endif
