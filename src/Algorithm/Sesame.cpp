#include "OfflineClustering/DbScan.hpp"
#include "WindowModel/WindowModel.hpp"
#include <stdio.h>

int internal(int a, int b)
{
  return a * b;
}

int main(int argc, char **argv)
{
  WindowModel window;
  DBScan db_scan;

  int one = 10;
  int two = 2;
  int result = internal(one, two);
  printf("%d\n", result);
  return 0;
}
