#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void seive(int p[]) {
  int buf;
  if (read(p[0], &buf, sizeof(buf)) == 0){
    return;
  }
  int primeNum = buf;
  printf("prime %d \n", primeNum);
  int pp[2];
  pipe(pp);
  if (fork() == 0) {
    // 子线程
    close(pp[1]);
    seive(pp);
  } else {
    // 父线程
    while (read(p[0], &buf, sizeof(buf)) != 0)
    {
      if (buf % primeNum != 0)
      {
        write(pp[1], &buf, sizeof(buf));
      } 
    }
    close(p[1]);
    close(p[0]);
    close(pp[0]);
    close(pp[1]);
    wait(0);
  }
}

int
main(int argc, char *argv[])
{
  int p[2];
  pipe(p);
  if (fork() == 0) {
    close(p[1]);
    seive(p);
  } else {
    for (int i = 2; i < 36; i++)
    {
      write(p[1], &i, sizeof(i));
    }
    close(p[0]);
    close(p[1]);
    wait(0);
  }
  exit(0);
}