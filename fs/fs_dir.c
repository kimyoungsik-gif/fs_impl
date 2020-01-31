dfksdfsd#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <assert.h>
#include "fs_dir.h"
#include "metadata.h"


int fs_opendir (const char *path, struct fuse_file_info *fi) {
	fi->keep_cache = 1;

	struct metadata* cur_m;
	int cur_data_block_idx;
	struct dir_data cur_dir_data;
	int cur_dir_inumber = root_inumber;
	char* result;
	result = strtok(path,"/");

	while(result != NULL) {
		pread(fd, cur_m, sizeof(struct metadata), inode_base_idx + cur_dir_inumber * INODE_SIZE );
		cur_data_block_idx = cur_m.datablock;
		pread(fd, cur_dir_data, sizeof(struct dir_data), data_block_base_idx + cur_data_block_idx * BLOCK_SIZE );
		
		int flag = 0;
		for(int i = 0; i < cur_data.child.size();i++){
			if(cur_dir_data.child[i] == result){
				cur_dir_inumber = cur_dir_data.child[i];	
				flag++;	
			}
		}

		if(flag!) {
			printf("error : no such directory\n");
			return -1;
		}

		result = strtok(NULL, "/");	
	}		
	
	return cur_dir_inumber;
	return 0;
}

int fs_mkdir (const char *path, mode_t mode) {
	int inode_bitmap_size = 1;
	int inode_size = 256;
	int data_bitmap_size = 1;
	int data_block_size = 102400000;
	char* bitmap = "1";
	char* dir_name = "mkdir_name";

	
	//A= fs_opendir(a,b)


	struct metadata m;
	m.mode = mode;
	m.nlink = 1;
	m.uid = 123;
	m.gid = 456;
	m.size = 12345;
	m.atime = 11;
	m.ctime = 22;
	m.mtime = 33;
	m.ino = inode_offset;
	m.data_ptr = data_block_offset;
	m.count=0;

	nw1 = pwrite(fd, bitmap, inode_bitmap_size,(off_t) inode_bitmap_offset);
	nw2 = pwrite(fd, m, inode_size, (off_t) inode_offset);

	inode_bitmap_offset	+= inode_bitmap_size;
	inode_offset += inode_size;
	data_bitmap_offset += data_bitmap_size;
	data_block_offset += data_block_size;

	return 0;
}

int fs_readdir (const char *path, void *buf, fuse_fill_dir_t filler, off_t off, struct fuse_file_info *fi, enum fuse_readdir_flags flags) {

	filler(buf, ".", NULL, 0, (enum fuse_fill_dir_flags)0);
	filler(buf, "..", NULL, 0, (enum fuse_fill_dir_flags)0);

	return 0;
}

int fs_rmdir (const char *path) {
	struct metadata *m;
	int offset=	fs_opendir(path,fi);
	pread(fd,m,sizeof(m),offset);
	
	if(m.count!=0)
	{printf("directory is not empty!!!!!!!!!");}

	else
	{
		//inode
		
	m.mode = mode;
	m.nlink = 0;
	m.uid =0;
	m.gid = 0;
	m.size = 0;
	m.atime = 0;
	m.ctime = 0;
	m.mtime = 0;
	m.ino = 0;
	m.data_ptr = 0;
	//bitmap free allocation please.

	

	



	return 0;
}

int fs_releasedir (const char *path, struct fuse_file_info *fi) {
	return 0;
}

int fs_fsyncdir (const char *path, int datasync, struct fuse_file_info *fi) {
	return 0;
}
