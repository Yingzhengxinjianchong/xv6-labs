#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  // create two pipes.
  int p_to_c[2]; // Pipe: Parent to Child.
  int c_to_p[2]; // Pipe: Child to Parent.
  pipe(p_to_c);
  pipe(c_to_p);

  // create child process.
  int pid = fork();

  // begin "ping-pong" a byte.
  if (pid == 0) {
     // --Child process--
     char received_byte;

     // not send in p_to_c, not receive in c_to_p.
     // pipe[0]:read;  pipe[1]:write.
     close(p_to_c[1]);
     close(c_to_p[0]);

     // read.
     if (read(p_to_c[0], &received_byte, 1) != 1) {
        printf("child: read error\n");
        exit(1);
     }
     printf("%d: received ping\n", getpid());

     // write.
     if (write(c_to_p[1], &received_byte, 1) != 1) {
        printf("child: write error\n");
        exit(1);
     }

     exit(0);
  }
  else {
     // --Parent process--
     char byte_to_send = 'A';
     char received_byte;

     // not send in c_to_p, not receive in p_to_c.
     close(p_to_c[0]);
     close(c_to_p[1]);

     // write.
     if (write(p_to_c[1], &byte_to_send, 1) != 1) {
        printf("parent: write error\n");
        exit(1);
     }

     // read.
     if (read(c_to_p[0], &received_byte, 1) != 1) {
        printf("parent: read error\n");
        exit(1);
     }
     printf("%d: received pong\n", getpid());

     exit(0);
  }
}
