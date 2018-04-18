/* Your project consists of building a tool to obtain a web object from a web server.

Details: Your tool should ask the user for the URL or the server's address 
and file path/name. 
It then establishes a connection using sockets to the server's address
and uses HTTP (over a TCP connection) to send a GET message requesting 
the object using the file path/name. 
Then, waits to receive a response from the server, which is shown on the screen. 
You may use any computer (your laptop or a machine in the DC).

*/

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
#include <netdb.h>
#include <netinet/in.h> 
#include <sys/socket.h>

// GET request subroutine
char* GET(char *path) {
  char req[1000] = {0};
  sprintf(req, "GET %s HTTP/1.0\r\n\r\n", path);
  return req;
}

// main func
int main (int argc, char *argv[]) {
	int i;

	// check number of arguments
	if (argc != 3) {
		printf ("Usage: <executable><host><file path>");
		return 1;
	}

	char host[1028] = strlen(argv[1])>0 ? argv[1]: "localhost";

	struct hostent *server;
	struct sockaddr_in serv_addr;
	int sockfd, bytes, sent, received, total;
    char response[4096];

    // request
    char req[4096] = GET(argv[2]);

    printf("Request constructed: \n%s\n", req);
	
	// create the socket
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Error: Could not create socket \n");
		return 1;
	}

	// lookup ip addr
	if ((server = gethostbyname(host)) == NULL) {
		printf("Error: no such host\n");
		return 1;
	}

	// fill in struct
	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(response, '0', sizeof(response));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = 80;
	memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);
	
	// // inet_pton converts argv[2] into sin_addr
	// if (inet_pton(AF_INET, argv[2], &serv_addr.sin_addr) < 0) {
	// 	printf("Error: inet_pton error \n");
	// 	return 1;
	// } else if (inet_pton(AF_INET, argv[2], &serv_addr.sin_addr) == 0) {
	// 	printf("Usage: src string not valid \n");
	// 	return 1;
	// }

	// connect the socket
	if (connect (sockfd, (struct sockaddr *)&serv_addr, sizeof (serv_addr)) < 0)
	{
		printf ("Error : Connect Failed \n");
		return 1;
	} 

	// Send GET request > print
	if (send(sockfd, req, strlen(req), 0) < 0) { // keep sending bytes?
		printf("Send failed...\n");
	}

	while (recv(sockfd, response, sizeof(response), 0) > 0) { // keep receiving bytes?
	   // fputs(response, stdout);
	   printf("Response: %s\n", response);
	}

	printf ("Finished\n");

	close(sockfd);
	return 0;
	
}