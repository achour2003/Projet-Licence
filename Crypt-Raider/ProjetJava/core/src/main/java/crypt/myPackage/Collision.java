package crypt.myPackage;

public interface Collision {
     void onCollision(Joueur player); // Action à effectuer lors d'une collision avec un joueur
     boolean isPassable(); // Indique si le mur peut être traversé

    public boolean isDestructible() ;
}
