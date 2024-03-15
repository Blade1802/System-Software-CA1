#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int runtrack()
{
    char *cmdcp = "sh";
    char *args[]={"sh", "/home/aayush/Desktop/System-Software-CA-1/Assignment1/track.sh", NULL};

    int pid = fork();

    if(pid == -1)
    {
        perror("Error: creating fork in track.h");
    }
    else if(pid == 0)
    {
        execvp(cmdcp, args);
        exit(1);
    }
    return 0;
}