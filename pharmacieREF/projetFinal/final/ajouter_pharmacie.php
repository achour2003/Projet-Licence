<?php
session_start();

// Connexion à la base de données
$conn = new mysqli("localhost", "root", "", "projetdaweb");

// Vérifier la connexion
if ($conn->connect_error) {
    die("Connexion échouée : " . $conn->connect_error);
}

// Récupérer les données du formulaire
$nom = $_POST['nom'];
$adresse = $_POST['adresse'];
$ouverture = $_POST['ouverture'];
$fermeture = $_POST['fermeture'];
$jours = $_POST['jours'];
$description = $_POST['description'];
$ville = $_POST['ville'];

// Requête SQL pour insérer la pharmacie dans la base de données
$sql = "INSERT INTO pharmacie (nom_pharmacie, adresse, heures_ouverture, heures_fermeture, jour_travail, description,ville)
        VALUES ('$nom', '$adresse', '$ouverture', '$fermeture', '$jours', '$description', '$ville')";

if ($conn->query($sql) === TRUE) {
    // Stocker l'ID de la pharmacie dans la session
    $_SESSION['nom_pharmacie'] = $nom;
    $email=$_SESSION["email"];
    $req="insert into pharmacien (email,nom_pharmacie) values('$email','$nom')";
    if($conn->query($req) === TRUE){
        // Rediriger vers la page d'ajout de médicaments
        header("Location: ac.php?login=success");
        exit();
    }

    echo "erreur dans l'insertion du pharmacien ";
} else {
    echo "Erreur lors de l'ajout de la pharmacie : " . $conn->error;
}
// Fermer la connexion à la base de données
$conn->close();
?>
