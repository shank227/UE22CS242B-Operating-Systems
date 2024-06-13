#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <x> <y>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int x = atoi(argv[1]);
    int y = atoi(argv[2]);

    char cmd[50];
    sprintf(cmd, "python3 -c \"print(%d * %d)\"", x, y);

    execlp("sh", "sh", "-c", cmd, NULL);

    perror("execlp");
    return EXIT_FAILURE;
}
