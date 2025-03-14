djerada achour , travaille  seul.
Ce serveur de chat permet aux utilisateurs de se connecter, d'envoyer des messages et de gérer des interactions via
 des commandes spéciales. Il est conçu pour supporter la gestion des utilisateurs, y compris les bannissements et
 les suspensions. Dans une tentative précédente, j'ai développé un serveur asynchrone multiclient qui permettait des 
interactions en temps réel sans blocage, mais je n'avais pas implémenté les messages privés et j'ai rencontré 
des difficultés pour lancer les applications avec les deux terminaux Ce nouveau cleint  vise à améliorer ces aspects, 
bien que je rencontre toujours des problèmes pour lancer le client avec les deux terminaux .
Commandes du Serveur

Les administrateurs peuvent entrer les commandes suivantes directement dans la console du serveur pour gérer les utilisateurs et les connexions :

    !start : Désactive la possibilité pour les nouveaux clients de se connecter. Cette commande est utile pour contrôler l'accès pendant des événements ou lorsque le nombre maximum de participants est atteint.
    @pseudo !ban : Bannit un utilisateur spécifique. Cela empêche l'utilisateur banni de se reconnecter au serveur.
    @pseudo !suspend : Suspend temporairement un utilisateur, qui ne pourra pas envoyer de messages tant que la suspension reste active.
    @pseudo !forgive : Rétablit un utilisateur précédemment suspendu ou banni, permettant sa pleine participation au chat.

Gestion des Clients

Les clients peuvent se connecter au serveur en fournissant un pseudonyme unique. Si le pseudonyme est déjà pris ou considéré comme invalide, le serveur demandera un autre nom. Une fois connectés, les utilisateurs peuvent :

    Envoyer des messages à l'ensemble des participants.
    Envoyer des messages privés en utilisant la commande @pseudo message.

Messages de Système

Les utilisateurs reçoivent des confirmations de leurs actions, incluant la connexion, ainsi que des notifications en cas de suspension ou de bannissement, et lors du rétablissement de leurs droits.