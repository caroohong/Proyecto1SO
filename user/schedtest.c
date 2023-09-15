#include "types.h"
#include "user.h"
#include "pstat.h"

int
main(int argc, char *argv[])
{
  if(argc < 2){
    printf(2, "Usage: %s [100M-iterations]\n", argv[0]);
    exit();
  }

  int itr = atoi(argv[1]);

  // Set the number of tickets for the current process
  int parent = 0;
  
  int pid = 0;
  int childpid = fork();

  if (childpid) {
    parent = 1;
    pid = getpid();
    printf(1, "Parent pid: '%d'\n", pid);
    settickets(30);
  }
  else {
    pid = getpid();
    printf(1, "Child pid: '%d'\n", pid);
    settickets(10);
  }

  int iterations = 100000000 * itr;
  printf(1, "iterations: '%d'\n", iterations);

  for (volatile int i = 0; i < iterations; i++);
  
  if (parent) {  
    struct pstat stats;
    if (getpinfo(&stats) < 0) {
      printf(2, "Call to getpinfo() failed!\n");
      exit();
    }

    // traverse entries and display per-process information
    for (int i = 0; i < sizeof(stats.pid)/sizeof(stats.pid[0]); i++){
      if (stats.pid[i]) {
          printf(1, "%d\t%d\t%d\n", stats.pid[i], stats.tickets[i], stats.ticks[i]);    
      }
    }
    wait();
  }
  
  exit();
}