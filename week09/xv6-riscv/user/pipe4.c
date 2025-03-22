#include "kernel/types.h"  // Include kernel type definitions for basic data types
#include "kernel/fcntl.h"  // Include file control options and constants
#include "user/user.h"     // Include user-level function declarations (fork, pipe, wait, etc.)

#define NULL 0            // Define NULL pointer as the constant 0

int
main(int argc, char *argv[])  // Main function with command-line arguments (unused in this program)
{
  int pid;                // Variable to store process ID returned by fork()
  int rv;                 // Variable to store return value from read() system call
  int pfd[2];             // Array to hold pipe file descriptors: pfd[0] for reading, pfd[1] for writing
  char buf[16];           // Buffer to store data read from the pipe

  pipe(pfd);              // Create a pipe and store the file descriptors in pfd array

  pid = fork();           // Create first child process; fork() returns child's PID to parent and 0 to child

  if(pid == 0){           // Code executed by the first child process (Child 1)
    // Child 1 (C1)       // This child will be the writer to the pipe

    printf("In child 1\n");
    close(pfd[0]);        // Close the read end of the pipe (Child 1 will only write)
    write(pfd[1], "foo\n", 4); // Write "foo\n" (4 bytes) to the pipe
    write(pfd[1], "bar\n", 4); // Write "bar\n" (4 bytes) to the pipe
    write(pfd[1], "baz\n", 4); // Write "baz\n" (4 bytes) to the pipe
    close(pfd[1]);        // Close the write end of the pipe to signal EOF to the reader
    exit(0);              // Exit Child 1 with status code 0
  }

  // In the parent          // Execution continues in the parent process after first fork

  pid = fork();           // Create second child process; reuses pid variable to store new child's PID

  if(pid == 0){           // Code executed by the second child process (Child 2)
    // Child 2 (C2)       // This child will be the reader from the pipe
    printf("In child 2\n");
    close(pfd[1]);        // Close the write end of the pipe (Child 2 will only read)
    while(1){             // Infinite loop to read data until pipe is closed
      rv = read(pfd[0], buf, 1); // Read one byte at a time from the pipe into buf
      if(rv > 0){         // If read() successfully read data (returns number of bytes read)
        buf[1] = '\0';    // Add null terminator after the read byte to make a C string
        printf("%s", buf);
      } else              // If read() returns 0 (EOF) or negative (error)
        break;            // Exit the loop - either pipe is closed or there was an error
    }
    close(pfd[0]);        // Close the read end of the pipe after reading all data
    exit(0);              // Exit Child 2 with status code 0
  }

  // In the parent        // Execution continues in the parent process after second fork
  close(pfd[0]);          // Parent closes the read end of the pipe (not needed by parent)
  close(pfd[1]);          // Parent closes the write end of the pipe (not needed by parent)

  wait(0);                // Wait for first child to terminate (either Child 1 or Child 2)
  wait(0);                // Wait for second child to terminate (the remaining child)

  printf("Parent finished.\n");
}
