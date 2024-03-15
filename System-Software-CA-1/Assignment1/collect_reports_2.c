#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <syslog.h>
#include <stdlib.h>


void collect_reports(void) {
    //printf("collect reports functionality could go here.. fork/cp will probably be used");
    pid_t pid;
    int status;

    // open file for error logging in append mode
    FILE *fptr;
    fptr = fopen("logs.txt", "a");
    if (fptr == NULL) {
        printf("Error opening log file.\n");
        return;
    }
    fprintf(fptr, "__________________collect_reports____________________\n");

    pid = fork();
    if (pid == -1) {
        fprintf(fptr, "ERROR. Fork failed.\n");
        exit(1);
    }
    else if (pid == 0) { // child process
        // move the contents of shareDirectory to dashboardDirectory
        /*find = find the directory
        -mindepth, 1 = find subdirectories with a minimum depth of 1
        -exec, mv = execute move command on the files found
        -t = setting target direcory
        {} and + = used to pass the list of found files as arguments to the cp command
        NULL = used to terminate the list of arguments
        */
        char *args[] = {"find", "shareDirectory", "-mindepth", "1", "-exec", "mv", "-t", "dashboardDirectory", "{}", "+", NULL};
        execvp("find", args);
        fprintf(fptr, "ERROR. execvp failed.\n");
        exit(1);
    }
    else { // parent process
        // wait for child to finish
        waitpid(pid, &status, 0);
        fprintf(fptr, "All contents of shareDirectory moved to dashboardDirectory.\n");
    }
    fprintf(fptr, "___________________________________________\n\n");
    fclose(fptr);
}

int main()
{
    collect_reports();
}
