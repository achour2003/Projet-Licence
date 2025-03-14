package crypt.myPackage;

import com.badlogic.gdx.graphics.Texture;

public class Porte extends GameObject {
    public Porte(Texture texture, float x, float y) {
        super(texture, x, y);
    }

    @Override
    public void onCollision(Joueur joueur) {
        if (isActive() && joueur.hasCle()) {
            System.out.println("La porte s'ouvre !");
            deactivate(); // Désactive la porte
        } else if (isActive()) {
            System.out.println("La porte est fermée ! Vous avez besoin de la clé.");
        }
    }

    @Override
    public boolean isPassable() {
        return !isActive(); // La porte est traversable uniquement lorsqu'elle est inactive
    }
}
