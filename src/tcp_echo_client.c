#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLINE 1000

void str_cli(FILE *fp, int sockfd){
	char sendline[MAXLINE], recvline[MAXLINE];
	while(fgets(sendline, MAXLINE, fp) != NULL){ // fgets reads up to newline char and null terminates string so the actual length is not MAXLINE
	write(sockfd, sendline, strlen(sendline));
	ssize_t n = read(sockfd, recvline, MAXLINE);
	if( n <= 0){ // read returns > 0 i.e. the number of bytes read, = 0 if connected terminated, -1 for error
		perror("str_cli: server terminated prematurely!");
		return;
	} 
	// when we type EOF char the str_cli function returns 
	// fputs(recvline, stdout); // expects null terminated string which read() does not do 
printf("Server replied: ");	
	fwrite(recvline, 1, n, stdout);
	}
}

int main(int argc, char **argv){
if (argc != 2) {
    fprintf(stderr, "usage: %s <server-ip>\n", argv[0]);
    exit(1);
}
	// creating a socket for the client
	int client_fd = socket(AF_INET, SOCK_STREAM, 0);

	// creating a struct to store the server info
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_port = htons(8080);
	server_addr.sin_family = AF_INET;

	inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

	// initiate a three way TCP handshake, the kernel acoosiates the socket (first argument) with a remote address and assigns it a local address and port 
	connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

	str_cli(stdin, client_fd);
	exit(0);
	// client socket is closed by the kernel, FIN(FINISH) tcp flag is sent to server to initiate graceful shutdown, server responds by ACK i.e. successfully recieved packet. when we do ctrl + c on server so we can in this in netstat -> 
	//CLOSE_WAIT mens that the other side (i.e. server) has closed connection
	//on servers entry we can see thing 
	//tcp        0      0 localhost:http-alt      localhost:51778         FIN_WAIT2 
	// FIN_WAIT2 means that this side has closed connection and is waiting for the other side to close theirs 
}
