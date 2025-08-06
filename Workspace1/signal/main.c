#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
int main(int argc, char **argv)
{
//	printf("testing signal\n");
//    raise(SIGSTOP);
//    printf("restarting program");

  // arlarm function test
  alarm(5);
  for(int i=1;i<10;i++){
      printf("testing alaram for %d second\n",i);
      sleep(1);
  }
	return 0;
}
