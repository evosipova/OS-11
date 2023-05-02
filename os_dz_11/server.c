#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
  int serv_sock, clnt_sock1, clnt_sock2;
  struct sockaddr_in serv_addr, clnt_addr;
  socklen_t clnt_addr_size;
  char message[BUF_SIZE];

  if (argc != 3) {
    printf("Usage: %s <IP> <port>\n", argv[0]);
    exit(1);
  }

  serv_sock = socket(PF_INET, SOCK_STREAM, 0);
  if (serv_sock == -1) {
    perror("socket() error");
    exit(1);
  }

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port = htons(atoi(argv[2]));

  if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
    perror("bind() error");
    exit(1);
  }

  if (listen(serv_sock, 2) == -1) {
    perror("listen() error");
    exit(1);
  }

  clnt_addr_size = sizeof(clnt_addr);
  clnt_sock1 = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
  if (clnt_sock1 == -1) {
    perror("accept() error");
    exit(1);
  }
  printf("Client 1 connected\n");

  clnt_sock2 = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
  if (clnt_sock2 == -1) {
    perror("accept() error");
    exit(1);
  }
  printf("Client 2 connected\n");

  while (1) {
    int str_len = read(clnt_sock1, message, BUF_SIZE);
    if (str_len == 0) {
      break;
    }

    message[str_len] = 0;
    printf("Received from client 1: %s", message);

    if (strcmp(message, "The End\n") == 0) {
      break;
    }

    write(clnt_sock2, message, str_len);
  }

  close(clnt_sock1);
  close(clnt_sock2);
  close(serv_sock);

  return 0;
}
