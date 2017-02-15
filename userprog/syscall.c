#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"


#define ARG_CODE 0
#define ARG_1 4
#define ARG_2 8
#define ARG_3 12

static void syscall_handler(struct intr_frame *);

void syscall_init(void) {
    intr_register_int(0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static uint32_t load_stack(struct intr_frame *f, int offset) {

    // need to add check for valid address
    // i.e. user can do bad things
    return *((uint32_t*)(f->esp + offset));

}

static int handle_write(int fd, const void *buffer, unsigned int length) {
    if (fd == STDOUT_FILENO) {
        putbuf((const char *) buffer, (size_t) length);
    } else {
        printf("handle_write does not support fd output\n");
    }
    return length;
}

static void syscall_handler(struct intr_frame *f) {
    int code = (int) load_stack(f, ARG_CODE);
    switch (code) {
        case SYS_WRITE: {
            int result = handle_write(
                    (int) load_stack(f, ARG_1),
                    (void *) load_stack(f, ARG_2),
                    (unsigned int) load_stack(f, ARG_3)); // set return value
            f->eax = result;
            break;
        }
        case SYS_EXIT: {
            struct thread *cur = thread_current ();
            cur->exit_code = (int) load_stack(f, ARG_1);
            thread_exit();
        }
        case SYS_HALT:{
            shutdown_power_off();
        }
        default:
            printf("SYS_CALL (%d) not implemented\n", code);
            thread_exit();
    }
}
