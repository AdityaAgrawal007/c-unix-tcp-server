#include "../include/utils.h"
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/wait.h>

void SIGCHLD_handler(int signo) {
  int saved_errno = errno;
  // we do not use wait over here because it will block
  pid_t pid;
  int stat;
  while (pid > 0)
  // -1 means wait for any child process, 2nd argument is used to specify where to store childs exit status, WNOHANG(macro constant) specifies do not block the caller if no child has exit
  // waitpid() returns process ID of terminated child and termination status throught a statloc pointer
  {
    pid = waitpid(-1, &stat, WNOHANG);
    printf("child %d terminated\n", pid);
  }

  errno = saved_errno;
  return;
}
