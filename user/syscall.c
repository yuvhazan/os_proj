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
    for (int i = 0; i < 1; i++) {
        if(fork()!=0){
            printf("cpu_id=%d\n",get_cpu());
        }
    }
}

int
main(int argc, char *argv[])
{
    test_cas();
    set_cpu(5);
    test_cas();
    exit(0);
}