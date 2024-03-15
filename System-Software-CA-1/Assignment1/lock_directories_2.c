#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <syslog.h>
#include <stdio.h>
#include <errno.h>

void lock_directories() {
    //printf("lock directory functionality should go here. fork/chmod will be used here to change permissions"); 
    
    pid_t pid;
    int status;

    // open file for error logging in append mode
    FILE *fptr;
    fptr = fopen("logs.txt", "a");
    if (fptr == NULL) {
        printf("Error opening logs file.\n");
        return;
    }
    fprintf(fptr, "__________________lock_directories____________________\n");

    pid = fork();
    if (pid == -1) {
        fprintf(fptr, "ERROR. Fork failed.\n");
        exit(1);
    }
    else if (pid == 0) { // child process
        char* directory_path = "shareDirectory";
        int result = chmod(directory_path, S_IRUSR | S_IRGRP | S_IROTH);
        if(result != 0) 
        {
            fprintf(fptr, "ERROR. Failed to lock shareDirectory.\n");
        } 
        else
        {
            fprintf(fptr, "SUCCESS. shareDirectory locked.\n");
        }

        char* directory_path2 = "dashboardDirectory";
        //int result2 = chmod(directory_path2, S_IRUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
        int result2 = chmod(directory_path2, S_IRUSR | S_IRGRP | S_IROTH);
        if(result2 != 0) 
        {
            fprintf(fptr, "ERROR. Failed to lock dashboardDirectory.\n");
        } 
        else
        {
            fprintf(fptr, "SUCCESS. dashboardDirectory locked.\n");
        }

        char* directory_path3 = "backupDirectory";
        int result3 = chmod(directory_path3, S_IRUSR | S_IRGRP | S_IROTH);
        //int result3 = chmod(directory_path3, S_IRUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
        if(result3 != 0) 
        {
            fprintf(fptr, "ERROR. Failed to lock backupDirectory.\n");
        } 
        else
        {
            fprintf(fptr, "SUCCESS. backupDirectory locked.\n");
        }
        exit(1);
    }
    else { // parent process
        waitpid(pid, &status, 0);
        fprintf(fptr, "___________________________________________\n\n");
        fclose(fptr);
    }
}

int main()
{
    lock_directories();
}