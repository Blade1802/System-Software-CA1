#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_FILENAME_LENGTH 100
#define MAX_DIRECTORY_LENGTH 100

void generate_reports()
{
    // open file for error logging in append mode
    FILE *fptr;
    fptr = fopen("logs.txt", "a");
    if (fptr == NULL) {
        printf("Error opening logs file.\n");
        return;
    }
    fprintf(fptr, "__________________generate_reports____________________\n");

    // ask user to select a department
    int choice;
    printf("Select your department:\n");
    printf("1. Warehouse\n");
    printf("2. Manufacturing\n");
    printf("3. Sales\n");
    printf("4. Distribution\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);


    // Get current date and time

    // get the current time as the number of seconds elapsed since the UNIX epoch (January 1, 1970, 00:00:00 UTC)
    time_t time_now = time(NULL);

    /* take the value of time_now and convert it to a struct tm structure that 
    has separate fields for the year, month, day, hour, minute, and second.
    &time_now is a pointer to the time_t value*/
    struct tm *t = localtime(&time_now);

    // format the struct tm value as a string
    char datetime_str[20];
    strftime(datetime_str, sizeof(datetime_str), "%Y-%m-%d %H:%M:%S", t);

    // Get the current username
    char *username = getenv("USER");

    // Create filename with chosen department and current date
    char department[20];
    switch (choice) {
        case 1:
            strcpy(department, "Warehouse");
            break;
        case 2:
            strcpy(department, "Manufacturing");
            break;
        case 3:
            strcpy(department, "Sales");
            break;
        case 4:
            strcpy(department, "Distribution");
            break;
        default:
            printf("Invalid choice. Exiting Program.\n");
            fprintf(fptr, "ERROR. User made an invalid choice\n");
            return;
    }

    // create a string for the filename
    char filename[MAX_FILENAME_LENGTH];
    snprintf(filename, MAX_FILENAME_LENGTH, "%s/%s-%s-Report.xml", "shareDirectory", datetime_str, department);

    // Create file and write to it
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error creating file\n");
        fprintf(fptr, "ERROR. File could not be created.\n");
        fprintf(fptr, "___________________________________________\n\n");
        fclose(fptr);
        return;
    }

    // Write the XML declaration
    fprintf(file, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");

    // Write the root element
    fprintf(file, "<data>\n");

    // Write some data
    fprintf(file, "  <report>\n");
    fprintf(file, "    <department>%s Report</department>\n", department);
    fprintf(file, "    <username>Report submitted by %s</username>\n", username);
    fprintf(file, "    <datetime>%s</datetime>\n", datetime_str);
    fprintf(file, "  </report>\n");

    // Write the closing root element
    fprintf(file, "</data>\n");

    printf("%s Report created successfully\n", department);
    fprintf(fptr, "SUCCESS. %s Report created successfully\n", department);

    fprintf(fptr, "___________________________________________\n");
    fclose(fptr);
}

int main()
{
    generate_reports();
}