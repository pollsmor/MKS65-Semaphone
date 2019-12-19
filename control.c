#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>

#define SEMKEY 24602
#define SHMKEY 24603
#define SEG_SIZE 1000

int main(int argc, char *argv[]) {
  if (argc > 1) {
    int semd;
    int shmd;
    int fd;

    if (strcmp(argv[1], "-c") == 0) {
      semd = semget(SEMKEY, 1, IPC_CREAT | IPC_EXCL | 0644);
      printf("semaphore created \n");

      shmd = shmget(SHMKEY, SEG_SIZE, IPC_CREAT | IPC_EXCL | 0644);
      printf("shared memory created \n");

      fd = open("story.txt", O_CREAT | O_TRUNC, 0644);
      printf("file created \n");

      return 0;
    }

    else if (strcmp(argv[1], "-v") == 0) {
      fd = open("story.txt", O_RDONLY);
      char story[SEG_SIZE];
      read(fd, story, SEG_SIZE);
      printf("%s", story);
      close(fd);

      return 0;
    }

    else if (strcmp(argv[1], "-r") == 0) {


      return 0;
    }

    else {
      printf("Invalid arguments. Only -c, -v, and -r are supported. \n");
      return 1;
    }
  }

  printf("You need to provide either the -c, -v, or -r argument to this program. \n");
  return 1;
}
