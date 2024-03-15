#include <unistd.h>
#include <syslog.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <time.h>

void check_file_uploads(void) {
   //printf("you can potentially use this place holder to determine if a file has been uploaded");

    DIR *dir;
    struct dirent *ent;
    int count = 0;
    char *sales_file = NULL;
    char *warehouse_file = NULL;
    char *manufacturing_file = NULL;
    char *distribution_file = NULL;
    char *files[4];
    char *missing_files[4] = { "Sales", "Warehouse", "Manufacturing", "Distribution" };

    // get the current time as the number of seconds elapsed since the UNIX epoch (January 1, 1970, 00:00:00 UTC)
    time_t time_now = time(NULL);

    /* take the value of time_now and convert it to a struct tm structure that 
    has separate fields for the year, month, day, hour, minute, and second.
    &time_now is a pointer to the time_t value*/
    struct tm *t = localtime(&time_now);

    // format the struct tm value as a string
    char datetime_str[20];
    strftime(datetime_str, sizeof(datetime_str), "%Y-%m-%d", t);

    // open file for error logging in append mode
    FILE *fptr;
    fptr = fopen("logs.txt", "a");
    if (fptr == NULL) {
        printf("Error opening log file.\n");
        return;
    }
    fprintf(fptr, "__________________check_file_uploads____________________\n");

    // Open the directory
    dir = opendir("shareDirectory");
    if (dir == NULL) {
        fprintf(fptr, "ERROR. Unable to open directory.\n");
        return;
    }

    // Read all the filenames in the directory
    while ((ent = readdir(dir)) != NULL) {
        // Check if the filename starts with todays date
        if (strncmp(ent->d_name, datetime_str, 10) == 0) 
        {
            // Add the filename to the files array
            files[count] = ent->d_name;
            count++;

            // Check if the filename contains sales, warehouse, Manufacturing, or Distribution
            if (strstr(ent->d_name, "Sales") != NULL) {
                sales_file = ent->d_name;
                missing_files[0] = NULL; // Mark category as found
                fprintf(fptr, "SUCCESS. Sales report is present.\n");
            }
            else if (strstr(ent->d_name, "Warehouse") != NULL) {
                warehouse_file = ent->d_name;
                missing_files[1] = NULL; // Mark category as found
                fprintf(fptr, "SUCCESS. Warehouse report is present.\n");
            }
            else if (strstr(ent->d_name, "Manufacturing") != NULL) {
                manufacturing_file = ent->d_name;
                missing_files[2] = NULL; // Mark category as found
                fprintf(fptr, "SUCCESS. Manufacturing report is present.\n");
            }
            else if (strstr(ent->d_name, "Distribution") != NULL) {
                distribution_file = ent->d_name;
                missing_files[3] = NULL; // Mark category as found
                fprintf(fptr, "SUCCESS. Distribution report is present.\n");
            }
        }
    }

    // Close the directory
    closedir(dir);

    // Check if there are exactly four files that match the criteria
    if (count == 4 && sales_file != NULL && warehouse_file != NULL &&
        manufacturing_file != NULL && distribution_file != NULL) {
        fprintf(fptr, "SUCCESS. All four reports are present.\n");
    }
    else 
    {
        fprintf(fptr, "ERROR. The following reports are missing:\n");
        for (int i = 0; i < 4; i++) {
            if (missing_files[i] != NULL) 
            {
                fprintf(fptr, "- %s\n", missing_files[i]);
            }
        }
    }
    fprintf(fptr, "___________________________________________\n\n");
    fclose(fptr);
}
