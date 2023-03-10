
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <pthread.h>

#include "returncode.h"

void returncode(char* s, int status)
{
	printf("returncode/status  : %d\n", status);
	printf("returncode/thread  : %x\n", pthread_self());
	printf("returncode/errno   : %x\n", errno);
	printf("returncode/message : %s\n", s);
}

