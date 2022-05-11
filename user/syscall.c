#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"

void test_cas() {
    for (int i = 0; i < 2; i++) {
        int parent_pid = getpid();
        fork();
        if(getpid()!=parent_pid){
            printf("ny pid = %d\n",getpid());
        }
    }
}

int
main(int argc, char *argv[])
{
    test_cas();
    exit(0);
}