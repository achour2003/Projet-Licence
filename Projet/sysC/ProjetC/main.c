#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stdprof.h"
#define malloc(taille) stdprof_malloc_fonction(taille, __LINE__, __FILE__)
#define free(pointeur) stdprof_free(pointeur)
#define NbrM 5
#define Python 1
#define nbrE 5
#define nbrR 3
#include "Projet.h"
int main(int argc, char *argv[]){
    FILE *fichier = fopen(argv[1], "r");
    equipement *e=definitionE();
    monstre *m=definitionM();
    nourriture* n = definitionNourriture();
    Rochet *r=definitionRochet();
    int ligne=22;
    int colonne=22;
    char **matrice=allouerMatrice(ligne,colonne);
    Rahan* rahan = initialiserRahan(50, 50, 10,1 ,ligne/2+1,colonne/2);/*prcq je pointe sur la meme matrice donc je liberer*/
    etapes etapee[100];
    int i=0;
    int tableau[150];
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <fichier_d_entree>\n", argv[0]);
        return 1;
    }

    
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }
    
    inisialisation(matrice,ligne,colonne);   
    inisialisationTab(tableau,100);
    etapee[i].matrice=matrice;
    etapee[i].lignes=ligne;
    etapee[i].colonnes=colonne;
    remplirMatrice(etapee[i].matrice,ligne,colonne,r,n,e,m,rahan);
    afficherMatrice(matrice,ligne,colonne);
    afficherMatrice(etapee[i].matrice,ligne,colonne);
    jouer(etapee,rahan,&i,&ligne,&colonne,n,m,e,r,tableau,fichier);
    printf("etape %d",i);
    Liberation(rahan,e,etapee,n,r,m,i,tableau,fichier);
   
    printf("success");
    return 0;
}


    /*char reponse;
      while ((reponse = fgetc(fichier)) != EOF){
    printf("\n>>");
    if (reponse == '\n') {
        continue;  // Ignorer les sauts de ligne
    }else{
    i++;
    switch(reponse){
        case 'b':
        
          etapee[i].matrice=  deplacerBas(rahan,etapee[i-1].matrice,&ligne,&colonne,n,m,e,r);
            etapee[i].lignes=ligne;
            etapee[i].colonnes=colonne;
            break;
        case 'g':
               

            etapee[i].matrice=deplacerGauche(rahan,etapee[i-1].matrice,&ligne,&colonne,n,m,e,r); 
            etapee[i].lignes=ligne;
            etapee[i].colonnes=colonne;     
            break;
        case 'h':

            etapee[i].matrice=deplacerHaut(rahan,etapee[i-1].matrice,&ligne,&colonne,n,m,e,r);
            etapee[i].lignes=ligne;
            etapee[i].colonnes=colonne;
            break;
        case 'd':
            etapee[i].matrice=deplacerDroite(rahan,etapee[i-1].matrice,&ligne,&colonne,n,m,e,r);
            etapee[i].lignes=ligne;
            etapee[i].colonnes=colonne;
            break;
        case 'a':
            afficherMatrice(etapee[i-1].matrice,ligne,colonne);
            i--;
            break;;
        default:
            quiter(rahan,e,m,n,r);
            LibererTout(etapee,i);
            fclose(fichier);
            return 0;
    }
  }
   }  */