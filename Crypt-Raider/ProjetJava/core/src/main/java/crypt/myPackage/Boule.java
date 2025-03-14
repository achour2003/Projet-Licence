
package crypt.myPackage;

import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.math.Rectangle;

import java.util.List;
import java.util.ArrayList;

public class Boule extends RollingEntity {
   /* private static final int TILE_SIZE = 80; // Taille d'une tuile
    private boolean isFalling = false;      // Indique si la boule est en train de tomber
    private float fallSpeed = 200f;         // Vitesse de chute (pixels par seconde)
    private float fallProgress = 0;         // Progression actuelle dans la chute
    private boolean hasExploded = false;    // Évite les explosions multiples
    private int SCREEN_WIDTH = 1200;
    private int SCREEN_HEIGHT = 720;
*/
    public Boule(Texture texture, float x, float y) {
        super(texture, x, y);
        bounds.set(x, y, texture.getWidth(), texture.getHeight());
    }

    @Override
    public boolean canFall(List<GameObject> gameObjects) {
        float newY = getY() - TILE_SIZE; // Calcul de la position une case en dessous
        Rectangle nextPosition = new Rectangle(getX(), newY, getWidth(), getHeight());

        for (GameObject obj : gameObjects) {
            if (obj.isActive() && nextPosition.overlaps(obj.getBounds()) && obj != this ){
                // Si c'est un joueur ou un ennemi, la chute continue pour collisionner
                if (obj instanceof Joueur || obj instanceof Enemie || obj instanceof Bombe) {
                    return true;
                } else if (obj instanceof PorteFin) {
                    return true;

                }
                // Sinon, la chute s'arrête pour tout objet non passable
                if (!obj.isPassable()) {

                    return false;
                }
            }
        }

        return true; // La chute est possible
    }

/*
    // Met à jour l'état de la boule (chute, collisions, etc.)
    public void update(float delta, List<GameObject> gameObjects, List<GameObject> objectsToRemoveGlobal) {


        if (isFalling) {
            fallProgress += fallSpeed * delta; // Progression de la chute

            if (fallProgress >= TILE_SIZE) {
                fallProgress = 0; // Réinitialiser pour la prochaine case
                setPosition(getX(), getY() - TILE_SIZE); // Descendre d'une case
                bounds.setPosition(getX(), getY());      // Mettre à jour les limites

                boolean collisionDetected = false;

                for (GameObject obj : gameObjects) {
                    if (obj.isActive() && bounds.overlaps(obj.getBounds())) {

                        System.out.println("Collision détectée avec : " + obj.getClass().getSimpleName());
                        System.out.println("isPassable : " + obj.isPassable());
                        System.out.println("Bounds de la boule : " + bounds);
                        System.out.println("Bounds de l'objet : " + obj.getBounds());

                        if (obj instanceof PorteFin) {
                            System.out.println("Collision avec PorteFin !");
                            objectsToRemoveGlobal.add(this); // Marquer la boule pour suppression
                            collisionDetected = true; // Collision détectée
                            break; // Sortir de la boucle (priorité à PorteFin)

                        } else if (obj instanceof Joueur || obj instanceof Enemie) {
                            System.out.println("Collision avec un joueur ou un ennemi ! Explosion.?????????????????????");
                            explode(gameObjects, objectsToRemoveGlobal); // Explosion dans toutes les cases environnantes
                            obj.deactivate(); // Désactiver le joueur ou l'ennemi touché
                            objectsToRemoveGlobal.add(obj); // Ajouter à la liste globale pour suppression
                            collisionDetected = true;
                            break; // Sortir de la boucle (priorité à l'explosion)

                        } else if (!obj.isPassable()/* && !(obj instanceof Boule)*//*) {
                            // Vérification des murs ou objets non passables
                            System.out.println("Collision avec un mur ou un objet non passable.");
                            collisionDetected = true; // Collision détectée
                            break; // Arrêter la chute
                        }else   collisionDetected = true; // Collision détectée

                    }/*else{
                        System.out.println("Collision avec un mur ou un objet non passable.");
                        collisionDetected = true; // Collision détectée
                        break; // Arrêter la chute
                    }*/
             /*   }


                if (collisionDetected) {
                    isFalling = false; // Arrêter la chute si une collision est détectée
                }
            }
        } else {
            // Vérifier si la boule doit commencer à tomber
            if (canFall(gameObjects)) {
                isFalling = true;
            }
        }
    }

    // Vérifie si la boule peut tomber dans la prochaine case
    private boolean canFall(List<GameObject> gameObjects) {
        float newY = getY() - TILE_SIZE; // Calcul de la position une case en dessous
        Rectangle nextPosition = new Rectangle(getX(), newY, getWidth(), getHeight());

        for (GameObject obj : gameObjects) {
            if (obj.isActive() && nextPosition.overlaps(obj.getBounds()) && obj != this ){
                // Si c'est un joueur ou un ennemi, la chute continue pour collisionner
                if (obj instanceof Joueur || obj instanceof Enemie) {
                    return true;
                } else if ( obj instanceof PorteFin) {
                    return true;
                }
                // Sinon, la chute s'arrête pour tout objet non passable
                if (!obj.isPassable()) {

                    return false;
                }
            }
        }

        return true; // La chute est possible
    }

    private void explode(List<GameObject> gameObjects, List<GameObject> objectsToRemoveGlobal) {
        if (hasExploded) return; // Empêcher les explosions multiples
        hasExploded = true;

        System.out.println("Boule explose, désactivant et supprimant les objets environnants !");

        float[][] offsets = {
            {-1, -1}, {0, -1}, {1, -1}, // Ligne au-dessus
            {-1,  0}, {0,  0}, {1,  0}, // Ligne au centre
            {-1,  1}, {0,  1}, {1,  1}  // Ligne en dessous
        };

        for (float[] offset : offsets) {
            float neighborX = getX() + offset[0] * TILE_SIZE;
            float neighborY = getY() + offset[1] * TILE_SIZE;

            // Vérifier que les coordonnées sont valides
            if (neighborX < 0 || neighborY < 0 || neighborX >= SCREEN_WIDTH || neighborY >= SCREEN_HEIGHT) {
                continue; // Ignorer les cases hors des limites
            }

            Rectangle explosionArea = new Rectangle(neighborX, neighborY, TILE_SIZE, TILE_SIZE);

            for (GameObject obj : gameObjects) {
                if (obj.isActive() && explosionArea.overlaps(obj.getBounds())) {
                    if (obj instanceof Joueur || obj instanceof Enemie || obj.isDestructible()) {
                        System.out.println("Désactivation de l'objet : " + obj.getClass().getSimpleName());
                        obj.deactivate();
                        objectsToRemoveGlobal.add(obj); // Ajouter à la liste globale pour suppression
                    }
                }
            }
        }

        // Désactiver et marquer la boule pour suppression
        deactivate();
        objectsToRemoveGlobal.add(this);

        System.out.println("Explosion terminée, objets marqués pour suppression.");
    }

    // Permet de pousser la boule dans une direction
    public boolean push(int dx, int dy, List<GameObject> gameObjects) {
        float newX = getX() + dx * TILE_SIZE;
        float newY = getY() + dy * TILE_SIZE;

        Rectangle nextPosition = new Rectangle(newX, newY, getWidth(), getHeight());

        // Vérifier les collisions avec d'autres objets
        for (GameObject obj : gameObjects) {
            if (obj.isActive() && nextPosition.overlaps(obj.getBounds())) {
                if (!obj.isPassable()) {
                    return false; // La boule ne peut pas être poussée à cet endroit
                }
            }
        }

        // Si pas de collisions bloquantes, déplacer la boule
        setPosition(newX, newY);
        bounds.setPosition(newX, newY); // Mettre à jour les limites
        return true; // La boule a été poussée
    }

    @Override
    public void onCollision(Joueur joueur) {
        System.out.println("Une boule est poussable !");
    }

    @Override
    public boolean isPassable() {
        return false; // La boule bloque le passage par défaut
    }

    @Override
    public boolean isDestructible() {
        return true; // Peut être détruit par une explosion
    }

              */
}

