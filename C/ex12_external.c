#include <stdio.h>

int main(int argc, char *argv[]) {
  int i = 0;
  int user_argc = argc - 1;
  if (user_argc == 1) {
    printf("You only have one argument. You suck.\n");
  } else if (user_argc == 0) {
    printf("You don't have any argument.\n");
  } else if (user_argc > 1 && user_argc < 4) {
    printf("Here is your arguments:\n");
    for (i = 0; i < user_argc; i++) {
      printf("%s ", argv[i + 1]);
    }
    puts("");
  } else {
    printf("You have to many arguments.\n");
  }

  return 0;
}
