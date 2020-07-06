#include "io.inc"
#include <stdio.h>

int main() {
  printInt(177);
  printf("%d\n", judgeResult & 255u);
  return judgeResult;
}