#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"

tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);
//void extract_program_args (const char* file_name, char* *program_args[10]);





#endif /* userprog/process.h */
