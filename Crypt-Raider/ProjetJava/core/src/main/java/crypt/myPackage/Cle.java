package crypt.myPackage;

import com.badlogic.gdx.graphics.Texture;

public class Cle extends GameObject {
    private Porte porteAssociee; // Porte associée à cette clé

    public Cle(Texture texture, float x, float y) {
        super(texture, x, y);
    }

    public void setPorteAssociee(Porte porte) {
        this.porteAssociee = porte;
    }

    @Override
    public void onCollision(Joueur joueur) {
        if (isActive()) {
            System.out.println("Clé récupérée !");
            joueur.pickUpCle();

            // Désactiver la porte associée si elle existe
            if (porteAssociee != null && porteAssociee.isActive()) {
                porteAssociee.deactivate();
                System.out.println("La porte associée disparaît !");
            }

            deactivate(); // Désactiver la clé elle-même
        }
    }

    @Override
    public boolean isPassable() {
        return true; // La clé est toujours traversable
    }

    public boolean isDestructible() {
        return false; // Peut être détruit par une explosion
    }


}
