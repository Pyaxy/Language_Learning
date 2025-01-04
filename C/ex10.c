#include <stdio.h>

int main(int argc, char *argv[]) {
  int i = 0;

  for (i = 1; i < argc; i++) {
    printf("arg %d: %s\n", i, argv[i]);
  }

  char *states[] = {"California", "Oregon", "Washtington", "Texas", NULL};

  int num_states = 5;
  states[1] = argv[1];
  for (i = 1; i < argc; i++) {
    printf("arg %d: %s\n", i, argv[i]);
  }
  for (int i = 0; i < num_states; i++) {
    printf("state %d: %s\n", i, states[i]);
  }
  return 0;
}
