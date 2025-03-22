#include "kernel/types.h"  // Include kernel type definitions needed for this program
#include "kernel/fcntl.h"  // Include file control options and constants for file operations
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

  pid = fork();           // Create a child process; fork() returns child's PID to parent and 0 to child

  if(pid == 0){           // Code executed by the child process (fork() returned 0)
    printf("In child\n");
    close(pfd[0]);        // Close the read end of the pipe (child will only write)
    write(pfd[1], "foo\n", 4); // Write "foo\n" (4 bytes including newline) to the pipe
    sleep(50);            // Sleep for 50 ticks to demonstrate that pipe acts as a buffer
    write(pfd[1], "bar\n", 4); // Write "bar\n" (4 bytes) to the pipe after sleeping
    write(pfd[1], "baz\n", 4); // Write "baz\n" (4 bytes) to the pipe immediately after
    close(pfd[1]);        // Close the write end of the pipe to signal EOF to the reader
    exit(0);              // Exit the child process with status code 0
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
