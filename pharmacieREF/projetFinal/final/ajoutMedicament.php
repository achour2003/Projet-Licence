<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Enregistrement de médicament</title>
    <link rel="stylesheet" href="CSS ajoutMedicament.css">
</head>
<body>
    <h2>Enregistrement de médicament</h2>
    <?php
// Vérifiez si le paramètre 'success' est défini dans l'URL et qu'il est égal à 1
if(isset($_GET['success']) && $_GET['success'] == 1) {
    echo '<p style="color: green;">Le médicament a été ajouté avec succès !</p>';
}
?>
<?php
// Récupérer le code d'erreur depuis l'URL
$error_code = isset($_GET['error']) ? $_GET['error'] : null;

// Afficher le message correspondant au code d'erreur
if ($error_code === "1") {
    $error_message = "Vous avez déjà enregistré ce médicament dans votre pharmacie.";
} elseif ($error_code === "2") {
    $error_message = "Erreur lors de l'ajout du médicament dans la table de liaison.";
} elseif ($error_code === "3") {
    $error_message = "Erreur lors de l'ajout du médicament dans la table de liaison.";
} elseif ($error_code === "4") {
    $error_message = "Erreur lors de l'ajout du médicament dans la table des médicaments.";
} else {
    $error_message = ""; // Aucun message par défaut
}
?>



        <?php if (!empty($error_message)) { ?>
            <p style="color: red;"><?php echo $error_message; ?></p>
        <?php } ?>
    <form action="ajoutMedicament1.php" method="post" enctype="multipart/form-data">
        <label for="nom_medicament">Nom du médicament:</label><br>
        <input type="text" id="nom_medicament" name="nom_medicament" required><br>
        <label for="description">Description:</label><br>
        <textarea id="description" name="description" required></textarea><br>
        <label for="categorie">categorie:</label><br>
        <input type="text" id="categorie" name="categorie" required><br>
        <label for="image">Image:</label><br>
        <input type="file" id="image" name="image"><br>
        <label for="prix">Prix:</label><br>
        <input type="number" id="prix" name="prix" required><br>
        <label for="quantite">Quantité:</label><br>
        <input type="number" id="quantite" name="quantite" required><br>
        <input type="submit" value="Enregistrer">
    </form>
    <a href="ac.php">revenir en arrière</a>
</body>
</html>
