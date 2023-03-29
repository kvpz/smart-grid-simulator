/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BACKLOG_QUEUE_SIZE 5 // max size permitted by most systems

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    // file descriptors representing sockets
    int sockfd, newsockfd;

    // port number server will be accepting connections on
    int portno;

    // size of the address of the client
    socklen_t clilen;

    // buffer to store characters transmitted to server
    char buffer[256];

    // contain the internet address of the server
    struct sockaddr_in serv_addr;

    // contain the internet address of the client connected to the server
    struct sockaddr_in cli_addr;

    // number of characters read or written
    int n;

    // check if a port number was potentially passed to the program
    if (argc < 2) {
	fprintf(stderr,"ERROR, no port provided\n");
	exit(1);
    }

    // create an internet domain TCP socket endpoint 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
	error("ERROR opening socket");

    // set all values to zero
    bzero((char *) &serv_addr, sizeof(serv_addr));
    // more secure methods for setting buffer to zero
    //memset((char *) &serv_addr, 0, sizeof(serv_addr));
    //memset_s((char *) &serv_addr, ); // most secure implementation

    // define variables with socket values
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET; // address family is of TCP type
    serv_addr.sin_addr.s_addr = INADDR_ANY; // accept connection from any ip address
    serv_addr.sin_port = htons(portno); // port number in network(host) byte order (big endian)

    // assign serv_addr to the socket file descriptor (i.e. "assign name to the socket")
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	error("ERROR on binding");

    // start listening on the socket for connections; max backlog queue size is fixed
    listen(sockfd, BACKLOG_QUEUE_SIZE);

    // block (wait) until client connects to the server
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,
		       (struct sockaddr *) &cli_addr,
		       &clilen);
    if (newsockfd < 0)
	error("ERROR on accept");

    // clear the buffer
    bzero(buffer,256);
    
    // read from the socket message sent by client
    n = read(newsockfd,buffer,255); // block until socket has something 
    if (n < 0) error("ERROR reading from socket");
    printf("Here is the message: %s\n", buffer);

    // respond to client with a message
    n = write(newsockfd,"I got your message",18);
    if (n < 0) error("ERROR writing to socket");
    close(newsockfd);
    close(sockfd);
    return 0;
}
