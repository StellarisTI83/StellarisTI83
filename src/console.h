#ifndef _CONSOLE_INCLUDE
#define _CONSOLE_INCLUDE

typedef struct LigneDeCommande Ligne;
struct LigneDeCommande{
    char commande[50];
    char reponse[50];
    Ligne *commandeSuivante;
};
typedef struct{
    char deniereCommande[50];
    int8_t cursor;
    bool nombreActive;
    char reponse[50];
    Ligne *premiereLigne;
} Console;


void AfficherConsole(Console *console, char *key, Fenetre *fenetre, EmpireListe *empireListe, Camera *camera, Date *date);

void FermerConsole(Console *console);
void NouvelleLigneConsole(char *commande, char *reponse, Console *console);
void RechercherCommande(char *commande, Console *console, EmpireListe *empireListe, Date *date);


#endif