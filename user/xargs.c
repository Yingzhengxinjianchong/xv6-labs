#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h" // include MAXARG

int main(int argc, char *argv[])
{
  char *command_argv[MAXARG]; // commands and arguments for exec.
  char line_buf[512]; // a line from stdin.
  char* p = line_buf; // current location to write.
  int base_argc = 0; // xargs's argc.
  int i;

  // without command.
  if (argc <= 1) {
     fprintf(2, "Usage: xargs <command> [args...]\n");
     exit(1);
  }

  // duplicate arguments.
  for (i = 1; i < argc; i++) {
      command_argv[base_argc++] = argv[i];
  }

  // circle.
  while (read(0, p, 1) == 1) {
      if (*p == '\n') {
         *p = 0;

         // line as argument.
         command_argv[base_argc] = line_buf;
         // the argument list of exec must end with a null pointer.
         command_argv[base_argc + 1] = 0;

         // fork-exec-wait circle.
         if (fork() == 0) {
            // --child process--
            exec(command_argv[0], command_argv);
            // if exec failed.
            fprintf(2, "xargs: exec %s failed\n", command_argv[0]);
            exit(1);
         }
         else {
            // --current process--
            wait(0);
         }
         p = line_buf;
      }
      else {
         // not '\n'
         p++;
         if (p >= line_buf + sizeof(line_buf)) {
            fprintf(2, "xargs: line too long \n");
            exit(1);
         }
      }
  }

  // if file doesn't end with '\n'.
  if (p > line_buf) {
     *p = 0;
     command_argv[base_argc] = line_buf;
     command_argv[base_argc + 1] = 0;
     if (fork() == 0) {
        exec(command_argv[0], command_argv);
        fprintf(2, "xargs: exec %s failed\n", command_argv[0]);
        exit(1);
     }
     else {
        wait(0);
     }
  }

  exit(0);
}
