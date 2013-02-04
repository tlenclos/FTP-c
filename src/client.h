#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include "common.h"
#include "common.c"

int data_port = 2000;

void clear_and_prompt(void);
void insertConsole(char *);
void init_client(char* hote, char* numero_port);
