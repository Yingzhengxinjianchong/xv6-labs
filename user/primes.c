#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieve(int left_pipe[2]) __attribute__((noreturn));

int main(int argc, char *argv[])
{
  // create the first pipe.
  int p[2];
  pipe(p);

  if (fork() == 0) {
     // --first child process--
     sieve(p);
  }
  else {
     // --main process--
     close(p[0]);

     // send number 2 to 280.
     for (int i = 2; i <= 280; i++) {
         if (write(p[1], &i, sizeof(i)) != sizeof(i)) {
            printf("primes: write error\n");
            exit(1);
         }
     }

     close(p[1]);

     wait(0);
     exit(0);
  }
  return 0;
}

void sieve(int left_pipe[2])
{
  // not write left pipe, just read.
  close(left_pipe[1]);

  int prime;
  // read the first number which must be a primes.
  if (read(left_pipe[0], &prime, sizeof(prime)) == 0) {
     // no number.
     close(left_pipe[0]);
     exit(0);
  }
  printf("prime %d\n", prime);

  // create right pipe.
  int right_pipe[2];
  pipe(right_pipe);

  // create child process.
  int pid = fork();
  if (pid == 0) {
     // --child process--
     close(left_pipe[0]); // new process don't need this pipe.
     sieve(right_pipe);
  }
  else {
     // --current process--
     close(right_pipe[0]);

     int num;
     // continue to read.
     while (read(left_pipe[0], &num, sizeof(num)) != 0) {
           if (num % prime != 0) {
              if (write(right_pipe[1], &num, sizeof(num)) != sizeof(num)) {
                 printf("primes: write error\n");
                 exit(1);
              }
           }
     }
     close(left_pipe[0]);
     close(right_pipe[1]);

    // waiting for child process to exit.
     wait(0);
     exit(0);
  }
}
