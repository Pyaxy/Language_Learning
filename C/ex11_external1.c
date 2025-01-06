
#include <stdio.h>

int main(int argc, char *argv[]) {
  int i = argc - 1;
  while (i >= 0) {
    printf("arg %d: %s\n", i, argv[i]);
    i--;
  }

  char *states[] = {"A", "B", "C", "D"};

  int num_states = 4;
  i = num_states - 1;
  while (i >= 0) {
    printf("State %d: %s\n", i, states[i]);
    i--;
  }
  return 0;
}
