package crypt.myPackage;

import java.io.*;

public class FileManager {
    private static final String SAVE_FILE = "save.txt";

    // Méthode pour lire le niveau sauvegardé
    public static int readLevelFromFile() {
        try (BufferedReader reader = new BufferedReader(new FileReader(SAVE_FILE))) {
            return Integer.parseInt(reader.readLine());
        } catch (IOException | NumberFormatException e) {
            // Retourne 0 si le fichier n'existe pas ou est corrompu
            return 0;
        }
    }

    // Méthode pour écrire le niveau dans le fichier
    public static void saveLevelToFile(int level) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(SAVE_FILE))) {
            writer.write(String.valueOf(level));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
