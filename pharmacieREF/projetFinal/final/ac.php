<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>PharmaREF</title>
    <link rel="stylesheet" href="CSS ac.css">
    <link href="https://fonts.googleapis.com/css2?family=Roboto:wght@400;700&display=swap" rel="stylesheet">
    <link rel="stylesheet" href="CSS rechercheMotCle.css">
    <link rel="stylesheet" href="css ac2.css">

</head>
<body>
    <header>
        <div class="top-nav">
            <div class="logo">
                <img src="logo.png" alt="Logo PharmaREF" style="height: 40px;">
                <h1>PharmaREF</h1>
            </div>
            <div class="nav-links">
                <a href="ac.php">Accueil</a>
                <a href="pharmacien.html">Pharmacien</a>
                <a href="recherche.html">Médicaments</a>
                <a href="rechercheMotCle.html">Recherche avancé</a>
                <?php
                if (isset($_GET['login']) && $_GET['login'] == "success") {
                    echo '<li><a href="ajoutMedicament.php">Ajouter un médicament </a></li>';
                    echo '<li><a href="modifier_medicament.php">Modifier un médicament</a></li>';
                    echo '<li><a href="supprimer_medicament.php">Supprimer un médicament</a></li>';
                }
                ?>
                <a href="../Projet TECHNO DU WEB/Login/Connexion.html" id="loginButton">Connexion</a>
                </div>
        </div>
    </header>
    <section id="home">
        <h2>Bienvenue sur PharmaREF</h2>
        <p>Explorez les Pharmacies disponibles dans votre ville ainsi que les medicaments et partagez vos avis.</p>
       
    </section>
    
    <section id="afficherMedicaments">
    <div class="search-container">
        <h1>Recherche de médicaments</h1>
        <input type="text" id="searchTerm" placeholder="Entrez un mot-clé...">
        <button id="searchButton">Rechercher</button>
        <div id="results"></div>
        <div id="medicamentsList"></div>
    </div>
</section>
    
    <script src="scripts.js"></script>
</body>
</html>