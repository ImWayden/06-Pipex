#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>   // open, close, read, write, access, dup, dup2, execve, fork, pipe, unlink
#include <fcntl.h>    // open
#include <stdlib.h>   // malloc, free
#include <stdio.h>    // perror
#include <string.h>   // strerror
#include <sys/types.h>  // wait, waitpid
#include <sys/wait.h>  // wait, waitpid

int main(int argc, char *argv[]) 
{

    char *command[] = {"sh", "-c", argv[1], NULL};
    execve("/bin/sh", command, NULL);

    perror("execve");
    exit(EXIT_FAILURE);

    return 0;
}