<?php

session_start();

// Vérifiez si l'utilisateur est déjà connecté, s'il l'est, redirigez-le vers la page d'accueil
if (!isset($_SESSION['email'])) {
    header("Location: index.php");
    exit();
}

// Récupérer l'email de la session
$email = $_SESSION['email'];

// Connexion à la base de données (à adapter avec vos propres informations de connexion)
$conn = new mysqli("localhost", "root", "", "projetdaweb");

// Vérifier la connexion
if ($conn->connect_error) {
    die("Connexion échouée : " . $conn->connect_error);
}

// Vérifiez si le formulaire de connexion a été soumis
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Récupérer les données soumises par le formulaire
    $mot_de_passe_saisi = $_POST['mot_de_passe'];

    // Requête SQL pour récupérer le mot de passe de l'utilisateur
    $sql = "SELECT mot_de_passe FROM user WHERE email = '$email'";
    $result = $conn->query($sql);

    if ($result->num_rows == 1) {
        // L'utilisateur existe, vérifier le mot de passe
        $row = $result->fetch_assoc();
        $mot_de_passe_bd = $row['mot_de_passe'];

        if (password_verify($mot_de_passe_saisi, $mot_de_passe_bd)) {
            // Mot de passe correct, vérifier si le nom_pharmacie et l'email sont dans la table medicament_pharmacie
            $nom_pharmacie = $_POST['nom_pharmacie'];

            $sql_check_pharmacie = "SELECT * FROM pharmacien WHERE email = '$email' AND nom_pharmacie = '$nom_pharmacie'";
            $result_check_pharmacie = $conn->query($sql_check_pharmacie);

            if ($result_check_pharmacie->num_rows == 1) {
                   // Mot de passe correct, stocker le nom de la pharmacie dans la session
            $row = $result_check_pharmacie->fetch_assoc();
            $_SESSION['nom_pharmacie'] = $row['nom_pharmacie'];
           
            //Rediriger l'utilisateur vers la page "ac.html" avec un paramètre pour indiquer la connexion réussie
            header("Location: ac.php?login=success");
            exit();
            } else {
                // Nom de pharmacie ou email non trouvé dans la table medicament_pharmacie
                $error_message = "Nom de pharmacie ou email incorrect.";
            }
        } else {
            // Mot de passe incorrect
            $error_message = "Mot de passe incorrect.";
        }
    } else {
        // L'utilisateur n'existe pas dans la base de données
        $error_message = "Utilisateur non trouvé.";
    }
}

// Fermer la connexion à la base de données
$conn->close();
?>

<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Connexion Pharmacien</title>
    <link rel="stylesheet" href="CSS connexionPharmacie.css">
</head>
<body>
    <header>
        <h1>Forum Pharmaceutique de Nice</h1>
    </header>
    <section>
        <h2>Connexion Pharmacien</h2>
        <?php if (isset($error_message)) { ?>
            <p style="color: red;"><?php echo $error_message; ?></p>
        <?php } ?>
        <form action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>" method="post">
            <label for="nom_pharmacie">Nom de Pharmacie:</label><br>
            <input type="text" id="nom_pharmacie" name="nom_pharmacie" required><br>
            <label for="mot_de_passe">Mot de Passe:</label><br>
            <input type="password" id="mot_de_passe" name="mot_de_passe" required><br>
            <input type="submit" value="Connexion">
        </form>
        <p>Vous n'avez pas de compte ? <a href="pharmacien.html">Inscrivez-vous ici</a>.</p>
    </section>
</body>
</html>