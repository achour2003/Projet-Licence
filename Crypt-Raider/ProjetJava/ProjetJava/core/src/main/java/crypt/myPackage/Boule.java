package crypt.myPackage;

import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.math.Rectangle;

import java.util.List;

public class Boule extends GameObject {
    private static final int TILE_SIZE = 80; // Taille d'une tuile
    private boolean isFalling = false;      // Indique si la boule est en train de tomber
    private float fallSpeed = 200f;         // Vitesse de chute (pixels par seconde)
    private float fallProgress = 0;        // Progression actuelle dans la chute

    public Boule(Texture texture, float x, float y) {
        super(texture, x, y);
    }


    public void update(float delta, List<GameObject> gameObjects) {
        if (isFalling) {
            fallProgress += fallSpeed * delta; // Progression de la chute

            if (fallProgress >= TILE_SIZE) {
                fallProgress = 0; // Réinitialiser pour la prochaine case
                setPosition(getX(), getY() - TILE_SIZE); // Descendre d'une case
                bounds.setPosition(getX(), getY());      // Mettre à jour les limites

                // Vérifier s'il y a un obstacle en dessous
                if (!canFall(gameObjects)) {
                    isFalling = false; // Arrêter la chute
                }
            }
        } else {
            // Vérifier si la boule doit commencer à tomber
            if (canFall(gameObjects)) {
                isFalling = true;
            }
        }
    }

    private boolean canFall(List<GameObject> gameObjects) {
        float newY = getY() - TILE_SIZE; // Calcul de la position une case en dessous
        Rectangle nextPosition = new Rectangle(getX(), newY, getWidth(), getHeight());

        for (GameObject obj : gameObjects) {
            if (obj.isActive() && nextPosition.overlaps(obj.getBounds())) {
                // Vérifier si l'objet est le joueur ou un obstacle impassable
                if (obj instanceof Joueur || !obj.isPassable()) {
                    return false; // La boule ne peut pas tomber ici
                }
            }
        }

        return true; // La chute est possible
    }



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
}
