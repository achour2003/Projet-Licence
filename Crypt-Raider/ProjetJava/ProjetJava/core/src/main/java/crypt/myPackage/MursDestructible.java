package crypt.myPackage;

import com.badlogic.gdx.graphics.Texture;

public class MursDestructible extends Murs {
    private boolean destroyed = false;

    public MursDestructible(Texture texture, float x, float y) {
        super(texture, x, y);
    }

    @Override
    public void onCollision(Joueur palyer) {
        if (!destroyed) {
            System.out.println("Mur destructible détruit !");
            destroyed = true;
            deactivate(); // Rendre le mur inactif
        }
    }

    @Override
    public boolean isPassable() {
        return destroyed; // Traversable uniquement après destruction
    }
}
