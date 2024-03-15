#include <time.h>
#include <stdio.h>

void update_date_to_tomorrow(struct tm *date)
{
    // Increment the day component of the date to move it to tomorrow
    date->tm_mday++;

    // Normalize the date to ensure it's a valid date
    mktime(date);
}

int main()
{
    // Get the current date/time
    time_t rawtime;
    struct tm *date;
    time(&rawtime);
    date = localtime(&rawtime);

    // Print the current date
    printf("Current date: %d-%02d-%02d\n", date->tm_year + 1900, date->tm_mon + 1, date->tm_mday);

    // Update the date to tomorrow
    update_date_to_tomorrow(date);

    // Print the updated date
    printf("Updated date: %d-%02d-%02d\n", date->tm_year + 1900, date->tm_mon + 1, date->tm_mday);

    return 0;
}