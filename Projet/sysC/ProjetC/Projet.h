#ifndef HEAUDER_H_INCLUDED
#define HEAUDER_H_INCLUDED

typedef struct {
 int lignes;
 int colonnes;
 char **matrice;
 }etapes; 
 typedef struct{
    int X,Y;
 }position;
 typedef struct{
    char description[150];
    char stats[20];
    char nom[40];
    position positionE[20];
}equipement;
/*typedef struct{
    char nomEquipement[20];

}nomEquipements;*/
 typedef struct{
     int pointVie;
     int sante;
     int force;
     int vitesse;
     position positionR;
     int Equipement[3];

 }Rahan;

 typedef struct { 
   char nom[50];
  int santePlus;
  char description[150];
  position positionN[20];
 }nourriture;
 typedef struct {
    char nom[50];
    int pointForce;
    int sante;
    position positionM[20];
 }monstre; 

typedef struct {
    position positionR[20];
    char nom[10];
}Rochet; 

nourriture* definitionNourriture();
int genererNombreAleatoire(int max);
monstre* definitionM();
equipement* definitionE();
void afficherEquipements(equipement* t, int taille);
void afficherMonstres(monstre *t, int taille);
 
void afficherNourritures(nourriture* n, int taille);
void afficherRochet(Rochet* r);
Rochet* definitionRochet();
void inisialisation(char **matrice,int lignes_actuelles,int colonnes_actuelles);

char** agrandirMatrice(char** matrice, int lignes_actuelles, int colonnes_actuelles,
                        int nouvelles_lignes, int nouvelles_colonnes ,char d);

void remplirMatrice(char** matrice, int lignes, int colonnes, Rochet* rochets, nourriture* nourritures,equipement* equipements, monstre* monstres, Rahan *rahan);
char** allouerMatrice(int lignes, int colonnes);
void afficherMatrice(char** matrice, int lignes, int colonnes);
 
void libererMatrice(char** matrice, int lignes);

void libererRahan(Rahan* rahan);
Rahan* initialiserRahan(int pointVie, int sante, int force, int vitesse, int X, int Y);
char ** deplacerHaut(Rahan* rahan,char **matrice,int *ligne,int *colonne,nourriture *n,
monstre *m,equipement* e, Rochet *r,int *tableau,int index,etapes *etape,FILE *file);
void Soigner(Rahan *rahan,int X,int Y,nourriture *n);
void santeR(Rahan *rahan,int s);
void equiper(Rahan *rahan,int q,int yR,equipement *e);
char ** deplacerBas(Rahan* rahan,char **matrice,int *ligne, int *colonne,nourriture *n,
monstre *m,equipement* e, Rochet *r,int *tableau,int index,etapes *etape,FILE *file);

void equiperRahan(Rahan *rahan,int classe);

void invocation(Rahan *rahan);
void modifierCordR(Rahan *rahan,int X,int Y);
char ** deplacerDroite(Rahan* rahan,char **matrice,int *ligne, int *colonne,nourriture *n,
monstre *m,equipement* e, Rochet *r,int *tableau,int index,etapes *etape,FILE *file);
void quiter(Rahan* rahan,equipement *e,monstre *m,nourriture *n,Rochet *r);
char ** deplacerGauche(Rahan* rahan,char **matrice,int *ligne, int *colonne,nourriture *n,
monstre *m,equipement* e, Rochet *r,int *tableau,int index,etapes *etape,FILE *file);
void modifierCordEMN(char d,nourriture *n,monstre *m,equipement *e, Rochet *r);

void inisialisationTab(int *tableau,int i);

void Liberation(Rahan *rahan,equipement *equipement,etapes *etapee,nourriture *nourriture,
Rochet *rochet,monstre *monstre,int index,int *tableau,FILE *file);

int combatre(Rahan* rahan, monstre* m, int q, int yR);
int combat(Rahan *rahan,int santeM,int forceM,char *nom);
void jouer(etapes *etapee,Rahan *rahan,int *i,int *ligne,int *colonne,nourriture *n,monstre *m,
    equipement *e,Rochet *r,int *tableau,FILE *fichier);

void sauterLigne(FILE *fichier);

#endif 