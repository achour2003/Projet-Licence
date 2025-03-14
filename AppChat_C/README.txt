Instructions pour compiler et exécuter le projet

1. Décompressez l'archive :
   tar -xzvf Memoire-Achour.tar.gz

2. Allez dans le répertoire du projet :
   cd Memoire-Achour

3. Compilez les fichiers :
   make

4. Exécutez le serveur :
   ./serveur

5. Exécutez le client dans un autre terminal :
   ./client

Note importante :
Si la liaison à la socket ne se fait pas (le message "la connexion au serveur a échoué" apparaît), fermez le terminal et recommencez les étapes 4 et 5. Assurez-vous également que le serveur est en cours d'exécution avant de démarrer le client.

Si vous rencontrez des problèmes supplémentaires, veuillez vérifier les points suivants :
- Assurez-vous que toutes les dépendances et bibliothèques nécessaires sont installées.
- Vérifiez les permissions des fichiers avec la commande `ls -la`.
- Pour toute autre question, référez-vous au fichier source et aux commentaires dans le code.

Bibliothèques nécessaires :
- pthread
- stdio
- stdlib
- string
- unistd
- arpa/inet
- netinet/in
- netdb
- signal

Commandes disponibles uniquement pour le modérateur

    !start : lance la partie. Plus aucun nouveau joueur ne peut se connecter.
    @PSEUDO !ban : tue le joueur désigné par le pseudo, comme s'il avait été tué par son mot mortel.
    @PSEUDO !suspend : gèle le terminal de saisie (il reçoit un signal SIGSTOP) de l'utilisateur concerné. C'est le serveur qui donne l'ordre au superviseur visé d'envoyer le signal au terminal de saisie (rappelons que le serveur n'est pas forcément sur la même machine que le client, il ne peut pas envoyer le signal SIGSTOP au terminal directement).
    @PSEUDO !forgive : restaure le terminal de saisie (il reçoit le signal SIGCONT) de l'utilisateur concerné.
!list : liste les pseudos, en indiquant leur état actuel (vivant, mort, crashed)


@PSEUDO pour envoyer des message prive 

