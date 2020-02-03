#ifndef __FS_MAIN__
#define __FS_MAIN__

#define FUSE_USE_VERSION 31
#include "global_types.h"
#include "metadata.h"

#ifdef MONITOR
#include "monitor.h"
//extern struct monitor *global_monitor;
#endif

int inode_finder(char*);
#endif
