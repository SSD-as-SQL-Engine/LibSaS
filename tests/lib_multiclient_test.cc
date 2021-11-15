#include "sas.h"
#include <pthread.h>

struct arg_struct {
  sas::SaS sas;
  int index;
};

void *MyWork(void *arg) {

  struct arg_struct *args = (struct arg_struct *)arg;
  sas::SaS sas = args->sas;
  int index = args->index;
  char buffer[4096];
  sprintf(buffer, "insert into test values (%d);", index);
  sas.executeQuery(std::string(buffer));

  fprintf(stderr, "[DEBUG] index %d submit ioctl!\n", index);
  return (void*)0;
}


int main(void) {
  pthread_t thds[10];

  sas::SaS sas;

  // 0. open the SaS database
  sas.open("/dev/nvme1n1");

  // 1. create test table 
  sas.executeQuery("create table test (a int);");

  // 2. select table 
  sas.executeQuery("select * from test;");

  int i=0;
  for (i=0; i<5; i++) {
    struct arg_struct args;
    args.index = i;
    args.sas = sas;
    fprintf(stderr, "create pthread : %d\n", i);
    pthread_create(&thds[i], 0, MyWork, (void *)&args); 
  }

  for (i=0; i<5; i++) {
    pthread_join(thds[i], 0);
  }

  return 0;
}
