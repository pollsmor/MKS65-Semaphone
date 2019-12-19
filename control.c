#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/sem.h>

#define KEY 24602

void create_sem(int *semd);

int main(int argc, char *argv[]) {
  if (argc > 1) {
    int semd;

    if (strcmp(argv[1], "-c") == 0) {
      create_sem(&semd);
      return 0;
    }

    else if (strcmp(argv[1], "-r") == 0) {
      return 0;
    }

    else if (strcmp(argv[1], "-v") == 0) {
      return 0;
    }

    else {
      printf("Invalid arguments. Only -c, -r, and -v are supported. \n");
      return 1;
    }
  }

  printf("You need to provide either the -c, -r, or -v argument to this program. \n");
  return 1;
}

void create_sem(int *semd) {
  *semd = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
  if (*semd == -1) {
    printf("error %d: %s \n", errno, strerror(errno));
    *semd = semget(KEY, 1, 0);
  }

  printf("semaphore created \n");
}
