#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *filename);

int main(int argc, char *argv[])
{
  if (argc != 3) {
     fprintf(2, "Usage: find <path> <filename>\n");
     exit(1);
  }
  find(argv[1], argv[2]);
  exit(0);
}

// recursion function.
void find(char *path, char *filename)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st; // data of data.

  // open the path for fd.
  fd = open(path, 0);
  if (fd < 0) {
     fprintf(2, "find: cannot open %s\n", path);
     return;
  }

  // get the status of the file.
  if (fstat(fd, &st) < 0) {
     fprintf(2, "find: cannot stat %s\n", path);
     close(fd);
     return;
  }

  // process by file type.
  switch (st.type) {
  case T_FILE:
    // if it is a file.
    break;
  case T_DIR:
    // if it is a directory.
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
       printf("find: path too long\n");
       break;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';

    // read the directory.
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0)
           continue;

        // skip "." and ".."
        if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
           continue;

        // make new path.
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        // get status of new path.
        if (stat(buf, &st) < 0) {
           printf("find: cannot stat %s\n", buf);
           continue;
        }

        // resusion or comparison.
        if (st.type == T_DIR) {
           find(buf, filename);
        }
        else if (st.type == T_FILE) {
           if (strcmp(de.name, filename) == 0) {
              printf("%s\n", buf);
           }
        }
    }
    break;
  }
  close(fd);
}
