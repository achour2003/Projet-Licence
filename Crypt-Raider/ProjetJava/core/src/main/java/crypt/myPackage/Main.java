package crypt.myPackage;
//
import com.badlogic.gdx.ApplicationAdapter;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.GL20;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.badlogic.gdx.utils.ScreenUtils;

import com.badlogic.gdx.Game;

public class Main extends Game {
    private int level;
    private Play play;

    public Main(int level) {
        this.level = level;
    }

    @Override
    public void create() {
        setScreen(new MainMenu(this)); // Affiche le menu principal au démarrage
    }

    public void startGame(int startingLevel) {
        this.level = startingLevel;
        play = new Play(level,this);
        setScreen(play); // Définit le jeu comme écran actif
    }

    @Override
    public void render() {
        super.render();
        if (play != null && play.getEND()) {
            play.saveLevel(level); // Sauvegarde le niveau terminé
            level++;
            startGame(level);
        }
    }

    @Override
    public void dispose() {
        super.dispose();
    }
}
