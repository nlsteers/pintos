#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"
#include "filesys/off_t.h"
#include "filesys/file.h"
#include "filesys/filesys.h"
#include "threads/malloc.h"
#include "devices/input.h"
#include "devices/shutdown.h"
#include "userprog/process.h"

#define ARG_CODE 0
#define ARG_1 4
#define ARG_2 8
#define ARG_3 12

static void syscall_handler(struct intr_frame *);
static struct file_info* get_file (int fd);

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

static void handle_exit (int exit_code){
  struct thread *cur = thread_current ();
  cur->exit_code = exit_code;
  thread_exit();
}

//static int i = 3;
static int handle_read(int fd, void *buffer, unsigned size) {
  struct thread *cur = thread_current ();
  //struct file* f = cur->files;
  struct list_elem *e;
  struct file_info *fi;

    if(fd == STDIN_FILENO) {
        int i;
        for(i = 0; i < (int)size; i++) {
          *(uint8_t *)(buffer+i) = input_getc();
        }
        return size;
    }
    if(fd == STDOUT_FILENO) {
      handle_exit(-1);
    }
    fi = get_file(fd);
    if (fi == NULL){
      handle_exit(-1);
    }
    int bytes_read_fr = file_read(fi->fp, buffer, size);
    return bytes_read_fr;


}

static struct file_info* get_file (int fd){

  struct thread *cur = thread_current ();
  //struct file* f = cur->files;
  struct list_elem *e;
  struct file_info *fi;



  for(e = list_begin(&cur->files); e != list_end(&cur->files); e = list_next(e)){
    fi = list_entry(e, struct file_info, fpelem);

    if(fi->fd == fd) {
      return fi;
    }
  }
  return NULL;
}
//
static int handle_open(char* file_name) {
  struct file* file = filesys_open(file_name);
  struct thread *cur = thread_current ();
  int fd;
  struct list_elem filepointer;

  if (file == NULL){
      fd = -1;
      //exit_handler(fd);
      return fd;
  }

  struct file_info *fi =   malloc(sizeof(struct file_info));
  if(list_size(&cur->files) == 0) {
    fd = 2; //only for init/first file --> will need to change when creating more files.
    fi->fd = fd;
    fi->fp = file;

    list_push_back(&cur->files, &fi->fpelem);
  }

  return fd;
}


static void syscall_handler(struct intr_frame *f) {
    int code = (int) load_stack(f, ARG_CODE);
    switch (code) {
        case SYS_HALT:{
            //DONE
            shutdown_power_off();
        }
        case SYS_EXIT: {
          //DONE
          handle_exit(load_stack(f, ARG_1));
          break;
        }
        case SYS_EXEC: {
            printf("EXEC Incomplete\n");
            //get thread
            struct thread *cur = thread_current ();
            //process execute to make a child
            //process_execute();
        }
        case SYS_WAIT: {

        }
        case SYS_CREATE: {
            printf("CREATE Incomplete\n");
        }
        case SYS_REMOVE: {
            printf("REMOVE Incomplete\n");
        }
        case SYS_OPEN: {
            //DOESN'T WORK

            //NEW CODE -------------
            char *fileName = (char *)load_stack(f, ARG_1);
            f->eax = handle_open(fileName);
            // ---------------------
            break;
        }
        case SYS_FILESIZE: {
            printf("FILESIZE Incomplete\n");
        }
        case SYS_READ: {
            //printf("READ Incomplete");
            struct thread *cur = thread_current ();
            int result = handle_read(
                    (int) load_stack(f, ARG_1),
                    (void *) load_stack(f, ARG_2),
                    (unsigned) load_stack(f, ARG_3));
            //have to  do something....
            f->eax = result; //maybe
            break;
        }
        case SYS_WRITE: {
            int result = handle_write(
                    (int) load_stack(f, ARG_1),
                    (void *) load_stack(f, ARG_2),
                    (unsigned int) load_stack(f, ARG_3)); // set return value
            f->eax = result;
            break;
        }
        case SYS_SEEK: {
            printf("SEEK Incomplete\n");
        }
        case SYS_TELL: {
            printf("TELL Incomplete\n");
        }
        case SYS_CLOSE: {
            printf("CLOSE Incomplete\n");
        }
        default:
            printf("SYS_CALL (%d) not recognised\n", code);
            thread_exit();
    }
}
