/***************************************************
 * Module name: syscall.h
 *
 * First written on 16/3/2017 by Prithpal Sooriya.
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
 * @name handle_close
 * @return void
 * @param int fd: the file descriptor for a particular file
 * @date 15/2/2017
 * @details closes a file from a given file descriptor
**************************************************/
static void handle_close(int fd);

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
 * @name handle_read
 * @return int : amount read
 * @param int fd: the file descriptor for a particular file
 * @param const void *buffer: the buffer contains the data to read.
 * @param unsigned int length: the length of (or up to) the buffer to read
 * @date 2/3/2017
 * @details reads data to a file
**************************************************/
static int handle_read(int fd, void *buffer, unsigned size);

#endif /* userprog/syscall.h */
