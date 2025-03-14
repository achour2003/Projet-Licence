<?php
session_start();
header('Content-Type: application/json'); // Déplacer le header en haut pour éviter les problèmes de sortie avant l'envoi du JSON

$conn = new mysqli("localhost", "root", "", "projetdaweb");
if ($conn->connect_error) {
    error_log("Connection failed: " . $conn->connect_error); // Utilisation de error_log pour les erreurs de connexion
    die(json_encode(['error' => 'Connection failed'])); // Envoyer une réponse JSON en cas d'erreur de connexion
}

// Récupération du terme de recherche
$search_term = isset($_GET['search']) ? $_GET['search'] : '';

$sql = "SELECT m.id_medicament, m.nom_medicament, m.description, m.categorie, mp.pharmacie, mp.prix, mp.quantite, m.image
        FROM medicament m
        JOIN medicament_pharmacie mp ON m.id_medicament = mp.id_medicament
        WHERE m.nom_medicament LIKE ? OR m.description LIKE ?";

$stmt = $conn->prepare($sql);
if (!$stmt) {
    error_log("Erreur de préparation de la requête: " . $conn->error); // Log de l'erreur de préparation
    die(json_encode(['error' => 'Error preparing statement'])); // Réponse JSON pour une erreur de préparation
}

$search_term = '%' . $search_term . '%';
$stmt->bind_param("ss", $search_term, $search_term);
$stmt->execute();
$result = $stmt->get_result();

$medicaments = [];

if ($result->num_rows > 0) {
    while ($row = $result->fetch_assoc()) {
        if (!empty($row['image'])) {
            $imageData = base64_encode($row['image']);
            $row['image'] = 'data:image/jpeg;base64,' . $imageData;
        } else {
            $defaultImageData = base64_encode(file_get_contents('rename.jpeg'));
            $row['image'] = 'data:image/jpeg;base64,' . $defaultImageData;
        }
        $medicaments[] = $row;
    }
    echo json_encode($medicaments);
} else {
    echo json_encode(['message' => 'Aucun résultat']);
}
$conn->close();
?>
