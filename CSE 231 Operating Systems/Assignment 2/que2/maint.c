#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <sched.h>

#define fp SCHED_OTHER
#define sp SCHED_RR
#define tp SCHED_FIFO



static struct timespec timestmp(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts;
}


static pid_t processgen(int scheduling_policy) {
  char * argv[]={};
  pid_t pid = fork();
  if (pid == 0) {
    struct sched_param param;
    param.sched_priority = 0;
    sched_setscheduler(0, scheduling_policy, &param);

    execvp("./ans.o", (char *const *) argv);
    exit(1);
  } else {
    return pid;
  }
}

int main() {
  struct timespec stp1,stp2,stp3;

  stp1=timestmp();
  pid_t pid1 = processgen(fp);
  stp2=timestmp();
  pid_t pid2 = processgen(sp);
  stp3=timestmp();
  pid_t pid3 = processgen(tp);

  int status;
  struct timespec etp1,etp2,etp3;
  waitpid(pid3, &status, 0);
  etp3=timestmp();
  waitpid(pid2, &status, 0);
  etp2= timestmp();
  waitpid(pid1, &status, 0);
  etp1= timestmp();
  double time_fifo = (etp3.tv_sec - stp3.tv_sec) +(etp3.tv_nsec - stp3.tv_nsec) / 1e9;
  double time_rr = (etp2.tv_sec - stp2.tv_sec) +(etp2.tv_nsec - stp2.tv_nsec) / 1e9;
  double time_other = (etp1.tv_sec - stp1.tv_sec) +(etp1.tv_nsec - stp1.tv_nsec) / 1e9;

  printf("Execution time for SCHED_OTHER: %f s\n", time_other);
  printf("Execution time for SCHED_RR: %f s\n", time_rr);
  printf("Execution time for SCHED_FIFO: %f s\n", time_fifo);

  FILE *fptr=fopen("output.txt","w");

  fprintf(fptr,"%f\n",time_fifo);
  fprintf(fptr,"%f\n",time_rr);
  fprintf(fptr,"%f",time_other);
  fclose(fptr);


  return 0;
}
