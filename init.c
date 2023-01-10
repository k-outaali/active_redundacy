#define CONFIGURE_INIT

#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <sched.h>

#include "system.h"
#include "redundancy.h"
#include "returncode.h"


int sensor1(void* arg) {
   printf("Call sensor1\n"); 
   return 100;
}

int val=98;
int sensor2(void* arg) {
   printf("Call sensor2\n"); 
   val++;
   return val;
}

int sensor3(void* arg) {
   printf("Call sensor3\n"); 
   return 100;
}




void* POSIX_Init(void *argument){

 int ret;
 int my_result;
 int diff;

 pthread_redundancy_attr_t attr;
 pthread_redundancy_t      red;
 voter_t func[3];

 pthread_redundancy_attr_init(&attr);
 pthread_redundancy_attr_setpriority(&attr, 100);
 pthread_redundancy_attr_setsize(&attr, 3);

 func[0] = sensor1; 
 func[1] = sensor2; 
 func[2] = sensor3;

 ret = pthread_redundancy_init(&red, attr, func, NULL);
 if(ret !=0 )
     returncode("pthread_redundancy_init init.c",ret);

 diff = pthread_redundancy_vote(&red, &my_result);
 if(ret != 0)
     returncode("pthread_redundancy_vote init.c",ret);
 printf("my_result = %d and diff = %d\n", my_result, diff);
 
 diff = pthread_redundancy_vote(&red, &my_result);
 if(ret != 0)
     returncode("pthread_redundancy_vote init.c",ret);
 printf("my_result = %d and diff = %d\n", my_result, diff);

 pthread_redundancy_destroy(&red);
 pthread_exit(NULL);
 return 0; 
}



