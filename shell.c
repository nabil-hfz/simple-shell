

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h> // pentru mkdir
#include <sys/errno.h>
#include <sys/fcntl.h>
#include <stdlib.h>
#include <glob.h>

#define MAX_COMMAND_LENGTH 100 // lungimea maxima a unei comenzi
#define MAX_HISTORY_LENGTH 10 // numarul maxim de comenzi in istoric

char history[MAX_HISTORY_LENGTH][MAX_COMMAND_LENGTH]; // istoric comenzi
int history_index = -1; // indexul curent in istoric

int main(int argc, char *argv[]) {
    printf("dbxcli> You can see the list of the command any time by type help\n");
    while (1) {
        printf("dbxcli> ");
        fflush(stdout);

        char command[MAX_COMMAND_LENGTH];
        fgets(command, MAX_COMMAND_LENGTH, stdin);
        command[strlen(command) - 1] = '\0'; // Sterg '\n' din final

        // adauga comanda in istoric
        if (history_index < MAX_HISTORY_LENGTH - 1) {
            history_index++;
        } else {
            for (int i = 0; i < MAX_HISTORY_LENGTH - 1; i++) {
                strcpy(history[i], history[i + 1]);
            }
        }
        strcpy(history[history_index], command);

        // parcurge comanda si identifica elementele
        char *token = strtok(command, " ");


        if (strcmp(token, "cp") == 0) {

            // handle cp command
            char *src = strtok(NULL, " ");
            char *dest = strtok(NULL, " ");

            if (src == NULL || dest == NULL) {
                printf("cp: Invalid arguments\n");
                continue;
            }

            glob_t glob_result;
            int glob_return = glob(src, GLOB_TILDE, NULL, &glob_result);
            if (glob_return != 0) {
                continue;
            }

            for (int i = 0; i < glob_result.gl_pathc; i++) {
                if (link(glob_result.gl_pathv[i], dest) != 0) {
                    if (errno == EEXIST) {
                        printf("cp: %s already exists\n", dest);
                    } else {
                        printf("cp: Failed to copy %s to %s\n", glob_result.gl_pathv[i], dest);
                    }
                }
            }

            globfree(&glob_result);
        }else if (strcmp(token, "mv") == 0) {
            // handle mv command
            char *src = strtok(NULL, " ");
            char *dst = strtok(NULL, " ");
            if (src == NULL || dst == NULL) {
                printf("mv: Invalid arguments\n");
                printf("Usage: mv <source> <destination>\n");
            } else {
                if (rename(src, dst) != 0) {
                    if(errno == EISDIR)
                        printf("mv: Failed to move %s to %s, because they are not of the same type\n", src, dst);
                    else
                        printf("mv: Failed to move %s to %s\n", src, dst);
                    printf("Reason: %s\n",strerror(errno));
                } else {
                    printf("mv: Successfully moved %s to %s\n", src, dst);
                }
            }
        }
        else if (strcmp(token, "mkdir") == 0) {
            char *dir = strtok(NULL, " ");
            if (dir == NULL) {
                printf("mkdir: Invalid arguments\n");
                continue;
            }
            if (mkdir(dir, 0777) != 0) {
                printf("mkdir: Failed to create directory %s\n", dir);
            }
        } else if (strcmp(token, "rm") == 0) {
            char *file = strtok(NULL, " ");

            if (file == NULL) {
                printf("rm: Invalid arguments\n");
                continue;
            }
            if (unlink(file) != 0) {
                printf("rm: Failed to remove %s\n", file);
            }
        } else if (strcmp(token, "ls") == 0) {
            system("ls");
        } else if (strcmp(token, "touch") == 0) {
            char *file = strtok(NULL, " ");

            if (file == NULL) {
                printf("touch: Invalid arguments\n");
                continue;
            }
            int fd = creat(file, S_IRWXU); // == open(path, O_CREAT | O_WRONLY | O_TRUNC, mode) call
            if (fd < 0) {
                printf("touch: Failed to create file %s\n", file);
                continue;
            }
            close(fd);
        } else if (strcmp(token, "history") == 0) {
            int start = history_index - MAX_HISTORY_LENGTH + 1;
            if (start < 0) {
                start = 0;
            }
            for (int i = start; i <= history_index; i++) {
                printf("%d: %s\n", i + 1, history[i]);
            }
        } else if (strcmp(token, "exit") == 0) {
            return 0;
        } else if (strcmp(token, "help") == 0) {
            // handle help command
            char *sub_token = strtok(NULL, " ");
            if (sub_token == NULL) {
                printf("Commands:\n");
                printf("cp <source> <destination> - Copy files and directories\n");
                printf("touch <file name> - Create a file\n");
                printf("mkdir <directory name> - Create a directory\n");
                printf("rm <file name> - Delete a file of directory\n");
                printf("ls - List current directory contents\n");
                printf("history - Show command history\n");
                printf("exit - Exit the shell\n");
                printf("help <command> - Show help for a specific command\n");
            } else if (strcmp(sub_token, "cp") == 0) {
                printf("cp <source> <destination> - Copy files and directories\n");
                printf("mv <source> <destination> - Move files and directories\n");
                printf("Examples:\n");
                printf("cp file.txt backup/ - Copies file.txt to the backup directory\n");
                printf("cp ~/*.txt backup/ - Copies all .txt files in the home directory to the backup directory\n");
            } else if (strcmp(sub_token, "history") == 0) {
                printf("history - Show command history\n");
                printf("Examples:\n");
                printf("history - Shows the last 10 commands entered in the shell\n");
            } else if (strcmp(sub_token, "exit") == 0) {
                printf("exit - Exit the shell\n");
                printf("Examples:\n");
                printf("exit - Exits the shell\n");
            } else {
                printf("Invalid command. Type 'help' for a list of commands.\n");
            }
        } else {
            // handle invalid command
            printf("Invalid command. Type 'help' for a list of commands.\n");
        }

    }
}

// COMPILING COMMANDS
// 1- gcc shell.c -o shell
// 2- ./shell



// TESTING COMMANDs
/*

dbxcli> touch testfile1.txt
dbxcli> ls
testfile1.txt

dbxcli> mkdir testdir
dbxcli> ls
testfile1.txt testdir/

dbxcli> cp testfile1.txt testfile2.txt
dbxcli> cp testdir testdir2
dbxcli> ls
testfile1.txt testfile2.txt testdir/

dbxcli> mv testfile1.txt testdir/
dbxcli> ls
testfile1.txt testdir/

dbxcli> rm testfile1.txt
dbxcli> ls
testdir/

dbxcli> exit

dbxcli> cp ~/*.txt backup/
dbxcli> cp ~/file? backup/
dbxcli> cp ~/file.txt ~/Desktop/backup_*.txt

 */
