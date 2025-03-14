package crypt.myPackage;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.GL20;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.badlogic.gdx.scenes.scene2d.Stage;
import com.badlogic.gdx.scenes.scene2d.ui.TextButton;
import com.badlogic.gdx.scenes.scene2d.ui.Skin;
import com.badlogic.gdx.scenes.scene2d.utils.ClickListener;
import com.badlogic.gdx.Screen;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;
//
//public class MainMenu implements Screen {
//    private final Main game;
//    private final Stage stage;
//    private final Skin skin;
//    private SpriteBatch batch;
//
//    public MainMenu(Main game) {
//        this.game = game;
//        this.stage = new Stage();
//        Gdx.input.setInputProcessor(stage);
//
//        // Créer un skin pour les boutons
//        skin = new Skin(Gdx.files.internal("uiskin.json")); // Assurez-vous d'avoir un fichier uiskin.json
//
//        // Ajouter le bouton Play
//        TextButton playButton = new TextButton("Play", skin);
//        playButton.setPosition(Gdx.graphics.getWidth() / 2f - 100, Gdx.graphics.getHeight() / 2f + 50);
//        playButton.addListener(new ClickListener() {
//            @Override
//            public void clicked(com.badlogic.gdx.scenes.scene2d.InputEvent event, float x, float y) {
//                game.startGame(1); // Démarre le jeu au premier niveau
//            }
//        });
//        stage.addActor(playButton);
//
//        // Ajouter le bouton Continue
//        TextButton continueButton = new TextButton("Continue", skin);
//        continueButton.setPosition(Gdx.graphics.getWidth() / 2f - 100, Gdx.graphics.getHeight() / 2f - 50);
//        continueButton.addListener(new ClickListener() {
//            @Override
//            public void clicked(com.badlogic.gdx.scenes.scene2d.InputEvent event, float x, float y) {
//                int savedLevel = loadSavedLevel();
//                game.startGame(savedLevel); // Démarre le jeu au niveau sauvegardé
//            }
//        });
//        stage.addActor(continueButton);
//
//        this.batch = new SpriteBatch();
//    }
//
//    @Override
//    public void show() {}
//
//    @Override
//    public void render(float delta) {
//        Gdx.gl.glClear(GL20.GL_COLOR_BUFFER_BIT);
//        stage.act(delta);
//        stage.draw();
//    }
//
//    @Override
//    public void resize(int width, int height) {}
//
//    @Override
//    public void pause() {}
//
//    @Override
//    public void resume() {}
//
//    @Override
//    public void hide() {}
//
//    @Override
//    public void dispose() {
//        stage.dispose();
//        skin.dispose();
//        batch.dispose();
//    }
//
//    private int loadSavedLevel() {
//        File file = new File("save.txt");
//        if (file.exists()) {
//            try (Scanner scanner = new Scanner(file)) {
//                return scanner.nextInt();
//            } catch (IOException e) {
//                e.printStackTrace();
//            }
//        }
//        return 0; // Par défaut, retourne le niveau 1 si aucun fichier de sauvegarde n'existe
//    }
//}


import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.badlogic.gdx.scenes.scene2d.Stage;
import com.badlogic.gdx.scenes.scene2d.ui.Skin;
import com.badlogic.gdx.scenes.scene2d.ui.TextButton;
import com.badlogic.gdx.scenes.scene2d.utils.ClickListener;
import com.badlogic.gdx.scenes.scene2d.InputEvent;
import com.badlogic.gdx.utils.ScreenUtils;
import com.badlogic.gdx.Screen;
import  java.util.Scanner ;
public class MainMenu implements Screen {
    private final Main game;
    private Stage stage;
    private Skin skin;
    private SpriteBatch batch;
    private Texture background; // Ajout du fond d'écran

    public MainMenu(Main game) {
        this.game = game;
        this.batch = new SpriteBatch();

        // Chargement du fond d'écran
        this.background = new Texture(Gdx.files.internal("menu.jpg"));

        // Initialisation de la scène
        stage = new Stage();
        Gdx.input.setInputProcessor(stage);

        // Chargement du skin
        skin = new Skin(Gdx.files.internal("uiskin.json"));

        // Création des boutons
        TextButton playButton = new TextButton("Jouer", skin);
        playButton.setPosition(300, 400);
        playButton.setSize(200, 50);
        playButton.addListener(new ClickListener() {
            @Override
            public void clicked(InputEvent event, float x, float y) {
                game.startGame(0); // Commence à partir du niveau 0
            }
        });

        TextButton continueButton = new TextButton("Continuer", skin);
        continueButton.setPosition(300, 300);
        continueButton.setSize(200, 50);
        continueButton.addListener(new ClickListener() {
            @Override
            public void clicked(InputEvent event, float x, float y) {
                int savedLevel = loadSavedLevel(); // Charge le niveau sauvegardé
                game.startGame(savedLevel);
            }
        });

        // Ajout des boutons à la scène
        stage.addActor(playButton);
        stage.addActor(continueButton);
    }

    @Override
    public void show() {
    }

    @Override
    public void render(float delta) {
        // Affichage du fond d'écran
        ScreenUtils.clear(0, 0, 0, 1);
        batch.begin();
        batch.draw(background, 0, 0, Gdx.graphics.getWidth(), Gdx.graphics.getHeight());
        batch.end();

        // Rendu des boutons
        stage.act(delta);
        stage.draw();
    }

    @Override
    public void resize(int width, int height) {
    }

    @Override
    public void pause() {
    }

    @Override
    public void resume() {
    }

    @Override
    public void hide() {
    }

    @Override
    public void dispose() {
        stage.dispose();
        batch.dispose();
        background.dispose();
        skin.dispose();
    }


    private int loadSavedLevel() {
        File file = new File("save.txt");
        if (file.exists()) {
            try (Scanner scanner = new Scanner(file)) {
                return scanner.nextInt();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return 0; // Par défaut, retourne le niveau 1 si aucun fichier de sauvegarde n'existe
    }
}
