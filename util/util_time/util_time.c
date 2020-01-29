#include <time.h>

double _calc_time_span(struct timespec start_time) {
	struct timespec curr_time;
	clock_gettime(CLOCK_REALTIME, &curr_time);
	unsigned long long start, end; 
	start = start_time.tv_sec * 1000000000L + start_time.tv_nsec;
	end = curr_time.tv_sec * 1000000000L + curr_time.tv_nsec;
	return (double)(end - start) / 1000000000L;
}

double _calc_utime_span(struct timespec start_time) {
	struct timespec curr_time;
	clock_gettime(CLOCK_REALTIME, &curr_time);
	unsigned long long start, end; 
	start = start_time.tv_sec * 1000000000L + start_time.tv_nsec;
	end = curr_time.tv_sec * 1000000000L + curr_time.tv_nsec;
	return (double)(end - start) / 1000L;
}

