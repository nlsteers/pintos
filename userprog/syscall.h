/***************************************************
 * Module name: syscall.h
 *
 * First written on 16/3/2017
 *
 * Module Description:
 * This contains all the prototypes and definitions for
 * syscall.c
***************************************************/
#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

/***************************************************
 * Include section:
 * #include <stdio.h> : standard library
 * #include <syscall-nr.h> : for syscall header, containing the definitions for the calls
 * #include "threads/interrupt.h" : for interrupt function when system call
 * #include "threads/thread.h" : to include the thread struct and other structs from here
 * #include "threads/init.h" : initialise/start
 * #include "threads/synch.h" : for locks, semaphores and their corresponding functions
 * #include "filesys/off_t.h" : to use the definition for off_t
 * #include "filesys/file.h" : to use the file functions
 * #include "filesys/filesys.h" : to use the filesys functions
 * #include "threads/malloc.h" : to allocate memory to structs
 * #include "devices/input.h" :
 * #include "devices/shutdown.h" : for shutdown_power_off
 * #include "userprog/process.h" : for process_execute and process_wait
 * #include "threads/vaddr.h" :
***************************************************/
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
#include "threads/vaddr.h"

/***************************************************
 * Defines section:
 * #define ARG_CODE 0
 * #define ARG_1 4
 * #define ARG_2 8
 * #define ARG_3 12
 * These are for the stating the locations on the stack
 *   of where the corresponding argument are located.
***************************************************/
#define ARG_CODE 0
#define ARG_1 4
#define ARG_2 8
#define ARG_3 12

/**************************************************
 * @name syscall_init
 * @return void
 * @param void
 * @date N/A
 * @details called and uses code 0x30 to indicate
 *    that a system call has been made. This will then call
 *    the syscall_handler (which our system call implementations
 *    are made).
**************************************************/
void syscall_init (void);

/**************************************************
 * @name load_stack
 * @return uint32_t: for the value stored inside stack
 * @param struct intr_frame *f: to get the esp.
 * @param int offset: to get particular value from the offset
 * @date N/A
 * @details gets particular value from the offset given on the stack
**************************************************/
static uint32_t load_stack(struct intr_frame *f, int offset);

/**************************************************
 * @name handle_halt
 * @return void
 * @param void
 * @date 15/2/2017
 * @details halts/shutdowns thus terminating Pintos.
 * @note we only call shutdown_power_off() from /threads/init.h
**************************************************/
static void handle_halt();

/**************************************************
 * @name handle_exit
 * @return void
 * @param int exit_code: the exit_code for a thread
 * @date 15/2/2017
 * @details exits a thread
 * @note accomodate closing if a child...
**************************************************/
static void handle_exit (int exit_code);

/**************************************************
 * @name handle_close
 * @return void
 * @param int fd: the file descriptor for a particular file
 * @date 15/2/2017
 * @details closes a file from a given file descriptor
**************************************************/
static void handle_close(int fd);

/**************************************************
 * @name handle_create
 * @return bool : for success creating file or failure creating file
 * @param const char *file_name: the file name for the new file to create
 * @param unsigned initial_size: the initial_size of the new file to create
 * @date 15/2/2017
 * @details creates a file from the given filename and of initiail size given
**************************************************/
static bool handle_create (const char *file_name, unsigned initial_size);

/**************************************************
 * @name handle_remove
 * @return bool : for success or failure when attempting to remove file
 * @param const char *file_name: the file name of the file to remove
 * @date 15/2/2017
 * @details creates a file from the given filename and of initiail size given
**************************************************/
static bool handle_remove (const char *file_name);

/**************************************************
 * @name handle_open
 * @return int : returns a non negative file descriptor. -1 if could not be opened.
 * @param char *file_name: the file name of the file to open
 * @date 22/2/2017
 * @details attempts to open a file from a given filename, returns a file decriptor
 *    if open was successful. Returns -1 if the file could not be opened.
 * @note File descriptors 1 and 2 are reserved for STDIN_FILENO and STDOUT_FILENO
 *    standard input and output.
 *    We have also stored the files as a list inside the each thread, and new file
 *    descriptors are created by looping through the list till finding a empty location (index+2).
**************************************************/
static int handle_open(char *file_name);

/**************************************************
 * @name handle_filesize
 * @return int : returns the size, in bytes, the size of the file from the given
 *    file descriptor.
 * @param int fd: the file descriptor for the corresponding file.
 * @date 22/2/2017
**************************************************/
static int handle_filesize (int fd);

/**************************************************
 * @name handle_read
 * @return int : amount read
 * @param int fd: the file descriptor for a particular file
 * @param const void *buffer: the buffer contains the data to read.
 * @param unsigned int length: the length of (or up to) the buffer to read
 * @date 22/2/2017
 * @details reads data to a file
**************************************************/
static int handle_read(int fd, void *buffer, unsigned size);

/**************************************************
 * @name handle_write
 * @return int : return the amount of bytes actually written
 * @param int fd: the file descriptor for a particular file
 * @param const void *buffer: the buffer contains the data to write.
 * @param unsigned int length: the length of (or up to) the buffer to write
 * @date 22/2/2017
 * @details writes data to a file
 * @note we have edited the original code to support the extra write function
 *    of Pintos
**************************************************/
static int handle_write(int fd, const void *buffer, unsigned int length);

/**************************************************
 * @name handle_seek
 * @return void
 * @param int fd: the file descriptor for a particular file
 * @param unsigned position: the position, in bytes, to start reading from.
 * @date 22/2/2017
 * @details changes the next byte to read (or write to) in an open file (from
 *    the corresponding file descriptor) from the position (in bytes) given.
 * @note starting position of the file is 0.
**************************************************/
static void handle_seek(int fd, unsigned position);

/**************************************************
 * @name handle_tell
 * @return off_t : returns the position, in bytes, of the next byte to read or to
 *    be written to.
 * @param int fd: the file desc********riptor for a particular file
 * @date 22/2/2017
 * @details returns the position of the next byte to be read/written in an open file
 *    from the corresponding file descriptor given.
 * @note starting position of the file is 0.
**************************************************/
static off_t handle_tell(int fd);

/**************************************************
 * @name handle_exec
 * @return tid_t : returns the new process id. Must return -1 if the program cannot
 *    be loaded or ran for any reason.
 * @param const char *file_name: the file name for the new process to run.
 *    E.g. "echo x y"
 * @date 1/3/2017
 * @details This returns the process ID for the new process. It will return -1
 *    if the program cannot be loaded or ran. A parent process cannot return from
 *    handle_exec until it knows if a child process is successfully loaded and can be ran.
 * @note All the code for handle_exec is not contained in /userprog/syscall.c, but
 *    instead contained in /userprog/process.c (particularly in the process_execute and start_process function).
**************************************************/
static tid_t handle_exec(const char *file_name);

/**************************************************
 * @name handle_wait
 * @return int : returns the child's status if the child successfully is ended.
 *    Returns -1 if there is an error (described more below).
 * @param int child_tid: is the childs process ID to be used to indicate what child
 *    the parent thread is waiting fget_fileor.
 * @date 10/3/2017
 * @details This returns the child thread's status if successfully waited, but also
 *    returns -1 if there is an error.
 *    These errors can occur by:
 *    - child_tid does not refer to a direct child of the calling process. (bad_pid test)
 *    - a child_tid that is alread waited, cannot be on wait twice.
 * @note all the code for handle_wait is not contained in /userprog/syscall.c, but
 *    instead contained in /userprog/process.c (particularly in the process_wait function)
**************************************************/
static int handle_wait(int child_tid);

/**************************************************
 * @name get_file
 * @return struct file_info* : returns a pointer to our struct file_info of the
 *    file descriptor given.
 * @param int fd: the file descriptor of file we looking for.
 * @date 22/3/2017
 * @details This returns the corresponding file_info from the list (inside the thread)
 *    by the given corresponding file descriptor. It will return NULL if a file could not be found.
 * @note This was added later in the code to allow our code to be more clean and understandable.
**************************************************/
static struct file_info* get_file (int fd);

#endif /* userprog/syscall.h */
