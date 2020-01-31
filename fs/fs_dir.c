#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <assert.h>
#include "fs_dir.h"
#include "metadata.h"


int fs_opendir (const char *path, struct fuse_file_info *fi) {
	fi->keep_cache = 1;

	return 0;
}

int fs_mkdir (const char *path, mode_t mode) {
	int inode_bitmap_size = 1;
	int inode_size = 256;
	int data_bitmap_size = 1;
	int data_block_size = 102400000;
	char* bitmap = "1";
	char* dir_name = "mkdir_name";
	

	metadata m;
	m.mode = mode;
	m.nlink = 0;
	m.uid = 123;
	m.gid = 456;
	m.size = 12345;
	m.atime = 11;
	m.ctime = 22;
	m.mtime = 33;
	m.ino = inode_offset;

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
	
	return 0;
}

int fs_releasedir (const char *path, struct fuse_file_info *fi) {
	return 0;
}

int fs_fsyncdir (const char *path, int datasync, struct fuse_file_info *fi) {
	return 0;
}
