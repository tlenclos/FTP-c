#ifndef COMMON_H
#define COMMON_H

void display_error(const char *msg);
ssize_t socket_send(int socket, char* message);

// Commandes FTP selon la RFC
int const nb_commandes = 33;
char const *commandes[33] = {
	// Controle d'accès
	"USER",
	"PASS",
	"ACCT",
	"CWD",
	"CDUP",
	"SMNT",
	"REIN",
	"QUIT",
	// Paramètres de tranfert
	"PORT",
	"PASV",
	"TYPE",
	"STRU",
	"MODE",
	// Service FTP
	"RETR",
	"STOR",
	"STOU",
	"APPE",
	"ALLO",
	"REST",
	"RNFR",
	"RNTO",
	"ABOR",
	"DELE",
	"RMD",
	"MKD",
	"PWD",
	"LIST",
	"NLST",
	"SITE",
	"SYST",
	"STAT",
	"HELP",
	"NOOP"
};

// Structure d'un client
typedef struct {

	int sock, pid;					// Socket de communication, pid du processus fils servant le client
	struct in_addr addrip;			// Adresse IP du client
	unsigned short int dataport;	// Port de communication de donnée (définis par le client)
	char curdir[256];				// Répertoire courant du client

} client_t, *client;

// Max buffer length
#define BUFFER_LENGTH 256

#endif
