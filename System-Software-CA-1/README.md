# System Software Daemon Assignment

## Contributers 
- Aayush Gaur <C20396243@mytudublin.ie>

## CTO Requirements

1. The company will offer a new shared directory for the 4 departments to upload their
xml reports. Authorised users can make changes to directory. The reports will be
collected on a nightly basis at 1am and moved to the directory for the dashboard
system for processing.
2. The dashboard directory content should be backed up every night.
3. The changes made to the department managers upload directory needs to
documented. The username of the user, the file they modified and the timestamp
should be recorded.
4. No changes should be allowed to be made to the directories (upload and reporting)
while the backup/transfer is happening.
5. If a change needs to be urgently made to the live site, it should be possible to make
the changes. (No users shouldn’t have write access to the new reporting directory)

## System Requirements

The following are the requirements for the assignment as choosen by the module lecturer

* Create a daemon to continually manage the operation of the requirements listed by
the CTO above.
* Identify new or modified xml reports and log details of who made the changes, this
should be generated as a text file report and stored on the server.
* The department managers must upload their xml report file by 11.30pm each night.
The uploads to the shared directory must be moved to the reporting directory. This
should happen at 1am. (This functionality needs to be self-contained in the solution.
Do NOT use cron)
* If a file wasn’t uploaded this should be logged in the system. (A naming convention
can be used to help with this task.)
* When the backup/transfer begins no user should be able to modify either the upload
or reporting directory.
* It must be possible to ask the daemon to backup and transfer at any time.
* IPC should be setup to allow all processes to report in on completion of a task.
(success or failure)
* Error logging and reporting should be included for all main tasks in the solution.
* Create a makefile to manage the creation of the executables.

## General Assumptions.

1. The company only has one server
2. Backups can be made to a different directory on the server
3. The upload and reporting directories should be two sub directories. (You can choose
this location in your system)

## How to Run (Linux)

* Download and unzip the code onto the Linux desktop.
* Using a linux terminal, cd into the assignment folder.
* Use the make command to create the executable files. 
* The daemon can be run manually using ./daemon
* To setup automatic execution of the daemon on boot-up, copy the `daemon.service` file to `/etc/systemd/system/` directory

## Video Demo

*  https://youtu.be/K4Iw8dOz4Y0