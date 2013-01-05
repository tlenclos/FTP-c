#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "client.h"

int main(int argc, char *argv[])
{
	// Initialisation des variables
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    // Arguments
    if (argc < 3) {
       fprintf(stderr,"Usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);

    // Initialisation
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // IPV4, intégrité+flux binaire
    if (sockfd < 0)
        error("ERREUR ouverture socket");

    server = gethostbyname(argv[1]); // On récupère l'host par son nom
    if (server == NULL) {
        fprintf(stderr,"ERREUR host inexistant\n");
        exit(0);
    }

    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; // IPV4
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno); // Conversion et assignation du port
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) // Connexion au serveur
        error("ERREUR de connexion");
    printf("Entrez un message: ");

    memset(buffer, 0, 256);
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer)); // Envoi du message
    if (n < 0)
         error("ERREUR ecriture du socket");

    memset(buffer, 0, 256);
    n = read(sockfd,buffer,255); // Lecture de la réponse
    if (n < 0)
         error("ERREUR lecture du socket");
    printf("%s\n",buffer); // Affichage de la réponse

    close(sockfd); // Fermeture du socket

    return 0;
}
