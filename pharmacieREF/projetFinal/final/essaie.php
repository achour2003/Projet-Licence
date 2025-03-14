
<?php
session_start();
// Connexion à la base de données
$conn = new mysqli("localhost", "root", "", "projetdaweb");

// Vérifier la connexion
if ($conn->connect_error) {
    die("Connexion échouée : " . $conn->connect_error);
}
$email=$_SESSION["email"];
// Requête SQL pour sélectionner tous les champs sauf le mot de passe
$sql = "SELECT * FROM user WHERE email='$email'";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    // Création d'un tableau pour stocker les données
    $data = array();

    // Parcourir les lignes de résultat
    while ($row = $result->fetch_assoc()) {
        // Supprimer le champ du mot de passe
        unset($row['mot_de_passe']);

        // Convertir le BLOB de la photo en une chaîne base64
        $photoData = base64_encode($row['photo']);
        // Ajouter les données au tableau
        $data[] = array(
            'nomU' => $row['nom'],
            'prenom' => $row['prenom'],
            'age' => $row['age'],
            'email' => $row['email'],
            'photo' => $photoData // Stocker la photo en base64
        );
    }

    // Retourner les données sous forme de réponse JSON
    header('Content-Type: application/json');
    echo json_encode($data);
} else {
    echo "Aucune donnée trouvée";
}

// Fermer la connexion à la base de données
$conn->close();
?>
