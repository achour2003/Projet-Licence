/*#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

#define MAX 80
#define PORT 42042
#define SA struct sockaddr

typedef struct {
    int sockfd;
    char pseudo[20];
    int is_banned;
    int is_suspended;
} client_t;

client_t clients[10]; // Maximum de 10 clients
int client_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
int server_running = 1; // Flag pour gérer l'état du serveur

void send_to_all(char *message, int sender_sock) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < client_count; i++) {
        if (clients[i].sockfd != sender_sock && !clients[i].is_banned) {
            write(clients[i].sockfd, message, strlen(message));
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void send_to_client(char *message, char *pseudo) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            
            write(clients[i].sockfd,message, strlen(message));
            break;
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void *client_handler(void *arg) {
    client_t *client = (client_t *)arg;
    char buff[MAX];
    int n;

    while (1) {
        bzero(buff, MAX);
        n = read(client->sockfd, buff, sizeof(buff));
        if (n <= 0) {
            break;
        }
        printf("Du client (%s): %s", client->pseudo, buff);

        if (strncmp(buff, "exit", 4) == 0) {
            printf("%s a quitté...\n", client->pseudo);
            break;
        } else if (client->is_banned) {
            write(client->sockfd, "Vous êtes banni.\n", strlen("Vous êtes banni.\n"));
        } else if (client->is_suspended) {
            write(client->sockfd, "Vous êtes suspendu et ne pouvez pas envoyer de messages.\n", strlen("Vous êtes suspendu et ne pouvez pas envoyer de messages.\n"));
        } else {
            if (buff[0] == '@') {
                // Handle private message
                char target_pseudo[20];
                sscanf(buff, "@%s", target_pseudo);
                char *private_message = strchr(buff, ' ') + 1;
                send_to_client(private_message, target_pseudo);
            } else {
                send_to_all(buff, client->sockfd);
            }
        }
    }

    close(client->sockfd);
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i].sockfd == client->sockfd) {
            for (int j = i; j < client_count - 1; j++) {
                clients[j] = clients[j + 1];
            }
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    free(client);
    pthread_exit(NULL);
}

void ban_client(char *pseudo) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            clients[i].is_banned = 1;
            write(clients[i].sockfd, "Vous êtes banni.\n", strlen("Vous êtes banni.\n"));
            close(clients[i].sockfd); // Fermer la connexion du client banni
            for (int j = i; j < client_count - 1; j++) {
                clients[j] = clients[j + 1];
            }
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void suspend_client(char *pseudo) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            clients[i].is_suspended = 1;
            write(clients[i].sockfd, "Le serveur a demandé de suspendre votre terminal de saisie.\n", strlen("Le serveur a demandé de suspendre votre terminal de saisie.\n"));
            write(clients[i].sockfd, "!!suspend\n", strlen("!!suspend\n")); // Commande interne au superviseur pour envoyer SIGSTOP
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void forgive_client(char *pseudo) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            clients[i].is_suspended = 0;
            write(clients[i].sockfd, "Le serveur a demandé de rétablir votre terminal de saisie.\n", strlen("Le serveur a demandé de rétablir votre terminal de saisie.\n"));
            write(clients[i].sockfd, "!!forgive\n", strlen("!!forgive\n")); // Commande interne au superviseur pour envoyer SIGCONT
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void *manage_server(void *arg) {
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("La création du socket a échoué...\n");
        exit(0);
    } else {
        printf("Socket créé avec succès..\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
        printf("La liaison du socket a échoué...\n");
        exit(0);
    } else {
        printf("Socket lié avec succès..\n");
    }

    if ((listen(sockfd, 5)) != 0) {
        printf("L'écoute a échoué...\n");
        exit(0);
    } else {
        printf("Serveur en écoute..\n");
    }
    len = sizeof(cli);

    while (1) {
        connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
        if (connfd < 0) {
            printf("Acceptation du serveur a échoué...\n");
            exit(0);
        } else {
            printf("Le serveur a accepté un client...\n");
        }

        if (server_running) {
            pthread_mutex_lock(&clients_mutex);
            client_t *new_client = (client_t *)malloc(sizeof(client_t));
            new_client->sockfd = connfd;
            new_client->is_banned = 0;
            new_client->is_suspended = 0;
            read(connfd, new_client->pseudo, sizeof(new_client->pseudo));
            clients[client_count++] = *new_client;
            pthread_mutex_unlock(&clients_mutex);

            pthread_t tid;
            pthread_create(&tid, NULL, client_handler, (void *)new_client);
        } else {
            write(connfd, "Nouvelles connexions désactivées.\n", strlen("Nouvelles connexions désactivées.\n"));
            close(connfd);
        }
    }

    close(sockfd);
    return NULL;
}

void handle_commands(char *command) {
    char pseudo[20];
    if (strcmp(command, "!start\n") == 0) {
        server_running = 0;
        printf("Nouvelles connexions désactivées.\n");
    } else if (sscanf(command, "@%s !ban", pseudo) == 1) {
        ban_client(pseudo);
    } else if (sscanf(command, "@%s !suspend", pseudo) == 1) {
        suspend_client(pseudo);
    } else if (sscanf(command, "@%s !forgive", pseudo) == 1) {
        forgive_client(pseudo);
    } else {
        printf("Commande inconnue : %s", command);
    }
}

int main() {
    pthread_t server_thread;
    pthread_create(&server_thread, NULL, manage_server, NULL);

    char command[MAX];
    while (1) {
        fgets(command, MAX, stdin);
        handle_commands(command);
    }

    return 0;
}*//*
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

#define MAX 80
#define PORT 42042
#define SA struct sockaddr

typedef struct {
    int sockfd;
    char pseudo[20];
    int is_banned;
    int is_suspended;
} client_t;

client_t clients[10]; // Maximum de 10 clients
int client_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
int server_running = 1; // Flag pour gérer l'état du serveur

void send_to_all(char *message, int sender_sock) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < client_count; i++) {
        if (clients[i].sockfd != sender_sock && !clients[i].is_banned) {
            write(clients[i].sockfd, message, strlen(message));
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void send_to_client(char *message, char *pseudo) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            write(clients[i].sockfd, message, strlen(message));
            break;
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void *client_handler(void *arg) {
    client_t *client = (client_t *)arg;
    char buff[MAX];
    int n;

    while (1) {
        bzero(buff, MAX);
        n = read(client->sockfd, buff, sizeof(buff));
        if (n <= 0) {
            break;
        }
        printf("Du client (%s): %s", client->pseudo, buff);

        if (strncmp(buff, "exit", 4) == 0) {
            printf("%s a quitté...\n", client->pseudo);
            break;
        } else if (client->is_banned) {
            write(client->sockfd, "Vous êtes banni.\n", strlen("Vous êtes banni.\n"));
        } else if (client->is_suspended) {
            write(client->sockfd, "Vous êtes suspendu et ne pouvez pas envoyer de messages.\n", strlen("Vous êtes suspendu et ne pouvez pas envoyer de messages.\n"));
        } else {
            char message_with_pseudo[MAX + 40]; // Taille du buffer pour inclure le pseudo
            snprintf(message_with_pseudo, sizeof(message_with_pseudo), "%s: %s", client->pseudo, buff);

            if (buff[0] == '@') {
                // Handle private message
                char target_pseudo[20];
                sscanf(buff, "@%s", target_pseudo);
                char *private_message = strchr(buff, ' ') + 1;
                snprintf(message_with_pseudo, sizeof(message_with_pseudo), "(Privé) %s: %s", client->pseudo, private_message);
                send_to_client(message_with_pseudo, target_pseudo);
            } else {
                send_to_all(message_with_pseudo, client->sockfd);
            }
        }
    }

    close(client->sockfd);
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i].sockfd == client->sockfd) {
            for (int j = i; j < client_count - 1; j++) {
                clients[j] = clients[j + 1];
            }
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    free(client);
    pthread_exit(NULL);
}

void ban_client(char *pseudo) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            clients[i].is_banned = 1;
            write(clients[i].sockfd, "Vous êtes banni.\n", strlen("Vous êtes banni.\n"));
            close(clients[i].sockfd); // Fermer la connexion du client banni
            for (int j = i; j < client_count - 1; j++) {
                clients[j] = clients[j + 1];
            }
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void suspend_client(char *pseudo) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            clients[i].is_suspended = 1;
            write(clients[i].sockfd, "Le serveur a demandé de suspendre votre terminal de saisie.\n", strlen("Le serveur a demandé de suspendre votre terminal de saisie.\n"));
            write(clients[i].sockfd, "!!suspend\n", strlen("!!suspend\n")); // Commande interne au superviseur pour envoyer SIGSTOP
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void forgive_client(char *pseudo) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            clients[i].is_suspended = 0;
            write(clients[i].sockfd, "Le serveur a demandé de rétablir votre terminal de saisie.\n", strlen("Le serveur a demandé de rétablir votre terminal de saisie.\n"));
            write(clients[i].sockfd, "!!forgive\n", strlen("!!forgive\n")); // Commande interne au superviseur pour envoyer SIGCONT
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void *manage_server(void *arg) {
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("La création du socket a échoué...\n");
        exit(0);
    } else {
        printf("Socket créé avec succès..\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
        printf("La liaison du socket a échoué...\n");
        exit(0);
    } else {
        printf("Socket lié avec succès..\n");
    }

    if ((listen(sockfd, 5)) != 0) {
        printf("L'écoute a échoué...\n");
        exit(0);
    } else {
        printf("Serveur en écoute..\n");
    }
    len = sizeof(cli);

    while (1) {
        connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
        if (connfd < 0) {
            printf("Acceptation du serveur a échoué...\n");
            exit(0);
        } else {
            printf("Le serveur a accepté un client...\n");
        }

        if (server_running) {
            pthread_mutex_lock(&clients_mutex);
            client_t *new_client = (client_t *)malloc(sizeof(client_t));
            new_client->sockfd = connfd;
            new_client->is_banned = 0;
            new_client->is_suspended = 0;
            read(connfd, new_client->pseudo, sizeof(new_client->pseudo));
            clients[client_count++] = *new_client;
            pthread_mutex_unlock(&clients_mutex);

            pthread_t tid;
            pthread_create(&tid, NULL, client_handler, (void *)new_client);
        } else {
            write(connfd, "Nouvelles connexions désactivées.\n", strlen("Nouvelles connexions désactivées.\n"));
            close(connfd);
        }
    }

    close(sockfd);
    return NULL;
}

void handle_commands(char *command) {
    char pseudo[20];
    if (strcmp(command, "!start\n") == 0) {
        server_running = 0;
        printf("Nouvelles connexions désactivées.\n");
    } else if (sscanf(command, "@%s !ban", pseudo) == 1) {
        ban_client(pseudo);
    } else if (sscanf(command, "@%s !suspend", pseudo) == 1) {
        suspend_client(pseudo);
    } else if (sscanf(command, "@%s !forgive", pseudo) == 1) {
        forgive_client(pseudo);
    } else {
        printf("Commande inconnue : %s", command);
    }
}

int main() {
    pthread_t server_thread;
    pthread_create(&server_thread, NULL, manage_server, NULL);

    char command[MAX];
    while (1) {
        fgets(command, MAX, stdin);
        handle_commands(command);
    }

    return 0;
}
*//*
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

#define MAX 80
#define PORT 42042
#define SA struct sockaddr

typedef struct {
    int sockfd;
    char pseudo[20];
    int is_banned;
    int is_suspended;
} client_t;

client_t clients[10]; // Maximum de 10 clients
int client_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
int server_running = 1; // Flag pour gérer l'état du serveur

void send_to_all(char *message, int sender_sock) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < client_count; i++) {
        if (clients[i].sockfd != sender_sock && !clients[i].is_banned) {
            write(clients[i].sockfd, message, strlen(message));
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void send_to_client(char *message, char *pseudo) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            write(clients[i].sockfd, message, strlen(message));
            break;
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void *client_handler(void *arg) {
    client_t *client = (client_t *)arg;
    char buff[MAX];
    int n;

    while (1) {
        bzero(buff, MAX);
        n = read(client->sockfd, buff, sizeof(buff));
        if (n <= 0) {
            break;
        }
        printf("Du client (%s): %s", client->pseudo, buff);

        if (strncmp(buff, "exit", 4) == 0) {
            printf("%s a quitté...\n", client->pseudo);
            break;
        } else if (client->is_banned) {
            write(client->sockfd, "Vous êtes banni.\n", strlen("Vous êtes banni.\n"));
        } else if (client->is_suspended) {
            write(client->sockfd, "Vous êtes suspendu et ne pouvez pas envoyer de messages.\n", strlen("Vous êtes suspendu et ne pouvez pas envoyer de messages.\n"));
        } else {
            char message_with_pseudo[MAX + 40]; // Taille du buffer pour inclure le pseudo
            snprintf(message_with_pseudo, sizeof(message_with_pseudo), "%s: %s", client->pseudo, buff);

            if (buff[0] == '@') {
                // Handle private message
                char target_pseudo[20];
                sscanf(buff, "@%s", target_pseudo);
                char *private_message = strchr(buff, ' ') + 1;
                snprintf(message_with_pseudo, sizeof(message_with_pseudo), "%s: %s", client->pseudo, private_message);
                send_to_client(message_with_pseudo, target_pseudo);
            } else {
                send_to_all(message_with_pseudo, client->sockfd);
            }
        }
    }

    close(client->sockfd);
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i].sockfd == client->sockfd) {
            for (int j = i; j < client_count - 1; j++) {
                clients[j] = clients[j + 1];
            }
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    free(client);
    pthread_exit(NULL);
}

void ban_client(char *pseudo) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            clients[i].is_banned = 1;
            write(clients[i].sockfd, "Vous êtes banni.\n", strlen("Vous êtes banni.\n"));
            close(clients[i].sockfd); // Fermer la connexion du client banni
            for (int j = i; j < client_count - 1; j++) {
                clients[j] = clients[j + 1];
            }
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void suspend_client(char *pseudo) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            clients[i].is_suspended = 1;
            write(clients[i].sockfd, "Le serveur a demandé de suspendre votre terminal de saisie.\n", strlen("Le serveur a demandé de suspendre votre terminal de saisie.\n"));
            write(clients[i].sockfd, "!!suspend\n", strlen("!!suspend\n")); // Commande interne au superviseur pour envoyer SIGSTOP
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void forgive_client(char *pseudo) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            clients[i].is_suspended = 0;
            write(clients[i].sockfd, "Le serveur a demandé de rétablir votre terminal de saisie.\n", strlen("Le serveur a demandé de rétablir votre terminal de saisie.\n"));
            write(clients[i].sockfd, "!!forgive\n", strlen("!!forgive\n")); // Commande interne au superviseur pour envoyer SIGCONT
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void *manage_server(void *arg) {
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("La création du socket a échoué...\n");
        exit(0);
    } else {
        printf("Socket créé avec succès..\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
        printf("La liaison du socket a échoué...\n");
        exit(0);
    } else {
        printf("Socket lié avec succès..\n");
    }

    if ((listen(sockfd, 5)) != 0) {
        printf("L'écoute a échoué...\n");
        exit(0);
    } else {
        printf("Serveur en écoute..\n");
    }
    len = sizeof(cli);

    while (1) {
        connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
        if (connfd < 0) {
            printf("Acceptation du serveur a échoué...\n");
            exit(0);
        } else {
            printf("Le serveur a accepté un client...\n");
        }

        if (server_running) {
            pthread_mutex_lock(&clients_mutex);
            client_t *new_client = (client_t *)malloc(sizeof(client_t));
            new_client->sockfd = connfd;
            new_client->is_banned = 0;
            new_client->is_suspended = 0;
            read(connfd, new_client->pseudo, sizeof(new_client->pseudo));
            clients[client_count++] = *new_client;
            pthread_mutex_unlock(&clients_mutex);

            pthread_t tid;
            pthread_create(&tid, NULL, client_handler, (void *)new_client);
        } else {
            write(connfd, "Nouvelles connexions désactivées.\n", strlen("Nouvelles connexions désactivées.\n"));
            close(connfd);
        }
    }

    close(sockfd);
    return NULL;
}

void handle_commands(char *command) {
    char pseudo[20];
    if (strcmp(command, "!start\n") == 0) {
        server_running = 0;
        printf("Nouvelles connexions désactivées.\n");
    } else if (sscanf(command, "@%s !ban", pseudo) == 1) {
        ban_client(pseudo);
    } else if (sscanf(command, "@%s !suspend", pseudo) == 1) {
        suspend_client(pseudo);
    } else if (sscanf(command, "@%s !forgive", pseudo) == 1) {
        forgive_client(pseudo);
    } else {
        printf("Commande inconnue : %s", command);
    }
}

int main() {
    pthread_t server_thread;
    pthread_create(&server_thread, NULL, manage_server, NULL);

    char command[MAX];
    while (1) {
        fgets(command, MAX, stdin);
        handle_commands(command);
    }

    return 0;
}
*//*
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

#define MAX 80
#define PORT 42042
#define SA struct sockaddr

typedef struct {
    int sockfd;
    char pseudo[20];
    int is_banned;
    int is_suspended;
} client_t;

client_t clients[10]; // Maximum de 10 clients
int client_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
int server_running = 1; // Flag pour gérer l'état du serveur

void send_to_all(char *message, int sender_sock) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < client_count; i++) {
        if (clients[i].sockfd != sender_sock && !clients[i].is_banned) {
            write(clients[i].sockfd, message, strlen(message));
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void send_to_client(char *message, char *pseudo) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            write(clients[i].sockfd, message, strlen(message));
            break;
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void *client_handler(void *arg) {
    client_t *client = (client_t *)arg;
    char buff[MAX];
    int n;

    while (1) {
        bzero(buff, MAX);
        n = read(client->sockfd, buff, sizeof(buff));
        if (n <= 0) {
            break;
        }
        printf("Du client (%s): %s", client->pseudo, buff);

        if (strncmp(buff, "exit", 4) == 0) {
            printf("%s a quitté...\n", client->pseudo);
            break;
        } else if (client->is_banned) {
            write(client->sockfd, "Vous êtes banni.\n", strlen("Vous êtes banni.\n"));
        } else if (client->is_suspended) {
            write(client->sockfd, "Vous êtes suspendu et ne pouvez pas envoyer de messages.\n", strlen("Vous êtes suspendu et ne pouvez pas envoyer de messages.\n"));
        } else {
            char message_with_pseudo[MAX + 40]; // Taille du buffer pour inclure le pseudo
            snprintf(message_with_pseudo, sizeof(message_with_pseudo), "%s: %s", client->pseudo, buff);

            if (buff[0] == '@') {
                // Handle private message
                char target_pseudo[20];
                sscanf(buff, "@%s", target_pseudo);
                char *private_message = strchr(buff, ' ') + 1;
                snprintf(message_with_pseudo, sizeof(message_with_pseudo), "%s: %s", client->pseudo, private_message);
                send_to_client(message_with_pseudo, target_pseudo);
            } else {
                send_to_all(message_with_pseudo, client->sockfd);
            }
        }
    }

    close(client->sockfd);
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i].sockfd == client->sockfd) {
            for (int j = i; j < client_count - 1; j++) {
                clients[j] = clients[j + 1];
            }
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    free(client);
    pthread_exit(NULL);
}

void ban_client(char *pseudo) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            clients[i].is_banned = 1;
            write(clients[i].sockfd, "Vous êtes banni.\n", strlen("Vous êtes banni.\n"));
            close(clients[i].sockfd); // Fermer la connexion du client banni
            for (int j = i; j < client_count - 1; j++) {
                clients[j] = clients[j + 1];
            }
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void suspend_client(char *pseudo) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            clients[i].is_suspended = 1;
            write(clients[i].sockfd, "Le serveur a demandé de suspendre votre terminal de saisie.\n", strlen("Le serveur a demandé de suspendre votre terminal de saisie.\n"));
            write(clients[i].sockfd, "!!suspend\n", strlen("!!suspend\n")); // Commande interne au superviseur pour envoyer SIGSTOP
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void forgive_client(char *pseudo) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            clients[i].is_suspended = 0;
            write(clients[i].sockfd, "Le serveur a demandé de rétablir votre terminal de saisie.\n", strlen("Le serveur a demandé de rétablir votre terminal de saisie.\n"));
            write(clients[i].sockfd, "!!forgive\n", strlen("!!forgive\n")); // Commande interne au superviseur pour envoyer SIGCONT
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void list_clients() {
    pthread_mutex_lock(&clients_mutex);
    
    printf("Liste des clients connectés :\n");
    for (int i = 0; i < client_count; i++) {
        printf("%s\n", clients[i].pseudo);
    }
    
    pthread_mutex_unlock(&clients_mutex);
}

void *manage_server(void *arg) {
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("La création du socket a échoué...\n");
        exit(0);
    } else {
        printf("Socket créé avec succès..\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
        printf("La liaison du socket a échoué...\n");
        exit(0);
    } else {
        printf("Socket lié avec succès..\n");
    }

    if ((listen(sockfd, 5)) != 0) {
        printf("L'écoute a échoué...\n");
        exit(0);
    } else {
        printf("Serveur en écoute..\n");
    }
    len = sizeof(cli);

    while (1) {
        connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
        if (connfd < 0) {
            printf("Acceptation du serveur a échoué...\n");
            exit(0);
        } else {
            printf("Le serveur a accepté un client...\n");
        }

        if (server_running) {
            pthread_mutex_lock(&clients_mutex);
            client_t *new_client = (client_t *)malloc(sizeof(client_t));
            new_client->sockfd = connfd;
            new_client->is_banned = 0;
            new_client->is_suspended = 0;
            read(connfd, new_client->pseudo, sizeof(new_client->pseudo));
            clients[client_count++] = *new_client;
            pthread_mutex_unlock(&clients_mutex);

            pthread_t tid;
            pthread_create(&tid, NULL, client_handler, (void *)new_client);
        } else {
            write(connfd, "Nouvelles connexions désactivées.\n", strlen("Nouvelles connexions désactivées.\n"));
            close(connfd);
        }
    }

    close(sockfd);
    return NULL;
}

void handle_commands(char *command) {
    char pseudo[20];
    if (strcmp(command, "!start\n") == 0) {
        server_running = 0;
        printf("Nouvelles connexions désactivées.\n");
    } else if (strcmp(command, "!liste\n") == 0) {
        list_clients();
    } else if (sscanf(command, "@%s !ban", pseudo) == 1) {
        ban_client(pseudo);
    } else if (sscanf(command, "@%s !suspend", pseudo) == 1) {
        suspend_client(pseudo);
    } else if (sscanf(command, "@%s !forgive", pseudo) == 1) {
        forgive_client(pseudo);
    } else {
        printf("Commande inconnue : %s", command);
    }
}

int main() {
    pthread_t server_thread;
    pthread_create(&server_thread, NULL, manage_server, NULL);

    char command[MAX];
    while (1) {
        fgets(command, MAX, stdin);
        handle_commands(command);
    }

    return 0;
}
*/
/*

//marche bien*/
/*
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

#define MAX 80
#define PORT 42042
#define SA struct sockaddr

typedef struct {
    int sockfd;
    char pseudo[20];
    int is_banned;
    int is_suspended;
} client_t;

client_t clients[10]; // Maximum de 10 clients
int client_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
int server_running = 1; // Flag pour gérer l'état du serveur

void send_to_all(char *message, int sender_sock) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < client_count; i++) {
        if (clients[i].sockfd != sender_sock && !clients[i].is_banned) {
            write(clients[i].sockfd, message, strlen(message));
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void send_to_client(char *message, char *pseudo) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            write(clients[i].sockfd, message, strlen(message));
            break;
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void list_clients_for_client(int client_sockfd) {
    pthread_mutex_lock(&clients_mutex);
    
    char message[MAX * 10]; // Buffer pour la liste des clients
    bzero(message, sizeof(message));
    strcat(message, "Liste des clients connectés :\n");
    strcat(message, "Modérateur\n"); // Ajouter le serveur comme modérateur
    for (int i = 0; i < client_count; i++) {
        strcat(message, clients[i].pseudo);
        strcat(message, "\n");
    }
    write(client_sockfd, message, strlen(message));
    
    pthread_mutex_unlock(&clients_mutex);
}

void *client_handler(void *arg) {
    client_t *client = (client_t *)arg;
    char buff[MAX];
    int n;

    while (1) {
        bzero(buff, MAX);
        n = read(client->sockfd, buff, sizeof(buff));
        if (n <= 0) {
            break;
        }
        printf("Du client (%s): %s", client->pseudo, buff);

        if (strncmp(buff, "exit", 4) == 0) {
            printf("%s a quitté...\n", client->pseudo);
            break;
        } else if (client->is_banned) {
            write(client->sockfd, "Vous êtes banni.\n", strlen("Vous êtes banni.\n"));
        } else if (client->is_suspended) {
            write(client->sockfd, "Vous êtes suspendu et ne pouvez pas envoyer de messages.\n", strlen("Vous êtes suspendu et ne pouvez pas envoyer de messages.\n"));
        } else if (strncmp(buff, "!liste", 6) == 0) {
            list_clients_for_client(client->sockfd);
        } else {
            char message_with_pseudo[MAX + 40]; // Taille du buffer pour inclure le pseudo
            snprintf(message_with_pseudo, sizeof(message_with_pseudo), "%s: %s", client->pseudo, buff);

            if (buff[0] == '@') {
                // Handle private message
                char target_pseudo[20];
                sscanf(buff, "@%s", target_pseudo);
                char *private_message = strchr(buff, ' ') + 1;
                snprintf(message_with_pseudo, sizeof(message_with_pseudo), "%s: %s", client->pseudo, private_message);
                send_to_client(message_with_pseudo, target_pseudo);
            } else {
                send_to_all(message_with_pseudo, client->sockfd);
            }
        }
    }

    close(client->sockfd);
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i].sockfd == client->sockfd) {
            for (int j = i; j < client_count - 1; j++) {
                clients[j] = clients[j + 1];
            }
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    free(client);
    pthread_exit(NULL);
}

void ban_client(char *pseudo) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            clients[i].is_banned = 1;
            write(clients[i].sockfd, "Vous êtes banni.\n", strlen("Vous êtes banni.\n"));
            close(clients[i].sockfd); // Fermer la connexion du client banni
            for (int j = i; j < client_count - 1; j++) {
                clients[j] = clients[j + 1];
            }
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void suspend_client(char *pseudo) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            clients[i].is_suspended = 1;
            write(clients[i].sockfd, "Le serveur a suspendu votre compte. Vous pouvez recevoir des messages mais pas en envoyer.\n", strlen("Le serveur a suspendu votre compte. Vous pouvez recevoir des messages mais pas en envoyer.\n"));
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void forgive_client(char *pseudo) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            clients[i].is_suspended = 0;
            write(clients[i].sockfd, "Le serveur a rétabli votre compte. Vous pouvez maintenant envoyer des messages.\n", strlen("Le serveur a rétabli votre compte. Vous pouvez maintenant envoyer des messages.\n"));
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void list_clients() {
    pthread_mutex_lock(&clients_mutex);
    
    printf("Liste des clients connectés :\n");
    printf("Modérateur\n"); // Ajouter le serveur comme modérateur
    for (int i = 0; i < client_count; i++) {
        printf("%s\n", clients[i].pseudo);
    }
    
    pthread_mutex_unlock(&clients_mutex);
}

int includes_command(const char *command) {
    if (strstr(command, "@") && strstr(command, "!ban")) {
        return 1;
    } else if (strstr(command, "@") && strstr(command, "!suspend")) {
        return 2;
    } else if (strstr(command, "@") && strstr(command, "!forgive")) {
        return 3;
    }
    return 0;
}

void *manage_server(void *arg) {
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("La création du socket a échoué...\n");
        exit(0);
    } else {
        printf("Socket créé avec succès..\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
        printf("La liaison du socket a échoué...\n");
        exit(0);
    } else {
        printf("Socket lié avec succès..\n");
    }

    if ((listen(sockfd, 5)) != 0) {
        printf("L'écoute a échoué...\n");
        exit(0);
    } else {
        printf("Serveur en écoute..\n");
    }
    len = sizeof(cli);

    while (1) {
        connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
        if (connfd < 0) {
            printf("Acceptation du serveur a échoué...\n");
            exit(0);
        } else {
            printf("Le serveur a accepté un client...\n");
        }

        if (server_running) {
            pthread_mutex_lock(&clients_mutex);
            client_t *new_client = (client_t *)malloc(sizeof(client_t));
            new_client->sockfd = connfd;
            new_client->is_banned = 0;
            new_client->is_suspended = 0;
            read(connfd, new_client->pseudo, sizeof(new_client->pseudo));
            clients[client_count++] = *new_client;
            pthread_mutex_unlock(&clients_mutex);

            pthread_t tid;
            pthread_create(&tid, NULL, client_handler, (void *)new_client);
        } else {
            write(connfd, "Nouvelles connexions désactivées.\n", strlen("Nouvelles connexions désactivées.\n"));
            close(connfd);
        }
    }

    close(sockfd);
    return NULL;
}

void handle_commands(char *command) {
    char pseudo[20];
    int command_type = includes_command(command);
    if (strncmp(command, "!start", 6) == 0) {
        server_running = 0;
        printf("Nouvelles connexions désactivées.\n");
    } else if (strncmp(command, "!liste", 6) == 0) {
        list_clients();
    } else if (command_type == 1 && sscanf(command, "@%s !ban", pseudo) == 1) {
        ban_client(pseudo);
    } else if (command_type == 2 && sscanf(command, "@%s !suspend", pseudo) == 1) {
        suspend_client(pseudo);
    } else if (command_type == 3 && sscanf(command, "@%s !forgive", pseudo) == 1) {
        forgive_client(pseudo);
    } else {
        printf("Commande inconnue : %s", command);
    }
}

int main() {
    pthread_t server_thread;
    pthread_create(&server_thread, NULL, manage_server, NULL);

    char command[MAX];
    while (1) {
        fgets(command, MAX, stdin);
        handle_commands(command);
    }

    return 0;
}
*/ /*
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

#define MAX 80
#define PORT 42042
#define SA struct sockaddr

typedef struct {
    int sockfd;
    char pseudo[20];
    int is_banned;
    int is_suspended;
} client_t;

client_t clients[10]; // Maximum de 10 clients
int client_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
int server_running = 1; // Flag pour gérer l'état du serveur

void send_to_all(char *message, int sender_sock) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < client_count; i++) {
        if (clients[i].sockfd != sender_sock && !clients[i].is_banned) {
            write(clients[i].sockfd, message, strlen(message));
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void send_to_client(char *message, char *pseudo) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            write(clients[i].sockfd, message, strlen(message));
            break;
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void list_clients_for_client(int client_sockfd) {
    pthread_mutex_lock(&clients_mutex);
    
    char message[MAX * 10]; // Buffer pour la liste des clients
    bzero(message, sizeof(message));
    strcat(message, "Liste des clients connectés :\n");
    strcat(message, "Modérateur\n"); // Ajouter le serveur comme modérateur
    for (int i = 0; i < client_count; i++) {
        strcat(message, clients[i].pseudo);
        strcat(message, "\n");
    }
    write(client_sockfd, message, strlen(message));
    
    pthread_mutex_unlock(&clients_mutex);
}

void *client_handler(void *arg) {
    client_t *client = (client_t *)arg;
    char buff[MAX];
    int n;

    while (1) {
        bzero(buff, MAX);
        n = read(client->sockfd, buff, sizeof(buff));
        if (n <= 0) {
            break;
        }
        printf("Du client (%s): %s", client->pseudo, buff);

        if (strncmp(buff, "exit", 4) == 0) {
            printf("%s a quitté...\n", client->pseudo);
            break;
        } else if (client->is_banned) {
            write(client->sockfd, "Vous êtes banni.\n", strlen("Vous êtes banni.\n"));
        } else if (client->is_suspended) {
            write(client->sockfd, "Vous êtes suspendu et ne pouvez pas envoyer de messages.\n", strlen("Vous êtes suspendu et ne pouvez pas envoyer de messages.\n"));
        } else if (strncmp(buff, "!liste", 6) == 0) {
            list_clients_for_client(client->sockfd);
        } else {
            char message_with_pseudo[MAX + 40]; // Taille du buffer pour inclure le pseudo
            snprintf(message_with_pseudo, sizeof(message_with_pseudo), "%s: %s", client->pseudo, buff);

            if (buff[0] == '@') {
                // Handle private message
                char target_pseudo[20];
                sscanf(buff, "@%s", target_pseudo);
                char *private_message = strchr(buff, ' ') + 1;
                snprintf(message_with_pseudo, sizeof(message_with_pseudo), "%s: %s", client->pseudo, private_message);
                if (!client->is_suspended) {
                    send_to_client(message_with_pseudo, target_pseudo);
                } else {
                    write(client->sockfd, "Vous êtes suspendu et ne pouvez pas envoyer de messages.\n", strlen("Vous êtes suspendu et ne pouvez pas envoyer de messages.\n"));
                }
            } else {
                if (!client->is_suspended) {
                    send_to_all(message_with_pseudo, client->sockfd);
                } else {
                    write(client->sockfd, "Vous êtes suspendu et ne pouvez pas envoyer de messages.\n", strlen("Vous êtes suspendu et ne pouvez pas envoyer de messages.\n"));
                }
            }
        }
    }

    close(client->sockfd);
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i].sockfd == client->sockfd) {
            for (int j = i; j < client_count - 1; j++) {
                clients[j] = clients[j + 1];
            }
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    free(client);
    pthread_exit(NULL);
}

void ban_client(char *pseudo) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            clients[i].is_banned = 1;
            write(clients[i].sockfd, "Vous êtes banni.\n", strlen("Vous êtes banni.\n"));
            close(clients[i].sockfd); // Fermer la connexion du client banni
            for (int j = i; j < client_count - 1; j++) {
                clients[j] = clients[j + 1];
            }
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void suspend_client(char *pseudo) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            clients[i].is_suspended = 1;
            printf("Client suspendu: %s\n", clients[i].pseudo); // Debug message
            write(clients[i].sockfd, "Le serveur a suspendu votre compte. Vous pouvez recevoir des messages mais pas en envoyer.\n", strlen("Le serveur a suspendu votre compte. Vous pouvez recevoir des messages mais pas en envoyer.\n"));
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void forgive_client(char *pseudo) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            clients[i].is_suspended = 0;
            printf("Client rétabli: %s\n", clients[i].pseudo); // Debug message
            write(clients[i].sockfd, "Le serveur a rétabli votre compte. Vous pouvez maintenant envoyer des messages.\n", strlen("Le serveur a rétabli votre compte. Vous pouvez maintenant envoyer des messages.\n"));
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void list_clients() {
    pthread_mutex_lock(&clients_mutex);
    
    printf("Liste des clients connectés :\n");
    printf("Modérateur\n"); // Ajouter le serveur comme modérateur
    for (int i = 0; i < client_count; i++) {
        printf("%s\n", clients[i].pseudo);
    }
    
    pthread_mutex_unlock(&clients_mutex);
}

int includes_command(const char *command) {
    if (strstr(command, "@") && strstr(command, "!ban")) {
        return 1;
    } else if (strstr(command, "@") && strstr(command, "!suspend")) {
        return 2;
    } else if (strstr(command, "@") && strstr(command, "!forgive")) {
        return 3;
    }
    return 0;
}

void *manage_server(void *arg) {
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("La création du socket a échoué...\n");
        exit(0);
    } else {
        printf("Socket créé avec succès..\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
        printf("La liaison du socket a échoué...\n");
        exit(0);
    } else {
        printf("Socket lié avec succès..\n");
    }

    if ((listen(sockfd, 5)) != 0) {
        printf("L'écoute a échoué...\n");
        exit(0);
    } else {
        printf("Serveur en écoute..\n");
    }
    len = sizeof(cli);

    while (1) {
        connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
        if (connfd < 0) {
            printf("Acceptation du serveur a échoué...\n");
            exit(0);
        } else {
            printf("Le serveur a accepté un client...\n");
        }

        if (server_running) {
            pthread_mutex_lock(&clients_mutex);
            client_t *new_client = (client_t *)malloc(sizeof(client_t));
            new_client->sockfd = connfd;
            new_client->is_banned = 0;
            new_client->is_suspended = 0;
            read(connfd, new_client->pseudo, sizeof(new_client->pseudo));
            clients[client_count++] = *new_client;
            pthread_mutex_unlock(&clients_mutex);

            pthread_t tid;
            pthread_create(&tid, NULL, client_handler, (void *)new_client);
        } else {
            write(connfd, "Nouvelles connexions désactivées.\n", strlen("Nouvelles connexions désactivées.\n"));
            close(connfd);
        }
    }

    close(sockfd);
    return NULL;
}

void handle_commands(char *command) {
    char pseudo[20];
    int command_type = includes_command(command);
    if (strncmp(command, "!start", 6) == 0) {
        server_running = 0;
        printf("Nouvelles connexions désactivées.\n");
    } else if (strncmp(command, "!liste", 6) == 0) {
        list_clients();
    } else if (command_type == 1 && sscanf(command, "@%s !ban", pseudo) == 1) {
        ban_client(pseudo);
    } else if (command_type == 2 && sscanf(command, "@%s !suspend", pseudo) == 1) {
        suspend_client(pseudo);
    } else if (command_type == 3 && sscanf(command, "@%s !forgive", pseudo) == 1) {
        forgive_client(pseudo);
    } else {
        printf("Commande inconnue : %s", command);
    }
}

int main() {
    pthread_t server_thread;
    pthread_create(&server_thread, NULL, manage_server, NULL);

    char command[MAX];
    while (1) {
        fgets(command, MAX, stdin);
        handle_commands(command);
    }

    return 0;
}
*/
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

#define MAX 80
#define PORT 42042
#define SA struct sockaddr

typedef struct {
    int sockfd;
    char pseudo[20];
    int is_banned;
    int is_suspended;
} client_t;

client_t clients[10]; // Maximum de 10 clients
int client_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
int server_running = 1; // Flag pour gérer l'état du serveur

void send_to_all(char *message, int sender_sock) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < client_count; i++) {
        if (clients[i].sockfd != sender_sock && !clients[i].is_banned) {
            write(clients[i].sockfd, message, strlen(message));
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void send_to_client(char *message, char *pseudo) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            write(clients[i].sockfd, message, strlen(message));
            break;
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void list_clients_for_client(int client_sockfd) {
    pthread_mutex_lock(&clients_mutex);
    
    char message[MAX * 10]; // Buffer pour la liste des clients
    bzero(message, sizeof(message));
    strcat(message, "Liste des clients connectés :\n");
    strcat(message, "Modérateur\n"); // Ajouter le serveur comme modérateur
    for (int i = 0; i < client_count; i++) {
        strcat(message, clients[i].pseudo);
        strcat(message, "\n");
    }
    write(client_sockfd, message, strlen(message));
    
    pthread_mutex_unlock(&clients_mutex);
}

void *client_handler(void *arg) {
    client_t *client = (client_t *)arg;
    char buff[MAX];
    int n;

    while (1) {
        bzero(buff, MAX);
        n = read(client->sockfd, buff, sizeof(buff));
        if (n <= 0) {
            break;
        }
        printf("Du client (%s): %s", client->pseudo, buff);

        if (strncmp(buff, "exit", 4) == 0) {
            printf("%s a quitté...\n", client->pseudo);
            break;
        } else if (client->is_banned) {
            write(client->sockfd, "Vous êtes banni.\n", strlen("Vous êtes banni.\n"));
        } else if (client->is_suspended) {
            write(client->sockfd, "Vous êtes suspendu et ne pouvez pas envoyer de messages.\n", strlen("Vous êtes suspendu et ne pouvez pas envoyer de messages.\n"));
        } else if (strncmp(buff, "!list", 6) == 0) {
            list_clients_for_client(client->sockfd);
        } else {
            char message_with_pseudo[MAX + 40]; // Taille du buffer pour inclure le pseudo
            snprintf(message_with_pseudo, sizeof(message_with_pseudo), "%s: %s", client->pseudo, buff);

            if (buff[0] == '@') {
                // Handle private message
                char target_pseudo[20];
                sscanf(buff, "@%s", target_pseudo);
                char *private_message = strchr(buff, ' ') + 1;
                snprintf(message_with_pseudo, sizeof(message_with_pseudo), "%s: %s", client->pseudo, private_message);
                send_to_client(message_with_pseudo, target_pseudo);
            } else {
                send_to_all(message_with_pseudo, client->sockfd);
            }
        }
    }

    close(client->sockfd);
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i].sockfd == client->sockfd) {
            for (int j = i; j < client_count - 1; j++) {
                clients[j] = clients[j + 1];
            }
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    free(client);
    pthread_exit(NULL);
}

void ban_client(char *pseudo) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            clients[i].is_banned = 1;
            write(clients[i].sockfd, "Vous êtes banni.\n", strlen("Vous êtes banni.\n"));
            close(clients[i].sockfd); // Fermer la connexion du client banni
            for (int j = i; j < client_count - 1; j++) {
                clients[j] = clients[j + 1];
            }
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void suspend_client(char *pseudo) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            clients[i].is_suspended = 1;
            write(clients[i].sockfd, "Le serveur a demandé de suspendre votre terminal de saisie.\n", strlen("Le serveur a demandé de suspendre votre terminal de saisie.\n"));
            write(clients[i].sockfd, "!!suspend\n", strlen("!!suspend\n")); // Commande interne au superviseur pour envoyer SIGSTOP
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void forgive_client(char *pseudo) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].pseudo, pseudo) == 0) {
            clients[i].is_suspended = 0;
            write(clients[i].sockfd, "Le serveur a demandé de rétablir votre terminal de saisie.\n", strlen("Le serveur a demandé de rétablir votre terminal de saisie.\n"));
            write(clients[i].sockfd, "!!forgive\n", strlen("!!forgive\n")); // Commande interne au superviseur pour envoyer SIGCONT
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void list_clients() {
    pthread_mutex_lock(&clients_mutex);
    
    printf("Liste des clients connectés :\n");
    printf("Modérateur\n"); // Ajouter le serveur comme modérateur
    for (int i = 0; i < client_count; i++) {
        printf("%s\n", clients[i].pseudo);
    }
    
    pthread_mutex_unlock(&clients_mutex);
}

void *manage_server(void *arg) {
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("La création du socket a échoué...\n");
        exit(0);
    } else {
        printf("Socket créé avec succès..\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
        printf("La liaison du socket a échoué...\n");
        exit(0);
    } else {
        printf("Socket lié avec succès..\n");
    }

    if ((listen(sockfd, 5)) != 0) {
        printf("L'écoute a échoué...\n");
        exit(0);
    } else {
        printf("Serveur en écoute..\n");
    }
    len = sizeof(cli);

    while (1) {
        connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
        if (connfd < 0) {
            printf("Acceptation du serveur a échoué...\n");
            exit(0);
        } else {
            printf("Le serveur a accepté un client...\n");
        }

        if (server_running) {
            pthread_mutex_lock(&clients_mutex);
            client_t *new_client = (client_t *)malloc(sizeof(client_t));
            new_client->sockfd = connfd;
            new_client->is_banned = 0;
            new_client->is_suspended = 0;
            read(connfd, new_client->pseudo, sizeof(new_client->pseudo));
            clients[client_count++] = *new_client;
            pthread_mutex_unlock(&clients_mutex);

            pthread_t tid;
            pthread_create(&tid, NULL, client_handler, (void *)new_client);
        } else {
            write(connfd, "Nouvelles connexions désactivées.\n", strlen("Nouvelles connexions désactivées.\n"));
            close(connfd);
        }
    }

    close(sockfd);
    return NULL;
}

void handle_commands(char *command) {
    char pseudo[20];
    if (strcmp(command, "!start\n") == 0) {
        server_running = 0;
        printf("Nouvelles connexions désactivées.\n");
    } else if (strcmp(command, "!list\n") == 0) {
        list_clients();
    } else if (sscanf(command, "@%s !ban", pseudo) == 1) {
        ban_client(pseudo);
    } else if (sscanf(command, "@%s !suspend", pseudo) == 1) {
        suspend_client(pseudo);
    } else if (sscanf(command, "@%s !forgive", pseudo) == 1) {
        forgive_client(pseudo);
    } else {
        printf("Commande inconnue : %s", command);
    }
}

int main() {
    pthread_t server_thread;
    pthread_create(&server_thread, NULL, manage_server, NULL);

    char command[MAX];
    while (1) {
        fgets(command, MAX, stdin);
        handle_commands(command);
    }

    return 0;
}