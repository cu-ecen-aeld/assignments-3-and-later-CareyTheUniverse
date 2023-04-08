#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


#define BUFFER_SIZE 1024
#define PORT 9000
#define DATA_FILE "/var/tmp/aesdsocketdata"


volatile sig_atomic_t stop_server = 0;


// Signal handler for SIGINT and SIGTERM
void signal_handler(int signal)
{
	if (signal == SIGINT || signal == SIGTERM)
	{
		syslog(LOG_INFO, "Caught signal, exiting");
		stop_server = 1;
	}
}


// Append data to the file
int append_to_file(const char* data) 
{
	int fd = open(DATA_FILE, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP);
	
	if (fd == -1) 
	{
		syslog(LOG_ERR, "Failed to open file %s, error: %s", DATA_FILE, strerror(errno));
		return -1;
	}
	
	size_t data_len = strlen(data);
	char data_new[data_len];
	memcpy(data_new, data, data_len);
	data_new[data_len] = '\n';
	data_new[data_len] = '\0';
	
	if (write(fd, data_new, strlen(data_new)) == -1) 
	{
		syslog(LOG_ERR, "Failed to write to file %s, error: %s", DATA_FILE, strerror(errno));
		close(fd);
		return -1;
	}
	
	close(fd);
	return 0;
}


int main(int argc, char *argv[])
{
	bool daemon_mode = false;

	// Check if -d argument is present
	if (argc > 1 && strcmp(argv[1], "-d") == 0) 
	{
		daemon_mode = true;
	}

	// Daemonize the program if -d argument is present
	if (daemon_mode) 
	{
		pid_t pid = fork();
		if (pid == -1) 
		{
			syslog(LOG_ERR, "Failed to fork into daemon process, error: %s", strerror(errno));
			return -1;
		} 
    		else if (pid != 0) 
    		{
			return 0;
		}
	}

	// Register signal handler for SIGINT and SIGTERM
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);

	// Create a socket
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (server_fd == -1) 
	{
		syslog(LOG_ERR, "Failed to create socket, error: %s", strerror(errno));
		return -1;
	    }

	// Bind socket to port
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);
	
	if (bind(server_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1) 
	{
		syslog(LOG_ERR, "Failed to bind socket, error: %s", strerror(errno));
		close(server_fd);
		return -1;
	}
	
	// Listen for incoming connections
	if (listen(server_fd, 1) == -1) 
	{
		syslog(LOG_ERR, "Failed to listen for connections, error: %s", strerror(errno));
		close(server_fd);
		return -1;
	}

	while (!stop_server) 
	{
		// Accept incoming connection
		struct sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);
		int client_fd = accept(server_fd, (struct sockaddr*) &client_addr, &client_addr_len);
		
		if (client_fd == -1) 
		{
			syslog(LOG_ERR, "Failed to accept incoming connection, error: %s", strerror(errno));
			continue;
		}
		
		// Log message for accepted connection
		char client_ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
		syslog(LOG_INFO, "Accepted connection from %s", client_ip);

		// Receive data and append to file
		char buffer[BUFFER_SIZE];
		memset(buffer, 0, BUFFER_SIZE);
		int bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
		
		if (bytes_received == -1) 
		{
			syslog(LOG_ERR, "Failed to receive data, error: %s", strerror(errno));
			close(client_fd);
			continue;
		}
		
		if (bytes_received == 0) 
		{
			syslog(LOG_INFO, "Connection closed by client");
			close(client_fd);
			continue;
		}
		
		// Append received data to file
		if (append_to_file(buffer) == -1) 
		{
			syslog(LOG_ERR, "Failed to append data to file");
		}
		
		// Send response
		char response[BUFFER_SIZE];
		strcpy(response, buffer);
		int bytes_sent = send(client_fd, response, strlen(response), 0);
		
		if (bytes_sent == -1) 
		{
			syslog(LOG_ERR, "Failed to send response, error: %s", strerror(errno));
			close(client_fd);
			continue;
		}
		
		close(client_fd);
	}

	close(server_fd);
	syslog(LOG_INFO, "Exiting");
	return 0;
}

