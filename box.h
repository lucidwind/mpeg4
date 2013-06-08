 /*
  *        算法描述：
  *                播放即提取每个sample，给codec解码
  *        1、确定时间,相对媒体的坐标时间
  *        2、检查time-to-sample atom，来确定给定时间的sample序号
  *        3、检查sample-to-chunk atom,来确定序号sample所在的chunk
  *        4、检查chunk-offset atom,来确定该chunk的偏移量
  *        5、检查sample-size atom,确定sample的便宜量和读取大小,从而获得一个sample
  */

typedef struct
{
        int file_fd;
        int track_offset;
}__box;

