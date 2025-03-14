package crypt.myPackage;

import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.math.Rectangle;
import java.util.List;

    public class PorteFin extends GameObject {
        private boolean isOpen = false; // État de la porte
        private boolean accessible = false; // La porte est verrouillée par défaut

        public PorteFin(Texture texture, float x, float y) {
            super(texture, x, y);
            bounds.set(x, y, texture.getWidth(), texture.getHeight());
        }


        // Vérifie si toutes les boules sont rentrées dans la porte
        public void checkAndOpen(List<GameObject> gameObjects) {
            boolean allBouleCollected = true;

            // Parcourir les objets pour vérifier si toutes les boules sont désactivées
            for (GameObject obj : gameObjects) {
                if (obj instanceof Boule && obj.isActive()) {
                    allBouleCollected = false;
                    break;
                }
            }

            // Ouvrir la porte si toutes les boules sont rentrées
            if (allBouleCollected) {
                isOpen = true;
            }
        }

        // Méthode appelée lorsque le joueur interagit avec la porte
        public void onPlayerInteract(Joueur joueur, List<GameObject> gameObjects) {
            if (isOpen) {
                System.out.println("Le joueur entre dans la porte de fin !");
                deactivate(); // Désactiver la porte elle-même
                gameObjects.remove(this); // Supprimer la porte de la liste des objets
            } else {
                System.out.println("La porte est fermée. Collectez toutes les boules pour l'ouvrir.");
            }
        }

        @Override
        public boolean isPassable() {
            return isOpen; // La porte devient passable une fois ouverte

        }

        @Override
        public boolean isDestructible() {
            return false;
        }

/*
        public void update(float delta, List<GameObject> gameObjects) {
            // Mettre à jour l'état de la porte en vérifiant les boules
            checkAndOpen(gameObjects);
        }
*/
        @Override
        public void onCollision(Joueur player) {

        }

        public boolean update(float delta, List<GameObject> gameObjects, List<GameObject> objectsToRemoveGlobal) {
            for (GameObject obj : gameObjects) {
                if (obj.isActive() && bounds.overlaps(obj.getBounds())) {

                    if (obj instanceof Boule) {
                        System.out.println("j'ai capturer une boule");
                        objectsToRemoveGlobal.add(obj);
                        Boule boule = (Boule) obj;
                        boule.deactivate();

                    } else if (obj instanceof Joueur) {
                        if (isOpen){
                            System.out.println("j'ai capturer un joueur   bien jouer ");
                            Joueur   joeur = (Joueur)obj;
                            joeur.deactivate();
                            objectsToRemoveGlobal.add(obj);
                            return true ;
                        }


                    }
                }
            }
            return false;
        }
    }
