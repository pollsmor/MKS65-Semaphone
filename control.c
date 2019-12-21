#include <stdio.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>

#define SEMKEY 732841
#define SHMKEY 732842
#define SEG_SIZE 1000

union semun {
  int              val;    /* Value for SETVAL */
  struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short  *array;  /* Array for GETALL, SETALL */
  struct seminfo  *__buf;  /* Buffer for IPC_INFO
                              (Linux-specific) */
};

int main(int argc, char *argv[]) {
  if (argc > 1) {
    int semd;
    union semun us;
    int shmd;
    int fd;

    if (strcmp(argv[1], "-c") == 0) {
      semd = semget(SEMKEY, 1, IPC_CREAT | IPC_EXCL | 0644);
      us.val = 1;
      semctl(semd, 0, SETVAL, us); //this and the above line set the initial amount of connections open to 1
      printf("semaphore created \n");

      shmd = shmget(SHMKEY, sizeof(int), IPC_CREAT | IPC_EXCL | 0644); //only needs to store size of last line
      printf("shared memory created \n");

      fd = open("story.txt", O_CREAT | O_TRUNC, 0644);
      printf("file created \n");

      return 0;
    }

    else if (strcmp(argv[1], "-v") == 0) {
      fd = open("story.txt", O_RDONLY);
      char story[SEG_SIZE];
      story[0] = '\0'; //wouldn't normally be an issue but tweird stuff happens if I run -v on an empty story
      read(fd, story, SEG_SIZE);
      if (strlen(story) != 0)
        *(strrchr(story, '\n') + 1) = '\0'; //remove any characters to the right of the last enter (newline) - this issue likes to pop up
      printf("The story so far: \n");
      printf("%s", story);
      close(fd);

      return 0;
    }

    else if (strcmp(argv[1], "-r") == 0) {
      semd = semget(SEMKEY, 1, 0);
      printf("trying to get in \n\n");
      while (semctl(semd, 0, GETVAL, us) == 0); //block until GETVAL returns 1

      //copypasted from -v
      fd = open("story.txt", O_RDONLY);
      char story[SEG_SIZE];
      story[0] = '\0'; //wouldn't normally be an issue but tweird stuff happens if I run -v on an empty story
      read(fd, story, SEG_SIZE);
      if (strlen(story) != 0)
        *(strrchr(story, '\n') + 1) = '\0'; //remove any characters to the right of the last enter (newline) - this issue likes to pop up
      printf("The story so far: \n");
      printf("%s \n", story);
      close(fd);

      shmd = shmget(SHMKEY, 0, 0);
      shmctl(shmd, IPC_RMID, 0);
      printf("shared memory removed \n");

      remove("story.txt");
      printf("file removed \n");

      semctl(semd, IPC_RMID, 0);
      printf("semaphore removed \n");

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
