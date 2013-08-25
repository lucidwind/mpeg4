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

struct stsc
{
	int first_chunk;
	int sample_per_chunk;
	int sample_des;
};

struct chunk_entry
{
	int chunk_at;
	int sample_per_chunk;
	int sample_des;
	int first_sample_index;
};

struct sample_table
{
	int sample_size;
	int sample_offset;
	int sample_at_chunk;
};

class track
{
public:
        track(int fd, uint64_t offset, uint64_t size);
        int32_t read_time();
        int32_t read_at();
        int32_t parse_track(uint64_t file_offset = 0);
        int32_t parse_track_head();
	int32_t parse_stsc(uint64_t offsets,uint64_t size);
	int32_t parse_stco(uint64_t offsets,uint64_t size);
	int32_t parse_stsz(uint64_t offsets,uint64_t size);
	int32_t parse_sample(void);


public:
        track *next;
        int file_fd;
        uint64_t track_offset;
        uint64_t track_size;
        uint64_t data_offset;
        uint64_t file_offset;
	bool	large_head;
	struct stsc *m_stsc;
	int m_stsc_len;
	struct chunk_entry *m_chunk_entry;
	int m_chunk_entry_len;
	struct sample_table *m_sample_table;
	int m_sample_table_len;

private:
        ~track();
};

#endif
