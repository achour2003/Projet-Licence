package crypt.myPackage;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.Input;
import com.badlogic.gdx.Screen;
import com.badlogic.gdx.assets.AssetManager;
import com.badlogic.gdx.graphics.Color;
import com.badlogic.gdx.graphics.GL20;
import com.badlogic.gdx.graphics.OrthographicCamera;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.Batch;
import com.badlogic.gdx.graphics.g2d.BitmapFont;
import com.badlogic.gdx.graphics.g2d.GlyphLayout;
import com.badlogic.gdx.maps.MapObject;
import com.badlogic.gdx.maps.objects.RectangleMapObject;
import com.badlogic.gdx.maps.tiled.TiledMap;
import com.badlogic.gdx.maps.tiled.TmxMapLoader;
import com.badlogic.gdx.maps.tiled.renderers.OrthogonalTiledMapRenderer;
import com.badlogic.gdx.math.Rectangle;

import java.io.*;
import java.util.ArrayList;
import java.util.List;

public class Play implements Screen {
    private TiledMap map;
    private OrthogonalTiledMapRenderer renderer;
    private OrthographicCamera camera;
    private int level ;
    private boolean END;
    private Main game; // Ajoutez cette variable pour référencer l'instance principale
    private boolean isGameOver = false;
    public  List<GameObject> gameObjects; // Liste des objets du jeu
    private List<GameObject> objectsToRemoveGlobal = new ArrayList<>();


    private static final int TILE_SIZE = 80; // Taille d'une tuile
    private float moveDelay = 0.17f;
    private float moveTimer = 0;
    public Play(int level, Main game) {
        this.game = game;
        this.level = level;
    }


    private void goToMainMenu() {
        // Remplacez GameMainMenuScreen par votre écran de menu principal
        game.setScreen(new MainMenu(game));
    }

    @Override
    public void show() {

        map = new TmxMapLoader().load("level"+level+".tmx");
        renderer = new OrthogonalTiledMapRenderer(map);
        camera = new OrthographicCamera();
        camera.setToOrtho(false, Gdx.graphics.getWidth(), Gdx.graphics.getHeight());

        gameObjects = new ArrayList<>();

        // Charger les murs
        for (MapObject object : map.getLayers().get("Murs").getObjects()) {
            if (object instanceof RectangleMapObject) {
                Rectangle rectangle = ((RectangleMapObject) object).getRectangle();
                String type = (String) object.getProperties().get("type");

                Texture destructible = new Texture("murr.PNG");
                Texture indestructible = new Texture("murIndistructible.png");
                Texture mangeable = new Texture("fromage.png");
                if ("Destructible".equals(type)) {
                    gameObjects.add(new MursDestructible(destructible, rectangle.x, rectangle.y));
                } else if ("Indestructible".equals(type)) {
                    gameObjects.add(new MursIndestructible(indestructible, rectangle.x, rectangle.y));
                } else if ("Mangeable".equals(type)) {
                    gameObjects.add(new MursMangeable(mangeable, rectangle.x, rectangle.y));
                }
            }
        }

        // Charger les clés et portes
        List<Porte> portes = new ArrayList<>();
        List<Cle> cles = new ArrayList<>();

        for (MapObject object : map.getLayers().get("Cle-Porte").getObjects()) {
            if (object instanceof RectangleMapObject) {
                Rectangle rectangle = ((RectangleMapObject) object).getRectangle();
                String type = (String) object.getProperties().get("type");

                if ("Cle".equals(type)) {
                    Cle cle = new Cle(new Texture("cle-removebg-preview.png"), rectangle.x, rectangle.y);
                    cles.add(cle);
                    gameObjects.add(cle);
                } else if ("Porte".equals(type)) {
                    Porte porte = new Porte(new Texture("porte.png"), rectangle.x, rectangle.y);
                    portes.add(porte);
                    gameObjects.add(porte);
                }
            }
        }

        // Associer chaque clé à une porte
        for (int i = 0; i < cles.size() && i < portes.size(); i++) {
            cles.get(i).setPorteAssociee(portes.get(i));
        }

        // Charger le joueur
        for (MapObject object : map.getLayers().get("Personnage").getObjects()) {
            if (object instanceof RectangleMapObject) {
                Rectangle rectangle = ((RectangleMapObject) object).getRectangle();
                String type = (String) object.getProperties().get("type");

                if ("Joueur".equals(type)) {
                    Joueur joueur = new Joueur(new Texture("perso-removebg-preview.png"), rectangle.x / TILE_SIZE, rectangle.y / TILE_SIZE);
                    gameObjects.add(joueur);
                } else if ("Enemie".equals(type)) {
                    Enemie enmie = new Enemie(new Texture("bbeee-removebg-preview.png"), rectangle.x / TILE_SIZE, rectangle.y / TILE_SIZE);
                    gameObjects.add(enmie);
                }
            }
        }

        // Charger les boules de type magique et rocher
        for (MapObject object : map.getLayers().get("Deplacable").getObjects()) {
            if (object instanceof RectangleMapObject) {
                Rectangle rectangle = ((RectangleMapObject) object).getRectangle();
                String type = (String) object.getProperties().get("type");

                if ("BouleMagique".equals(type)) {
                    Boule bouleMagique = new Boule(new Texture("boule-removebg-preview(1).png"), rectangle.x, rectangle.y);
                    gameObjects.add(bouleMagique);
                } else if ("Rocher".equals(type)) {
                    Rocher rocher = new Rocher(new Texture("rocherere-removebg-preview.png"), rectangle.x, rectangle.y);
                    gameObjects.add(rocher);
                } else if ("Bombe".equals(type)) {
                    Bombe bombe = new Bombe(new Texture("boumbaaa-removebg-preview.png"), rectangle.x , rectangle.y);
                    gameObjects.add(bombe);

                }
            }
        }


        // Charger les portes de fin
        for (MapObject object : map.getLayers().get("Fin").getObjects()) {
            if (object instanceof RectangleMapObject) {
                Rectangle rectangle = ((RectangleMapObject) object).getRectangle();

                PorteFin porteFin = new PorteFin(new Texture("fin.png"), rectangle.x, rectangle.y);
                System.out.println("fin");
                // Comptez les boules magiques pour définir le total



                gameObjects.add(porteFin);
            }
        }

        for (GameObject object : gameObjects) {
            System.out.println(object.getClass().getSimpleName());

        }

    }

    private Joueur getJoueurFromGameObjects() {
        for (GameObject obj : gameObjects) {
            if (obj instanceof Joueur) {
                return (Joueur) obj;
            }
        }
        return null; // Retourne null si aucun joueur n'est trouvé (ce qui ne devrait pas arriver).
    }

    private void handleInput(float delta) {
        moveTimer -= delta;
        if (moveTimer > 0) return;

        Joueur joueur = getJoueurFromGameObjects();
        if (joueur == null) return;

        int dx = 0, dy = 0;

        if (Gdx.input.isKeyPressed(Input.Keys.LEFT)) dx = -1;
        if (Gdx.input.isKeyPressed(Input.Keys.RIGHT)) dx = 1;
        if (Gdx.input.isKeyPressed(Input.Keys.UP)) dy = 1;
        if (Gdx.input.isKeyPressed(Input.Keys.DOWN)) dy = -1;

        if (dx != 0 || dy != 0) {
            // Sauvegarder la position initiale
            float oldX = joueur.getX();
            float oldY = joueur.getY();

            joueur.moveByTile(dx, dy, gameObjects);

            // Libérer la case précédente si le joueur a bougé
            if (oldX != joueur.getX() || oldY != joueur.getY()) {
                // Implémentez la logique pour libérer l'ancienne case ici
            }

            moveTimer = moveDelay;
        }
    }
    private void drawGameOverMessage() {
        BitmapFont font = new BitmapFont(); // Assurez-vous d'avoir une instance BitmapFont
        font.getData().setScale(2.5f); // Ajustez la taille du texte
        font.setColor(Color.RED);

        // Récupérer les dimensions de l'écran
        float screenWidth = Gdx.graphics.getWidth();
        float screenHeight = Gdx.graphics.getHeight();

        // Afficher "Game Over" au centre de l'écran
        String message = "Game Over";
        GlyphLayout layout = new GlyphLayout(font, message);
        float x = (screenWidth - layout.width) / 2;
        float y = (screenHeight + layout.height) / 2;
        font.draw(renderer.getBatch(), message, x, y);
    }


    @Override
    public void render(float delta) {
        Gdx.gl.glClearColor(0, 0, 0, 1);
        Gdx.gl.glClear(GL20.GL_COLOR_BUFFER_BIT);

        handleInput(delta); // Gérer les entrées utilisateur

        // Mise à jour de la caméra
        renderer.setView(camera);
        renderer.render();


        // Démarrer le batch pour le rendu des entités
        renderer.getBatch().begin();
        for (GameObject obj : gameObjects) {
            if (obj instanceof Joueur && !obj.isActive()){
                drawGameOverMessage();
                isGameOver = true;
            }
            if (isGameOver && Gdx.input.isKeyJustPressed(Input.Keys.ANY_KEY)) {
                goToMainMenu(); // Méthode pour revenir au menu principal
                return; // Arrête l'exécution pour éviter les autres actions
            }
            if (obj.isActive()) {
                if (obj instanceof Enemie) {
                    Enemie enemie = (Enemie) obj;
                    Joueur joueur = getJoueurFromGameObjects(); // Assurez-vous d'avoir cette méthode
                    if(joueur != null) {
                        enemie.moveAI(delta, joueur, gameObjects, objectsToRemoveGlobal); // Appel de moveAI
                        //enemie.move(delta , gameObjects);
                    }
                    //enemie.moveAI(delta, joueur, gameObjects); // Appel de moveAI
                }else if (obj instanceof Boule) {
                ((Boule) obj).update(delta, gameObjects, objectsToRemoveGlobal);
                } else if (obj instanceof PorteFin) {

                    PorteFin porteFin = (PorteFin)obj;
                    porteFin.checkAndOpen(gameObjects);

                    END =porteFin.update(delta, gameObjects, objectsToRemoveGlobal);

                } else if (obj instanceof Rocher) {
                    ((Rocher) obj).update(delta, gameObjects, objectsToRemoveGlobal);

                } else if (obj instanceof Bombe) {
                    ((Bombe) obj).update(delta, gameObjects, objectsToRemoveGlobal);

                }
                // obj.update(delta);
                obj.draw(renderer.getBatch());
            }
        }



// Supprimer les objets marqués après l'itération
        //System.out.println(objectsToRemoveGlobal);
        gameObjects.removeAll(objectsToRemoveGlobal);
        objectsToRemoveGlobal.clear();
        renderer.getBatch().end(); // Terminer le batch
    }

    // Méthode pour compter le nombre de boules magiques


    public void saveLevel(int level) {
        File file = new File("save.txt");
        try (FileWriter writer = new FileWriter(file)) {
            writer.write(String.valueOf(level));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public boolean getEND(){
        return END  ;
    }

    @Override
    public void resize(int width, int height) {
        camera.viewportWidth = width;
        camera.viewportHeight = height;
        camera.update();
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
        map.dispose();
        renderer.dispose();
    }
}
