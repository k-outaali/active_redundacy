
#include <sched.h>
#include <semaphore.h>

#ifndef  REDUNDANCY_H		
#define  REDUNDANCY_H  yes

#define CONFIGURE_REDUNDANCY_MAXIMUM_VOTING 10


typedef int (*voter_type) (void*);


typedef struct pthread_redundancy_attr {
  int priority;
  int size;
} pthread_redundancy_attr_t;


typedef struct pthread_redundancy{
  sem_t      wait_for_barrier[CONFIGURE_REDUNDANCY_MAXIMUM_VOTING];
  sem_t      completion_barrier[CONFIGURE_REDUNDANCY_MAXIMUM_VOTING];
  int        results[CONFIGURE_REDUNDANCY_MAXIMUM_VOTING];
  voter_type voters [CONFIGURE_REDUNDANCY_MAXIMUM_VOTING];
  void*      arg;
  struct     pthread_redundancy_attr attr;
  pthread_t tid[CONFIGURE_REDUNDANCY_MAXIMUM_VOTING];
  // Extend this struct if needed! 
} pthread_redundancy_t;




extern int pthread_redundancy_attr_init(struct pthread_redundancy_attr *a);
extern int pthread_redundancy_attr_setpriority(struct pthread_redundancy_attr *a, int prio);
extern int pthread_redundancy_attr_getpriority(struct pthread_redundancy_attr a);
extern int pthread_redundancy_attr_setsize(struct pthread_redundancy_attr *a, int s);
extern int pthread_redundancy_attr_getsize(struct pthread_redundancy_attr a);
extern int pthread_redundancy_attr_destroy(struct pthread_redundancy_attr *a);


extern int pthread_redundancy_init(struct pthread_redundancy* r, struct pthread_redundancy_attr a, voter_type v [], void* arg);
extern int pthread_redundancy_vote(struct pthread_redundancy* r, int* result);
extern int pthread_redundancy_destroy(struct pthread_redundancy* r);



#endif				


