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
	char* temp = (char*)path;
	int ind = inode_finder(temp);	

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

	char* temp = (char*)path;
	return 0;

}

int fs_chmod (const char *path, mode_t mode, struct fuse_file_info *fi) {
	struct metadata meta;
	char* temp = (char*)path;
	int ind = inode_finder(temp);	

	int nr1 = pread(fd, &meta, sizeof(meta), (off_t) ind);
	meta.mode = mode;
	int nw1 = pwrite(fd, &meta, sizeof(meta), (off_t) ind);

	return 0;
}

int fs_chown (const char *path, uid_t uid, gid_t gid, struct fuse_file_info *fi) {
	struct metadata meta;
	char* temp = (char*)path;
	int ind = inode_finder(temp);	
	fi->fh = ind;

	int nr1 = pread(fd, &meta, sizeof(meta), (off_t) ind);
	meta.uid = uid;
	meta.gid = gid;
	int nw1 = pwrite(fd, &meta, sizeof(meta), (off_t) ind);
	return 0;
}

int fs_rename (const char *oldpath, const char *newpath, unsigned int flags) {
	//oldpath_parentname_myname
	char* oldp = (char*)oldpath;
	char* newp = (char*)newpath;
	string Po_path(oldp);
	string old_name(oldp);

	if(oldpath == "/") {
		Po_path = oldp;
		old_name = oldp;
		root_inumber = inode_base_idx;
	}
	else {
		int size = Po_path.size();
		int index;
		for (int i = size; i >= 0; i--){
			if(Po_path[i] == '/'){
				index = i;
				break;
			}
		}

		Po_path.replace(index,size,"");
		old_name.replace(0,index+1,"");
	}

	//newpath_parentname_myname
	string Pn_path(newp);
	string new_name(newp);
	if(newpath == "/") {
		root_inumber = inode_base_idx;
	}
	else {
		int size = Pn_path.size();
		int index;
		for (int i = size; i >= 0; i--){
			if(Pn_path[i] == '/'){
				index = i;
				break;
			}
		}

		Pn_path.replace(index,size,"");
		new_name.replace(0,index+1,"");
	}

	//name change
	char* temppp;
	strcpy(temppp,Pn_path.c_str());
	struct metadata p_meta;
	struct dir_entry temp_e;
    struct dir_entry p_e;
    int p_id = inode_finder(temppp);	
	int ind = inode_finder(oldp);

	pread(fd,&p_meta,sizeof(p_meta),p_id);
	
	for(int i = 0; i< p_meta.data_ptr.size();i++){
        pread(fd,&temp_e,BLOCK_SIZE,p_meta.data_ptr[i]);
		p_e.entry.insert(temp_e.entry.begin(),temp_e.entry.end());
    }


	map<string,int>::iterator it;
    for(it = p_e.entry.begin(); it != p_e.entry.end(); it++){
	    if(it == p_e.entry.find(old_name)){
	        p_e.entry.erase(old_name);
            p_e.entry.insert(make_pair(new_name,ind));
			break;
		}
	}

	int i = 0;
	for(it = p_e.entry.begin(); it != p_e.entry.end() ; it++){
        for(int j = 0; j < 16; j++){
			temp_e.entry.insert(*it);
		}
		pwrite(fd,&temp_e,BLOCK_SIZE,p_meta.data_ptr[i]);
		i++;
	}

	return 0;
}

int fs_access (const char *path, int mask) {

	struct metadata meta;
	char* temp = (char*) path;
	int ind = inode_finder(temp);	

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
