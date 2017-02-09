#include <stdio.h>
#include <syscall.h>
#include <string.h>

int main(int argc , char *argv[])
{
    create("myfile",100);
    write(1,"hello\n",6);
    char *buffer = (char*)malloc(10);
    read(0,buffer,6);

    exit(5);
}