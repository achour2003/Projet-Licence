
<?php
session_start();

// Vérifiez si le bouton de connexion a été cliqué
if (isset($_POST['valider'])) {
    $email = $_POST['email'];

    // Connectez-vous à la base de données
    $conn = new mysqli("localhost", "root", "", "projetdaweb");
    if ($conn->connect_error) {
        die("Connexion échouée : " . $conn->connect_error);
    }

    // Préparez la requête pour récupérer le mot de passe haché
    $stmt = $conn->prepare("SELECT mot_de_passe FROM user WHERE email = ?");
    $stmt->bind_param("s", $email);
    $stmt->execute();
    $result = $stmt->get_result();

    if ($row = $result->fetch_assoc()) {
        // Utilisez password_verify pour comparer le mot de passe soumis avec le hachage
        if (password_verify($_POST['password'], $row['mot_de_passe'])) {
            // Les identifiants sont corrects, ouvrez une session et redirigez vers le profil
            $_SESSION['email'] = $email;
            header("Location: profile.html"); // Redirigez vers le profil
            exit();
        } else {
            echo "Le mot de passe est incorrect.";
        }
    } else {
        echo "Aucun utilisateur trouvé avec cet e-mail.";
    }

    $stmt->close();
    $conn->close();
}
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Connexion</title>
    <link rel="stylesheet" href="CSS connexion.css">
</head>
<body>
    <h1>Connexion</h1>
    <form action="" method="post">
        <input type="email" name="email" placeholder="Adresse e-mail" required>
        <input type="password" name="password" placeholder="Mot de passe" required>
        <input type="submit" value="Se connecter" name="valider">
    </form>
</body>
</html>
