#include <dirent.h> 
#include <stdbool.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h> 

bool match(const char *pattern, const char *filename) { 
    if (!pattern || !filename) { 
        return false; 
    } 
    
    if (!*pattern && !*filename) { 
        return true; 
    } 
    
    if (*pattern == '*') { 
        if (!*(pattern + 1)) { 
            return true; 
        } 
        for (size_t i = 0; *(filename + i); i++) { 
            if (match(pattern + 1, filename + i)) { 
                return true; 
            } 
        } 
        return false; 
    } 
    
    if (*pattern == *filename) { 
        return match(pattern + 1, filename + 1); 
    }
    return false; 
} 

int main(int argc, char *argv[]) { 
    DIR *dir; 
    struct dirent *ent; 
    char *dirpath, *pattern; 
    size_t dirlen; 
    
    if (argc < 2) { 
        fprintf(stderr, "Incorrect use:%s\n", argv[0]); 
        exit(EXIT_FAILURE); 
    } 
    dirpath = argv[1]; 
    dirlen = strlen(dirpath); 
    if (dirpath[dirlen - 1] == '/') { 
        dirpath[dirlen - 1] = '\0'; 
    } 
    if (argc > 2) { 
        pattern = argv[2]; 
    } 
    else { 
        pattern = "*"; 
    } 
    
    if ((dir = opendir(dirpath)) != NULL) { 
        while ((ent = readdir(dir)) != NULL) { 
            if (ent->d_type == DT_REG && match(pattern, ent->d_name)) { 
                printf("%s/%s\n", dirpath, ent->d_name); 
            } 
        } 
        closedir(dir); 
    } 
    else { 
        perror("Could not open directory"); 
        exit(EXIT_FAILURE); 
    } 

    return 0; 
}
