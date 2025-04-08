#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define MAX_LINES 1000
#define MAX_LINE_LEN 100

char lines[MAX_LINES][MAX_LINE_LEN];
int line_count = 0;

// Read lines from the given file descriptor
void
read_lines(int fd)
{
  char buf[512];
  int n, i, j = 0;
  char c;

  line_count = 0;
  memset(lines, 0, sizeof(lines));

  while((n = read(fd, buf, sizeof(buf))) > 0)
    for(i = 0; i < n; i++){
      c = buf[i];
      if(c == '\n'){
        // End of line
        lines[line_count][j] = '\0';
        line_count++;
        j = 0;

        if(line_count >= MAX_LINES){
          fprintf(2, "sort: too many lines\n");
          exit(1);
        }
      } else if(j < MAX_LINE_LEN - 1)
        // Add character to current line
        lines[line_count][j++] = c;
    }

  // Handle the last line if it doesn't end with a newline
  if(j > 0){
    lines[line_count][j] = '\0';
    line_count++;
  }

  if(n < 0){
    fprintf(2, "sort: read error\n");
    exit(1);
  }
}

// Sort lines using bubble sort
void
sort_lines()
{
  int i, j;
  char temp[MAX_LINE_LEN];

  for(i = 0; i < line_count - 1; i++)
    for(j = 0; j < line_count - i - 1; j++)
      if(strcmp(lines[j], lines[j + 1]) > 0){
        // Swap lines
        strcpy(temp, lines[j]);
        strcpy(lines[j], lines[j + 1]);
        strcpy(lines[j + 1], temp);
      }
}

// Print sorted lines
void
print_lines()
{
  int i;

  for(i = 0; i < line_count; i++)
    printf("%s\n", lines[i]);
}

int
main(int argc, char *argv[])
{
  int fd;

  if(argc <= 1)
    // Read from stdin
    read_lines(0);
  else {
    // Read from file
    if((fd = open(argv[1], O_RDONLY)) < 0){
      fprintf(2, "sort: cannot open %s\n", argv[1]);
      exit(1);
    }
    read_lines(fd);
    close(fd);
  }

  sort_lines();
  print_lines();

  exit(0);
}