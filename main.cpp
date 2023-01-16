//#include <iostream>
//
//int main() {
//    std::cout << "Hello, World!" << std::endl;
//    return 0;
//}
//– Implemented a system function to provide data (PID, status, statistics related to execution, etc.)
//about all descendants of a given process (in DFS order). Write programs in userland to display the resulting trees, using the new system function.





//To implement a system function to provide data about all descendants of a given process, you could use the procfs file system, which provides a view of the system's process information in the form of a file hierarchy. You can access the information about a specific process by reading the files in the corresponding directory under /proc.
//
//The following is an example of how the system function could be implemented in C:

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


//This function takes the pid of the process and the level of the current process and recursively iterates over all its child processes directory, prints their PIDs and then call the function again with the child process pid and level + 1 as arguments.
//
//You can use this function by calling print_tree(pid, 0); with the desired process pid.
//This function uses the DFS algorithm, so it will iterate over all descendant of the given process in a depth first search order and print their pid in the tree format.
//
//You can also gather more information about the process status and statistics by reading files under the proc directory like /proc/[pid]/status or /proc/[pid]/stat , and parse them accordingly to extract the needed