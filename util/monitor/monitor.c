#include <signal.h>
#include <unistd.h>
#include "monitor.h"

extern struct monitor *global_monitor;


void monitor_alarm (int signo) {
	monitor_print (global_monitor);
	alarm(5);
	return;
}

void register_monitor_alarm () {
	struct sigaction act;

	act.sa_handler = monitor_alarm;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGALRM, &act, NULL);

	alarm(10);

	return;
}

void monitor_init (struct monitor **mon) {
	*mon = (struct monitor *)malloc(sizeof(struct monitor));
	(*mon)->io_cnt.md_read = 0;
	(*mon)->io_cnt.md_write = 0;
	(*mon)->io_cnt.md_delete = 0;
	(*mon)->io_cnt.d_read = 0;
	(*mon)->io_cnt.d_write = 0;
	(*mon)->io_cnt.d_delete = 0;
	(*mon)->total_time.fs_read_total = 0;
	(*mon)->total_time.fs_read_wait_total = 0;
	(*mon)->total_time.fs_read_1_total = 0;
	(*mon)->total_time.fs_read_2_total = 0;
	(*mon)->total_time.fs_read_3_total = 0;
	(*mon)->total_time.fs_read_4_total = 0;
	(*mon)->total_time.dev_get_total = 0;
	(*mon)->output_file = fopen(MONITOR_PATH, "w");
	(*mon)->seq_num = 1;
	pthread_mutex_init(&((*mon)->mutex), NULL);
	register_monitor_alarm();
}

void monitor_update (struct monitor *mon, int type, int cnt) {
	uint64_t cnt64 = cnt;
	//pthread_t self = pthread_self();
	//printf("monitor type %d cnt %d tid %d\n", type, cnt,self);
	switch(type) {
	case MDR:
		__sync_add_and_fetch(&mon->io_cnt.md_read, cnt64);
		break;
	case MDW:
		__sync_add_and_fetch(&mon->io_cnt.md_write, cnt64);
		break;
	case MDD:
		__sync_add_and_fetch(&mon->io_cnt.md_delete, cnt64);
		break;
	case DR:
		__sync_add_and_fetch(&mon->io_cnt.d_read, cnt64);
		break;
	case DW:
		__sync_add_and_fetch(&mon->io_cnt.d_write, cnt64);
		break;
	case DD:
		__sync_add_and_fetch(&mon->io_cnt.d_delete, cnt64);
		break;
	}
}

void monitor_update_time (struct monitor *mon, int type, double elapsed_time) {
	uint64_t _time = elapsed_time;
	switch (type) {
		case FS_READ:
			__sync_add_and_fetch(&mon->total_time.fs_read_total, _time);
			break;
		case DEV_GET:
			__sync_add_and_fetch(&mon->total_time.dev_get_total, _time);
			break;
		case FS_READ_WAIT:
			__sync_add_and_fetch(&mon->total_time.fs_read_wait_total, _time);
			break;
		case FS_READ_1:
			__sync_add_and_fetch(&mon->total_time.fs_read_1_total, _time);
			break;
		case FS_READ_2:
			__sync_add_and_fetch(&mon->total_time.fs_read_2_total, _time);
			break;
		case FS_READ_3:
			__sync_add_and_fetch(&mon->total_time.fs_read_3_total, _time);
			break;
		case FS_READ_4:
			__sync_add_and_fetch(&mon->total_time.fs_read_4_total, _time);
			break;

	}
}


void monitor_print (struct monitor *mon) {
	fprintf(mon->output_file, "STAT #%ld ----- IO COUNT -----\n", mon->seq_num);
	fprintf(mon->output_file, "Metadata  Read    Count: \t%zu\n", mon->io_cnt.md_read);
	fprintf(mon->output_file, "Metadata  write   Count: \t%zu\n", mon->io_cnt.md_write);
	fprintf(mon->output_file, "Metadata  delete  Count: \t%zu\n", mon->io_cnt.md_delete);
	fprintf(mon->output_file, "data      Read    Count: \t%zu\n", mon->io_cnt.d_read);
	fprintf(mon->output_file, "data      write   Count: \t%zu\n", mon->io_cnt.d_write);
	fprintf(mon->output_file, "data      delete  Count: \t%zu\n", mon->io_cnt.d_delete);
	fprintf(mon->output_file, "STAT #%ld ----- TOTAL TIME -----\n", mon->seq_num);
	fprintf(mon->output_file, "FS_READ        : \t%zu\n", mon->total_time.fs_read_total);
	fprintf(mon->output_file, "FS_READ_WAIT   : \t%zu\n", mon->total_time.fs_read_wait_total);
	fprintf(mon->output_file, "FS_READ_1       : \t%zu\n", mon->total_time.fs_read_1_total);
	fprintf(mon->output_file, "FS_READ_2       : \t%zu\n", mon->total_time.fs_read_2_total);
	fprintf(mon->output_file, "FS_READ_3       : \t%zu\n", mon->total_time.fs_read_3_total);
	fprintf(mon->output_file, "FS_READ_4       : \t%zu\n", mon->total_time.fs_read_4_total);
	fprintf(mon->output_file, "DEV_GET        : \t%zu\n", mon->total_time.dev_get_total);
	fprintf(mon->output_file, "STAT #%ld ----- END -----\n\n", mon->seq_num++);
	//fprintf(mon->output_file, "pending_queue cnt: %ld\n, request_queue cnt: %ld\n", lfqueue_size(aio_hdlr->pending_queue), lfqueue_size(aio_hdlr->req_pool));
	fflush(mon->output_file);
}

void monitor_destroy (struct monitor *mon) {
	fflush(mon->output_file);
	fclose(mon->output_file);
	free(mon);
}
