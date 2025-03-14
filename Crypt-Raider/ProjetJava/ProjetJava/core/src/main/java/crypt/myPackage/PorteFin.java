package crypt.myPackage;

import com.badlogic.gdx.graphics.Texture;
import java.util.List;
import java.util.ArrayList; // Ajoute ceci

public class PorteFin extends GameObject {
    private boolean accessible = false; // La porte est verrouillée par défaut

    public PorteFin(Texture texture, float x, float y) {
        super(texture, x, y);
    }




    @Override
    public void onCollision(Joueur joueur) {
        // Vérifie si toutes les boules sont placées
        if (accessible) {
            joueur.deactivate(); // Désactiver le joueur
            System.out.println("Porte ouverte. Vous avez terminé le niveau !");
        } else {
            System.out.println("La porte est verrouillée. Placez toutes les BouleMagique correctement !");
        }
    }

    @Override
    public boolean isPassable() {
        return accessible; // La porte devient passable uniquement lorsqu'elle est accessible
    }
}
