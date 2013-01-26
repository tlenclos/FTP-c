#include "common.h"

void display_error(const char *msg)
{
    printf("Error : %s\n", msg);
}

ssize_t socket_send(int socket, char* message)
{
	int n;
	char buffer[BUFFER_LENGTH];
	memset(buffer, '\0', BUFFER_LENGTH);
	sprintf(buffer, "%s", message);
	n = write(socket,buffer, strlen(buffer));

	if(n < 0)
		display_error("Error while sending message");

	return n;
}
