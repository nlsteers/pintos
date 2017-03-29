/***************************************************
 * Module name: process.h
 *
 * First written on 22/3/2017
 *
 * Module Description:
 * This contains all the prototypes and definitions for process.c
***************************************************/
#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

/***************************************************
 * Include section:
 * #include "threads/thread.h" : contains thread structures and functions
 * #include <debug.h> : default, was already included
 * #include <inttypes.h> : default, was already included
 * #include <round.h> : default, was already included
 * #include <stdio.h> : standard i/o
 * #include <stdlib.h> : standard library
 * #include <string.h> : for string functions (strlcpy)
 * #include "userprog/gdt.h" : default, was already included
 * #include "userprog/pagedir.h" : default, was already included. Used to allocate pages/etc
 * #include "userprog/tss.h" : default, was already included
 * #include "filesys/directory.h" : default, was already included
 * #include "filesys/file.h" : used for file struct and functions
 * #include "filesys/filesys.h" : used for filesys functions
 * #include "threads/flags.h" : default, was already included
 * #include "threads/init.h" : default, was already included
 * #include "threads/interrupt.h" : default, was already included
 * #include "threads/palloc.h" : default, was already included. To get and free
 * #include "threads/malloc.h" : default, was already included. Allocate memory
 * #include "threads/vaddr.h" : default, was already included
 * #include "threads/synch.h" : used to access semaphores and their functions
 * #include "lib/string.h"  : used to access string functions.
***************************************************/
#include "threads/thread.h"
#include <debug.h>
#include <inttypes.h>
#include <round.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "userprog/gdt.h"
#include "userprog/pagedir.h"
#include "userprog/tss.h"
#include "filesys/directory.h"
#include "filesys/file.h"
#include "filesys/filesys.h"
#include "threads/flags.h"
#include "threads/init.h"
#include "threads/interrupt.h"
#include "threads/palloc.h"
#include "threads/malloc.h"
#include "threads/vaddr.h"
#include "threads/synch.h"
#include "lib/string.h"

/***************************************************
 * Struct section:
***************************************************/

/**************************************************
 * @name struct address
 * @attribute char* argument: the argument for the program
 * @attribute char* pointer: the pointer location for this argument
**************************************************/
struct address {
    char* argument;
    char* pointer;
};

/***************************************************
 * Prototypes section:
***************************************************/

/**************************************************
 * @name process_execute
 * @return tid_t : returns the thread id
 * @param const char *file_name: the name of the program and its arguments
 * @date 22/3/2017 - the latest edit
 * @details called to execute a program given to it. It will return the ID for the
 *    child.
**************************************************/
tid_t process_execute (const char *file_name);

/**************************************************
 * @name process_wait
 * @return int : returns the status of the child thread or -1 if there were any issues.
 * @param void
 * @date 22/3/2017 - the latest edit
 * @details used to allow the parent to wait till a child is finished.
**************************************************/
int process_wait (tid_t);

/**************************************************
 * @name process_exit
 * @return void
 * @param void
 * @date N/A
 * @details used to end a process
**************************************************/
void process_exit (void);

/**************************************************
 * @name process_activate
 * @return void
 * @param void
 * @date N/A
 * @details activate a process - this was code already supplied
**************************************************/
void process_activate (void);

/**************************************************
 * @name extract_program_args
 * @return void
 * @param const char* file_name: the program name and its arguments
 * @param void **esp: the pointer to the stack pointer used to push the contents for loading
 * @date 15/2/2017
 * @details used to push the contents from load onto the stack pointer in the correct order.
**************************************************/
void extract_program_args (const char* file_name, void **esp);

#endif /* userprog/process.h */
