package crypt.myPackage;

import com.badlogic.gdx.graphics.Texture;

public class Enemie extends Entity{
    private static final int TILE_SIZE = 80; // Taille d'une tuile

    public Enemie(Texture texture, float x, float y) {
        super(texture, x * TILE_SIZE, y * TILE_SIZE, 200); // Position initiale en "cases"
    }

    @Override
    public void move(float delta) {

    }

    @Override
    public void onCollision(Joueur player) {

    }

    @Override
    public boolean isPassable() {
        return false;
    }

}


