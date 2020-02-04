#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <assert.h>
#include <unistd.h>
#include "fs_file.h"
#include "metadata.h"
#include "fs_main.h"

extern struct monitor *global_monitor;


int fs_open (const char *path, struct fuse_file_info *fi) {
	fi->keep_cache = 1;
	
	return 0;
}

int fs_create (const char *path, mode_t mode, struct fuse_file_info *fi) {
	fi->keep_cache = 1;
	char* bitmap = "1";
    string P_path = path;
    string my_name = path;
	
    //parent_and_my_name_find
    if(path == "/") {
    	P_path = path;
        my_name = path;
        root_inumber = inode_base_idx;
    }
    else {
        int size = P_path.size();
        int index;
        for (int i = size; i >= 0; i--){
        	if(P_path[i] == "/"){
            	index = i;
                break;
            }
        }
 
        P_path.replace(index,size,"");
        my_name.replace(0,index+1,"");
	}
	
	//parent_change
	struct metadata p_meta;
	struct dir_entry e;
	int p_id = inode_finder(P_path);
	
	pread(fd,(char*)&p_meta,sizeof(p_meta),p_id);
    pread(fd,(char*)&e,sizeof(e),p_meta.data_ptr);
	
	e.entry.push_back(make_pair(my_name, inode_offset));
	p_meta.count++;
	p_meta.size++;
	int P_I = pwrite(fd, p_meta, sizeof(struct metadata), (off_t) p_id);
	int P_D = pwrite(fd, e, sizeof(struct dir_entry), (off_t) p_meta.data_ptr);

	//child_create
	struct metadata m;
    m.mode = mode;
    m.nlink = 1;
    m.uid = USER_ID;
    m.gid = GROUP_ID;
    m.size = 2;
    m.atime = 11;
    m.ctime = 22;
    m.mtime = 33;
    m.ino = inode_offset;
    m.data_ptr = data_block_offset;
    m.inode_bitmap_ptr = inode_bitmap_offset;
    m.data_bitmap_ptr = data_bitmap_offset;
    m.count = 0;

	fi->fh = inode_offset;
	int nw1 = pwrite(fd, bitmap, BITMAP_SIZE, (off_t) inode_bitmap_offset);
    int nw2 = pwrite(fd, m, INODE_SIZE, (off_t) inode_offset);

	inode_bitmap_offset += BITMAP_SIZE;
    inode_offset += INODE_SIZE;
    data_bitmap_offset += BITMAP_SIZE;
    data_block_offset += BLOCK_SIZE;

	return 0;
}

int fs_read (const char *path, char *buf, size_t size, off_t off, struct fuse_file_info *fi) {
	//sdfsdfsdafsf	
	return 0;
}

int fs_write (const char *path, const char *buf, size_t size, off_t off, struct fuse_file_info *fi) {
	
	
	return 0;
}

int fs_flush (const char *path, struct fuse_file_info *fi) {

	return 0;
}

int fs_unlink (const char *path) {
	
	return 0;
}

int fs_truncate (const char *path, off_t off, struct fuse_file_info *fi) {
	
	return 0;
}

int fs_release (const char *path, struct fuse_file_info *fi) {

	return 0;
}

int fs_fsync (const char *path, int isdatasync, struct fuse_file_info *fi) {
	
	return 0;
}
