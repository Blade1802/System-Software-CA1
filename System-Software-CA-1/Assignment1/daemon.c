/*
 *
 * At 11:30 pm , there will be a check for the xml files that have been uploaded or
 * files that have not been upladed
 *
 * At 1:00 am, the xml reports will be backed up from xml_upload_directory to
 * dashboard_directory
 *
 * Directories are locked during transfer / backup
 *
 * Kill -2 signal will enable the daemon to transfer / backup at any given time
 *
 * */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <syslog.h>
#include <sys/stat.h>
#include <time.h>
#include "backup_dashboard.h"
#include "check_file_uploads.h"
#include "collect_reports.h"
#include "lock_directories.h"
#include "sig_handler.h"
#include "unlock_directories.h"
#include "update_timer.h"
#include "track.h"
#include <signal.h>
#include <fcntl.h>
#include <sys/file.h>

#define LOCK_FILE "/home/aayush/Desktop/System-Software-CA-1/Assignment1/daemon.pid"

// Function to release the lock and close the lock file
void releaseLock(int lock_fd)
{
   flock(lock_fd, LOCK_UN); // Release the lock
   close(lock_fd);          // Close the lock file
   remove(LOCK_FILE);       // Delete the lock file
}

int main()
{
   // Implementation for Singleton Pattern (Only one instance running)
   int lock_fd;

   // Try to open the lock file in read-write mode
   lock_fd = open(LOCK_FILE, O_CREAT | O_RDWR, 0644);
   if (lock_fd == -1)
   {
      printf("Failed to open lock file");
      exit(EXIT_FAILURE);
   }

   // Try to acquire a lock on the lock file
   if (flock(lock_fd, LOCK_EX | LOCK_NB) == -1)
   {
      printf("Another instance of the daemon is already running.\n");
      close(lock_fd);
      exit(EXIT_FAILURE);
   }
   
   // open file for error logging in append mode
   FILE *fptr;
   fptr = fopen("logs.txt", "a");
   if (fptr == NULL)
   {
      printf("Error opening logs file.\n");
      return 1;
   }
   fprintf(fptr, "__________________daemon____________________\n");
   fprintf(fptr, "Daemon is running.\n");

   time_t now;
   struct tm backup_time;
   time(&now); /* get current time; same as: now = time(NULL)  */
   backup_time = *localtime(&now);
   backup_time.tm_hour = 1;
   backup_time.tm_min = 0;
   backup_time.tm_sec = 0;

   // Create a child process
   int pid = fork();

   if (pid > 0)
   {
      // if PID > 0 :: this is the parent
      // this process performs printf and finishes
      // sleep(10);  // uncomment to wait 10 seconds before process ends

      exit(EXIT_SUCCESS);
   }
   else if (pid == 0)
   {
      // Step 1: Create the orphan process
      fprintf(fptr, "Daemon Step 1 complete.\n");

      // Step 2: Elevate the orphan process to session leader, to loose controlling TTY
      // This command runs the process in a new session
      if (setsid() < 0)
      {
         fprintf(fptr, "ERROR. setsid failed.\n");
         exit(EXIT_FAILURE);
      }
      fprintf(fptr, "Daemon Step 2 complete.\n");

      // We could fork here again , just to guarantee that the process is not a session leader
      int pid = fork();
      if (pid > 0)
      {
         exit(EXIT_SUCCESS);
      }
      else
      {

         // Step 3: call umask() to set the file mode creation mask to 0
         umask(0);
         fprintf(fptr, "Daemon Step 3 complete.\n");
         // Step 4: Change the current working dir to root.
         // This will eliminate any issues of running on a mounted drive,
         // that potentially could be removed etc..
         if (chdir("/") < 0)
         {
            fprintf(fptr, "ERROR. chdir failed.\n");
            exit(EXIT_FAILURE);
         }
         fprintf(fptr, "Daemon Step 4 complete.\n");

         // Step 5: Close all open file descriptors
         /* Close all open file descriptors*/
         int x;
         for (x = sysconf(_SC_OPEN_MAX); x >= 0; x--)
         {
            if (close(x) == -1)
            {
               // handle error here
               fprintf(fptr, "ERROR. All file descriptors not closed.\n");
               break; // exit the loop if there was an error
            }
         }
         fprintf(fptr, "Daemon Step 5 complete.\n");

         if (runtrack() == 0)
         {
            openlog("Tracking", LOG_PID | LOG_CONS, LOG_USER);
            syslog(LOG_INFO, "Started file tracking");
         }

         struct tm check_uploads_time;
         time(&now); /* get current time; same as: now = time(NULL)  */
         check_uploads_time = *localtime(&now);
         check_uploads_time.tm_hour = 23;
         check_uploads_time.tm_min = 30;
         check_uploads_time.tm_sec = 0;

         // close log file
         fprintf(fptr, "___________________________________________\n");
         fclose(fptr);

         while (1)
         {
            sleep(1);

            if (signal(SIGINT, sig_handler) == SIG_ERR)
            {
               syslog(LOG_ERR, "ERROR: daemon.c : SIG_ERR RECEIVED");
            }

            // countdown to 23:30
            time(&now);
            double seconds_to_files_check = difftime(now, mktime(&check_uploads_time));
            // syslog(LOG_INFO, "%.f seconds until check for xml uploads", seconds_to_files_check);
            if (seconds_to_files_check == 0)
            {
               check_file_uploads();

               // change to tommorow's day
               update_timer(&check_uploads_time);
            }

            // countdown to 1:00
            time(&now);
            double seconds_to_transfer = difftime(now, mktime(&backup_time));
            // syslog(LOG_INFO, "%.f seconds until backup", seconds_to_files_check);
            if (seconds_to_transfer == 0)
            {
               lock_directories();
               collect_reports();
               backup_dashboard();
               sleep(30);
               unlock_directories();
               // after actions are finished, start counting to next day
               update_timer(&backup_time);
            }
         }
      }
      closelog();

      releaseLock(lock_fd);

      return 0;
   }
   else
   {
      perror("Failed to fork");
      releaseLock(lock_fd); // Ensure lock release in case of failure
      exit(EXIT_FAILURE);
   }
}