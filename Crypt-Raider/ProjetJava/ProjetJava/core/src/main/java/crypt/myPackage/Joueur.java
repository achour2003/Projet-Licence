package crypt.myPackage;

import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.math.Rectangle;
import java.util.List;

public class Joueur extends Entity {
    private boolean hasCle;
    private static final int TILE_SIZE = 80; // Taille d'une tuile

    public Joueur(Texture texture, float x, float y) {
        super(texture, x * TILE_SIZE, y * TILE_SIZE, 200); // Position initiale en "cases"
        this.hasCle = false; // Initialisation sans clé
    }
    @Override
    public void move(float delta) {
        // Contrôlé par handleInput dans Play.java
    }

    public void moveByTile(int dx, int dy, List<GameObject> gameObjects) {
        float newX = getX() + dx * TILE_SIZE;
        float newY = getY() + dy * TILE_SIZE;

        Rectangle nextPosition = new Rectangle(newX, newY, getWidth(), getHeight());

        for (GameObject obj : gameObjects) {
            if (obj.isActive() && nextPosition.overlaps(obj.getBounds())) {
                // Si l'objet est un ennemi, empêcher le joueur de se déplacer
                if (obj instanceof Enemie) {
                    System.out.println("Collision avec un ennemi ! Déplacement bloqué.");
                    return; // Le joueur ne bouge pas
                }

                // Si l'objet est une boule
                if (obj instanceof Boule) {
                    Boule boule = (Boule) obj;
                    if (!boule.push(dx, dy, gameObjects)) {
                        return; // La boule bloque le joueur
                    }
                    setPosition(newX, newY); // Déplacer le joueur
                    return; // Le joueur a été déplacé
                }

                // Gérer les autres collisions
                obj.onCollision(this);
                if (!obj.isPassable()) {
                    return; // Collision bloquante
                }
            }
        }

        // Déplacer le joueur si aucune collision
        setPosition(newX, newY);
    }

    public boolean hasCle() {
        return hasCle;
    }

    public void pickUpCle() {
        hasCle = true;
    }


    @Override
    public void onCollision(Joueur player) {

    }

    @Override
    public boolean isPassable() {
        return false;
    }
}
