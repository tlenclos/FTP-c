#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "server.h"

int main(int argc, char *argv[])
{
	// Initialisation des variables
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	// Arguments
	if (argc < 2) {
	 fprintf(stderr,"ERREUR, veuillez rentrer un numero de port\n");
	 exit(1);
	}
	portno = atoi(argv[1]);

	// Initilisation
	sockfd = socket(AF_INET, SOCK_STREAM, 0); // IPV4, intégrité+flux binaire
	if (sockfd < 0)
		error("ERREUR, ouverture du socket impossible");
	memset((char *) &serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET; // IPV4
	serv_addr.sin_addr.s_addr = INADDR_ANY; // Accepter tout types d'adresses
	serv_addr.sin_port = htons(portno); // Conversion et assignation du port
	// On bind le socket et l'adresse
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERREUR, binding impossible de l'adresse au serveur");

	listen(sockfd,5); // Autoriser 5 connexions simultannées
	clilen = sizeof(cli_addr); // Taille de l'adresse du client
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen); // On attend les connexions
	if (newsockfd < 0)
	  error("ERREUR, accept");
	printf("Ecoute sur le port %i", portno);

	memset(buffer, 0, 256); // Assignation du buffer pour les messages du client
	n = read(newsockfd,buffer,255); // Lecture du message
	if (n < 0) error("ERREUR, lecture du socket");
	printf("Message recus: %s\n",buffer);
	n = write(newsockfd,"Message recus",18); // Envoi de la reponse
	if (n < 0) error("ERREUR ecriture socket");

	close(newsockfd); // Fermeture du socket
	close(sockfd); // Fermeture du socket

	return 0;
}
