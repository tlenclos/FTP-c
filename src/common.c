#include "common.h"

void display_error(const char *msg)
{
    printf("Error : %s\n", msg);
}

ssize_t socket_send(int socket, char* message)
{
	int n;
	n = write(socket,message, strlen(message));

	if(n < 0)
		display_error("Error while sending message");

	return n;
}

ssize_t socket_send_with_code(int socket, char* message, int code)
{
	char codestr[3];
	sprintf(codestr, "%d", code);
	char newmessage[strlen(message)+strlen(codestr)+1];
	strcpy(newmessage, codestr);
	strcat(newmessage, "-");
	strcat(newmessage, message);

	return socket_send(socket, newmessage);
}
