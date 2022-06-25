#include "kernel/param.h"
#include "kernel/fcntl.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path, int type)
{
  
  static char buf[DIRSIZ+1];
  char *p;

  int sym = (type == T_SYMLINK) ? 1:0;
  char name[MAXPATH];
  
  if(sym){
    readlink((const char*) path, name, MAXPATH);
  }

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;

  char* ar = "->";
  if(sym){
    memmove(p+strlen(p), ar, strlen(ar));
    memmove(p+strlen(p), name, strlen(name));
  }

  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
ls(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, O_SYMLINK_IGNORE)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  char buffer[MAXPATH];
  switch(st.type){
  case T_FILE:{
    printf("%s %d %d %l\n", fmtname(path,st.type), st.type, st.ino, st.size);
    break;
  }

  case T_DIR:{
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      if(st.type == T_SYMLINK){
        readlink(buf,buffer, MAXPATH);
      }
      printf("%s %d %d %d\n", fmtname(buf,st.type), st.type, st.ino, st.size);
    }
    break;
  }

  case T_SYMLINK:{
    printf("%s->%s %d %d %l\n", fmtname(path, st.type), buffer+4, st.type, st.ino, st.size);
    break;
  }
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    ls(".");
    exit(0);
  }
  for(i=1; i<argc; i++)
    ls(argv[i]);
  exit(0);
}
