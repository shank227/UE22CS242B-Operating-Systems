#define _XOPEN_SOURCE 700 
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <errno.h>

time_t parse_date(const char *date_str) {
    struct tm tm = {0};
    if (strptime(date_str, "%Y-%m-%d", &tm) == NULL) {
        fprintf(stderr, "Error parsing date: %s\n", date_str);
        return -1;
    }
    tm.tm_isdst = -1; 
    return mktime(&tm);
}

void change_ownership(const char *path, time_t specified_time, const char *user_name) {
    struct passwd *pwd = getpwnam(user_name);
    if (!pwd) {
        fprintf(stderr, "No such user: %s\n", user_name);
        return;
    }

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
        if (stat(full_path, &info) == -1) {
            perror("Failed to get file info");
            continue;
        }

        if (S_ISREG(info.st_mode) && info.st_mtime > specified_time) {
            if (chown(full_path, pwd->pw_uid, -1) == -1) {
                perror("Failed to change file owner");
            } else {
                printf("Changed ownership of '%s' to user '%s'.\n", full_path, user_name);
            }
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <directory> <YYYY-MM-DD> <username>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *directory = argv[1];
    char *date_str = argv[2];
    char *new_owner = argv[3];
    time_t specified_time = parse_date(date_str);

    if (specified_time == -1) {
        return EXIT_FAILURE;
    }

    change_ownership(directory, specified_time, new_owner);
    return EXIT_SUCCESS;
}
