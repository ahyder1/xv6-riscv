#include "kernel/types.h"
#include "kernel/pstat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc < 2){
    fprintf(2, "usage: time command [args...]\n");
    exit(1);
  }

  int start = uptime();
  int pid = fork();

  if(pid < 0){
    fprintf(2, "time: fork failed\n");
    exit(1);
  }

  if(pid == 0){
    exec(argv[1], &argv[1]);
    fprintf(2, "time: exec failed\n");
    exit(1);
  }

  struct rusage usage;
  int status;

  if(wait2(&status, &usage) < 0){
    fprintf(2, "time: wait2 failed\n");
    exit(1);
  }

  int end = uptime();
  int elapsed = end - start;
  int percent = 0;

  if(elapsed > 0)
    percent = (usage.cputime * 100) / elapsed;

  printf("elapsed time: %d ticks, cpu time: %d ticks, %d%% CPU\n",
         elapsed, usage.cputime, percent);

  exit(0);
}
