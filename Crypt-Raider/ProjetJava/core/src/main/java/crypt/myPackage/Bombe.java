package crypt.myPackage;

import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.math.Rectangle;

import java.util.List;

public class Bombe extends RollingEntity {
    public Bombe(Texture texture, float x, float y) {
        super(texture, x, y);
        bounds.set(x, y, texture.getWidth(), texture.getHeight());
    }

public void update(float delta, List<GameObject> gameObjects, List<GameObject> objectsToRemoveGlobal) {


    if (isFalling) {
        fallProgress += fallSpeed * delta; // Progression de la chute

        if (fallProgress >= TILE_SIZE) {
            fallProgress = 0; // Réinitialiser pour la prochaine case
            setPosition(getX(), getY() - TILE_SIZE); // Descendre d'une case
            bounds.setPosition(getX(), getY());      // Mettre à jour les limites

            boolean collisionDetected = false;

            for (GameObject obj : gameObjects) {
                if (obj.isActive() && bounds.overlaps(obj.getBounds()) && obj != this   )  {

                    System.out.println("Collision détectée avec : " + obj.getClass().getSimpleName());
                    System.out.println("isPassable : " + obj.isPassable());
                    System.out.println("Bounds de la boule : " + bounds);
                    System.out.println("Bounds de l'objet : " + obj.getBounds());

                    if (obj instanceof PorteFin) {
                        System.out.println("Collision avec un joueur ou un ennemi ! Explosion.?????????????????????");
                        explode(gameObjects, objectsToRemoveGlobal); // Explosion dans toutes les cases environnantes
                        //obj.deactivate(); // Désactiver le joueur ou l'ennemi touché
                        //objectsToRemoveGlobal.add(obj); // Ajouter à la liste globale pour suppression
                        collisionDetected = true;
                        break; // Sortir de la boucle (priorité à l'explosion)


                    } else if (obj instanceof Joueur ) {
                        System.out.println("Collision avec un joueur ou un ennemi ! Explosion.?????????????????????");
                        explode(gameObjects, objectsToRemoveGlobal); // Explosion dans toutes les cases environnantes
                        //obj.deactivate(); // Désactiver le joueur ou l'ennemi touché
                        //objectsToRemoveGlobal.add(obj); // Ajouter à la liste globale pour suppression
                        collisionDetected = true;
                        break; // Sortir de la boucle (priorité à l'explosion)

                    } else if ( obj instanceof Enemie) {
                        System.out.println("Collision avec un joueur ou un ennemi ! Explosion.?????????????????????");
                        explode(gameObjects, objectsToRemoveGlobal); // Explosion dans toutes les cases environnantes
                        obj.deactivate(); // Désactiver le joueur ou l'ennemi touché
                        //objectsToRemoveGlobal.add(obj); // Ajouter à la liste globale pour suppression
                        collisionDetected = true;
                        break; // Sortir de la boucle (priorité à l'explosion)
                    } else if (!obj.isPassable()/* && !(obj instanceof Boule)*/) {
                        System.out.println("Collision avec un joueur ou un ennemi ! Explosion.?????????????????????");
                        explode(gameObjects, objectsToRemoveGlobal); // Explosion dans toutes les cases environnantes
                        //obj.deactivate(); // Désactiver le joueur ou l'ennemi touché
                        //objectsToRemoveGlobal.add(obj); // Ajouter à la liste globale pour suppression
                        collisionDetected = true;
                        break; // Sortir de la boucle (priorité à l'explosion)

                    }else   collisionDetected = true; // Collision détectée

                }/*else{
                        System.out.println("Collision avec un mur ou un objet non passable.");
                        collisionDetected = true; // Collision détectée
                        break; // Arrêter la chute
                    }*/
            }


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

    @Override
    public boolean canFall(List<GameObject> gameObjects) {
        float newY = getY() - TILE_SIZE; // Calcul de la position une case en dessous
        Rectangle nextPosition = new Rectangle(getX(), newY, getWidth(), getHeight());

        for (GameObject obj : gameObjects) {
            if (obj.isActive() && nextPosition.overlaps(obj.getBounds()) && obj != this ){
                // Si c'est un joueur ou un ennemi, la chute continue pour collisionner
                if (obj instanceof Joueur || obj instanceof Enemie) {
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

}
