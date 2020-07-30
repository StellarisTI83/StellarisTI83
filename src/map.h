#ifndef _MAP_INCLUDE
#define _MAP_INCLUDE

#define RAYON_DE_VUE_SYSTEME 160

void DessinerVueMap(SystemeStellaire* systemeStellaires, Camera* camera, EmpireListe* empireListe);
void DessinerPlanetesHabitables(SystemeStellaire* systemeStellaire, int i, int x, int y);

void DessinerFlottesMap(EmpireListe* empireListe, Empire* joueur, SystemeStellaire* systemeStellaires, Camera* camera);

void DessinerHyperlane(int8_t niveauConnaissance1, int8_t niveauConnaissance2, int16_t x, int16_t y, int16_t xLn, int16_t yLn, Camera* camera);

void KeyActionNormalMap(EmpireListe *empireListe, SystemeStellaire *systemeStellaires, Camera *camera, char *key, FlotteListe *flotteJoueur, Date *date, Fenetre *fenetre, Empire *joueur, Console *console);

void KeyActionNormal(EmpireListe *empireListe, SystemeStellaire *systemeStellaires, Camera *camera, char *key, FlotteListe *flotteJoueur, Date *date, Fenetre *fenetre, Empire *joueur, Console *console);

void DessinerVueSysteme(SystemeStellaire* systemeStellaires, Camera* camera, Fenetre *fenetre, EmpireListe* empireListe, char *key);
void DessinerEtoile(SystemeStellaire*, Camera*, Fenetre*, char*);
void CouleurEtoile(int type);

void DessinerPlanete(SystemeStellaire*, Planete*, Camera*, Fenetre*, int, char*);
void CouleurPlanete(char);

void DessinerBase(SystemeStellaire*, Camera*, Fenetre*, char*);

void DessinerFlottesSysteme(EmpireListe *empireListe, Camera *camera, Fenetre *fenetre, char* key);


#endif _MAP_INCLUDE