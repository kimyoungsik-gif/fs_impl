# fs_impl

Usage:

reference url
	https://libfuse.github.io/doxygen/structfuse__operations.html

generate fs_impl
	1) compile fs_impl
		fs_impl$ make

excute fs_impl
	./fs_impl <mount_point> -o default_permissions,allow_others

excute fs_impl with debug mode (we can use printf)
	./fs_impl <mount_point> -o default_permissions,allow_others,debug	
