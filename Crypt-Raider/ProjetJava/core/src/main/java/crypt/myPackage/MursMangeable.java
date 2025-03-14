package crypt.myPackage;

import com.badlogic.gdx.graphics.Texture;

public class MursMangeable extends Murs {
    private boolean eaten = false;

    public MursMangeable(Texture texture, float x, float y) {
        super(texture, x, y);
    }

    @Override
    public void onCollision(Joueur entity) {
        if (isActive()) {
            System.out.println("Mur mangeable consommé !");
            eaten = true;
            deactivate(); // Rendre le mur inactif
        }
    }

    @Override
    public boolean isPassable() {
        return eaten; // Traversable uniquement après avoir été mangé
    }

    @Override
    public boolean isDestructible() {
        return true; // Peut être détruit par une explosion
    }
}
