package crypt.myPackage;

import com.badlogic.gdx.graphics.Texture;

public abstract class Murs extends GameObject {
    public Murs(Texture texture, float x, float y) {
        super(texture, x, y);
    }

    @Override
    public boolean isPassable() {
        return false; // Les murs ne sont pas traversables par défaut
    }
}
