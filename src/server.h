#include <stdio.h>
#include "common.h"

// Variables globales pour le serveur
#define MAX_USERS 0		// Maximum d'utilisateurs connectés
int nb_users = 0;       // Utilisateurs actuellement connectés
int server_port = 0;	// Port d'écoute du serveur
client clients[]; 		// Tableau de tout les clients
