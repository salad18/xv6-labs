#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"


int
main(int argc, char *argv[])
{
  // 用来存储新的参数
  char *args[MAXARG];
  int count = 0;
  // argv[0]是xargs
  for (int i = 1; i < argc; i++)
  { 
    args[count] = argv[i];
    count++;
  }
  // 一个个读取字符
  char arg[100];
  char *c = arg;
  int readCount = 0;
  while (read(0, c, 1) > 0)
  {
    if (*c == '\n')
    {
      *c = '\0';
      args[count] = arg;
      argc++;
      if (count + readCount >= MAXARG)
      {
        write(2, "exceed max arugument limit", 27);
      }
      if (fork() == 0)
      {
        exec(args[0], args);
      }
      else
      {
        wait(0);
        c = arg;
        readCount = 0;
      }
    }
    else {
      c++;
    }
  }
  
  
  exit(0);
}
