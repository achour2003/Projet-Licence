import socket
import select
import threading


TAILLE_MAX = 4096
ADRESSE = '134.59.2.162'
PORT = 42042
 
 



def creer_serveur():
   # Crée et configure le socket du serveur
   serveur_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
   serveur_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
   serveur_sock.bind((ADRESSE, PORT))
   serveur_sock.listen()
   print(f"Le serveur est opérationnel sur {ADRESSE}:{PORT}")
   return serveur_sock

def gerer_entrees(etat, clients_actifs, pseudos, pauses, interdictions):
   
   while True:
        commande = input("Entrez votre commande : ").strip()
        match commande:
            case "!start":
                etat[0] = True
                print("Nouvelles connexions désactivées.")
            case _ if commande.startswith("@") and " !ban" in commande:
                traiter_bannissement(commande, pseudos, clients_actifs, interdictions)
            case _ if commande.startswith("@") and " !suspend" in commande:
                traiter_suspension(commande, pseudos, pauses)
            case _ if commande.startswith("@") and " !forgive" in commande:
                traiter_retablissement(commande, pseudos, pauses, interdictions)
            case _:
                print("Commande invalide. les commande diponible \n'@pseudo !ban', '@pseudo !suspend' ou '@pseudo !forgive'.")

def traiter_bannissement(commande, pseudos, clients_actifs, interdictions):
   # Bannit un utilisateur
   parties = commande.split(" ")
   if len(parties) >= 2:
       pseudo_ban = parties[0][1:]
       if pseudo_ban in pseudos:
           client_ban = pseudos[pseudo_ban]
           client_ban.send("Vous êtes banni.".encode('utf-8'))
           clients_actifs.pop(client_ban, None)
           pseudos.pop(pseudo_ban, None)
           interdictions.add(client_ban)
           client_ban.close()

def traiter_suspension(commande, pseudos, pauses):
   # Suspend un utilisateur
   parties = commande.split(" ")
   if len(parties) >= 2:
       pseudo_suspend = parties[0][1:]
       if pseudo_suspend in pseudos:
           client_suspend = pseudos[pseudo_suspend]
           pauses.add(client_suspend)
           client_suspend.send("Vous êtes suspendu. Vous ne pouvez plus envoyer de messages.".encode('utf-8'))
           print(f"{pseudo_suspend} a été suspendu.")
       else:
           print("Utilisateur non trouvé.")

def traiter_retablissement(commande, pseudos, pauses, interdictions):
   # Rétablit un utilisateur
   parties = commande.split(" ")
   if len(parties) >= 2:
       pseudo_forgive = parties[0][1:]
       if pseudo_forgive in pseudos:
           client_forgive = pseudos[pseudo_forgive]
           pauses.discard(client_forgive)
           interdictions.discard(client_forgive)
           client_forgive.send("le serveur a decider de retablir la connexion avec vous . maintenant vous pouvez envoyer des message.".encode('utf-8'))
           print(f"{pseudo_forgive} a été rétabli.")
       else:
           print("l'user n'exist pas .")

def gerer_connexions(serveur_sock, etat, clients_actifs, pseudos, pauses, interdictions):
   # Gère les connexions et les messages des clients
   try:
       while True:
           lecture_sockets = [serveur_sock] + list(clients_actifs.keys())
           lecture_prete, _, _ = select.select(lecture_sockets, [], [])

           for sock in lecture_prete:
               if sock == serveur_sock:
                   if not etat[0]:
                       accepter_connexion(sock, clients_actifs, pseudos)
                   else:
                       refuser_connexion(sock)
               else:
                   traiter_client(sock, clients_actifs, pseudos, pauses, interdictions)
   finally:
       nettoyer_serveur(serveur_sock, clients_actifs)

def accepter_connexion(sock, clients_actifs, pseudos):
   # Accepte une nouvelle connexion
   client_sock, adresse = sock.accept()
   client_sock.send("Entrez votre pseudonyme:".encode('utf-8'))
   pseudo = client_sock.recv(TAILLE_MAX).decode('utf-8').strip()

   while pseudo in pseudos or not pseudo:
       client_sock.send("Pseudonyme déjà pris ou invalide, essayez un autre:".encode('utf-8'))
       pseudo = client_sock.recv(TAILLE_MAX).decode('utf-8').strip()

   clients_actifs[client_sock] = pseudo
   pseudos[pseudo] = client_sock
   print(f"{pseudo} connecté depuis {adresse}")
   client_sock.send("Bienvenue !".encode('utf-8'))

def refuser_connexion(sock):
   # Refuse une nouvelle connexion
   new_sock, _ = sock.accept()
   new_sock.send("la partie a commencé.".encode('utf-8'))
   new_sock.close()

def traiter_client(sock, clients_actifs, pseudos, pauses, interdictions):
   # Traite les messages d'un client
   if sock in interdictions:
       return
   if sock in pauses:
       gerer_message_suspendu(sock, clients_actifs, pseudos)
   else:
       gerer_message_normal(sock, clients_actifs, pseudos, pauses, interdictions)

def gerer_message_suspendu(sock, clients_actifs, pseudos):
   # Gère les messages des utilisateurs suspendus
   message = sock.recv(TAILLE_MAX).decode('utf-8')
   if not message:
       pseudo = clients_actifs.pop(sock, None)
       if pseudo:
           pseudos.pop(pseudo, None)
       sock.close()
   else:
       sock.send("Vous êtes suspendu et ne pouvez pas envoyer de messages.".encode('utf-8'))

def gerer_message_normal(sock, clients_actifs, pseudos, pauses, interdictions):
   # Gère les messages des utilisateurs normaux
   message = sock.recv(TAILLE_MAX).decode('utf-8')
   if not message:
       pseudo = clients_actifs.pop(sock, None)
       if pseudo:
           pseudos.pop(pseudo, None)
       sock.close()
   else:
       print(f"Message de {clients_actifs[sock]} : {message}")
       diffuser_message(message, sock, clients_actifs, pseudos, pauses, interdictions)

def diffuser_message(message, expediteur, clients_actifs, pseudos, pauses, interdictions):
   # Diffuse un message à tous les utilisateurs connectés
   pseudo_expediteur = clients_actifs[expediteur]
   if expediteur in pauses:
       expediteur.send("Vous êtes suspendu et ne pouvez pas envoyer de messages.".encode('utf-8'))
       return

   if message == '!list':
       envoyer_liste_utilisateurs(expediteur, pseudos, pauses, interdictions)
   elif message.startswith('@'):
       envoyer_message_prive(message, pseudo_expediteur, pseudos, interdictions)
   else:
       envoyer_message_global(message, pseudo_expediteur, clients_actifs, expediteur, interdictions)

def envoyer_liste_utilisateurs(expediteur, pseudos, pauses, interdictions):
   # Envoie la liste des utilisateurs connectés
   utilisateurs = []
   for pseudo, sock in pseudos.items():
       if sock in interdictions:
           continue
       etat = "suspendu" if sock in pauses else "actif"
       utilisateurs.append(f"{pseudo} ({etat})")
   liste_str = ", ".join(utilisateurs)
   expediteur.send(f"Utilisateurs connectés : {liste_str}".encode('utf-8'))

def envoyer_message_prive(message, pseudo_expediteur, pseudos, interdictions):
   # Envoie un message privé à un utilisateur
   pseudo_cible, _, contenu = message[1:].partition(' ')
   if pseudo_cible in pseudos:
       sock_cible = pseudos[pseudo_cible]
       if sock_cible not in interdictions:
           sock_cible.send(f"{pseudo_expediteur} : {contenu}".encode('utf-8'))

def envoyer_message_global(message, pseudo_expediteur, clients_actifs, expediteur, interdictions):
   # Envoie un message à tous les utilisateurs
   for client in clients_actifs:
       if client != expediteur and client not in interdictions:
           client.send(f"{pseudo_expediteur} : {message}".encode('utf-8'))

def nettoyer_serveur(serveur_sock, clients_actifs):
   # Ferme le serveur et tous les clients connectés
   serveur_sock.close()
   for client_sock in clients_actifs.values():
       client_sock.close()

def lancer_serveur():
   # Lance le serveur
   sock_serveur = creer_serveur()
   clients_actifs = {}
   pseudos = {}
   pauses = set()
   interdictions = set()
   etat = [False]

   thread_admin = threading.Thread(target=gerer_entrees, args=(etat, clients_actifs, pseudos, pauses, interdictions))
   thread_admin.daemon = True
   thread_admin.start()

   gerer_connexions(sock_serveur, etat, clients_actifs, pseudos, pauses, interdictions)

if __name__ == "__main__":
   lancer_serveur()

