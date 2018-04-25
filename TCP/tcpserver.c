/* 
 * tcpserver.c - A simple TCP echo server 
 * Usage: ./tcpserver <port>
 * Example: ./tcpserver 8080
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1024

/*
 *Error - wrapper for perror
 */
void error(char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char **argv) {
  int parentfd; /*Parent socket*/
  int childfd; /*Child socket*/
  int portno; /*Port to listen on*/
  int clientlen; /*Byte size of client's address*/
  struct sockaddr_in serveraddr; /*Server's addr*/
  struct sockaddr_in clientaddr; /*Client addr*/
  struct hostent *hostp; /*Client host info*/
  char buf[BUFSIZE]; /*Message buffer*/
  char *hostaddrp; /*Dotted decimal host addr string*/
  int optval; /*Flag value for setsockopt*/
  int n; /*Message byte size*/

  /* 
   *Check command line arguments 
   */
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }
  portno = atoi(argv[1]);

  /* 
   *Socket: create the parent socket 
   */
  parentfd = socket(AF_INET, SOCK_STREAM, 0);
  if (parentfd < 0) 
    error("ERROR opening socket");

  /* setsockopt: Handy debugging trick that lets 
   * us rerun the server immediately after we kill it; 
   * otherwise we would have to wait about 20 secs. 
   * Eliminates "ERROR on binding: Address already in use" error. 
   */
  optval = 1;
  setsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR, 
	     (const void *)&optval , sizeof(int));

  /*
   *Build the server's Internet address
   */
  bzero((char *) &serveraddr, sizeof(serveraddr));

  /*This is an Internet address*/
  serveraddr.sin_family = AF_INET;

  /*Let the system figure out our IP address*/
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

  /*This is the port we will listen on*/
  serveraddr.sin_port = htons((unsigned short)portno);

  /* 
   *bind: associate the parent socket with a port 
   */
  if (bind(parentfd, (struct sockaddr *) &serveraddr, 
	   sizeof(serveraddr)) < 0) 
    error("ERROR on binding");

  /* 
   *listen: make this socket ready to accept connection requests 
   */
  if (listen(parentfd, 5) < 0) /* allow 5 requests to queue up */ 
    error("ERROR on listen");

  /* 
   *Main loop: wait for a connection request, echo input line, 
   * then close connection.
   */
  clientlen = sizeof(clientaddr);
  while (1) {

    /* 
     *accept: wait for a connection request 
     */
    childfd = accept(parentfd, (struct sockaddr *) &clientaddr, &clientlen);
    if (childfd < 0) 
      error("ERROR on accept");
    
    /* 
     *gethostbyaddr: determine who sent the message 
     */
    hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
			  sizeof(clientaddr.sin_addr.s_addr), AF_INET);
    if (hostp == NULL)
      error("ERROR on gethostbyaddr");
    hostaddrp = inet_ntoa(clientaddr.sin_addr);
    if (hostaddrp == NULL)
      error("ERROR on inet_ntoa\n");
    printf("server established connection with %s (%s)\n", 
	   hostp->h_name, hostaddrp);
    
    /* 
     *read: read input string from the client
     */
    bzero(buf, BUFSIZE);
    n = read(childfd, buf, BUFSIZE);
    if (n < 0) 
      error("ERROR reading from socket");
    printf("server received %d bytes: %s", n, buf);
    
    /* 
     *write: echo the input string back to the client 
     */
    n = write(childfd, buf, strlen(buf));
    if (n < 0) 
      error("ERROR writing to socket");

    close(childfd);
  }
}
