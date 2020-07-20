#ifndef _NOUVELLE_PARTIE_INCLUDE
#define _NOUVELLE_PARTIE_INCLUDE

int NouvellePartieAvertissement(Empire *joueur, Parametres *parametres);

int QuitterNouvellePartieAvertissement();
int NouvellePartieEspece(Empire *joueur, Parametres *parametres);
int NouvellePartieGouvernement(Empire *joueur, Parametres *parametres);
int NouvellePartiePrincipes(Empire *joueur, Parametres *parametres);
int NouvellePartieNom(Empire *joueur, Parametres *parametres);
int NouvellePartieDrapeau(Empire *joueur, Parametres *parametres);
int NouvellePartieParametres(Empire *joueur, Parametres *parametres);
void ChargementNouvellePartie(EmpireListe *empireListe, Empire *joueur, Parametres *parametres, Date *date, SystemeStellaire *systemeStellaires, Camera *camera, FlotteListe *flotteJoueur, Fenetre *fenetre, Marche *marche);
int ChargementNouvellePartieGalaxie(Parametres *parametres, ti_var_t *sauvegarde, SystemeStellaire *systemeStellaires, FlotteListe *flotteJoueur, Camera *camera);


void PrintText(const char *str, int x, int y, int taille, int color);
#endif