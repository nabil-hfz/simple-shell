# simple-shell
A Simple C shell that is able to create, move, delete, list and use wild-card feature on files and folders.

- COMPILING COMMANDS:

1- gcc shell.c -o shell

2- ./shell



- TESTING COMMANDs

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


dbxcli> cp ~/*.txt backup/

dbxcli> cp ~/file? backup/

dbxcli> cp ~/file.txt ~/Desktop/backup_*.txt


dbxcli> exit
