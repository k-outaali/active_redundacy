


#include  <stdio.h>			
#include  <stdlib.h>			
#include  <string.h>		
#include  <semaphore.h>	

#include  "redundancy.h"			
#include  "returncode.h"			


int id = -1;


int pthread_redundancy_attr_destroy(struct pthread_redundancy_attr *a){
   return 0;
}


int pthread_redundancy_attr_init(struct pthread_redundancy_attr *a){

   a->size = 1;
   a->priority = 0;
   return 0;
}

int pthread_redundancy_attr_setpriority(struct pthread_redundancy_attr *a, int prio){

  a->priority = prio;
  return 0;
}

int pthread_redundancy_attr_getpriority(struct pthread_redundancy_attr a){

  return a.priority;
}

int pthread_redundancy_attr_setsize(struct pthread_redundancy_attr *a, int s){

  a->size = s;
  return 0;
}

int pthread_redundancy_attr_getsize(struct pthread_redundancy_attr a){

  return a.size;
}

int pthread_redundancy_destroy(struct pthread_redundancy *r) {

  int i;
  int ret;


  // Initially, cont=1 and threads are in a while loop until 
  // become cont=1
  //
  //r->cont=0;

  // Wakeup threads from their private semaphore to allow them
  // to finish
  //
  for(i=0; i<r->attr.size; i++){
    ret = sem_post(&r->wait_for_barrier[i]);
    if(ret!=0){
      returncode("pthread_redundancy_destroy sem_post",ret);
      return -1;
    }
  }

  return 0;
}


void *redudancy_thread(void *arg){

  id++;
  int ret;
  pthread_redundancy_t *r = arg;

  while(1) {
    ret = sem_wait(&(r->wait_for_barrier[id]));
    if(ret != 0){
      returncode("sem_wait failed", ret);
    }
    /*
    if (cont==0)
      pthread_exit(NULL);
    */
    r->results[id] = r->voters[id](arg);
    ret = sem_post(&(r->completion_barrier[id]));
    if(ret != 0){
      returncode("sem_post failed", ret);
    }
  }
}


int pthread_redundancy_init(struct pthread_redundancy* r, struct pthread_redundancy_attr attr, voter_t v[], void* arg) {
  
  int i, ret;
  id = -1;
  r->attr = attr;
  r->arg = arg;
  //r->cont = 1;
  for(i = 0; i < r->attr.size; i++) {
    r->voters[i] = v[i];

    ret = sem_init(&(r->wait_for_barrier[i]), 0, 0);
    if(ret != 0){
      returncode("sem_init failed", ret);
    }

    ret = sem_init(&(r->completion_barrier[i]), 0, 0);
    if(ret != 0){
      returncode("sem_init failed", ret);
    }

    ret = pthread_create(&(r->tid[i]), NULL, redudancy_thread, (void *) r);
    if(ret != 0){
      returncode("pthread_create failed", ret);
    }

  }
  return 0;
}









