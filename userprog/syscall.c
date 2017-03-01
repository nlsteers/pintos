#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"
#include "threads/synch.h"
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

static void handle_close(int fd) {
  struct file_info *fi;
  fi = get_file(fd);
  if(fi != NULL) {
    file_close(fi->fp);
    list_remove(&fi->fpelem);
    free(fi);
  }
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
  //struct thread *cur = thread_current (); --> unused
  //struct file* f = cur->files;
  //struct list_elem *e; --> unused

  struct file_info *fi;

    //Read the file (Standard In)
    if(fd == STDIN_FILENO) {
        int i;
        for(i = 0; i < (int)size; i++) {
          *(uint8_t *)(buffer+i) = input_getc();
        }
        return size;
    }
    //Check error if Standard Out
    if(fd == STDOUT_FILENO) {
      handle_exit(-1); //maybe need to return?
    }
    //Check if the file is NULL
    fi = get_file(fd);
    if (fi == NULL){
      handle_exit(-1); //maybe need to return?
    }
    //Read each bytes from file
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

static int handle_open(char* file_name) {

  struct file* file = filesys_open(file_name);
  struct thread *cur = thread_current ();
  int fd;
  //struct list_elem filepointer; --> unused

  if (file == NULL){
      fd = -1;
      //exit_handler(fd);
      return fd;
  }

  struct file_info *fi = malloc(sizeof(struct file_info)); //allocate size for file to open.

  fd = 2; //only for init/first file --> will need to change when creating more files.
  while(get_file(fd) != NULL) {
    fd++;
  }
    fi->fd = fd;
    fi->fp = file;
    list_push_back(&cur->files, &fi->fpelem); //push the file onto the back of the linked list.

  return fd;
}

static int handle_filesize (int fd)
{
  struct file_info *fi = get_file (fd);
  if( fi == NULL ) handle_exit(-1);
  return file_length (fi->fp);
}

static tid_t handle_exec (const char *file_name){
  return process_execute ((char *)file_name);;
}

static void syscall_handler(struct intr_frame *f) {
  //NOTE: If you want to know what the params are for these syscalls, look into lib/user/syscall.c.
  //NOTE: lib/user/syscall.c also includes return values (which inside here means storing into registers)
    int code = (int) load_stack(f, ARG_CODE);

    switch (code) {
        case SYS_HALT:{
          shutdown_power_off();
          break;
        }
        case SYS_EXIT: {
          handle_exit(load_stack(f, ARG_1));
          break;
        }
        case SYS_EXEC: {
          f->eax = handle_exec((const char *)load_stack(f, ARG_1));
          break;
        }
        case SYS_WAIT: {
          f->eax = process_wait((int) load_stack(f, ARG_1));
          break;
        }
        case SYS_CREATE: {
          printf("CREATE Incomplete\n");
          break;
        }
        case SYS_REMOVE: {
          printf("REMOVE Incomplete\n");
          break;
        }
        case SYS_OPEN: {
          char *fileName = (char *)load_stack(f, ARG_1);
          f->eax = handle_open(fileName);
          break;
        }
        case SYS_FILESIZE: {
          f->eax = handle_filesize((int) load_stack(f, ARG_1));
          break;
        }
        case SYS_READ: {
          int result = handle_read(
                  (int) load_stack(f, ARG_1),
                  (void *) load_stack(f, ARG_2),
                  (unsigned) load_stack(f, ARG_3));
          f->eax = result;
          break;
        }
        case SYS_WRITE: {
          //needs debugging
          int result = handle_write(
                  (int) load_stack(f, ARG_1),
                  (void *) load_stack(f, ARG_2),
                  (unsigned int) load_stack(f, ARG_3)); // set return value
          f->eax = result;
          break;
        }
        case SYS_SEEK: {
          printf("SEEK Incomplete\n");
          break;
        }
        case SYS_TELL: {
          printf("TELL Incomplete\n");
          break;
        }
        case SYS_CLOSE: {
          //printf("CLOSE\n"); //Debugging
          handle_close((int)load_stack(f, ARG_1));
          break;
        }
        default:
          printf("SYS_CALL (%d) not recognised\n", code);
          thread_exit();
    }
}
