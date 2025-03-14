import socket
import sys
import select

# Configuration du serveur
SERVEUR_IP = '134.59.2.162'
 
SERVEUR_PORT = 42042
BUFFER_SIZE = 4096

def se_connecter(serveur_ip, serveur_port):
   client_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
   try:
       client_sock.connect((serveur_ip, serveur_port))
       print("Connexion au serveur de chat réussie.")
   except socket.error as err:
       print(f"Erreur de connexion au serveur : {err}")
       sys.exit()
   return client_sock

def envoyer_message(sock, message):
   try:
       sock.send(message.encode('utf-8'))
   except socket.error as err:
       print(f"Erreur d'envoi du message : {err}")
       sys.exit()

def recevoir_message(sock):
   try:
       return sock.recv(BUFFER_SIZE).decode('utf-8')
   except socket.error as err:
       print(f"Erreur de réception du message : {err}")
       sys.exit()

def main():
   client_socket = se_connecter(SERVEUR_IP, SERVEUR_PORT)
   
   try:
       while True:
           reponse_du_serveur = recevoir_message(client_socket)
        
           print(reponse_du_serveur)
           match True:
                case _ if "Entrez votre pseudonyme:" in reponse_du_serveur or "Pseudonyme déjà pris" in reponse_du_serveur:
                    username = input(">>> ")
                    envoyer_message(client_socket, username)
                case _ if "Bienvenue !" in reponse_du_serveur:
                    print("Connexion établie!!")
                    break
                case _ if "la partie a commencé." in reponse_du_serveur:
                    print("Connexion impossible : Le jeu a déjà commencé.")
                    sys.exit()
                case _ if "Vous avez été banni." in reponse_du_serveur:
                    print("le serveur vous a banni")
                    sys.exit()
                    
       while True:
           ready_sockets, _, _ = select.select([sys.stdin, client_socket], [], [])

           for notified_socket in ready_sockets:
               if notified_socket == client_socket:
                   server_message = recevoir_message(client_socket)
                   if not server_message:
                       print("Déconnexion du serveur.")
                       sys.exit()
                   else:
                       print(server_message)
               else:
                   user_message = input()
                   if user_message.lower() == 'exit':
                       envoyer_message(client_socket, 'exit')
                       return
                   envoyer_message(client_socket, user_message)

   finally:
       client_socket.close()

if __name__ == "__main__":
   main()