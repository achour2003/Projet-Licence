# Crypt_Raider

A [libGDX](https://libgdx.com/) project generated with [gdx-liftoff](https://github.com/libgdx/gdx-liftoff).

This project was generated with a template including simple application launchers and an `ApplicationAdapter` extension that draws libGDX logo.
Instructions pour Exécuter le Jeu

Pour exécuter le jeu Crypt-Raider, suivez attentivement les étapes suivantes :
1. Télécharger et décompresser le projet

   Téléchargement : Récupérez le fichier Crypt-Raider.zip disponible sur la plateforme Moodle.
   Décompression : Décompressez le fichier dans un répertoire de votre choix sur votre ordinateur.

2. Naviguer vers le projet du jeu

   Une fois le fichier décompressé, ouvrez le dossier Crypt-Raider.
   À l'intérieur, localisez le sous-dossier ProjetJava qui contient le jeu.

3. Ouvrir l’invite de commande (CMD)

   Accès à la commande :
   Sous Windows : Appuyez sur les touches Win + R, tapez cmd, puis appuyez sur Entrée.
   Sous Linux/Mac : Utilisez le terminal.
   Naviguer vers le dossier du projet :
   À l’aide de la commande cd, déplacez-vous vers le dossier ProjetJava :

        cd chemin\vers\Crypt-Raider\ProjetJava

4. Exécuter le jeu

   Une fois dans le dossier ProjetJava, exécutez la commande appropriée pour votre système :
   Pour Windows :

gradlew.bat lwjgl3:run

Pour Linux/Mac :

        ./gradlew lwjgl3:run

    Le jeu Crypt-Raider démarrera automatiquement après quelques instants.

Notes et Prérequis

    Prérequis système :
        Java Development Kit (JDK) version 8 ou supérieure doit être installé.
        Pour vérifier si Java est installé, tapez la commande suivante dans votre terminal ou CMD :

    java -version

    Si Java n'est pas installé, téléchargez-le depuis le site officiel Oracle Java ou utilisez un gestionnaire de paquets.

Dépannage :

    Si le jeu ne se lance pas, vérifiez que vous êtes dans le bon répertoire.
    Assurez-vous que tous les fichiers nécessaires (notamment build.gradle et gradlew) sont présents dans le dossier ProjetJava.
## Platforms

- `core`: Main module with the application logic shared by all platforms.
- `lwjgl3`: Primary desktop platform using LWJGL3; was called 'desktop' in older docs.

## Gradle

This project uses [Gradle](https://gradle.org/) to manage dependencies.
The Gradle wrapper was included, so you can run Gradle tasks using `gradlew.bat` or `./gradlew` commands.
Useful Gradle tasks and flags:

- `--continue`: when using this flag, errors will not stop the tasks from running.
- `--daemon`: thanks to this flag, Gradle daemon will be used to run chosen tasks.
- `--offline`: when using this flag, cached dependency archives will be used.
- `--refresh-dependencies`: this flag forces validation of all dependencies. Useful for snapshot versions.
- `build`: builds sources and archives of every project.
- `cleanEclipse`: removes Eclipse project data.
- `cleanIdea`: removes IntelliJ project data.
- `clean`: removes `build` folders, which store compiled classes and built archives.
- `eclipse`: generates Eclipse project data.
- `idea`: generates IntelliJ project data.
- `lwjgl3:jar`: builds application's runnable jar, which can be found at `lwjgl3/build/libs`.
- `lwjgl3:run`: starts the application.
- `test`: runs unit tests (if any).

Note that most tasks that are not specific to a single project can be run with `name:` prefix, where the `name` should be replaced with the ID of a specific project.
For example, `core:clean` removes `build` folder only from the `core` project.
