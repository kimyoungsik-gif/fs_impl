#ifndef __METADATA__
#define __METADATA__

#define FUSE_USE_VERSION 31
#include <fuse3/fuse.h>
#include <sys/stat.h>
#include "global_types.h"
#include <list>

struct metadata {
    mode_t			mode;
    nlink_t			nlink;
    uid_t			uid;
    gid_t			gid;
    size_t			size;
    time_t			atime;
    time_t			ctime;
    time_t			mtime;
    uint64_t	        	ino;
    list<int>			data_ptr; //direct block pointer
    list<int>			indir_ptr; //indirect block pointer
    int				count;
    int 			cur_data_offset; //current file offset position
	int				data_bitmap_ptr;
	int				inode_bitmap_ptr;
};

#endif
