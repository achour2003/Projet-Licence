package crypt.myPackage;

import com.badlogic.gdx.graphics.Texture;

public class MursIndestructible extends Murs {
    public MursIndestructible(Texture texture, float x, float y) {
        super(texture, x, y);
    }

    @Override
    public void onCollision(Joueur entity) {
        // Aucun effet, le mur est indestructible
        System.out.println("Collision avec un mur indestructible. Impossible de le traverser !");
    }

    @Override
    public boolean isPassable() {
        return false; // Jamais traversable
    }
    public boolean isDestructible() {
        return false; // Peut être détruit par une explosion
    }
}
