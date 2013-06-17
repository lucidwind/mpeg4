#ifndef __MPEG4_H
#define __MPEG4_H

#include <stdint.h>
#include <track.h>

/*
  *        算法描述：
  *                播放即提取每个sample，给codec解码
  *        1、确定时间,相对媒体的坐标时间
  *        2、检查time-to-sample atom，来确定给定时间的sample序号
  *        3、检查sample-to-chunk atom,来确定序号sample所在的chunk
  *        4、检查chunk-offset atom,来确定该chunk的偏移量
  *        5、检查sample-size atom,确定sample的便宜量和读取大小,从而获得一个sample
  */


class mpeg4
{
public:
        mpeg4(int fd,uint64_t offset);
        track * get_track(int index);
        int32_t count_track();

        int32_t parse_chunk(uint64_t file_offset);
        int32_t parse_trackhead();

public:
        int file_fd;
        uint64_t file_offset;
        int track_num;
        track *first_track;
        track *last_track;

private:
        virtual ~mpeg4();
};


#endif
