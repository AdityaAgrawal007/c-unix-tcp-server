#ifndef UTILS_H
#define UTILS_H

typedef void Sigfunc(int);

Sigfunc *Signal(int signal_no, Sigfunc *function_ptr);
void SIGCHLD_handler(int signo);

#endif
