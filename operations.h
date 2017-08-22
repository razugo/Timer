#ifndef OPERATIONS_H
#define OPERATIONS_H
#define TICKS_PER_SECOND 25

int setupTimer(int argc, char ** argv);
void printTime();
void empty_handler(int signum);
void alrm_handler(int signum);
struct termios setupTerminal();
int characterHandler(char c, struct sigaction sa, struct sigaction empty);

#endif
