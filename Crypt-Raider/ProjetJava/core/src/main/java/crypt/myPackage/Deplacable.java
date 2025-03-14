package crypt.myPackage;

public interface Deplacable {
    void move(float dx, float dy);
    default  boolean deplacable(){
        return true;
    }
}
