#ifndef __FS_MAIN__
#define __FS_MAIN__

#define FUSE_USE_VERSION 31
#include "global_types.h"

#ifdef MONITOR
#include "monitor.h"
//extern struct monitor *global_monitor;
#endif
long long super_node_base_idx;
long long inode_bitmap_base_idx;
long long block_bitmap_base_idx;
long long inode_base_idx;
long long data_block_base_idx;

long long super_node_end_idx;
long long inode_bitmap_end_idx;
long long block_bitmap_end_idx;
long long inode_end_idx;
long long data_block_end_idx;

#endif
