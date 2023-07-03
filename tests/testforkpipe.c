#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int fd[2];
    int tab[] = {-1, 0, 3, 55, 4, 8};
    int tabsize = sizeof(tab) / sizeof(int);
    int sum = 0;
    int tmpsum = 0;
    int totalsum = 0;
    int id1;
    int id2;
    int start;
    int end;

    if(pipe(fd) == -1)
        return 1;
    id1 = fork();
    if(id1 == -1)
        return 2;
    if(id1)
    {
        id2 = fork();
        if(!id2)
        {
            start = tabsize / 3;
            end = tabsize / 3 * 2;
        }
        else
        {
            start = tabsize / 3 * 2;
            end = tabsize; 
        }
    }
    else
    {
        start = 0;
        end = tabsize / 3;
    }
    while(start < end)
    {
        sum += tab[start];
        start++;
    }
    if(id1)
    {
        if(id2)
        {
            while(wait(NULL) != -1)
                ;
            close(fd[1]);
        }
        read(fd[0], &tmpsum, sizeof(int));
        close(fd[0]);
        sum += tmpsum;
        if(id2 == 0)
        {
            write(fd[1], &sum, sizeof(int));
            close(fd[1]);
        }
        else
        {
            printf("sum = %d",sum);
        }
    }
    else
    {
        close(fd[0]);
        write(fd[1], &sum, sizeof(int));
        close(fd[1]);
        return 0;
    }
    return 0;
}