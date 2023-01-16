
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_PATH_LEN 100

void print_tree(pid_t pid, int level) {
    char path[MAX_PATH_LEN];
    sprintf(path, "/proc/%d", pid);

    // check if the directory exists
    struct stat st;
    if (stat(path, &st) < 0 || !S_ISDIR(st.st_mode)) {
        return;
    }

    // print current process information
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    printf("PID: %d\n", pid);

    // read the task directory to get child processes
    DIR* dir = opendir(path);
    if (!dir) {
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        // check if the entry is a child process directory
        if (entry->d_type == DT_DIR && entry->d_name[0] >= '0' && entry->d_name[0] <= '9') {
            int child_pid = atoi(entry->d_name);
            print_tree(child_pid, level + 1);
        }
    }
    closedir(dir);
}

