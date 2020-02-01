#ifndef __GLOBAL_TYPES__
#define __GLOBAL_TYPES__

#include <stdint.h> 
#include <vector>
#define BLOCK_SIZE	4096
#define INODE_SIZE  256
#define PAGESIZE	4096
#define BITMAP_SIZE 1
#define DEVSIZE 1024L * 1024L * 1024L * 10L

//start address
#define super_node_base_idx 0
#define inode_bitmap_base_idx 104857600
#define block_bitmap_base_idx 209715200
#define inode_base_idx 314572800
#define data_block_base_idx 1.0486e9

//end address
#define super_node_end_idx 104857600
#define inode_bitmap_end_idx 209715200
#define block_bitmap_end_idx 314572800
#define inode_end_idx 1.0486e9
#define data_block_end_idx 1.0486e10

//ID
#define USER_ID		123456789
#define GROUP_ID	123456789

//current offset
struct disk_offset{
	long long inode_bitmap_offset;
	long long data_bitmap_offset;
	long long inode_offset;
	long long data_block_offset;
}


File *fd;
vector<fuse_file_info> fi;_
#endif
