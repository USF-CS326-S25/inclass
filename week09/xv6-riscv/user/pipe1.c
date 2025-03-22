#include "kernel/types.h"  // Include kernel type definitions for the program
#include "kernel/fcntl.h"  // Include file control options and constants
#include "user/user.h"     // Include user-level function declarations and definitions

#define NULL 0            // Define NULL pointer as the constant 0

int
main(int argc, char *argv[])  // Main function with command-line arguments
{
  int pid;                // Variable to store process ID from fork()
  int rv;                 // Variable to store return value from read() system call
  int pfd[2];             // Array to hold the pipe file descriptors: pfd[0] for reading, pfd[1] for writing
  char buf[16];           // Buffer to store data read from the pipe

  pipe(pfd);              // Create a pipe and store the file descriptors in pfd array

  pid = fork();           // Create a child process; fork() returns child's PID to parent and 0 to child

  if(pid == 0){           // Code executed by the child process (fork() returned 0)
    printf("In child\n");
    close(pfd[0]);        // Close the read end of the pipe in the child (we only need to write)
    write(pfd[1], "foo", 3); // Write the string "foo" (3 bytes) to the pipe
    close(pfd[1]);        // Close the write end of the pipe after writing
    exit(0);              // Exit the child process with status code 0
  } else {                // Code executed by the parent process (fork() returned child's PID)
    printf("In parent\n");
    close(pfd[1]);        // Close the write end of the pipe in the parent (we only need to read)
    rv = read(pfd[0], buf, 3); // Read up to 3 bytes from the pipe into buf; rv will store actual bytes read
    printf("rv = %d\n", rv);
    buf[3] = '\0';        // Add null terminator to make buf a proper C string
    printf("buf = %s\n", buf);
    close(pfd[0]);        // Close the read end of the pipe after reading
    wait(0);              // Wait for any child process to terminate and discard its exit status
  }

  printf("Parent finished.\n");
}
