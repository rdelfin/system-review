#define _GNU_SOURCE

#include <fcntl.h>
#include <dirent.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define MULTI_BUFFER_SIZE (1024*BUFFER_SIZE)


struct linux_dirent {
   long           d_ino;
   off_t          d_off;
   unsigned short d_reclen;
   char           d_name[];
};

int get_files_in_dir(const char* path, char* string_buf, uint64_t strbuf_len, char* pointer_buf[], uint64_t pointbuf_len);

int matches(const char* str, const char* pattern);

int main(int argc, char* argv[]) {
    int i, j;

    char string_buf[MULTI_BUFFER_SIZE];
    char* pointer_buf[BUFFER_SIZE];

    int elems = get_files_in_dir(".", string_buf, MULTI_BUFFER_SIZE, pointer_buf, BUFFER_SIZE);

    printf("Files: \n");

    for(i = 0; i < elems; i++)
        printf("\t%s\n", pointer_buf[i]);

    printf("\nGlobs:\n");

    for(i = 1; i < argc; i++) {   
        printf("\"%s\":\n", argv[i]);
        
        for(j = 0; j < elems; j++)
            if(matches(argv[i], pointer_buf[j]))
                printf("\t%s\n", pointer_buf[j]);
    }
}

int matches(const char* str, const char* pattern) {
    return strcmp(str, pattern) == 0;
}

int get_files_in_dir(const char* path, char* string_buf, uint64_t strbuf_len, char* pointer_buf[], uint64_t pointbuf_len) {
    int fd;
    struct stat dir_stat;
    char dir_buf[BUFFER_SIZE];
    int nread;
    int offset;
    struct linux_dirent* dirent;
    uint64_t strbuf_offset = 0, pointbuf_offset = 0;

    if((fd = open(path, O_RDONLY | O_DIRECTORY)) < 0) {
        fprintf(stderr, "Error opening directory %s.\n", path);
        return -1;
    }

    if(fstat(fd, &dir_stat) < 0) {
        fprintf(stderr, "Error getting status for directory %s.\n", path);
        return -1;
    }
    
    if(!S_ISDIR(dir_stat.st_mode)) {
        fprintf(stderr, "The path \"%s\" provided is not a directory.", path);
        return -1;
    }

    nread = syscall(SYS_getdents, fd, dir_buf, BUFFER_SIZE);

    for(offset = 0; offset < nread; offset += dirent->d_reclen) {
        dirent = (struct linux_dirent*)(dir_buf + offset);
        int len = strlen(dirent->d_name);

        // Skip elements that would go over buffer
        if(strbuf_offset + len + 1 > strbuf_len || pointbuf_offset + 1 > pointbuf_len)
            continue;
        
        pointer_buf[pointbuf_offset] = string_buf + strbuf_offset;
        strncpy(string_buf + strbuf_offset, dirent->d_name, len+1);
        
        strbuf_offset += len+1;
        pointbuf_offset++;
    }

    return pointbuf_offset;
}
