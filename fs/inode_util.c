#include "fs_main.h"
#include <string.h>

using namespace std;

int inode_finder(char* path) {

        struct dir_entry cur_dir_entry;
        struct metadata cur_m;

        int cur_dir_inode =  root_inumber;

        char* result;
        result = strtok(path,"/");

        map<string,int>::iterator it;

        while(result != NULL) {

                pread(fd,(char*)&cur_m, sizeof(cur_m), cur_dir_inode);

                bool search_dir = false;

                for(int i = 0; i < cur_m.data_ptr.size(); i++) {
                        int cur_ptr = *cur_m.data_ptr.begin() + i;
                        pread(fd, (char*)&cur_dir_entry,BLOCK_SIZE , cur_ptr);
                        it = cur_dir_entry.entry.find(result);

                        if(it != cur_dir_entry.entry.end()) {
                                cur_dir_inode = it->second;
                                search_dir = true;
                                break;
                        }
                }

                if(search_dir == false) {
                        printf("error : no such directory\n");
                        return 0;
                }



                result = strtok(NULL, "/");
        }

        return cur_dir_inode;
}
