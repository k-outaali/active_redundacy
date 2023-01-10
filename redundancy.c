


#include  <stdio.h>			
#include  <stdlib.h>			
#include  <string.h>		
#include  <semaphore.h>	

#include  "redundancy.h"			
#include  "returncode.h"			


pthread_mutex_t id_mutex;


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

  static int id = -1;
  int ret, this_id;
  ret = pthread_mutex_lock(&id_mutex);
  if(ret != 0){
    returncode("sem_init failed", ret);
  }
  id++;
  this_id = id;
  ret = pthread_mutex_unlock(&id_mutex);
  if(ret != 0){
    returncode("sem_init failed", ret);
  }
  pthread_redundancy_t *r = arg;

  while(1) {
    ret = sem_wait(&(r->wait_for_barrier[this_id]));
    if(ret != 0){
      returncode("sem_wait failed", ret);
    }
    /*
    if (cont==0)
      pthread_exit(NULL);
    */
    r->results[this_id] = r->voters[this_id](r->arg);
    ret = sem_post(&(r->completion_barrier[this_id]));
    if(ret != 0){
      returncode("sem_post failed", ret);
    }
  }
}


int pthread_redundancy_init(struct pthread_redundancy* r, struct pthread_redundancy_attr attr, voter_t v[], void* arg) {
  
  int i, ret;
  //id = -1;
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
  ret = pthread_mutex_init(&id_mutex, NULL);
  if(ret != 0){
    returncode("sem_init failed", ret);
  }
  return 0;
}

int pthread_redundancy_vote(struct pthread_redundancy* r, int* result){

  int i, j, ret;
  int most_freq, max_freq, freqs[r->attr.size];

  for(i = 0; i < r->attr.size; i++){
    ret = sem_post(&(r->wait_for_barrier[i]));
    if(ret != 0){
      returncode("sem_post failed", ret);
    }
  }

  for(i = 0; i < r->attr.size; i++){
    freqs[i] = 0;
    ret = sem_wait(&(r->completion_barrier[i]));
    if(ret != 0){
      returncode("sem_wait failed", ret);
    }
  }
  
  for (i = 0; i < r->attr.size; i++){
    for (j = i + 1; j < r->attr.size; j++){
      if(r->results[i] == r->results[j]){
        freqs[i]++;
      }
    }
  }
  max_freq = freqs[0];
  most_freq = r->results[0];
  for(i = 0; i < r->attr.size - 1; i++){
    if(freqs[i] < freqs[i + 1]){
      max_freq = freqs[i + 1];
      most_freq = r->results[i + 1];
    }
  }
  *result = most_freq;
  return r->attr.size - max_freq - 1;
}






