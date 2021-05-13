#include <stdio.h>

#include "module.h"

int internal(int a, int b)
{
  return a * b;
}

int main(int argc, char **argv)
{
  int one = 10;
  int two = 2;
  int result = internal(one, two);
  result += external(result);
  printf("%d\n", result);
  return 0;
}
