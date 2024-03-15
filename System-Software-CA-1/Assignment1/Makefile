CC=gcc

all: daemon backup reports lock unlock check collect

daemon: daemon.c backup_dashboard.c check_file_uploads.c collect_reports.c lock_directories.c sig_handler.c unlock_directories.c update_timer.c
	$(CC) -o daemon daemon.c backup_dashboard.c check_file_uploads.c collect_reports.c lock_directories.c sig_handler.c unlock_directories.c update_timer.c

run-daemon: daemon
	./daemon
	
backup: backup_dashboard_2.c
	$(CC) -o backup_dashboard_2 backup_dashboard_2.c

run-backup: backup
	./backup_dashboard_2

reports: generate_reports_2.c
	$(CC) -o generate_reports_2 generate_reports_2.c

run-reports: reports
	./generate_reports_2

lock: lock_directories_2.c
	$(CC) -o lock_directories_2 lock_directories_2.c

run-lock: lock
	./lock_directories_2

unlock: unlock_directories_2.c
	$(CC) -o unlock_directories_2 unlock_directories_2.c

run-unlock: unlock
	./unlock_directories_2

check: check_file_uploads_2.c
	$(CC) -o check_file_uploads_2 check_file_uploads_2.c

run-check: check
	./check_file_uploads_2

collect: collect_reports_2.c
	$(CC) -o collect_reports_2 collect_reports_2.c

run-collect: collect
	./collect_reports_2

clean:
	rm -f daemon backup_dashboard_2 generate_reports_2 lock_directories_2 unlock_directories_2 check_file_uploads_2 collect_reports_2

