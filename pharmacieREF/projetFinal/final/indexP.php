<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Formulaire d'inscription</title>
    <link rel="stylesheet" href="CSS indexP.css">
    
</head>
<body>
    <div id="titre">
        <h1>Inscription</h1>
    </div>
    <form action="traitement2.php" method="post" enctype="multipart/form-data">
        <label for="nom">Nom :</label>
        <input type="text" id="nom" name="nom" required><br>

        <label for="prenom">Prénom :</label>
        <input type="text" id="prenom" name="prenom" required><br>

        <label for="age">Âge :</label>
        <input type="number" id="age" name="age" required><br>

        <label for="adresse">Adresse :</label>
        <input type="text" id="adresse" name="adresse" required><br>

        <label for="email">E-mail :</label>
        <input type="email" id="email" name="email" required><br>

        <label for="motdepasse">Mot de passe :</label>
        <input type="password" id="motdepasse" name="motdepasse" required><br>
        
        <label for="photo">Photo de profil :</label>
        <input type="file" name="photo">

        <input type="submit" value="S'inscrire">
    </form>

    <?php
    // Vérification de la présence du paramètre erreur dans l'URL
    if (isset($_GET['erreur']) && $_GET['erreur'] == 1) {
        echo "<p style='color:red'>Cet email est déjà utilisé. Veuillez en choisir un autre.</p>";
    }
    ?>

</body>
</html>
