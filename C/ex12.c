#include <stdio.h>

int main(int argc, char *argv[]) {
  int i = 0;
  if (argc == 1) {
    printf("You only have one argument. You suck.\n");
  } else if (argc > 1 && argc < 4) {
    printf("Here is your arguments:\n");
    for (i = 0; i < argc; i++) {
      printf("%s ", argv[i]);
    }
    puts("");
  } else {
    printf("You have to many arguments.\n");
  }

  return 0;
}
