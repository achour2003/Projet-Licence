package crypt.myPackage;

import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.math.Rectangle;
import java.util.List;

public class Enemie extends Entity {
    private static final int TILE_SIZE = 80; // Taille d'une tuile
    private String direction = "left"; // Direction initiale par défaut : "up", "down", "left", "right"
    private float moveDelay = 0.9f; // Temps d'attente entre les mouvements (en secondes)
    private float moveTimer = 0;    // Chronomètre pour le délai de mouvement

    public Enemie(Texture texture, float x, float y) {
        super(texture, x * TILE_SIZE, y * TILE_SIZE, 200); // Position initiale en "cases"
    }

    public void move(float delta, List<GameObject> gameObjects,List<GameObject> objectsToRemoveGlobal) {
        moveTimer -= delta;
        if (moveTimer > 0) return; // Attendre que le délai soit écoulé

        float newX = getX();
        float newY = getY();

        // Vérifier la prochaine position selon la direction actuelle
        switch (direction) {
            case "up":
                newY += TILE_SIZE;
                break;
            case "down":
                newY -= TILE_SIZE;
                break;
            case "left":
                newX -= TILE_SIZE;
                break;
            case "right":
                newX += TILE_SIZE;
                break;
        }

        // Vérifier les collisions pour la direction actuelle
        if (canMove(newX, newY, gameObjects,objectsToRemoveGlobal)) {
            // Si pas d'obstacle, avancer dans la direction actuelle
            setPosition(newX, newY);
            bounds.setPosition(newX, newY);
        } else {
            // Changer de direction pour contourner l'obstacle
            adjustDirection(gameObjects,objectsToRemoveGlobal);
        }

        moveTimer = moveDelay; // Réinitialiser le délai après le mouvement
    }

    public void moveAI(float delta, Joueur joueur, List<GameObject> gameObjects , List<GameObject> objectsToRemoveGlobal) {
        moveTimer -= delta;
        if (moveTimer > 0) return; // Attendre que le délai soit écoulé

        float targetX = joueur.getX();
        float targetY = joueur.getY();
        float currentX = getX();
        float currentY = getY();

        float newX = currentX;
        float newY = currentY;

        // Priorité de mouvement pour se rapprocher du joueur
        if (Math.abs(targetX - currentX) > Math.abs(targetY - currentY)) {
            if (targetX > currentX) {
                newX += TILE_SIZE; // Déplacement à droite
            } else {
                newX -= TILE_SIZE; // Déplacement à gauche
            }
        } else {
            if (targetY > currentY) {
                newY += TILE_SIZE; // Déplacement en haut
            } else {
                newY -= TILE_SIZE; // Déplacement en bas
            }
        }

        // Vérifier si le mouvement est possible
        if (canMove(newX, newY, gameObjects,objectsToRemoveGlobal)) {
            setPosition(newX, newY);
            bounds.setPosition(newX, newY);
        } else {
            // Essayer une direction alternative si bloqué
            if (newX != currentX) {
                // Si tentative horizontale échoue, essayer verticale
                if (targetY > currentY && canMove(currentX, currentY + TILE_SIZE, gameObjects,objectsToRemoveGlobal)) {
                    setPosition(currentX, currentY + TILE_SIZE);
                    bounds.setPosition(currentX, currentY + TILE_SIZE);
                } else if (targetY < currentY && canMove(currentX, currentY - TILE_SIZE, gameObjects,objectsToRemoveGlobal)) {
                    setPosition(currentX, currentY - TILE_SIZE);
                    bounds.setPosition(currentX, currentY - TILE_SIZE);
                }
            } else if (newY != currentY) {
                // Si tentative verticale échoue, essayer horizontale
                if (targetX > currentX && canMove(currentX + TILE_SIZE, currentY, gameObjects,objectsToRemoveGlobal)) {
                    setPosition(currentX + TILE_SIZE, currentY);
                    bounds.setPosition(currentX + TILE_SIZE, currentY);
                } else if (targetX < currentX && canMove(currentX - TILE_SIZE, currentY, gameObjects,objectsToRemoveGlobal)) {
                    setPosition(currentX - TILE_SIZE, currentY);
                    bounds.setPosition(currentX - TILE_SIZE, currentY);
                }
            }
        }

        moveTimer = moveDelay; // Réinitialiser le délai après le mouvement
    }

    private void adjustDirection(List<GameObject> gameObjects, List<GameObject> objectsToRemoveGlobal) {
        // Essayer les directions dans l'ordre des bords
        if (direction.equals("right")) {
            if (canMove(getX(), getY() + TILE_SIZE, gameObjects,objectsToRemoveGlobal)) {
                direction = "up"; // Contourner en montant
            } else if (canMove(getX(), getY() - TILE_SIZE, gameObjects,objectsToRemoveGlobal)) {
                direction = "down"; // Contourner en descendant
            }
        } else if (direction.equals("left")) {
            if (canMove(getX(), getY() + TILE_SIZE, gameObjects,objectsToRemoveGlobal)) {
                direction = "up"; // Contourner en montant
            } else if (canMove(getX(), getY() - TILE_SIZE, gameObjects,objectsToRemoveGlobal)) {
                direction = "down"; // Contourner en descendant
            }
        } else if (direction.equals("up")) {
            if (canMove(getX() + TILE_SIZE, getY(), gameObjects,objectsToRemoveGlobal)) {
                direction = "right"; // Contourner en allant à droite
            } else if (canMove(getX() - TILE_SIZE, getY(), gameObjects,objectsToRemoveGlobal)) {
                direction = "left"; // Contourner en allant à gauche
            }
        } else if (direction.equals("down")) {
            if (canMove(getX() + TILE_SIZE, getY(), gameObjects,objectsToRemoveGlobal)) {
                direction = "right"; // Contourner en allant à droite
            } else if (canMove(getX() - TILE_SIZE, getY(), gameObjects,objectsToRemoveGlobal)) {
                direction = "left"; // Contourner en allant à gauche
            }
        }
    }

    private boolean canMove(float x, float y, List<GameObject> gameObjects , List<GameObject> objectsToRemoveGlobal) {
        Rectangle testPosition = new Rectangle(x, y, getWidth(), getHeight());

        for (GameObject obj : gameObjects) {
            if (obj.isActive() && testPosition.overlaps(obj.getBounds())) {
                if(obj instanceof Joueur){
                    onCollision((Joueur) obj);
                    ((Joueur) obj).setEnvie(false);
                    obj.deactivate();
                    //objectsToRemoveGlobal.add(obj);

                }
                return false; // Mouvement bloqué
            }
        }

        return true; // Mouvement possible
    }

    @Override
    public void onCollision(Joueur player) {
        System.out.println("Collision avec le joueur ! test");

        // Ajouter une logique si nécessaire (par exemple, réduire la santé du joueur)
    }

    @Override
    public boolean isPassable() {
        return false;
    }

    @Override
    public boolean isDestructible() {
        return true; // Peut être détruit par une explosion
    }

    @Override
    public void move(float delta) {

    }
}
