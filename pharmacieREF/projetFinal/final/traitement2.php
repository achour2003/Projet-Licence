<?php/*
    // Connexion à la base de données (remplacez les valeurs par celles de votre propre base de données)
    $serveur = "localhost";
    $utilisateur = "root";
    $motdepasse = "";
    $base_de_donnees = "projetdaweb";

    $connexion = new mysqli($serveur, $utilisateur, $motdepasse, $base_de_donnees); 

    // Vérification de la connexion
    if ($connexion->connect_error) {
        
        die("Échec de la connexion : " . $connexion->connect_error);
    }

    $nom = $_POST['nom'];
    $prenom = $_POST['prenom'];
    $age = $_POST['age'];
    $adresse = $_POST['adresse'];
    $email = $_POST['email'];
    $motdepasse = password_hash($_POST['motdepasse'], PASSWORD_DEFAULT);

    $sql_verif_email = "SELECT * FROM user WHERE email = '$email'";
    $resultat_verif_email = $connexion->query($sql_verif_email);
    if ($resultat_verif_email->num_rows > 0) {
        // L'email existe déjà, afficher un message d'erreur
        // Redirection vers la page d'inscription avec un message d'erreur
        header("Location: indexP.php?erreur=1");
        exit();
         
    } else {
       // $photo_nom = $_FILES['photo']['name'];
       // $photo_tmp = $_FILES['photo']['tmp_name'];
       // $photo_taille = $_FILES['photo']['size'];
        
        // Vérifier si une image a été téléchargée
        $image = $_FILES['photo']['tmp_name'];
        $imgContent = addslashes(file_get_contents($image));
    
        // Insérer l'image dans la base de données
        
        
        
        if ($image) {
            // Lire le contenu de l'image
            $photo_contenu = file_get_contents($photo_tmp);
    
            // Requête d'insertion dans la table avec l'image
            $sql = "INSERT INTO user (nom, prenom, age, adresse, email, mot_de_passe, photo) VALUES (?, ?, ?, ?, ?, ?, ?)";
    
            // Préparation de la requête avec des paramètres pour l'image et les autres informations
            $stmt = $connexion->prepare($sql);
            $stmt->bind_param("ssisssb", $nom, $prenom, $age, $adresse, $email, $motdepasse, $imgContent); // 'b' indique que le paramètre est un blob
            //$insert = $mysqli->query("INSERT into images (image_data) VALUES ('$imgContent')");
            // Exécution de la requête

            if ($stmt->execute()) {
                // Après une inscription réussie, redirigez l'utilisateur vers la page de profil
                header("Location: profile.html");
                exit();
            } else {
                echo "Erreur lors de l'inscription : " . $connexion->error;
            }
        } else {
            echo "Veuillez télécharger une photo de profil.";
        }
   



        // Fermeture de la connexion
        $connexion->close();
    }
     
*/?>
<?php
session_start();
// Connexion à la base de données (remplacez les valeurs par celles de votre propre base de données)
$serveur = "localhost";
$utilisateur = "root";
$motdepasse = "";
$base_de_donnees = "projetdaweb";

$connexion = new mysqli($serveur, $utilisateur, $motdepasse, $base_de_donnees); 

// Vérification de la connexion
if ($connexion->connect_error) {
    die("Échec de la connexion : " . $connexion->connect_error);
}

$nom = $_POST['nom'];
$prenom = $_POST['prenom'];
$age = $_POST['age'];
$adresse = $_POST['adresse'];
$email = $_POST['email'];
$motdepasse = password_hash($_POST['motdepasse'], PASSWORD_DEFAULT);

$sql_verif_email = "SELECT * FROM user WHERE email = '$email'";
$resultat_verif_email = $connexion->query($sql_verif_email);
if ($resultat_verif_email->num_rows > 0) {
    // L'email existe déjà, afficher un message d'erreur
    // Redirection vers la page d'inscription avec un message d'erreur
    header("Location: indexP.php?erreur=1");
    exit();
} else {
    // Vérifier si une image a été téléchargée
    $image = $_FILES['photo']['tmp_name'];
    $imgContent = addslashes(file_get_contents($image));

    if ($image) {
        // Requête d'insertion dans la table avec l'image
        $sql = "INSERT INTO user (nom, prenom, age, adresse, email, mot_de_passe, photo) VALUES ('$nom', '$prenom', $age, '$adresse', '$email', '$motdepasse', '$imgContent')";
        
        // Exécution de la requête
        if ($connexion->query($sql) === TRUE) {
            // Après une inscription réussie, redirigez l'utilisateur vers la page de profil
            $_SESSION['email']=$email;
            header("Location: profile.html");
            exit();
        } else {
            echo "Erreur lors de l'inscription : " . $connexion->error;
        }
    } else {
        echo "Veuillez télécharger une photo de profil.";
    }
}

$connexion->close();
?>


