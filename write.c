#include <stdio.h>
#include <string.h>
#include <errno.h>
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

int main() {
  int semd;
  union semun us;
  int shmd;
  int *last_line_size;
  int fd;

  shmd = shmget(SHMKEY, 0, 0);
  last_line_size = shmat(shmd, 0, 0);
  semd = semget(SEMKEY, 1, 0);
  printf("trying to get in \n");
  while (semctl(semd, 0, GETVAL, us) == 0);

  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_op = -1;
  semop(semd, &sb, 1); //take over the connection so no other programs can use it

  char last_addition[*last_line_size + 1];
  fd = open("story.txt", O_RDONLY);
  lseek(fd, -1 * *last_line_size, SEEK_END);
  //printf("Bytes read: %d \n", *last_line_size);
  read(fd, last_addition, *last_line_size);
  last_addition[*last_line_size] = '\0';

  printf("Last addition: %s \n", last_addition);

  printf("Your addition: ");
  char addition[100];
  fgets(addition, sizeof(addition), stdin);
  addition[strlen(addition) -1] = '\0'; //strip the newline at the end
  int len = strlen(addition);
  fd = open("story.txt", O_WRONLY | O_APPEND);
  write(fd, addition, len);
  *last_line_size = strlen(addition);
  printf("The story file has now been updated. \n");

  shmdt(last_line_size);
  sb.sem_op = 1;
  semop(semd, &sb, 1); //release the connection

  return 0;
}
