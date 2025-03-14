#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Projet.h"
#include<time.h>
#include"stdprof.h"
#define NbrM 7
#define Python 1
#define nbrE 3
#define nbrTe 3
#define nbrTn 2
#define nbrn 5
#define nbrTm 2
#define nbrTr 1
#define nbrR 7
#define malloc(taille) stdprof_malloc_fonction(taille, __LINE__, __FILE__)
#define free(pointeur) stdprof_free(pointeur)
int i=0;

int genererNombreAleatoire(int max) {

    return rand() % max;
}
monstre* definitionM() {
    int j;
    monstre *t = stdprof_malloc_fonction(2 * sizeof(monstre), __LINE__, __FILE__);
    /*il y a un worning ici et je ne trouve pas ou est le probleme */
    snprintf(t[0].nom, sizeof(t[0].nom), "gorack");
    t[0].sante = 10;
    t[0].pointForce = 3;
    
    for (j = 0; j < NbrM; j++) {
        t[0].positionM[j].X = genererNombreAleatoire(20);
        t[0].positionM[j].Y = genererNombreAleatoire(20);
    }

    snprintf(t[1].nom, sizeof(t[1].nom), "python");
    t[1].sante = 100;
    t[1].pointForce = 10;

    for ( j = 0; j < Python; j++) {
        t[1].positionM[j].X = genererNombreAleatoire(20);
        t[1].positionM[j].Y = genererNombreAleatoire(20);
    }
    return t;
}

equipement* definitionE()
{       

    /* Utiliser stdprof_malloc_fonction pour allouer de la mémoire pour trois équipements*/
    equipement* t = (equipement*)stdprof_malloc_fonction(nbrTe * sizeof(equipement), __LINE__, __FILE__);
    int i ;
    int j;
    /* Initialiser les propriétés des équipements*/
    snprintf(t[0].description, sizeof(t[0].description), "bottes en peau de crocro");
    snprintf(t[0].stats, sizeof(t[0].stats), "V+10");
    snprintf(t[0].nom, sizeof(t[0].nom), "chaussures");

    snprintf(t[1].description, sizeof(t[1].description), "slip en peau de mamouth");
    snprintf(t[1].stats, sizeof(t[1].stats), "P+10");
    snprintf(t[1].nom, sizeof(t[1].nom), "protections");

    snprintf(t[2].description, sizeof(t[2].description), "lance en bois");
    snprintf(t[2].stats, sizeof(t[2].stats), "F+5");
    snprintf(t[2].nom, sizeof(t[2].nom), "armes");

    /* Initialiser les positions des équipements*/
    for( i = 0; i < nbrTe; i++){
        for( j = 0; j < nbrE; j++) {
            t[i].positionE[j].X = genererNombreAleatoire(20);  
            t[i].positionE[j].Y = genererNombreAleatoire(20);
        }
    }

    return t;
}

nourriture* definitionNourriture() {
    nourriture* n = stdprof_malloc_fonction(2 * sizeof(nourriture), __LINE__, __FILE__);
    int j ;
    int i;
    snprintf(n[0].nom, sizeof(n[0].nom), "pomme");
    n[0].santePlus = 3;
    snprintf(n[0].description, sizeof(n[0].description), "Fruit joli mais sans grand intérêt");

    snprintf(n[1].nom, sizeof(n[1].nom), "gnou");
    n[1].santePlus = 25;
    snprintf(n[1].description, sizeof(n[1].description), "Animal élégant et raffiné, symbole de liberté");

    for ( j = 0; j < nbrTn; j++) {
        for ( i = 0; i < nbrn; i++) {
            n[j].positionN[i].X = genererNombreAleatoire(20);
            n[j].positionN[i].Y = genererNombreAleatoire(20);
        }
    }
    return n;
}

void remplirMatrice(char** matrice, int lignes, int colonnes, Rochet* rochets,
 nourriture* nourritures,  equipement* equipements, 
  monstre* monstres, Rahan *rahan) {
    int i;
    int j;
    int x;
    int y;
    int Xr;
    int Xy;
    /* Remplir la matrice avec des rochets*/
    for ( i = 0; i < nbrTr; i++) {
        for ( j = 0; j < nbrR; j++) {
             x = rochets[i].positionR[j].X;
             y = rochets[i].positionR[j].Y;

            if (x > 0 && x < lignes-1 && y > 0 && y < colonnes-1) {
                matrice[x][y] = '#';
            }
        }
    }
    

    /* Remplir la matrice avec des nourritures*/
    for ( i = 0; i < nbrTn; i++) {
        for ( j = 0; j < nbrn; j++) {
             x = nourritures[i].positionN[j].X;
             y = nourritures[i].positionN[j].Y;

            if (x > 0 && x < lignes-1 && y > 0 && y < colonnes-1) {
                matrice[x][y] = '*';
            }
        }
    }

    /* Remplir la matrice avec des équipements*/
    for ( i = 0; i < nbrTe; i++) {
        for ( j = 0; j < nbrE; j++) {
             x = equipements[i].positionE[j].X;
             y = equipements[i].positionE[j].Y;

            if (x > 0 && x < lignes-1 && y > 0 && y < colonnes-1) {
                matrice[x][y] = '!';
            }
        }
    }

    /* Remplir la matrice avec des monstres*/
    for ( i = 0; i < nbrTm-1; i++) {
        for ( j = 0; j <NbrM; j++) {
             x = monstres[i].positionM[j].X;
             y = monstres[i].positionM[j].Y;

            if (x > 0 && x < lignes-1 && y > 0 && y < colonnes-1) {
                matrice[x][y] = '@';
            }
        }
    }
     x = monstres[1].positionM[0].X;
     y = monstres[1].positionM[0].Y;
    if (x > 0 && x < lignes-1 && y > 0 && y < colonnes-1) {
                matrice[x][y] = '@';
    }
    matrice[lignes/2][colonnes/2]='Y';
     Xr=rahan->positionR.X;
     Xy=rahan->positionR.Y;
    matrice[Xr][Xy]='R';
    
}
 

void afficherEquipements(equipement* t, int taille) {
    int i;
    int j ;
    for ( i = 0; i < taille; i++) {
        printf("Equipement %d:\n", i + 1);
        printf("Description: %s\n", t[i].description);
        printf("Stats: %s\n", t[i].stats);
        printf("Nom: %s\n", t[i].nom);

        printf("Positions:\n");
        for ( j = 0; j < nbrE; j++) {
            printf("Position %d: X = %d, Y = %d\n", j + 1, t[i].positionE[j].X, t[i].positionE[j].Y);
        }

        printf("\n");
    }
}
/*
Rochet* definitionRochet() {
    Rochet* r = malloc(sizeof(Rochet));

    snprintf(r->nom, sizeof(r->nom), "Rocher");

    for (int i = 0; i < nbrR; i++) {
        r->positionR[i].X = genererNombreAleatoire(20);
        r->positionR[i].Y = genererNombreAleatoire(20);
    }

    return r;
}*/
Rochet* definitionRochet() {
    Rochet* r = stdprof_malloc_fonction(sizeof(Rochet), __LINE__, __FILE__);
    int i;
    snprintf(r->nom, sizeof(r->nom), "Rocher");

    for ( i = 0; i < nbrR; i++) {
        r->positionR[i].X = genererNombreAleatoire(20);
        r->positionR[i].Y = genererNombreAleatoire(20);
    }

    return r;
}
void afficherRochet(Rochet* r) {
    int i;
    printf("Rochet:\n");
    printf("Nom: %s\n", r->nom);

    printf("Positions:\n");
    for ( i = 0; i < nbrR; i++) {
        printf("Position %d: X = %d, Y = %d\n", i + 1, r->positionR[i].X, r->positionR[i].Y);
    }

    printf("\n");
} 
 


char** agrandirMatrice(char** matrice, int lignes_actuelles, int colonnes_actuelles,
                        int nouvelles_lignes, int nouvelles_colonnes ,char d) {
    char** nouvelleMatrice = allouerMatrice(nouvelles_lignes, nouvelles_colonnes); 
    int j,i;   
    if(d=='H'){
        for( j=0;j<nouvelles_colonnes;j++)
            nouvelleMatrice[0][j]=matrice[0][j];
        
        for ( i = 0; i < nouvelles_lignes-1; i++) {
            for (j = 0; j < nouvelles_colonnes; j++) {
                if(i<lignes_actuelles && j<colonnes_actuelles &&( i!=0 )){
                    nouvelleMatrice[i+1][j] = matrice[i][j];
                }else if(j==0 || j==nouvelles_colonnes-1){
                    nouvelleMatrice[i+1][j]='|';
                }else{
                    nouvelleMatrice[i+1][j]=' ';
                }
            }
        }
    }if(d=='d'){
       for( i=0;i<nouvelles_lignes;i++)
            nouvelleMatrice[i][nouvelles_colonnes-1]=matrice[i][colonnes_actuelles-1];

        for ( i = 0; i < nouvelles_lignes; i++) {
            for ( j = 0; j < nouvelles_colonnes-1; j++) {
                    if((j!=(colonnes_actuelles-1)||(j==colonnes_actuelles-1 && (i==0 || i== lignes_actuelles-1)))){
                        nouvelleMatrice[i][j] = matrice[i][j];
                    }else{
                        nouvelleMatrice[i][j]=' ';
                    }
            }
        }
    }
    if(d=='b'){
        for( j=0;j<nouvelles_colonnes;j++){
            nouvelleMatrice[nouvelles_lignes-1][j]=matrice[lignes_actuelles-1][j];
        }

        for ( i = 0; i < nouvelles_lignes-1; i++) {
            for ( j = 0; j < nouvelles_colonnes; j++) {
                    if(i!=nouvelles_lignes-2){
                        nouvelleMatrice[i][j] = matrice[i][j];
                    }else if(j==0 || j== nouvelles_colonnes-1){
                        nouvelleMatrice[i][j]='|';
                    }else{
                        nouvelleMatrice[i][j]=' ';
                    }
            }
        }


    }
    if(d=='g'){
        for( i=0;i<nouvelles_lignes; i++){

            nouvelleMatrice[i][0]=matrice[i][0];
        }
            for ( i = 0; i < nouvelles_lignes; i++) {
            for ( j = 0; j < colonnes_actuelles; j++) {
                    if((i!=0||i!=lignes_actuelles-1 )&&(((j==0)&& (i==0 || i== nouvelles_lignes-1) )||(j!=0 ))){
                        nouvelleMatrice[i][j+1] = matrice[i][j];
                    }else{
                        nouvelleMatrice[i][j+1]=' ';
                    }
            }
        }


    }
    /*libererMatrice(encienne,lignes_actuelles);*/
    

    return nouvelleMatrice;
}



char** allouerMatrice(int lignes, int colonnes) {
    char** matrice = (char**)stdprof_malloc_fonction(lignes * sizeof(char*), __LINE__, __FILE__);
    int i;
    for ( i = 0; i < lignes; i++) {
        matrice[i] = (char*)stdprof_malloc_fonction(colonnes * sizeof(char), __LINE__, __FILE__);
    }

    return matrice;
}


void afficherMatrice(char** matrice, int lignes, int colonnes) {
   
    int j;
    int i;
    printf("   ");
    for ( j = 0; j < colonnes ; j++) {
        if (j < 9) {
            printf("%d", j + 1);
        } else {
            printf(" %d", j + 1);
        }
    }
    printf("\n");

    for ( i = 0; i < lignes; i++) {
        if (i > 0 && i < lignes - 1) {
            if (i < 9) {
                printf(" %d ", i + 1);
            } else {
                printf("%d ", i + 1);
            }
        } else {
            printf("   ");
        }

        for ( j = 0; j < colonnes; j++) {
            printf("%c ", matrice[i][j]);
        }
        printf("\n");
    }
}

 
void inisialisation(char **matrice,int lignes_actuelles,int colonnes_actuelles){
    int i,j;
    for ( i = 0; i < lignes_actuelles; i++) {
        for ( j = 0; j < colonnes_actuelles; j++) {
            if ((j == 0 || j == colonnes_actuelles-1) && (i!=0 && i!=lignes_actuelles-1)) {
                matrice[i][j] = '|';
            } else if ((i == lignes_actuelles-1 || i == 0) && j % 2) {
                matrice[i][j] = '-';
            } else if ((i == lignes_actuelles-1 || i == 0) && (j % 2 == 0)) {
                matrice[i][j] = '+';
            } else {
                matrice[i][j] = ' ';
            }
        }
    }
} 
 
 
Rahan* initialiserRahan(int pointVie, int sante, int force, int vitesse, int X, int Y) {
    Rahan* rahan = (Rahan*)stdprof_malloc_fonction(sizeof(Rahan), __LINE__, __FILE__);
    rahan->pointVie = pointVie;
    rahan->sante = sante;
    rahan->force = force;
    rahan->vitesse = vitesse;
    /* insperer du tri par comptage si je n'ai aucun equipement dans 0 et 1 sinon et chaque case du tableau corespend a une classe d'equipemment.*/  
    rahan->Equipement[0]=0;
    rahan->Equipement[1]=0;
    rahan->Equipement[2]=0;
    rahan->positionR.X = X;
    rahan->positionR.Y = Y;

    return rahan;
} 
 
 
void afficherMonstres(monstre *t, int taille) {
    int i,j;
    for (i = 0; i < taille; i++) {
        printf("Monstre %d:\n", i + 1);
        printf("Nom: %s\n", t[i].nom);
        printf("Sante: %d\n", t[i].sante);
        printf("Point de force: %d\n", t[i].pointForce);

        printf("Positions:\n");
        for ( j = 0; j < 5; j++) {
            printf("Position %d: X = %d, Y = %d\n", j + 1, t[0].positionM[j].X, t[0].positionM[j].Y);
        }
        printf("Position %d: X = %d, Y = %d\n", 0 + 1, t[1].positionM[0].X, t[1].positionM[0].Y);

        printf("\n");
    }
}
void afficherNourritures(nourriture* n, int taille) {
    int i,j;
    for ( i = 0; i < taille; i++) {
        printf("Nourriture %d:\n", i + 1);
        printf("Nom: %s\n", n[i].nom);
        printf("Santé Plus: %d\n", n[i].santePlus);
        printf("Description: %s\n", n[i].description);

        printf("Positions:\n");
        for ( j = 0; j < 3; j++) {
            printf("Position %d: X = %d, Y = %d\n", j + 1, n[i].positionN[j].X, n[i].positionN[j].Y);
        }

        printf("\n");
    }
}
void libererRahan(Rahan* rahan) {
    stdprof_free(rahan);
}

char ** deplacerHaut(Rahan* rahan,char **matrice,int *ligne, int *colonne,nourriture *n,
monstre *m,equipement* e, Rochet *r,int *tableau,int index,etapes *etape,FILE *file)
{   
    int xR = rahan->positionR.X;
    int yR = rahan->positionR.Y;
    int q=xR-1;
    if(q==0){  
        int Nligne=*ligne+1;
        char **matrice2= agrandirMatrice(matrice,*ligne,*colonne,Nligne,*colonne,'H');
        modifierCordR(rahan,xR+1,yR);
        modifierCordEMN('g',n,m,e,r);
        *ligne=*ligne+1;
                tableau[index]=1;

           
        return matrice2;
    }else if(matrice[q][yR]=='#' || matrice[q][yR]=='Y' ){
      
        return matrice;

    }else if(matrice[q][yR]=='*'){
        
        Soigner(rahan,q,yR,n);
        matrice[q+1][yR] =' ';
        matrice[q][yR] ='R';
        modifierCordR(rahan,q,yR);
        return matrice;


    }else if(matrice[q][yR]=='!'){
        
        equiper(rahan,q,yR,e);
        matrice[q+1][yR] =' ';
        matrice[q][yR] ='R';
        modifierCordR(rahan,q,yR);
        return matrice;

    }else if(matrice[q][yR] == ' '){

        matrice[q+1][yR] =' ';
        matrice[q][yR] ='R';
         modifierCordR(rahan,q,yR);
        return matrice;

    }else if(matrice[q][yR] == '@'){
        if(combatre(rahan,m,q,yR)){
            Liberation(rahan,e,etape,n,r,m,index,tableau,file);
        }else{
            matrice[q+1][yR] =' ';
            matrice[q][yR] ='R';
            modifierCordR(rahan,q,yR);
            return matrice;
        }
    }
    return matrice;


}

int combatre(Rahan *rahan,monstre *m,int x,int y){
    int i;
    for( i=0;i<NbrM;i++){
        if(m[0].positionM[i].X ==x && m[0].positionM[i].Y ==y){
            if(combat(rahan,m[0].sante,m[0].pointForce,m[0].nom)){
                printf("on a perdu");
                return 1;
            }else{
                m[0].positionM[i].X =-1; /* pour que je ne reviendrer pas a ce dernier;*/
                printf("on a gagner ");
                return 0;
            }
        }
    }
    printf("on l'a pas trouver dans le priier ");
    /* si il tombe sur le Python */
    for( i=0;i<Python;i++){
        if(m[1].positionM[i].X ==x && m[i].positionM[i].Y ==y){
            if(combat(rahan,m[1].sante,m[1].pointForce,m[1].nom)){
                m[1].positionM[i].X =-1; /* pour que je ne reviendrer pas a ce dernier;*/
                printf("on a gagner ");
                return 0;
            }else{
                printf("on a perdu");

                return 1;
            }
        }
    }
    printf("on l'a pas trouver du tous ");
    return 0;/*Protection*/
}
char ** deplacerBas(Rahan* rahan,char **matrice,int *ligne, int *colonne,nourriture *n,
monstre *m,equipement* e, Rochet *r,int *tableau,int index,etapes *etape,FILE *file)
{   
    int xR = rahan->positionR.X;
    int yR = rahan->positionR.Y;
    int vitesse =1;
    int q=xR+vitesse;
    if(q>=*ligne-1){  
        
        int Nligne=*ligne+1;
              
        char **matrice2= agrandirMatrice(matrice,*ligne,*colonne,Nligne,*colonne,'b');
        *ligne=*ligne+1;  
                tableau[index]=1;
   
        return matrice2;
    }else if(matrice[q][yR]=='#' || matrice[q][yR]=='Y' ){
      
        return matrice;

    }else if(matrice[q][yR]=='*'){
        
        Soigner(rahan,q,yR,n);
        matrice[q-vitesse][yR] =' ';
        matrice[q][yR] ='R';
        modifierCordR(rahan,q,yR);
        return matrice;

    }else if(matrice[q][yR]=='!'){
        
        equiper(rahan,q,yR,e);
        matrice[q-vitesse][yR] =' ';
        matrice[q][yR] ='R';
        modifierCordR(rahan,q,yR);
        return matrice;

    }else if(matrice[q][yR] == ' '){
        
        matrice[q-vitesse][yR] =' ';
        matrice[q][yR] ='R';
        modifierCordR(rahan,q,yR);
        return matrice;

    }else if(matrice[q][yR] == '@'){
        if(combatre(rahan,m,q,yR)){
            Liberation(rahan,e,etape,n,r,m,index,tableau,file);
        }else{
            matrice[q-vitesse][yR] =' ';
        matrice[q][yR] ='R';
        modifierCordR(rahan,q,yR);
        return matrice;
        }
    }
    return matrice;

}

char ** deplacerDroite(Rahan* rahan,char **matrice,int *ligne, int *colonne,nourriture *n,
monstre *m,equipement* e, Rochet *r,int *tableau,int index,etapes *etape,FILE *file)
{   
    int xR = rahan->positionR.X;
    int yR = rahan->positionR.Y;
    int vitesse =1;
    int q=yR+vitesse;
    if(q>=*colonne-1){  
        
        int NClonne=*colonne+1;
              
        char **matrice2= agrandirMatrice(matrice,*ligne,*colonne,*ligne,NClonne,'d');
        tableau[index]=1;

        *colonne=*colonne+1;     
        return matrice2;
    }else if(matrice[xR][q]=='#' || matrice[xR][q]=='Y' ){
      
        return matrice;

    }else if(matrice[xR][q]=='*'){
        
        Soigner(rahan,q,yR,n);
        matrice[xR][q-1] =' ';
        matrice[xR][q] ='R';
        modifierCordR(rahan,xR,q);
        return matrice;
    }else if(matrice[xR][q]=='!'){
        
        equiper(rahan,xR,q,e);
        matrice[xR][q-1] =' ';
        matrice[xR][q] ='R';
        modifierCordR(rahan,xR,q);
        return matrice;

    }else if(matrice[xR][q] == ' '){
        
        matrice[xR][q-1] =' ';
        matrice[xR][q] ='R';
        modifierCordR(rahan,xR,q);
        return matrice;

    }else if(matrice[q][yR] == '@'){
        if(combatre(rahan,m,q,yR)){
            Liberation(rahan,e,etape,n,r,m,index,tableau,file);
        }else{
            matrice[xR][q-1] =' ';
            matrice[xR][q] ='R';
            modifierCordR(rahan,xR,q);
            return matrice;
        }
    }
        return matrice;

}

char ** deplacerGauche(Rahan* rahan,char **matrice,int *ligne, int *colonne,nourriture *n,
monstre *m,equipement* e, Rochet *r,int *tableau,int index,etapes *etape,FILE *file)
{   
    int xR = rahan->positionR.X;
    int yR = rahan->positionR.Y;
    int vitesse =1;
    int q=yR-vitesse;
    if(q<=1){  
        
         int NColonne = *colonne + 1;
        char **matrice2 = agrandirMatrice(matrice, *ligne, *colonne, *ligne, NColonne, 'g');
        tableau[index]=1;
        /* Ajuster les coordonnées après l'agrandissement*/
        modifierCordR(rahan, xR, yR + 1);
        modifierCordEMN('g',n,m,e,r);
        *colonne = NColonne;
        return matrice2;
        
    }else if(matrice[xR][q]=='#' || matrice[xR][q]=='Y' ){
      
        return matrice;

    }else if(matrice[xR][q]=='*'){
        
        Soigner(rahan,q,yR,n);
        matrice[xR][q+1] =' ';
        matrice[xR][q] ='R';
        modifierCordR(rahan,xR,q);
        return matrice;

    }else if(matrice[xR][q]=='!'){
        
        equiper(rahan,q, yR,e);
        matrice[xR][q+1] =' ';
        matrice[xR][q] ='R';
        modifierCordR(rahan,xR,q);
        return matrice;

    }else if(matrice[xR][q] == ' '){
        
        matrice[xR][q+1] =' ';
        matrice[xR][q] ='R';
        modifierCordR(rahan,xR,q);
        return matrice;
    }else if(matrice[xR][q] == '@'){
        if(combatre(rahan,m,xR,q)){
             printf("je libbere");            
            Liberation(rahan,e,etape,n,r,m,index,tableau,file);
        }else{
            matrice[xR][q+1] =' ';
            matrice[xR][q] ='R';
            modifierCordR(rahan,xR,q);
            return matrice;
            
        }
    }
    return matrice;
}
void quiter(Rahan* rahan,equipement *e,monstre *m,nourriture *n,Rochet *r)
{  
    stdprof_free(n);
    stdprof_free(m);
    stdprof_free(e);
    stdprof_free(r);
    libererRahan(rahan);
    
}
void modifierCordEMN(char d,nourriture *n,monstre *m,equipement *e, Rochet *r){
    int i,j;
    /*j'utilise cette fonction dans le cas ou j'agrandie la matrice et donc les cordonnées change si on agrandie 
    la matrice sur la gauche et en haut mais pas en bas et a droite */
    if(d=='g'){
        for( i=0;i<nbrTe;i++){
            for( j=0;j<nbrE;j++){
                e[i].positionE[j].Y=e[i].positionE[j].Y+1;
            }
        }
        for( i=0;i<nbrTm;i++){
            for( j=0;j<NbrM;j++){
                m[i].positionM[j].Y=m[i].positionM[j].Y+1;
            }
        }
        for( i=0;i<nbrTn;i++){
            for( j=0;j<nbrn;j++){
                 n[i].positionN[j].Y=n[i].positionN[j].Y+1;
            }
        }
        for( i=0;i<nbrR;i++){
            r->positionR[i].Y+=1;
        }

    }else if(d=='h'){
        for( i=0;i<nbrTe;i++){
            for( j=0;j<nbrE;j++){
                e[i].positionE[j].X=e[i].positionE[j].X+1;
            }
        }
        for( i=0;i<nbrTm;i++){
            for( j=0;j<NbrM;j++){
                m[i].positionM[j].X=m[i].positionM[j].X+1;
            }
        }
        for( i=0;i<nbrTn;i++){
            for( j=0;j<nbrn;j++){
                 n[i].positionN[j].X=n[i].positionN[j].X+1;
            }
        }
        for( i=0;i<nbrR;i++){
            r->positionR[i].X+=1;
        }

    }    
}
void santeR(Rahan *rahan,int s){
    int santeRahan=rahan->sante;
    int pointVie=rahan->pointVie;
    santeRahan+=s;
    if(pointVie<=santeRahan){
        rahan->sante=pointVie;
    }else{
        rahan->sante=santeRahan;
    }

}


void equiper(Rahan *rahan,int q,int yR,equipement *e){
    int i,j;
   
    for( i=0 ;i<nbrTe;i++){
        for( j=0 ;j<nbrE;j++){
            if(e[i].positionE[j].X==q && e[i].positionE[j].Y==yR){
                if(strcmp(e[i].nom, "chaussures") == 0){
                    equiperRahan(rahan,0);
                }else if(strcmp(e[i].nom, "protections") == 0){/*une suggestion de chat gpt pour eviter les worning*/
                    equiperRahan(rahan,1);
                }else{
                    equiperRahan(rahan,2);
                }
            }
        }
    }

}
void equiperRahan(Rahan *rahan,int classe){
    if(classe==0){
        if(rahan->Equipement[classe]==0){
            rahan->vitesse=2;/* c'est un choix;*/
            rahan->Equipement[classe]=1;/* pour que les prochain tours on ne rajoute pas */
            printf("\n,bottes en peau de crocro, V+10, chaussures}\n");
        }
        return;
    }else if(classe==1){
        if(rahan->Equipement[classe]==0){
            rahan->pointVie+=10;
            rahan->Equipement[classe]=1;
            printf("\n{slip en peau de mamouth, P+10, protections}\n");
        }
        return;
    }else if(classe==2){
         if(rahan->Equipement[classe]==0){
            rahan->force+=5;
            rahan->Equipement[classe]=1;
            printf("\n{lance en bois, F+5, armes} \n");
        }
        return;
    }    
    

}
void invocation(Rahan *rahan){
    printf("\n");
    printf("la force :%d\n les points de vie %d\n la santé :%d\n vitesse : %d \n les dordonee:Xr: %d Xy %d \n les equipement %d %d %d ",rahan->force,rahan->pointVie,rahan->sante,rahan->vitesse,rahan->positionR.X,rahan->positionR.Y,rahan->Equipement[0],rahan->Equipement[1],rahan->Equipement[2]);
}
void modifierCordR(Rahan *rahan,int X,int Y){
    rahan->positionR.X = X;
    rahan->positionR.Y = Y;
}
void Soigner(Rahan *rahan,int X,int Y,nourriture *n){
    int i,j;
    for(i=0;i<nbrTe;i++){
        for( j=0;j<nbrTe;j++) {
            if(n[i].positionN[j].X==X && n[i].positionN[j].Y==Y){
                if(i==0){
                    santeR(rahan,3);
                    return;
                }else{
                    santeR(rahan,25);
                    return; /*  si on trouve que y a par azard une autre nourriture qui a la meme cordoée */
                }
            }
        }
    }

}

void libererMatrice(char **matrice, int lignes) {
    int i;
    for ( i = 0; i < lignes; i++) {
         stdprof_free(matrice[i]); /* Libérer chaque ligne*/
    }
    stdprof_free(matrice); /* Libérer le tableau de pointeurs de lignes*/
}

  /* 
int combat(Rahan *rahan,monstre *monstre ){
    while(monstre->sante>0 && rahan->sante>0){
        monstre->sante-=rahan->force; // rahan frape en premier 
        if(monstre->sante>0){
            rahan->sante-=monstre->pointForce;
        }
    
    }
    if(rahan->sante<=0){
        printf("\n GAME OVER\n");
        return 1;
    }else{
        monstre->positionM.X=-1;
        monstre->positionM.Y=-1;
        printf("bravo !!! tu as tue le %s",monstre->nom);
        return 0;
    }

}*/ 
void Liberation(Rahan *rahan,equipement *equipement,etapes *etapee,nourriture *nourriture,
    Rochet *rochet,monstre *monstre,int index,int *tableau,FILE *file){
    int j;
    libererRahan(rahan);
    quiter(rahan,equipement,monstre,nourriture,rochet);
    for ( j = 0; j <= index; j++) {
        if(tableau[j]==1){
        libererMatrice(etapee[j].matrice, etapee[j].lignes);
        }
    }
    fclose(file);
}
void inisialisationTab(int *tableau,int i){
    int j=0;
     tableau[j]=1; 

    for( j=1; j <i; j++){
        tableau[j]=0;
    }
}
int combat(Rahan *rahan,int santeM,int forceM,char *nom){

    while(santeM>0 && rahan->sante>0){
        santeM-=rahan->force; /* rahan frape en premier*/ 
        printf("rahan frape ");
        if(santeM>0){ /*si il est mort il ne peut pas rendre le coup*/
            rahan->sante-=forceM;
            printf("rahan encaisse");
        }
    
    }
    if(rahan->sante<=0){
        printf("\n GAME OVER\n");
        return 1;
    }else{
        printf("bravo !!! tu as tue le %s",nom);
        return 0;
    }
}
void jouer(etapes *etapee,Rahan *rahan,int *i,int *ligne,int *colonne,nourriture *n,monstre *m,
    equipement *e,Rochet *r,int *tableau,FILE *fichier){
            char reponse;
            int j;
    while ((reponse = fgetc(fichier)) != EOF){
    if (reponse == '\n') {
        printf("je saute \n");
        continue;  

    }else if(reponse == '#'){
            sauterLigne(fichier);
    }else{
    (*i)++;
  
    printf("\n>>");

    switch(reponse){
        case 'b':
             printf("\n %c\n ",reponse);
            for(j=0;j< rahan->vitesse;j++){
            etapee[*i].matrice=deplacerBas(rahan,etapee[*i-1].matrice,ligne,colonne,n,m,e,r,tableau,*i,etapee,fichier);
            etapee[*i].lignes=*ligne;
            etapee[*i].colonnes=*colonne;
            (*i)++;
            }
            (*i)--;

            break;
        case 'g':
            printf("\n %c\n ",reponse);
            for(j=0;j< rahan->vitesse;j++){
            etapee[*i].matrice=deplacerGauche(rahan,etapee[*i-1].matrice,ligne,colonne,n,m,e,r,tableau,*i,etapee,fichier);
            etapee[*i].lignes=*ligne;
            etapee[*i].colonnes=*colonne;
            (*i)++;
            }     
            (*i)--;

            break;
        case 'h':
            printf("\n %c\n ",reponse);
            for(j=0;j< rahan->vitesse;j++){
            etapee[*i].matrice=deplacerHaut(rahan,etapee[*i-1].matrice,ligne,colonne,n,m,e,r,tableau,*i,etapee,fichier);
            etapee[*i].lignes=*ligne;
            etapee[*i].colonnes=*colonne;
            (*i)++;
            }
            (*i)--;

            break;
        case 'd':
             printf("\n %c\n ",reponse);
            for(j=0;j< rahan->vitesse;j++){
            etapee[*i].matrice=deplacerDroite(rahan,etapee[*i-1].matrice,ligne,colonne,n,m,e,r,tableau,*i,etapee,fichier);
            etapee[*i].lignes=*ligne;
            etapee[*i].colonnes=*colonne;
            (*i)++;
            }
            (*i)--;
            break;
        case 'a':
            printf("\n %c ",reponse);
            afficherMatrice(etapee[*i-1].matrice,*ligne,*colonne);
            (*i)--;
            break;
        case 'i':
            invocation(rahan);
            (*i)--;
            break;
        default:
            printf("\n %c kechkech \n\n",reponse);
            Liberation(rahan,e,etapee,n,r,m,*i,tableau,fichier);
            printf("\n je quite");
            exit(1);
    }
  }
   }  


    }
    
void sauterLigne(FILE *fichier) {
    int reponse;
    while ((reponse = fgetc(fichier)) != EOF && reponse != '\n') {
        /* Passer au caractère suivant jusqu'à la fin de la ligne*/
    }
}