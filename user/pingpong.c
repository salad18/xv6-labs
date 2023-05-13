#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char buf[2];
  int p[2];
  pipe(p);

  int pid = fork();
  if (pid < 0) {
    write(2, "error fork", 11);
  }
  if (pid == 0) {
    read(p[0], buf, 1);
    printf("%d: received ping\n", getpid());
    write(p[1], "c", 1);
    close(p[1]);
  } else {
    write(p[1], "p", 1);
    read(p[0], buf, 1);
    printf("%d: received pong\n", getpid());
    close(p[0]);
  }
  exit(0);
}
