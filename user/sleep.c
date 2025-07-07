#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  // print error message if user forgets to pass the argument.
  if(argc<=1){
     fprintf(2,"Error: no argument. You should use as 'sleep ticks'.\n");
     exit(1);
  }

  // convert the string to an integer by atoi.
  // argv[0] is the paragram's name, argv[1] is the ticks.
  int ticks = atoi(argv[1]);

  // call the system sleep.
  sleep(ticks);

  // exit normally.
  exit(0);
}
