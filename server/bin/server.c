#include <dbg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
  int sockfd, newsockfd, portno;
  socklen_t clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;
  check(argc == 2, "USAGE: server <port>")
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  check(sockfd >= 0, "error opening socket");
  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  check(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) >= 0,
        "ERROR on binding"
       );
  while (1) {
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,
        (struct sockaddr *) &cli_addr,
        &clilen);
    check(newsockfd >= 0, "error on accept");
    bzero(buffer,256);
    n = read(newsockfd,buffer,255);
    check(n >= 0, "error reading from socket");
    log_info("received: %s",buffer);
    n = write(newsockfd, buffer, strlen(buffer));
    check(n >= 0, "Error writing to socket");
  }
  close(newsockfd);
  close(sockfd);
  return 0;

error:
  return -1;
}
