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

    return 0;
}

int fs_utimens (const char *path, const struct timespec ts[2], struct fuse_file_info *fi) {

	return 0;

}

int fs_chmod (const char *path, mode_t mode, struct fuse_file_info *fi) {

    return 0;
}

int fs_chown (const char *path, uid_t uid, gid_t gid, struct fuse_file_info *fi) {
    
    return 0;
}

int fs_rename (const char *oldpath, const char *newpath, unsigned int flags) {
    
    return 0;
}

int fs_access (const char *path, int mask) {

	return 0;
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
