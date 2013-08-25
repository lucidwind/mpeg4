#include "track.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

track::track(int fd,uint64_t offset,uint64_t size)
{
        file_fd = fd;
        track_offset = offset;
        track_size = size;
	large_head = false;
	m_stsc = NULL;
}

track::~track()
{

}


int32_t track::read_time()
{

}

int32_t track::read_at()
{

}

int num = 0;
int32_t track::parse_track(uint64_t file_offset)
{
        if(num == 0)
                file_offset = track_offset;
        num ++;
        if(file_offset >= track_size + track_offset)
        {
                return -1;
        }
        uint64_t chunk_offset;
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
		large_head = true;
        }

        char chunk_name[5];
        make4cc_string(chunk_type,chunk_name);
        printf("   current chunk_name [%s] and chunk size %lld\n",chunk_name,chunk_size);

        switch (chunk_type)
        {
        case make4cc('t', 'r', 'a', 'k'):
        case make4cc('e', 'd', 't', 's'):
        case make4cc('m', 'd', 'i', 'a'):
        case make4cc('m', 'i', 'n', 'f'):
        case make4cc('d', 'i', 'n', 'f'):
        case make4cc('s', 't', 'b', 'l'):
        case make4cc('m', 'p', '4', 'a'): //contain atom
        {
                result = parse_track(data_offset);
                if(result)
                        return result;
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
		if(large_head)
			parse_stco(file_offset + 16,chunk_size);
		else
			parse_stco(file_offset + 8,chunk_size);
		printf("stco %lld %lld\n",file_offset,chunk_size);
                file_offset += chunk_size; 
                break;
        }
        case make4cc('s', 't', 's', 'z'):
        {
		if(large_head)
			parse_stsz(file_offset + 16,chunk_size);
		else
			parse_stsz(file_offset + 8,chunk_size);
                file_offset += chunk_size; 
                break;
        }
        case make4cc('s', 't', 's', 'c'):
        {
		if(large_head)
			parse_stsc(file_offset + 16,chunk_size);
		else
			parse_stsc(file_offset + 8,chunk_size);
		printf("stsc %lld %lld\n",file_offset,chunk_size);
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
                parse_track_head();
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
        result =  parse_track(file_offset);
	large_head = false;
        if(result)
                return result;
        return 0;       
}

int32_t track::parse_track_head()
{
        printf("here into %s\n",__func__);         
}

int32_t track::parse_stsc(uint64_t offset,uint64_t size)
{
	printf("offset %lld size %lld\n",offset,size);
        int result = lseek(file_fd,offset,SEEK_SET);
	uint32_t current;

        read(file_fd, &current, sizeof(int));
	uint32_t version = ntohl(current);	
	printf("version %d\n",version);
        read(file_fd, &current, sizeof(int));
	uint32_t entries = ntohl(current);	
	printf("entries %d\n",entries);
	m_stsc = (stsc *)malloc( (entries + 1) * sizeof(struct stsc));
	int i;
	m_stsc_len = entries;
	for(i = 1; i <= entries; i ++)
	{
		uint32_t hdr[3] = {0};                     //Contain type and siz
		if(read(file_fd, hdr, 12) < 12)
		{
			printf("io error ");
		}

		uint32_t first_chunk = ntohl(hdr[0]);
		uint32_t sample_per_chunk = ntohl(hdr[1]);
		uint32_t sample_des = ntohl(hdr[2]);

		m_stsc[i].first_chunk = first_chunk;
		m_stsc[i].sample_per_chunk = sample_per_chunk;
		m_stsc[i].sample_des = sample_des;
//		printf("current%d %d %d %d\n",i,m_stsc[i].first_chunk,m_stsc[i].sample_per_chunk,m_stsc[i].sample_des);
	}
	return result;
}

int32_t track::parse_stco(uint64_t offset,uint64_t size)
{
	printf("offset %lld size %lld\n",offset,size);
        int result = lseek(file_fd,offset,SEEK_SET);
	uint32_t current;

        read(file_fd, &current, sizeof(int));
	uint32_t version = ntohl(current);	
	printf("version %d\n",version);
        read(file_fd, &current, sizeof(int));
	uint32_t entries = ntohl(current);	
	printf("entries %d\n",entries);
	m_chunk_entry_len = entries;
	m_chunk_entry = (chunk_entry *)malloc( (entries + 1) * sizeof(struct chunk_entry));
	memset(m_chunk_entry,0,(entries + 1) * sizeof(struct chunk_entry));
	int i;
	for(i = 1; i <= entries; i ++)
	{
		uint32_t hdr[1] = {0};                     //Contain type and siz
		if(read(file_fd, hdr, 4) < 4)
		{
			printf("io error ");
		}

		uint32_t chunk_offset = ntohl(hdr[0]);
		m_chunk_entry[i].chunk_at = chunk_offset;

		//printf("current chunk_offset%d \n",m_chunk_entry[i].chunk_at);
	}
}

int32_t track::parse_stsz(uint64_t offset,uint64_t size)
{
	printf("stsz offset %lld size %lld\n",offset,size);
        int result = lseek(file_fd,offset,SEEK_SET);
	uint32_t current;

        read(file_fd, &current, sizeof(int));
	uint32_t version = ntohl(current);	
	printf("version %d\n",version);
        read(file_fd, &current, sizeof(int));
	uint32_t sample_s = ntohl(current);	
	printf("sample_s %d\n",sample_s);
        read(file_fd, &current, sizeof(int));
	uint32_t entries = ntohl(current);	
	printf("entries %d\n",entries);
	m_sample_table = (struct sample_table *)malloc((entries +1) * sizeof(struct sample_table));
	m_sample_table_len = entries;
	int i;
	for(i = 1; i <= entries; i ++)
	{
		uint32_t hdr[1] = {0};                     //Contain type and siz
		if(read(file_fd, hdr, 4) < 4)
		{
			printf("io error ");
		}

		uint32_t sample_size = ntohl(hdr[0]);
		m_sample_table[i].sample_size = sample_size;
//		printf("current sample size %d \n",sample_size);
	}

}

int32_t track::parse_sample(void)
{
	parse_track();
	int last_chunk_num = m_chunk_entry_len + 1; //totally chunk num
	int i, index;
	for( i = m_stsc_len ; i > 0 ; i -- )
	{
		//printf("current stsc at %d\n",i);
		int first_chunk_num = m_stsc[i].first_chunk;	
		//printf("first chunk %d last chunk %d\n",first_chunk_num,last_chunk_num);
		int index;
		for(index = first_chunk_num; index < last_chunk_num; index ++)
		{
			//printf("current index %d\n",index);
			m_chunk_entry[index].sample_per_chunk = m_stsc[i].sample_per_chunk;	
			m_chunk_entry[index].sample_des = m_stsc[i].sample_des;
		}
		last_chunk_num = first_chunk_num;
	}
	for(i = 1; i <= m_chunk_entry_len; i ++)
	{
		//printf("entry[%d] %d %d %d\n",i,m_chunk_entry[i].sample_per_chunk,m_chunk_entry[i].chunk_at,m_chunk_entry[i].sample_des);
	}
	
	int sample_index = 1;
	for(i = 1; i <= m_chunk_entry_len; i ++)
	{
		m_chunk_entry[i].first_sample_index = sample_index;
		int index_in_chunk;
//		printf("%d \t %d\n",i, m_chunk_entry[i].sample_per_chunk);
		for(index_in_chunk = 1; index_in_chunk <= m_chunk_entry[i].sample_per_chunk;index_in_chunk++ )
		{
			m_sample_table[sample_index].sample_at_chunk = i;
			//printf("sample %d \t at %d\n",sample_index,i);
			int first_sample = m_chunk_entry[i].first_sample_index;
			int current_sample = sample_index;
			int offset_sample = current_sample - first_sample;
	//		printf("first %d\t current %d\t off %d\n",first_sample,current_sample,offset_sample);
			int s_index;
			m_sample_table[first_sample].sample_offset = m_chunk_entry[i].chunk_at;
			int offset = 0;
			for(s_index = 0; s_index < offset_sample; s_index ++)
			{
				offset += m_sample_table[first_sample + s_index].sample_size;	
				//printf("current offset %d\n",offset);
			}
			m_sample_table[current_sample].sample_offset = m_sample_table[first_sample].sample_offset + offset;
			sample_index ++;
		}
	}
	//int fd_dump = open("./dump",O_RDWR|O_CREAT);
	//unsigned char buffer[100000];
	for(i = 1; i <= m_sample_table_len; i ++)
	{
		/*
		printf("current i %d\n",i);
		memset(buffer,0,sizeof(buffer));
		lseek(file_fd,m_sample_table[i].sample_offset,SEEK_SET);
		read(file_fd,buffer,m_sample_table[i].sample_size);
		write(fd_dump,buffer,m_sample_table[i].sample_size);
		*/
		printf("sample[%d]\t size[%d]\t offset%d\t chunk%d\n",i,m_sample_table[i].sample_size,m_sample_table[i].sample_offset,m_sample_table[i].sample_at_chunk);
	}

}
