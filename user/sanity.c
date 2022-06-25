#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"


void
writeBlocks(int fd, int numBlocks, char* buf){
    int i;
    int w;
    int pcount = 0;
    for(i = 0; i < numBlocks; i++, pcount++){
    w = write(fd, buf, sizeof(buf));
    
    if(w < 0){
        printf("failed writing direct block number %d\n", i);
        exit(1);
    }
  }
}


void
main(int argc, char *argv[])
{
  int fd;
  char buf[1024];
  int i;
  char* name = "sanityTest.txt";
  for(i=0; i<1024; i++){
      buf[i] = '0';
  }
  fd = open(name, O_CREATE | O_RDWR);
  if(fd < 0){
      printf("failed opening file\n");
      exit(1);
  }
  writeBlocks(fd, 12, buf);
  printf("finished writing 12KB (direct)\n");
  writeBlocks(fd, 256, buf);
  printf("finished writing 268KB (single indirect)\n");
  writeBlocks(fd, 256*256, buf);
  printf("finished writing 10MB (double indirect)\n");
  unlink(name);
  close(fd);
  exit(0);
}