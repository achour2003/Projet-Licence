
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

//#define PORT 8080
#define PORT 42042    
#define SA struct sockaddr

int suspended = 0; // Indicateur de suspension

void *receive_handler(void *sockfd) {
    int sock = *(int *)sockfd;
    char buff[80];

    while (1) {
        bzero(buff, sizeof(buff));
        int n = read(sock, buff, sizeof(buff));
        if (n > 0) {
            if (strncmp(buff, "!!suspend", 9) == 0) {
                suspended = 1;
                printf("Le serveur a demandé de suspendre votre terminal de saisie.\n");
            } else if (strncmp(buff, "!!forgive", 9) == 0) {
                suspended = 0;
                printf("Le serveur a demandé de rétablir votre terminal de saisie.\n");
            } else {
                printf("Du Serveur : %s\n", buff);
            }
        } else if (n == 0) {
            printf("Le serveur s'est déconnecté.\n");
            exit(0);
        } else {
            printf("Erreur de lecture du serveur.\n");
            exit(1);
        }
    }

    return NULL;
}

void func(int sockfd) {
    char buff[80];
    int n;

    pthread_t tid;
    pthread_create(&tid, NULL, receive_handler, (void *)&sockfd);

    while (1) {
        if (suspended) {
            sleep(1); // Attendre avant de vérifier à nouveau
            continue;
        }

        bzero(buff, sizeof(buff));
        printf("Entrez le message : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;

        write(sockfd, buff, strlen(buff));
        if (strncmp(buff, "exit", 4) == 0) {
            printf("Client Quitter...\n");
            break;
        }
    }

    pthread_cancel(tid);
    pthread_join(tid, NULL);
}

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // création du socket et vérification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("la création du socket a échoué...\n");
        exit(0);
    } else
        printf("Socket créé avec succès..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assignation IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connexion du socket client au socket serveur
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("la connexion au serveur a échoué...\n");
        exit(0);
    } else
        printf("connecté au serveur..\n");

    // envoyer le pseudonyme au serveur
    char pseudo[20];
    printf("Entrez votre pseudonyme : ");
    scanf("%s", pseudo);
    write(sockfd, pseudo, strlen(pseudo));

    // consommer le caractère '\n' laissé par scanf
    getchar();

    // fonction pour la communication
    func(sockfd);

    // fermeture du socket
    close(sockfd);
    return 0;
}
