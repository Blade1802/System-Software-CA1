#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

void backup_dashboard(void)
{
    pid_t pid;
    int status;

    // open file for error logging in append mode
    FILE *fptr;
    fptr = fopen("logs.txt", "a");
    if (fptr == NULL)
    {
        printf("Error opening file.\n");
        return;
    }
    fprintf(fptr, "__________________backup_dashboard____________________\n");

    pid = fork();
    if (pid == -1)
    {
        fprintf(fptr, "ERROR. Fork failed.\n");
        exit(1);
    }
    else if (pid == 0)
    { // child process
        // copy the contents of dashboardDirectory to backupDirectory
        /*find = find the directory
        -mindepth, 1 = find subdirectories with a minimum depth of 1
        -exec, cp = execute copy command on the files found
        -t = setting target direcory
        {} and + = used to pass the list of found files as arguments to the cp command
        NULL = used to terminate the list of arguments
        */
        char *args[] = {"find", "dashboardDirectory", "-mindepth", "1", "-exec", "cp", "-t", "backupDirectory", "{}", "+", NULL};
        execvp("find", args);
        fprintf(fptr, "ERROR. execvp failed.\n");
        exit(1);
    }
    else
    { // parent process
        // wait for the child process to finish
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        {
            fprintf(fptr, "SUCCESS. All contents of dashboardDirectory copied to backupDirectory.\n");
        }
        else
        {
            fprintf(fptr, "ERROR. Failed to copy contents of dashboardDirectory to backupDirectory.\n");
        }
    }
    fprintf(fptr, "___________________________________________\n\n");
    fclose(fptr);
}
