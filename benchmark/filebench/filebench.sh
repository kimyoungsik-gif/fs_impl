#!/bin/bash

#kvfs="kvfs_no_cache"
#kvfs="kvfs_buffer_dir"
#kvfs="kvfs_buffer_wb"
#kvfs="kvfs_buffer_wb_no_md"

#kvfs=kvfs_no_cache_page_cache
#kvfs=kvfs_caching_dir
#kvfs=kvfs_caching_wb
kvfs=koofs_100

kvfs_dir="/home/koo/src/koofs"
benchmark_dir="${kvfs_dir}/benchmark/filebench"
workload_dir="${benchmark_dir}/workloads/used"
workload_pre_dir="${benchmark_dir}/workloads/used_pre"

output_dir="${benchmark_dir}/result/kvfs"
output_file="${output_dir}/${kvfs}"
output_file_pre="${output_dir}/${kvfs}_pre"

monitor_file="../../output/monitor.out"

ext4_dir="/mnt/ext4"
fuse_dir="/mnt/fuse"

remove_data() {
	rm -rf $ext4_dir/*
	rm -rf $fuse_dir/*
	rm $monitor_file
}

flush() {
	sync
	echo 3 > /proc/sys/vm/drop_caches
}

print_monitor() {
	cat $monitor_file >> $output_file
}

print_monitor_pre() {
	cat $monitor_file >> $output_file_pre
}

init() {
	$kvfs_dir/$kvfs $fuse_dir
	sleep 1
}

destroy() {
	umount $fuse_dir
}

do_kvfs_benchmark() {
	for workload in ${workload_dir}/*
	do
		echo "=============================================="
		echo ${workload}
		/home/koo/format.sh 1
		sleep 10
		remove_data
		flush
		init
		echo "=============================================================================================" >> ${output_file}
		echo ${workload} >> ${output_file}
#		cat ${workload} >> ${output_file}
		filebench -f ${workload} >> ${output_file}
		sleep 30
		flush
		destroy
		sleep 30
		print_monitor
		sleep 200
	done
}

do_kvfs_benchmark_pre() {
	for workload in ${workload_pre_dir}/*
	do
		echo "=============================================="
		echo ${workload}
		/home/koo/format.sh 1
		sleep 10
		remove_data
		flush
		init
		echo "=============================================================================================" >> ${output_file_pre}
		echo ${workload} >> ${output_file_pre}
#		cat ${workload} >> ${output_file_pre}
		filebench -f ${workload} >> ${output_file_pre}
		sleep 30
		flush
		destroy
		sleep 30
		print_monitor_pre
		sleep 200
	done
}

do_kvfs_benchmark
