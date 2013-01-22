#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "client.h"

int main(int argc, char *argv[])
{
	// Initialisation des variables
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    // Arguments
    if (argc < 3)
    {
       fprintf(stderr,"Usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);

    // Initialisation
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // IPV4, intégrité+flux binaire
    if (sockfd < 0)
        display_error("ERREUR ouverture socket");

    // IP ou hostname en paramêtre ?
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    server = gethostbyname(argv[1]); // On récupère l'host par son nom
    if (server != NULL)
    {
        bcopy((char *)server->h_addr,
             (char *)&serv_addr.sin_addr.s_addr,
             server->h_length);
    } else
    {
    	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.95");
    	if (serv_addr.sin_addr.s_addr == INADDR_NONE)
    	{
			fprintf(stderr,"Socket Error # 11001, Host not found:\n");
			exit(0);
		}
    }

    serv_addr.sin_family = AF_INET; // IPV4
    serv_addr.sin_port = htons(portno); // Conversion et assignation du port
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) // Connexion au serveur
        display_error("Socket Error # 10061, Connection refused");
    printf("Entrez un message: ");

    memset(buffer, 0, BUFFER_LENGTH);
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer)); // Envoi du message
    if (n < 0)
         display_error("ERREUR ecriture du socket");

    n = read(sockfd,buffer,255); // Lecture de la réponse
    if (n < 0)
         display_error("ERREUR lecture du socket");
    printf("%s\n",buffer); // Affichage de la réponse

    close(sockfd); // Fermeture du socket

    return 0;
}
