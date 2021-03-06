
/* Name: Jennifer Luo
   COEN 162
   proxy HTTP GET request with TCP
 */

/* Generic */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Network */
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h> 
#include <sys/socket.h>

// main func
int main (int argc, char *argv[]) {
	int i;

	// check number of arguments
	if (argc != 3) {
		printf ("Usage: <executable><host><file path>");
		return 1;
	}

	struct hostent *server;
	struct sockaddr_in serv_addr;
	int sockfd = 0;
	int bytes, sent, received, total;
	char response[4096];

	// request
	char req[4096];
	memset(req, '0', sizeof(req));
	sprintf(req, "GET %s HTTP/1.0\r\n\r\n", argv[2]);

	printf("Request constructed: \n%s\n", req);

	// create the socket
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Error: Could not create socket \n");
		return 1;
	}

	// lookup ip addr
	if ((server = gethostbyname(argv[1])) == NULL) {
		printf("Error: no such host\n");
		return 1;
	}

	// fill in struct
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(80);
	memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

	// connect to socket
	if ( connect (sockfd, (struct sockaddr *)&serv_addr, sizeof (serv_addr)) < 0 )
	{
		printf ("Error : Connect Failed \n");
		return 1;
	} 

	// Send GET request > print
	if (send(sockfd, req, strlen(req), 0) < 0) { // keep sending bytes?
		printf("Send failed...\n");
	}

	// -----------------------------------------------FIX BElow------------------	

	memset(response, 0, sizeof(response));
	total = sizeof(response)-1;
	received = 0;
	printf("Response: \n");
	while (1) {
		printf("%s", response);
		memset(response, 0, sizeof(response));
		bytes = recv(sockfd, response, 1024, 0);
		if (bytes < 0)
			printf("ERROR reading response from socket");
		if (bytes == 0)
			break;
		received+=bytes;
	}

	if (received == total)
		printf("ERROR storing complete response from socket");

	// receiving at socket and writing to response
	//	while (recv(sockfd, response, sizeof(response), 0) > 0) { // keep receiving bytes?
	//		printf("Response: %s\n\n", response);
	//		printf("One cycle is over\n");
	//	}

	printf ("Finished\n");

	close(sockfd);
	return 0;

}
