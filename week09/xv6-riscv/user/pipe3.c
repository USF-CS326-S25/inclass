#include "kernel/types.h"  // Include kernel type definitions for basic data types
#include "kernel/fcntl.h"  // Include file control options and constants
#include "user/user.h"     // Include user-level function declarations (fork, pipe, exec, etc.)

#define NULL 0            // Define NULL pointer as the constant 0

int
main(int argc, char *argv[])  // Main function with command-line arguments (unused in this program)
{
  int pid;                // Variable to store process ID returned by fork()
  int rv;                 // Variable to store return values from system calls
  int pfd[2];             // Array to hold pipe file descriptors: pfd[0] for reading, pfd[1] for writing
  char buf[16];           // Buffer to store data read from the pipe
  char *argv_child[10];   // Array to store arguments for the child process's exec() call

  pipe(pfd);              // Create a pipe and store the file descriptors in pfd array

  pid = fork();           // Create a child process; fork() returns child's PID to parent and 0 to child

  if(pid == 0){           // Code executed by the child process (fork() returned 0)
    printf("In child\n");
    argv_child[0] = "ls"; // Set the program name ("ls") as the first argument
    argv_child[1] = NULL; // Null-terminate the argument list (required by exec)

    close(1);             // Close file descriptor 1 (standard output)
    dup(pfd[1]);          // Duplicate pfd[1] to the lowest available file descriptor (which is now 1)
    close(pfd[1]);        // Close the original pfd[1] (now unnecessary since we have a copy as fd 1)
    close(pfd[0]);        // Close the read end of the pipe (child will only write via stdout)

    rv = exec(argv_child[0], argv_child); // Replace the current process image with the "ls" program
    if(rv < 0){           // If exec() fails, it returns -1 and the original program continues
      printf("Cannot exec(\"%s\")\n", argv_child[0]);
      exit(-1);           // Exit with error code -1
    }
  } else {                // Code executed by the parent process (fork() returned child's PID)
    printf("In parent\n");
    close(pfd[1]);        // Close the write end of the pipe (parent will only read)
    while(1){             // Infinite loop to read data until pipe is closed
      rv = read(pfd[0], buf, 1); // Read one byte at a time from the pipe into buf
      if(rv > 0){         // If read() successfully read data (returns number of bytes read)
        buf[1] = '\0';    // Add null terminator after the read byte to make a C string
        printf("%s", buf);
      } else              // If read() returns 0 (EOF) or negative (error)
        break;            // Exit the loop - either pipe is closed or there was an error
    }
    close(pfd[0]);        // Close the read end of the pipe after reading all data
    wait(0);              // Wait for child process to terminate and discard its exit status
  }

  printf("Parent finished.\n");
}
