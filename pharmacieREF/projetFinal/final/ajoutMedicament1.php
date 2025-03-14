
<?php
/*session_start();
// Connexion à la base de données (à adapter avec vos propres informations de connexion)
$conn = new mysqli("localhost", "root", "", "projetdaweb");

// Vérifier la connexion
if ($conn->connect_error) {
    die("Connexion échouée : " . $conn->connect_error);
}


// Récupération des données soumises par le formulaire
$nom_medicament = $_POST['nom_medicament'];
$description = $_POST['description'];
$image = $_FILES['image']['tmp_name']; // À traiter pour l'enregistrement de l'image dans la base de données
$pharmacie_id=$_SESSION['nom_pharmacie']
// Requête SQL pour vérifier si le médicament existe déjà
$sql = "SELECT id_medicament FROM medicaments WHERE nom_medicament = '$nom_medicament'";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    // Le médicament existe déjà, récupérer son ID
    $row = $result->fetch_assoc();
    $id_medicament = $row['id_medicament'];
    echo "Le médicament existe déjà dans la base de données. Son ID est : $id_medicament";
} else {
    // Le médicament n'existe pas, l'insérer dans la base de données
    $sql_insert = "INSERT INTO medicaments (nom_medicament, description, image) VALUES ('$nom_medicament', '$description', '$image')";
    if ($conn->query($sql_insert) === TRUE) {
        echo "Le médicament a été ajouté avec succès.";
    } else {
        echo "Erreur lors de l'ajout du médicament : " . $conn->error;
    }
}

// Fermeture de la connexion à la base de données
$conn->close();*/
?>

<?php/*
session_start();
if(!isset($_SESSION['nom_pharmacie'])) {
    header("Location: indexP.php");
    exit();
}
// Connexion à la base de données (à adapter avec vos propres informations de connexion)
$conn = new mysqli("localhost", "root", "", "projetdaweb");

// Vérifier la connexion
if ($conn->connect_error) {
    die("Connexion échouée : " . $conn->connect_error);
}


// Récupération des données soumises par le formulaire
$nom_medicament = $_POST['nom_medicament'];
$description = $_POST['description'];
$image = $_FILES['image']['tmp_name']; // À traiter pour l'enregistrement de l'image dans la base de données
$prix = $_POST['prix'];
$quantite = $_POST['quantite'];
$categorie=$_POST['categorie'];

// Récupération de l'ID de la pharmacie depuis la session (à adapter selon votre système de gestion des sessions)

$pharmacie_id = $_SESSION['nom_pharmacie'];


// Requête SQL pour vérifier si le médicament existe déjà dans la base de données
$sql_check_medicament = "SELECT id_medicament FROM medicament WHERE nom_medicament = '$nom_medicament'";
$result_check_medicament = $conn->query($sql_check_medicament);

if ($result_check_medicament->num_rows > 0) {
    // Le médicament existe déjà dans la base de données
    // Vérifier maintenant s'il a déjà été enregistré par le pharmacien actuel
    $row = $result_check_medicament->fetch_assoc();
    $id_medicament = $row['id_medicament'];
    
    $sql_check_liaison = "SELECT id_medicament FROM medicament_pharmacie WHERE id_medicament = '$id_medicament' AND pharmacie = '$pharmacie_id'";
    $result_check_liaison = $conn->query($sql_check_liaison);
    
    if ($result_check_liaison->num_rows > 0) {
        // Le médicament a déjà été enregistré par le pharmacien actuel
        header("Location: ajoutMedicament.php?error=1");

    } else {
        // Le médicament existe mais n'a pas encore été enregistré par ce pharmacien, l'ajouter dans la table de liaison
        $sql_insert_liaison = "INSERT INTO medicament_pharmacie (id_medicament, pharmacie, prix, quantite) VALUES ('$id_medicament', '$pharmacie_id', '$prix', '$quantite')";
        if ($conn->query($sql_insert_liaison) === TRUE) {
            header("Location: ajoutMedicament.php?success=1");
            exit();
        } else {
            header("Location: ajoutMedicament.php?error=3");
                }
    }
} else {
    // Le médicament n'existe pas encore dans la base de données, l'ajouter dans les deux tables (medicaments et liaison_medicament_pharmacie)
    $sql_insert_medicament = "INSERT INTO medicament (nom_medicament, description, image, categorie) VALUES ('$nom_medicament', '$description', '$image','$categorie')";
    if ($conn->query($sql_insert_medicament) === TRUE) {
        // Récupération de l'ID du médicament inséré
        $id_medicament = $conn->insert_id;
        
        // Ajouter le médicament dans la table de liaison
        $sql_insert_liaison = "INSERT INTO medicament_pharmacie (id_medicament, pharmacie, prix, quantite) VALUES ('$id_medicament', '$pharmacie_id', '$prix', '$quantite')";
        if ($conn->query($sql_insert_liaison) === TRUE) {
            header("Location: ajoutMedicament.php?success=1");
            exit();
        } else {
            header("Location: ajoutMedicament.php?error=3");
        }
    } else {
        header("Location: ajoutMedicament.php?error=4");
    }
}

// Fermeture de la connexion à la base de données
$conn->close();*/
?>
<?php
session_start();

// Redirection si l'utilisateur n'est pas connecté
if (!isset($_SESSION['nom_pharmacie'])) {
    header("Location: indexP.php");
    exit();
}

// Connexion à la base de données
$conn = new mysqli("localhost", "root", "", "projetdaweb");
if ($conn->connect_error) {
    die("Connexion échouée : " . $conn->connect_error);
}

// Traitement du formulaire si la méthode POST est utilisée
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $nom_medicament = $_POST['nom_medicament'];
    $description = $_POST['description'];
    $image = $_FILES['image']['tmp_name'];
    $prix = $_POST['prix'];
    $quantite = $_POST['quantite'];
    $categorie = $_POST['categorie'];
    $pharmacie_id = $_SESSION['nom_pharmacie'];

    // Préparation de la requête pour vérifier l'existence du médicament
    $stmt_check = $conn->prepare("SELECT id_medicament FROM medicament WHERE nom_medicament = ?");
    $stmt_check->bind_param("s", $nom_medicament);
    $stmt_check->execute();
    $result_check = $stmt_check->get_result();

    if ($result_check->num_rows > 0) {
        $row = $result_check->fetch_assoc();
        $id_medicament = $row['id_medicament'];

        // Vérification si le médicament est déjà enregistré sous cette pharmacie
        $stmt_liaison_check = $conn->prepare("SELECT id_medicament FROM medicament_pharmacie WHERE id_medicament = ? AND pharmacie = ?");
        $stmt_liaison_check->bind_param("is", $id_medicament, $pharmacie_id);
        $stmt_liaison_check->execute();
        $result_liaison_check = $stmt_liaison_check->get_result();

        if ($result_liaison_check->num_rows > 0) {
            header("Location: ajoutMedicament.php?error=1");
            exit();
        } else {
            $stmt_insert_liaison = $conn->prepare("INSERT INTO medicament_pharmacie (id_medicament, pharmacie, prix, quantite) VALUES (?, ?, ?, ?)");
            $stmt_insert_liaison->bind_param("isii", $id_medicament, $pharmacie_id, $prix, $quantite);
            if ($stmt_insert_liaison->execute()) {
                header("Location: ajoutMedicament.php?success=1");
                exit();
            } else {
                header("Location: ajoutMedicament.php?error=3");
            }
        }
    } else {
        if (is_uploaded_file($image)) {
            $imageContent = file_get_contents($image);
        } else {
            $imageContent = null; // Handle the case where no image is provided
        }
        $stmt_insert = $conn->prepare("INSERT INTO medicament (nom_medicament, description, image, categorie) VALUES (?, ?, ?, ?)");
        $null = NULL;
        $stmt_insert->bind_param("ssbs", $nom_medicament, $description, $null, $categorie);
        $stmt_insert->send_long_data(2, $imageContent);

        if ($stmt_insert->execute()) {
            $id_medicament = $conn->insert_id;
            $stmt_insert_liaison = $conn->prepare("INSERT INTO medicament_pharmacie (id_medicament, pharmacie, prix, quantite) VALUES (?, ?, ?, ?)");
            $stmt_insert_liaison->bind_param("isii", $id_medicament, $pharmacie_id, $prix, $quantite);
            if ($stmt_insert_liaison->execute()) {
                header("Location: ajoutMedicament.php?success=1");
                exit();
            } else {
                header("Location: ajoutMedicament.php?error=3");
            }
        } else {
            header("Location: ajoutMedicament.php?error=4");
        }
    }
    $stmt_check->close();
    $stmt_liaison_check->close();
    $stmt_insert->close();
    $stmt_insert_liaison->close();
    $conn->close();
}
?>
