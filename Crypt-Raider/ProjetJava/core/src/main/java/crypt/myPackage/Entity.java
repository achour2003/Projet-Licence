package crypt.myPackage;

import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.Batch;
import com.badlogic.gdx.graphics.g2d.Sprite;

public abstract class Entity extends GameObject {
    protected float speed;

    public Entity(Texture texture, float x, float y, float speed) {
        super(texture,x,y);
        this.speed = speed;
        //setPosition(x, y);
    }

    // Méthode abstraite pour le mouvement (spécifique à chaque entité)
    public abstract void move(float delta);

    @Override
    public void draw(Batch batch) {
        super.draw(batch);
    }
}
