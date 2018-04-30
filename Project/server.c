#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int readline(int fd, char *str);
void *office(void *arg);

int main(int argc,char *argv[], char* env[]){

  if(argc != 4){
    printf("wrong arguments: server <num_room_seats> <num_ticket_offices> <open_time>\n");
    return -1;
  }

  if(argv[1] <= 0 || argv[2] <= 0 || argv[3] <= 0){
    printf("argument values not acceptable, must be positive integers\n");
    return -2;
  }

  unsigned int seats = argv[1];
  unsigned int nOffices = argv[2];
  unsigned int open_time = argv[3];

  mkfifo("requests", 0660);
  int fd = open("requests", O_RDONLY);

  for (unsigned int i = 0; i < nOffices; i++) {
    pthread_t office;
    pthread_create(&office, NULL, office, fd);
    pthread_exit(NULL);
  }

  close(fd);

  return 0;
}

int readline(int fd, char *str){
  int n;
  do{
    n = read(fd,str,1);
  }
  while (n>0 && *str++ != '\0');
  return (n>0);
}

void *office(void *arg){
  int fd = arg;
  char str[200];
  while(readline(fd,str)){
    printf("%s",str);
  }
  return NULL;
}
