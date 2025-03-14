/*package crypt.myPackage;

import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.math.Rectangle;
import java.util.List;

public abstract class ObjetExplosif extends GameObject {
    public ObjetExplosif(Texture texture, float x, float y) {
        super(texture, x, y);
    }

    // Méthode pour les objets explosifs
    public abstract void explode(List<GameObject> gameObjects);

    // Méthode commune pour le mouvement vertical (chute)
    public boolean fall(float delta, float fallSpeed, List<GameObject> gameObjects) {
        float newY = getY() - fallSpeed * delta; // Descend à une vitesse définie
        Rectangle nextPosition = new Rectangle(getX(), newY, getWidth(), getHeight());

        for (GameObject obj : gameObjects) {
            if (obj.isActive() && nextPosition.overlaps(obj.getBounds())) {
                // Si collision avec un joueur ou un ennemi, déclenche une explosion
                if (obj instanceof Joueur || obj instanceof Enemie) {
                    explode(gameObjects);
                    obj.deactivate(); // Désactiver l'objet touché
                    return false; // Arrête la chute après l'explosion
                }
                if (!obj.isPassable()) {
                    return false; // Arrête la chute sur un obstacle non passable
                }
            }
        }

        // Si pas d'obstacle, met à jour la position
        setPosition(getX(), newY);
        bounds.setPosition(getX(), newY);
        return true; // Continue la chute
    }

    @Override
    public boolean isDestructible() {
        return true; // Peut être détruit par une explosion
    }
}
*/
// a supprimer
