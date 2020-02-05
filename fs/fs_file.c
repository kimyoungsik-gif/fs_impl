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
        	if(P_path[i] == '/'){
            	index = i;
                break;
            }
        }
 
        P_path.replace(index,size,"");
        my_name.replace(0,index+1,"");
	}
	
	//parent_change
	char* temppp;
    strcpy(temppp,P_path.c_str());
	struct metadata p_meta;
	struct dir_entry temp_e;
	struct dir_entry p_e;
	int p_id = inode_finder(temppp);

	pread(fd,(char*)&p_meta,sizeof(p_meta),p_id);
    for(int i = 0; i< p_meta.data_ptr.size();i++){
	     pread(fd,&temp_e,BLOCK_SIZE,p_meta.data_ptr[i]);
         p_e.entry.insert(temp_e.entry.begin(),temp_e.entry.end());
    }
 
	p_e.entry.insert(make_pair(my_name, dioff.inode_offset));
	p_meta.count++;
	p_meta.size++;
	int P_I = pwrite(fd, &p_meta, sizeof(struct metadata), (off_t) p_id);
    
	map<string,int>::iterator it;
	int i = 0;
	for(it = p_e.entry.begin(); it != p_e.entry.end() ; it++){
	    for(int j = 0; j < 16; j++){
		    temp_e.entry.insert(*it);
   		}
   		pwrite(fd,&temp_e,BLOCK_SIZE,p_meta.data_ptr[i]);
    	i++;
    }

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
    m.ino = dioff.inode_offset;
    m.data_ptr[0] = dioff.data_block_offset;
    m.data_bitmap_ptr[0] = dioff.data_bitmap_offset;
    m.count = 0;

	fi->fh = dioff.inode_offset;
	int nw1 = pwrite(fd, bitmap, BITMAP_SIZE, (off_t) dioff.inode_bitmap_offset);
    int nw2 = pwrite(fd, &m, INODE_SIZE, (off_t) dioff.inode_offset);

	dioff.inode_bitmap_offset += BITMAP_SIZE;
    dioff.inode_offset += INODE_SIZE;
    dioff.data_bitmap_offset += BITMAP_SIZE;
    dioff.data_block_offset += BLOCK_SIZE;

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
