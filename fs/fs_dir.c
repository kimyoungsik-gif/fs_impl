#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <assert.h>
#include "fs_dir.h"
#include "metadata.h"
#include <string>

int fs_opendir (const char *path, struct fuse_file_info *fi) {
	fi->keep_cache = 1;
	char* temp = (char*)path;
	int open_dir_inumber = inode_finder(temp);
	fi->fh = open_dir_inumber;
	return 0; 
}

int fs_mkdir (const char *path, mode_t mode) {
	char* bitmap = "1";	
	char* temp = (char*)path;
	string P_path = temp;
	string name = temp;

	//parent_Information
	if(path == "/") {
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
		name.replace(0,index+1,"");
		char my_name[252];
		strcpy(my_name, name.c_str());
	}

	//traverse implement

	struct metadata p_meta;
	struct dir_entry p_e;
	struct dir_entry my_e;
	int p_id = inode_finder(P_path.c_str());

	pread(fd,(char*)&p_meta,sizeof(p_meta),p_id);
	pread(fd,(char*)&p_e,sizeof(p_e),p_meta.data_ptr);

	p_e.entry.push_back(make_pair(my_name, inode_offset));
	p_meta.count++;

	if(p_meta.count% 16 == 0) {
		P_meta.size++;
		p_meta.data_ptr.push_back(data_block_offset);
		int P_D = pwrite(fd, e, sizeof(struct dir_entry), (off_t) p.meta.data_ptr[size-1]);
		data_block_offset += BLOCK_SIZE;

	}
	int P_I = pwrite(fd, p_meta, sizeof(struct metadata), (off_t) p_id);

	//child_Information
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
	m.data_bitmap_ptr = data_bitmap_offset;
	m.count = 0;

	int nw1 = pwrite(fd, bitmap, BITMAP_SIZE, (off_t) inode_bitmap_offset);
	int nw2 = pwrite(fd, m, INODE_SIZE, (off_t) inode_offset);

	inode_bitmap_offset += BITMAP_SIZE;
	inode_offset += INODE_SIZE;
	data_bitmap_offset += BITMAP_SIZE;
	data_block_offset += BLOCK_SIZE;

	return 0;
}

int fs_readdir (const char *path, void *buf, fuse_fill_dir_t filler, off_t off, struct fuse_file_info *fi, enum fuse_readdir_flags flags) {

	filler(buf, ".", NULL, 0, (enum fuse_fill_dir_flags)0);
	filler(buf, "..", NULL, 0, (enum fuse_fill_dir_flags)0);

	return 0;
}

int fs_rmdir (const char *path) {
	struct metadata m;
	char* nobitmap = "0";
	char* temp = (char*)path;

	int offset = inode_finder(temp);
	pread(fd,m,sizeof(m), offset);

	if(m.conut != 0) {

		printf("directory is not empty!!!!!!"); }

	else{

		string P_path = path;
		string my_name = path;

		if(path == '/') {

			P_path = path;
			my_name = path;
			root_inumber = inode_base_ids;
		}
		else {
			int size = P_path.size();
			int index;
			for(int i = size; i >= 0 ; i--) {
				if(P_path[i] == '/') {
					index = i;
					break;
				}

			}

			P_path.replace(index,size,"");
			my_name.replace(0,index+1,"");


		}

		struct metadata p_meta;
		struct dir_entry e;
		int p_id = inode_finder(path);

		pread(fd,&p_meta,sizeof(p_meta),p_id);
		pread(fd,&e,sizeof(e),p_meta.data_ptr);
		P_meta.count--;
		P_meta.size--;

		//bitmap free allocation please.
		inode_bitmap_ofs = inode_bitmap_base_idx + ((p_id - inode_base_idx)/INODE_SIZE);
		int nw1 = pwrite(fd,nobitmap,BITMAP_SIZE,(off_t) inode_bitmap_ofs);

		for(int  i = 0; i< p_meta.data_ptr.size(); i++) {

			int cur_ptr = p_meta.data_ptr.begin() + i;
			data_bitmap_ofs = data_bitmap_base_idx + (( cur_ptr - inode_base_idx)/BLOCK_SIZE);
			int nw2 = pwrite(fd,nobitmap,BITMAP_SIZE,(off_t) data_bitmap_ofs);

		}

		map<string,int>::iterator it;
		for(it = map.begin(); it != map.end(); it++) {
			if(it == e.entry.find(my_name)) {
				e.entry.erase(my_name);
				break;
			}
		}
	}

	return 0;
}

int fs_releasedir (const char *path, struct fuse_file_info *fi) {
	int close_dir_inumber = inode_finder("path");
	//close implement

	//
	return 0;
}

int fs_fsyncdir (const char *path, int datasync, struct fuse_file_info *fi) {
	return 0;
}
