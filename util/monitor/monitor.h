#ifndef __MONITOR__
#define __MONITOR__

#include <sys/time.h>
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define MONITOR_PATH "/home/koo/src/koofs/output/monitor.out"
#define MDR		0
#define MDW		1
#define MDD		2
#define DR		3
#define DW		4
#define DD		5
#define FS_READ 6
#define DEV_GET 7
#define FS_READ_WAIT 8
#define FS_READ_1 9
#define FS_READ_2 10
#define FS_READ_3 11
#define FS_READ_4 12

struct io_count {
	uint64_t md_read;
	uint64_t md_write;
	uint64_t md_delete;
	uint64_t d_read;
	uint64_t d_write;
	uint64_t d_delete;
};

struct time_measure {
	uint64_t fs_read_total;
	uint64_t fs_read_wait_total;
	uint64_t fs_read_1_total;
	uint64_t fs_read_2_total;
	uint64_t fs_read_3_total;
	uint64_t fs_read_4_total;
	uint64_t dev_get_total;
};

struct monitor {
	FILE *output_file;
	struct io_count io_cnt;
	struct time_measure total_time;
	pthread_mutex_t mutex;
	uint64_t seq_num;
};


void monitor_init (struct monitor **mon);
void monitor_update (struct monitor *mon, int type, int cnt);
void monitor_update_time (struct monitor *mon, int type, double elapsed);
void monitor_print (struct monitor *mon);
void monitor_destroy (struct monitor *mon);

#endif
