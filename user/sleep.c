#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if (argc != 2)
  {
    write(1, "Need an argument \n", 19);
  }
  int time = atoi(argv[1]);
  sleep(time);
  exit(0);
}
