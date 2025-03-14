
<?php
session_start();

$conn = new mysqli("localhost", "root", "", "projetdaweb");
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql = "SELECT m.id_medicament, m.nom_medicament, m.description, m.categorie, mp.pharmacie, mp.prix, mp.quantite, m.image
        FROM medicament m
        JOIN medicament_pharmacie mp ON m.id_medicament = mp.id_medicament";

$result = $conn->query($sql);
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
    echo "Aucun résultat";
}

$conn->close();
?>
