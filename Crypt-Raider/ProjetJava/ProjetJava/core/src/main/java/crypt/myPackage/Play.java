package crypt.myPackage;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.Input;
import com.badlogic.gdx.Screen;
import com.badlogic.gdx.assets.AssetManager;
import com.badlogic.gdx.graphics.GL20;
import com.badlogic.gdx.graphics.OrthographicCamera;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.maps.MapObject;
import com.badlogic.gdx.maps.objects.RectangleMapObject;
import com.badlogic.gdx.maps.tiled.TiledMap;
import com.badlogic.gdx.maps.tiled.TmxMapLoader;
import com.badlogic.gdx.maps.tiled.renderers.OrthogonalTiledMapRenderer;
import com.badlogic.gdx.math.Rectangle;

import java.util.ArrayList;
import java.util.List;

public class Play implements Screen {
    private TiledMap map;
    private OrthogonalTiledMapRenderer renderer;
    private OrthographicCamera camera;


    public static List<GameObject> gameObjects; // Liste des objets du jeu


    private static final int TILE_SIZE = 80; // Taille d'une tuile
    private float moveDelay = 0.17f;
    private float moveTimer = 0;


    @Override
    public void show() {

        map = new TmxMapLoader().load("map.tmx");
        renderer = new OrthogonalTiledMapRenderer(map);
        camera = new OrthographicCamera();
        camera.setToOrtho(false, Gdx.graphics.getWidth(), Gdx.graphics.getHeight());

        gameObjects = new ArrayList<>();

        // Charger les murs
        for (MapObject object : map.getLayers().get("Murs").getObjects()) {
            if (object instanceof RectangleMapObject) {
                Rectangle rectangle = ((RectangleMapObject) object).getRectangle();
                String type = (String) object.getProperties().get("type");

                Texture destructible = new Texture("murr.png");
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
                    Boule boule = new Boule(new Texture("rocherere-removebg-preview.png"), rectangle.x, rectangle.y);
                    gameObjects.add(boule);
                }
            }
        }


        // Charger les portes de fin
        for (MapObject object : map.getLayers().get("Fin").getObjects()) {
            if (object instanceof RectangleMapObject) {
                Rectangle rectangle = ((RectangleMapObject) object).getRectangle();
                PorteFin porteFin = new PorteFin(new Texture("fin.png"), rectangle.x, rectangle.y);

                // Comptez les boules magiques pour définir le total



                gameObjects.add(porteFin);
            }
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

        // Récupérer le joueur depuis gameObjects
        Joueur joueur = getJoueurFromGameObjects();
        if (joueur == null) {
            System.out.println("Erreur : Joueur introuvable dans la liste gameObjects.");
            return;
        }

        int dx = 0, dy = 0;

        // Gérer les entrées clavier
        if (Gdx.input.isKeyPressed(Input.Keys.LEFT)) dx = -1;
        if (Gdx.input.isKeyPressed(Input.Keys.RIGHT)) dx = 1;
        if (Gdx.input.isKeyPressed(Input.Keys.UP)) dy = 1;
        if (Gdx.input.isKeyPressed(Input.Keys.DOWN)) dy = -1;

        if (dx != 0 || dy != 0) {
            joueur.moveByTile(dx, dy, gameObjects);
            moveTimer = moveDelay;
        }
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

        // Mettre à jour et dessiner les objets actifs
        for (GameObject obj : gameObjects) {
            if (obj.isActive()) {
                // Mise à jour spécifique des boules
                if (obj instanceof Boule) {
                    ((Boule) obj).update(delta, gameObjects);
                }

                // Vérifie si la porte de fin peut s'ouvrir


                // Dessiner l'objet
                obj.draw(renderer.getBatch());
            }
        }/*
        // Dessiner les objets actifs
        for (GameObject obj : gameObjects) {
            if (obj.isActive()) {
                obj.draw(renderer.getBatch());
            }
        }*/


        // Dessiner le joueur
       // joueur.draw(renderer.getBatch());
        //enmie.draw(renderer.getBatch());


        // Vérifie si le joueur est actif
        /*if (!joueur.isActive()) {
            System.out.println("Jeu terminé. Merci d'avoir joué !");
            Gdx.app.exit(); // Quittez le jeu
            return;
        }*/

        renderer.getBatch().end(); // Terminer le batch
    }

    // Méthode pour compter le nombre de boules magiques





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
