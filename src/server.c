#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "server.h"

client clients[MAX_USERS];
char buffer[BUFFER_LENGTH];

void handle_clients(int sockfd, struct sockaddr_in cli_addr) {
	int n, socket_newclient;
	socklen_t clilen = sizeof(cli_addr); // Taille de l'adresse du client

	socket_newclient = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen); // On attend les connexions
	if (socket_newclient < 0)
		display_error("ERREUR, accept");

	if(nb_users == MAX_USERS)
	{
		socket_send(socket_newclient, "421 Service not available. Too many users.");
	}
	else
	{
		// Nouveau client
		clients[nb_users]->addrip.s_addr = cli_addr.sin_addr.s_addr; // IP client
		clients[nb_users]->sock = socket_newclient; // Socket client
		clients[nb_users]->dataport = 0; // Port du client
		clients[nb_users]->pid = 0; // PID sous processus gérant le client
		strcpy(clients[nb_users]->curdir, "/"); // Répertoire par défaut du client
		nb_users++;

		n = read(socket_newclient,buffer,255); // Lecture du message
		if (n < 0) display_error("ERREUR, lecture du socket");

		printf("%s\n", buffer);
		socket_send(socket_newclient, "Message recus.");
	}
}

int main(int argc, char *argv[])
{
	// Initialisation des variables
	int sockfd, portno;
	struct sockaddr_in serv_addr, cli_addr;
	int i;

	// Arguments
	if (argc < 2) {
	 fprintf(stderr,"ERREUR, veuillez rentrer un numero de port\n");
	 exit(1);
	}
	portno = atoi(argv[1]);

	// Initilisation serveur
	sockfd = socket(AF_INET, SOCK_STREAM, 0); // IPV4, intégrité+flux binaire
	if (sockfd < 0)
		display_error("ERREUR, ouverture du socket impossible");
	memset((char *) &serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET; // IPV4
	serv_addr.sin_addr.s_addr = INADDR_ANY; // Accepter tout types d'adresses
	serv_addr.sin_port = htons(portno); // Conversion et assignation du port
	// On bind le socket et l'adresse
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		display_error("ERREUR, binding impossible de l'adresse au serveur");

	printf("Ecoute sur le port %i\n", portno);
	listen(sockfd, MAX_USERS); // Autoriser 5 connexions simultannées

	// On alloue la mémoire pour les clients
	for (i = 0; i < MAX_USERS; i++) {
		clients[i]=(client_t *)malloc(sizeof(client_t));
		clients[i]->sock = 0;
	}

	// Boucle principale
	int stop = 0;
	while(stop == 0) {
		// Connexion des clients
		handle_clients(sockfd, cli_addr);

		// Voir http://linux.die.net/man/3/fd_set pour gérer les sockets multiples
		if(nb_users > 0)
		{
			for(i=0; i<nb_users; i++)
			{
				if (clients[i]->sock)
				{
					read(clients[i]->sock,buffer,BUFFER_LENGTH);
					printf("Client %d : %s\n", i, buffer);
				}
				// TODO : Détecter fermeture de connexion d'un client
			}
		}
	}

	close(sockfd); // Fermeture du socket
	return 0;
}
