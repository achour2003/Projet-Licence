package crypt.myPackage;

import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.math.Rectangle;

import java.util.List;
public class Rocher extends RollingEntity{

        public Rocher(Texture texture, float x, float y) {
            super(texture, x, y);
            bounds.set(x, y, texture.getWidth(), texture.getHeight());
        }

    }



