#define _XOPEN_SOURCE 700 
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h> 
#include <unistd.h> 
#include <string.h>
#include <time.h>

time_t parse_date(char *date_str) {
    struct tm tm = {0};
    if (strptime(date_str, "%Y-%m-%d", &tm) == NULL) {
        fprintf(stderr, "Error parsing date.\n");
        return -1;
    }
    tm.tm_isdst = -1;  
    return mktime(&tm);
}

void truncate_files(const char *path, time_t specified_time) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("Failed to open directory");
        return;
    }

    struct dirent *entry;
    struct stat info;
    char full_path[1024];

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        if (lstat(full_path, &info) == -1) {
            perror("Failed to get file info");
            continue;
        }

        if (S_ISREG(info.st_mode) && info.st_mtime > specified_time) {
            off_t new_size = info.st_size / 2;
            // Truncate the file to half of its original size
            if (truncate(full_path, new_size) == -1) {
                perror("Failed to truncate file");
            } else {
                printf("Truncated '%s' to %ld bytes.\n", full_path, new_size);
            }
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <directory> <YYYY-MM-DD>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *directory = argv[1];
    char *date_str = argv[2];
    time_t specified_time = parse_date(date_str);

    if (specified_time == -1) {
        return EXIT_FAILURE;
    }

    truncate_files(directory, specified_time);
    return EXIT_SUCCESS;
}
