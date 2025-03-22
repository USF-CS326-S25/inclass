#include "kernel/types.h"  // Include header file for basic type definitions in xv6
#include "kernel/fcntl.h"  // Include header file for file control options like O_CREATE, O_WRONLY
#include "user/user.h"     // Include header file for user-level functions like fork, exec, wait

#define NULL 0             // Define NULL as 0, a common convention in C for representing null pointers

int
main(int argc, char *argv[])  // Main function that takes command line arguments
{
  int pid, fd;             // Declare variables for process ID and file descriptor
  char *argv_child[10];    // Create an array to hold arguments for the child process (max 10 arguments)
  argv_child[0] = "ls";    // Set the first argument to "ls" (the command to execute)
  argv_child[1] = NULL;    // Set the second argument to NULL to terminate the argument list
  int rv;                  // Declare a variable to store the return value from exec()

  fd = open("out.txt", O_CREATE | O_WRONLY);  // Open file "out.txt" with create and write-only flags
  if(fd < 0){              // Check if file opening failed (returns negative file descriptor)
    printf("Cannot open(\"%s\")\n", "out.txt");
    exit(-1);              // Exit with error code -1 if file cannot be opened
  }

  pid = fork();            // Create a child process; fork() returns child PID to parent, 0 to child

  if(pid == 0){            // This code block executes only in the child process (when pid == 0)
    printf("In child\n");

    close(1);              // Close standard output (file descriptor 1)
    dup(fd);               // Duplicate fd to the lowest available descriptor (which is now 1/stdout)
    close(fd);             // Close the original file descriptor (no longer needed after duplication)

    rv = exec(argv_child[0], argv_child);  // Replace current process with "ls" command
    if(rv < 0)             // Check if exec failed (should not reach here if exec succeeds)
      printf("Cannot exec(\"%s\")\n", argv_child[0]);
    exit(0);               // Exit child process with success code (this line only reached if exec fails)
  } else {                 // This code block executes only in the parent process
    close(fd);             // Close the file descriptor in the parent (not needed by parent)
    printf("In parent\n");
    wait(0);               // Wait for the child process to complete, discarding its exit status
  }

  printf("Parent finished.\n");  // Print completion message (only parent reaches this point)
}
