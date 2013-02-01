#include "client.h"

// Vider le tampon et demander une commande
void clear_and_prompt()
{
	printf("%s","client>");
	fflush(stdout);
}

int open_data_socket(struct sockaddr_in serv_addr, int dataport, int asclient) {
	struct sockaddr_in to;
	int sd, tolen;

	to.sin_family = AF_INET;
	to.sin_port = htons(dataport);
	to.sin_addr.s_addr = asclient == 1 ? serv_addr.sin_addr.s_addr : INADDR_ANY;

	tolen = sizeof(to);

	if( (sd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("Erreur socket data\n");
		return -1;
	}

	if(asclient == 1)
	{
		if(0 > connect(sd, (struct sockaddr*)&to, tolen))
		{
			printf("Erreur connect data socket\n");
			return -1;
		}
	}
	else
	{
		if (bind(sd, (struct sockaddr *) &to, tolen) < 0)
		{
			printf("Erreur bind data socket\n");
			return -1;
		}

		// Ecoute du serveur
		if(listen(sd, 1) < 0)
		{
			printf("Erreur listen data socket\n");
			return -1;
		}
	}

	return sd;
}

void cmd_retr(struct sockaddr_in serv_addr) {
	// On écoute une connexion nouvelle connexion sur le port 2000 (défaut) pour la réception du fichier
    int server_datasocket = 0;
    struct sockaddr_in from;
	socklen_t fromlen = sizeof(from);

	int datasocket = open_data_socket(serv_addr, data_port, 0); // client devient serveur
	if(datasocket > 0) {
		server_datasocket = accept(datasocket, (struct sockaddr *) &from, &fromlen);
	} else {
		printf("ouverture datasocket fail\n");
		exit(0);
	}

	int fd, recv = 1, writesize = 1;
	char bufferfile[BUFFER_LENGTH];

	// Enregistrement du fichier
	if(0 > (fd = open("test", O_CREAT|O_TRUNC|O_WRONLY, S_IRUSR|S_IWUSR)))
	{
		perror("open");
		exit(1);
	}

	int size_received = 0;
	while( writesize != 0 )
	{
		recv = read(server_datasocket, bufferfile, sizeof(bufferfile));
		writesize = write(fd, bufferfile, recv);
		size_received += recv;

		if( writesize == 0 )
		{
			printf("Réception de \"%s\" (%d)\n", "test", size_received);
			close(server_datasocket);
		}
	}
}

// Main
int main(int argc, char *argv[])
{
	// Initialisation des variables
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[BUFFER_LENGTH];
    memset(buffer, '\0', BUFFER_LENGTH);

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
    }
    else
    {
    	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    	if (serv_addr.sin_addr.s_addr == INADDR_NONE)
    	{
            fprintf(stderr,"Socket Error # 11001, Host not found:\n");
			exit(0);
        }
    }

    serv_addr.sin_family = AF_INET; // IPV4
    serv_addr.sin_port = htons(portno); // Conversion et assignation du port
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) // Connexion au serveur
    {
    	display_error("Socket Error # 10061, Connection refused");
    	exit(1);
    }

    n = read(sockfd,buffer,255); // Premiere message du serveur
    printf("%s\n",buffer); // Affichage de la réponse serveur

    clear_and_prompt();
    memset(buffer, 0, BUFFER_LENGTH);

    fgets(buffer,BUFFER_LENGTH,stdin);
    n = write(sockfd,buffer,strlen(buffer)); // Envoi du message
    memset(buffer, 0, BUFFER_LENGTH);

    n = read(sockfd,buffer,255);
    printf("%s\n",buffer);

    fgets(buffer,BUFFER_LENGTH,stdin);
	n = write(sockfd,buffer,strlen(buffer)); // Envoi du message
	memset(buffer, 0, BUFFER_LENGTH);

	n = read(sockfd,buffer,255);
	printf("%s\n",buffer);

	fgets(buffer,BUFFER_LENGTH,stdin);
	n = write(sockfd,buffer,strlen(buffer)); // Envoi du message
	memset(buffer, 0, BUFFER_LENGTH);

    n = read(sockfd,buffer,255);
    printf("%s\n",buffer);

    close(sockfd); // Fermeture du socket

    return 0;
}
