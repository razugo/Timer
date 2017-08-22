#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include "operations.h"

int main(int argc, char ** argv)
{
	int loop = setupTimer(argc, argv);
	struct sigaction sa, old_alarm,  empty;
	struct itimerval timer;
	struct termios old_tio = setupTerminal();

	memset ( &sa, 0, sizeof ( sa ) ) ;
	memset ( &empty, 0, sizeof ( empty ) ) ;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGALRM);

	timer.it_value.tv_sec = 0 ;
   	timer.it_value.tv_usec = 1000000 / TICKS_PER_SECOND;
   	timer.it_interval.tv_sec = 0;
   	timer.it_interval.tv_usec = 1000000 / TICKS_PER_SECOND;

	sa.sa_handler = &alrm_handler;
	empty.sa_handler = &empty_handler;
	sigaction(SIGALRM, &sa, &old_alarm);
	setitimer(ITIMER_REAL, &timer, NULL);

	while(loop == 0)
	{
		loop = characterHandler(getchar(), sa, empty);	
	}

	tcsetattr(0,TCSANOW,&old_tio);
	sigaction(SIGALRM, &old_alarm, NULL);
	printf("\n");

	return 0;
}

	

