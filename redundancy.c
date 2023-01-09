


#include  <stdio.h>			
#include  <stdlib.h>			
#include  <string.h>			

#include  "redundancy.h"			
#include  "returncode.h"			





int pthread_redundancy_attr_destroy(struct pthread_redundancy_attr_t *a) {
   return 0;
}


int pthread_redundancy_attr_init(struct pthread_redundancy_attr_t *a) {
   a->size = 1;
   a->priority = 0;
   return 0;
}


int pthread_redundancy_destroy(struct pthread_redundancy_t* r) {

  int i;
  int status;


  // Initially, cont=1 and threads are in a while loop until 
  // become cont=1
  //
  r->cont=0;

  // Wakeup threads from their private semaphore to allow them
  // to finish
  //
  for(i=0; i<r->attr.size; i++)
    {
    status=sem_post(&r->waitfor_barrier[i]);
    if(status!=0)
        {
           returncode("pthread_redundancy_destroy sem_post",status);
           return status;
        }
    }

  return 0;
}





int pthread_redundancy_init(struct pthread_redundancy_t* r, struct pthread_redundancy_attr_t a, voter_type v [], void* arg) {
  
  int i;

  r->attr = a;
  r->arg = arg;
  r->cont = 1;
  for(i = 0; i < r->attr.size; i++) 
    {
    r->voters[i] = v[i];

    ***
    }

    ***

    return 0;
}









