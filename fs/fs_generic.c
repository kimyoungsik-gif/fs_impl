#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "fs_generic.h"
#include "metadata.h"
#include "fs_dir.h"
#include "fs_file.h"

struct monitor *global_monitor;

int fs_getattr (const char *path, struct stat *stbuf, struct fuse_file_info *fi) {
	struct metadata meta;
	int ind = inode_finder(path);	

	int nr1 = pread(fd, &meta, sizeof(meta), (off_t) ind);
	stbuf->st_mode = meta.mode;
	stbuf->st_nlink = meta.nlink;
	stbuf->st_uid = meta.uid;
	stbuf->st_gid = meta.gid;
	stbuf->st_size = meta.size;
	stbuf->st_ino = meta.ino;

	return -ENOENT;
}

int fs_utimens (const char *path, const struct timespec ts[2], struct fuse_file_info *fi) {

	return 0;

}

int fs_chmod (const char *path, mode_t mode, struct fuse_file_info *fi) {
	struct metadata meta;
	int ind = inode_finder(path);	

	int nr1 = pread(fd, &meta, sizeof(meta), (off_t) ind);
	meta.mode = mode;
	int nw1 = pwrite(fd, &meta, sizeof(meta), (off_t) ind);

	return 0;
}

int fs_chown (const char *path, uid_t uid, gid_t gid, struct fuse_file_info *fi) {
	struct metadata meta;
	int ind = inode_finder(path);	
	fi->fh = ind;

	int nr1 = pread(fd, &meta, sizeof(meta), (off_t) ind);
	meta.uid = uid;
	meta.gid = gid;
	int nw1 = pwrite(fd, &meta, sizeof(meta), (off_t) ind);
	return 0;
}

int fs_rename (const char *oldpath, const char *newpath, unsigned int flags) {
	//oldpath_parentname_myname
	string Po_path = oldpath;
	string old_name = oldpath;
	if(oldpath == "/") {
		Po_path = oldpath;
		old_name = oldpath;
		root_inumber = inode_base_idx;
	}
	else {
		int size = Po_path.size();
		int index;
		for (int i = size; i >= 0; i--){
			if(Po_path[i] == "/"){
				index = i;
				break;
			}
		}

		Po_path.replace(index,size,"");
		old_name.replace(0,index+1,"");
	}

	//newpath_parentname_myname
	string Pn_path = newpath;
	string new_name = newpath;
	if(newpath == "/") {
		Pn_path = newpath;
		new_name = newpath;
		root_inumber = inode_base_idx;
	}
	else {
		int size = Pn_path.size();
		int index;
		for (int i = size; i >= 0; i--){
			if(Pn_path[i] == "/"){
				index = i;
				break;
			}
		}

		Pn_path.replace(index,size,"");
		new_name.replace(0,index+1,"");
	}

	//name change
	struct metadata p_meta;
<<<<<<< HEAD
	struct dir_entry p_e;
	int p_id = inode_finder(Pn_path);	
	int ind = indode_finder(oldpath);
=======
    struct dir_entry p_e;
    int p_id = inode_finder(Pn_path);	
	int ind = inode_finder(oldpath);
>>>>>>> 8b3523f89cd0020e4d5bb1003cb7ba04a26d77cc

	pread(fd,&p_meta,sizeof(p_meta),p_id);
	pread(fd,&p_e,sizeof(p_e),p_meta.data_ptr);

	map<string,int>::iterator it;
<<<<<<< HEAD
	for(it = map.begin(); it!=map.end(); it++){
		if(it == e.entry.find(my_name)){
			e.entry.erase(new_name);
			e.entry.push_back(make_pair(new_name,ind));
=======
    for(it = p_e.begin(); it!=p_e.end(); it++){
	    if(it == p_e.entry.find(old_name)){
	        p_e.entry.erase(old_name);
            p_e.entry.push_back(make_pair(new_name,ind));
>>>>>>> 8b3523f89cd0020e4d5bb1003cb7ba04a26d77cc
			break;
		}
	}
	int nw1 = pwrite(fd, &p_e, sizeof(p_e), (off_t) p_meta.data_ptr);

	return 0;
}

int fs_access (const char *path, int mask) {

	struct metadata meta;
	int ind = inode_finder(path);	

	int nr1 = pread(fd, &meta, sizeof(meta), (off_t) ind);
	if (mask == meta.mode){
		return 0;
	}

	return -1;
}

int fs_symlink (const char *from, const char *to) {

	return 0;
}

int fs_readlink (const char *path, char *buf, size_t size) {

	return 0;
}


void *fs_init (struct fuse_conn_info *conn, struct fuse_config *cfg) {
	cfg->use_ino = 1;
	cfg->kernel_cache = 1;
	cfg->negative_timeout = 120;
	cfg->attr_timeout = 240;
	cfg->entry_timeout = 240;
	conn->want |= FUSE_CAP_WRITEBACK_CACHE;

#ifdef MONITOR
	monitor_init(&global_monitor);
#endif
	// file system stotrage space
	if((fd = open("disk", O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1 ) {
		printf("File open error.\n");
		return NULL;
	}

	//disk offset init
	dioff.inode_bitmap_offset = 0;
	dioff.data_bitmap_offset = 0;
	dioff.inode_offset = 0;
	dioff.data_block_offset = 0;


	fs_mkdir("/", 0755);

	return NULL;

}

void fs_destroy (void *private_data) {

#ifdef MONITOR
	monitor_print(global_monitor);
	monitor_destroy(global_monitor);
#endif

	return;
}
