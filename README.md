# Assignment 3: File Systems

We pledge our honor that we have abided by the Stevens Honor System.

Robert Herley, Andrew Chen

## How to run

First, make the project.
Then, start the program with the following parameters:

```bash
# Parameter Syntax
$ ./fs -f <file list> -d <dir list> -s <disk size> -b <block size>

# An Example using file_list.txt, dir_list.txt, disk size of 512 and block size of 4
$ ./fs -f file_list.txt -d dir_list.txt -s 512 -b 4
```

The input file list and dir list should be assembled by using the command `find ./ -type d > dir_list.txt` for directories and `find ./ -type f -ls > file_list.txt` for files.

## CLI Commands

Here's a list of all the possible CLI Commands as outlined in the spec:

```
cd [directory] - set specified directory as the current directory
cd.. - set parent directory as current directory
ls - list all files and sub-directories in current directory
mkdir [name] - create a new directory in the current directory
create [name] - create a new file in the current directory
append [name] [bytes] - append a number of bytes to the file
remove [name] [bytes] - delete a number of bytes from the file
delete [name] - delete the file or directory
exit - deallocate data structures and exit program
dir - print out directory tree in breadth-first order
prfiles - print out all file information
prdisk - print out disk space information
```

And here are slight additions to the commands:

```
cd - with no params, goes to the root directory
prdisk - also gives a colored ascii block diagram of allocations
dir - creates an ascii breadth first tree similar to unix tree
```

Some of the commands have color and ascii colors, so be sure use a terminal that supports both of those.
