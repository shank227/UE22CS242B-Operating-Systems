#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <limits.h>

void changePermission(const char *path, const char *date_str, mode_t mode) {
    int year, month, day;
    if (sscanf(date_str, "%d-%d-%d", &year, &month, &day) != 3) {
        fprintf(stderr, "Invalid date format\n");
        return;
    }
    struct tm specified_date = {0};
    specified_date.tm_year = year - 1900; 
    specified_date.tm_mon = month - 1; 
    specified_date.tm_mday = day;
    time_t specified_time = mktime(&specified_date);

    if (specified_time == -1) {
        perror("mktime error");
        return;
    }

    DIR *dir = opendir(path);
    if (!dir) {
        perror("Unable to open directory");
        return;
    }

    struct dirent *entry;
    struct stat info;
    char full_path[PATH_MAX];
    char timebuf[80];
    struct tm *timeinfo;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        if (lstat(full_path, &info) == -1) {
            perror("Unable to get file status");
            continue;
        }

        timeinfo = localtime(&info.st_mtime);
        strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", timeinfo);
        printf("Checking file: %s, Last modified: %s\n", full_path, timebuf);

        if (S_ISREG(info.st_mode) && info.st_mtime > specified_time) {
            if (chmod(full_path, mode) == -1) {
                perror("Unable to change permission");
            } else {
                printf("Changed permission of %s to %o\n", full_path, mode);
            }
        }
    }

    if (closedir(dir) == -1) {
        perror("Unable to close directory");
    }
}

int main() {
    char directory[PATH_MAX];
    char date_str[11]; 
    char permission_str[4]; 

    printf("Enter directory path: ");
    if (scanf("%1023s", directory) != 1) {  
        fprintf(stderr, "Error reading directory path.\n");
        return EXIT_FAILURE;
    }

    printf("Enter date in YYYY-MM-DD format: ");
    if (scanf("%10s", date_str) != 1) {
        fprintf(stderr, "Error reading date.\n");
        return EXIT_FAILURE;
    }

    printf("Enter new permission in octal: ");
    if (scanf("%3s", permission_str) != 1) {
        fprintf(stderr, "Error reading permission.\n");
        return EXIT_FAILURE;
    }

    mode_t new_permission = strtol(permission_str, NULL, 8);
    changePermission(directory, date_str, new_permission);
    return 0;
}
