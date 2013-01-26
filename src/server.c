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
int number_file_descriptor_set = 0;

// Gestion des nouveaux clients
void handle_clients(int socket_server, struct sockaddr_in cli_addr)
{
	int socket_newclient;
	socklen_t clilen = sizeof(cli_addr); // Taille de l'adresse du client

	socket_newclient = accept(socket_server, (struct sockaddr *) &cli_addr, &clilen); // On attend les connexions
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

		socket_send(socket_newclient, "Bienvenue sur le serveur FTP.");
	}
}

// Suppression d'un client
void remove_handle_client(client client)
{
	close(client->sock);
	client->pid = 0;
	client->sock = 0;
	client->dataport = 0;
	nb_users--;
}

// Main
int main(int argc, char *argv[])
{
	// Initialisation des variables
	int socket_server, portno;
	struct sockaddr_in serv_addr, cli_addr;
	int i;
	fd_set read_sd, rsd; // Descripteur pour la lecture d'un socket

	// Arguments
	if (argc < 2) {
	 fprintf(stderr,"ERREUR, veuillez rentrer un numero de port\n");
	 exit(1);
	}
	portno = atoi(argv[1]);

	// Initilisation serveur
	socket_server = socket(AF_INET, SOCK_STREAM, 0); // IPV4, intégrité+flux binaire
	if (socket_server < 0)
		display_error("ERREUR, ouverture du socket impossible");
	memset((char *) &serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET; // IPV4
	serv_addr.sin_addr.s_addr = INADDR_ANY; // Accepter tout types d'adresses
	serv_addr.sin_port = htons(portno); // Conversion et assignation du port
	// On bind le socket et l'adresse
	if (bind(socket_server, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		display_error("ERREUR, binding impossible de l'adresse au serveur");

	printf("Ecoute sur le port %i\n", portno);
	listen(socket_server, MAX_USERS); // Autoriser 5 connexions simultannées

	// On alloue la mémoire pour les clients
	for (i = 0; i < MAX_USERS; i++) {
		clients[i]=(client_t *)malloc(sizeof(client_t));
		clients[i]->sock = 0;
	}

	// Descripteurs pour l'écoute
	FD_ZERO(&read_sd);
	FD_SET(1, &read_sd); // Entrée clavier
	FD_SET(socket_server, &read_sd); // Socket du serveur

	// Boucle principale
	int stop = 0;
	while(!stop) {
		rsd = read_sd;

		// On ajoute tout les descripteurs pour les clients
		for (i = 0; i < nb_users; i++)
			if (clients[i]->sock)
				FD_SET(clients[i]->sock, &rsd);

		// Select bloque tant qu'un descripteur n'est pas actif
		// TODO : Modifier la valeur qui est arbitraire (maximum du nb de descripteurs)
		select(12, &rsd, (fd_set *)0, (fd_set *)0, (struct timeval *)0);

		// Gestion des commandes serveur
		if (FD_ISSET(1, &rsd))
		{
			read(1, buffer, BUFFER_LENGTH);
			printf("Votre commande est : %s", buffer);
			printf("Fermeture du serveur...\n");
			break;
		}

		// Gestion des nouveau clients
		if (FD_ISSET(socket_server, &rsd))
		{
			handle_clients(socket_server, cli_addr);
		}

		// Gestion des utilisateurs connectés
		for(i=0; i<nb_users; i++) // On boucle sur les utilisateurs connectés
		{
			if (clients[i]->sock && FD_ISSET(clients[i]->sock, &rsd))
			{
				int data_read = read(clients[i]->sock,buffer,BUFFER_LENGTH);

				if(data_read > 0)
				{
					// Lecture des données
					printf("Client %d : %s", i, buffer);
				}
				else
				{
					// Déconnexion du client
					printf("Deconnexion du client %d\n", i);
					remove_handle_client(clients[i]);
				}
			}
		}
	}

	close(socket_server); // Fermeture du socket
	return 0;
}
