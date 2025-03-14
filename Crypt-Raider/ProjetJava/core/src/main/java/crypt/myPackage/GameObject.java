package crypt.myPackage;

import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.Sprite;
import com.badlogic.gdx.math.Rectangle;

public abstract class GameObject extends Sprite implements Collision{
    protected Rectangle bounds;
    private boolean active = true;
    private int TILE_SIZE = 80 ;

    public GameObject(Texture texture, float x, float y) {
        super(texture);
        setPosition(x, y);
        bounds = new Rectangle(x, y, TILE_SIZE, TILE_SIZE); // Assurez-vous que TILE_SIZE est cohérent

    }

    public Rectangle getBounds() {
        return bounds;
    }

    public boolean isActive() {
        return active;
    }

    public void deactivate() {
        this.active = false;
    }


}
