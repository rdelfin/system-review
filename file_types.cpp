#include <stdio.h>

#include <sys/stat.h>

int main(int argc, char* argv[]) {
  for(int i = 1; i < argc; i++) {
    struct stat stat_obj;

    if(lstat(argv[i], &stat_obj) < 0) {
      printf("There was an error reading file \"%s\"\n", argv[i]);
    } else {
      printf("%s: ", argv[i]);

      if(S_ISREG(stat_obj.st_mode))
        printf("Regular");
      else if(S_ISDIR(stat_obj.st_mode))
        printf("Directory");
      else if(S_ISCHR(stat_obj.st_mode))
        printf("Character Device");
      else if(S_ISBLK(stat_obj.st_mode))
        printf("Block Device");
      else if(S_ISFIFO(stat_obj.st_mode))
        printf("FIFO (aka Named Pipe)");
      else if(S_ISLNK(stat_obj.st_mode))
        printf("Symbolic (or Soft) Link");
      else if(S_ISSOCK(stat_obj.st_mode))
        printf("Socket");

      printf("\n");
    }
  }
}
