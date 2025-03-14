document.getElementById('searchButton').addEventListener('click', function() {
    var searchTerm = document.getElementById('searchTerm').value;
    fetch('rechercheMotCle.php?search=' + encodeURIComponent(searchTerm))
    .then(response => {
        if (!response.ok) {
            throw new Error('Network response was not ok');
        }
        return response.json(); 
    })
    .then(data => {
        const resultsContainer = document.getElementById('results');
        resultsContainer.innerHTML = ''; 

        if (data.message) {
            resultsContainer.innerHTML = data.message;
        } else {
            data.forEach(medicament => {
                const div = document.createElement('div');
                div.classList.add('medicament-info');
                div.innerHTML = `<h4>${medicament.nom_medicament}</h4>
                                <p>Description: ${medicament.description}</p>
                                <img src="${medicament.image}" alt="Image de médicament">
                                <p>Catégorie: ${medicament.categorie}</p>
                                <p>Pharmacie: ${medicament.pharmacie}</p>
                                <p>Prix: €${medicament.prix}</p>
                                <p>Quantité: ${medicament.quantite}</p>`;
                resultsContainer.appendChild(div);
            });
        }
    })
    .catch(error => {
        console.error('Erreur lors de la récupération des données:', error);
        document.getElementById('results').innerHTML = 'Erreur lors de la recherche. Veuillez réessayer plus tard.';
    });
});