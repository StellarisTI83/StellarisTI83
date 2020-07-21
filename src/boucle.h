#ifndef _BOUCLE_INCLUDE
#define _BOUCLE_INCLUDE

#define MENU_QUITTER -1
#define MENU_AUCUN 0
#define MENU_SYSTEME 1
#define MENU_MARCHE 2
#define MENU_FLOTTE 3
#define MENU_RECHERCHE 4
#define MENU_CONTACTS 5

#define MENU_SYSTEME_FLOTTES 1
#define MENU_SYSTEME_ETOILE 2
#define MENU_SYSTEME_PLANETE_RESUME 3
#define MENU_SYSTEME_PLANETE_POPULATION 4
#define MENU_SYSTEME_PLANETE_ARMEE 5
#define MENU_SYSTEME_FLOTTE_DETAILS 6
#define MENU_SYSTEME_STATION_RESUME 7
#define MENU_SYSTEME_STATION_MODULES 8
#define MENU_SYSTEME_STATION_CHANTIER 9

int StellarisBoucle(ti_var_t *sauvegarde, EmpireListe *empireListe, Empire *joueur, Parametres *parametres, Date *date, SystemeStellaire *systemeStellaires, Camera *camera, FlotteListe *flotteJoueur, Fenetre *fenetre, Marche *marche);	
	void StellarisTemps(EmpireListe *empireListe, Date *date, char *key, SystemeStellaire* systemeStellaires);

	void StellarisMap(EmpireListe *empireListe, SystemeStellaire *systemeStellaires, Camera *camera, char *key, FlotteListe *flotteListe, Date *date, Fenetre *fenetre, Empire *joueur);

	int StellarisHUD(EmpireListe *empireListe, Empire *joueur, Date *date, char *key, Camera *camera, SystemeStellaire *systemeStellaires, Fenetre *fenetre, Parametres *parametres, ti_var_t* sauvegarde, Marche *marche);
		void PrintHUD(const unsigned int nombre, const int change, int x, int y);

		int Menus(EmpireListe *empireListe, Empire *joueur, Date *date, char *key, Camera *camera, SystemeStellaire *systemeStellaires, Fenetre *fenetre, Parametres *parametres, ti_var_t *sauvegarde, Marche *marche);
			int8_t MenuQuitter(char* key, ti_var_t* sauvegarde, EmpireListe* empireListe, Empire* joueur, Parametres* parametres, Date* date, SystemeStellaire* systemeStellaires, Camera* camera, Marche* marche, Fenetre* fenetre);

			void MenuSysteme(char* key, EmpireListe* empireListe, Empire* joueur, Parametres* parametres, Date* date, SystemeStellaire* systemeStellaires, Camera* camera, Marche* marche, Fenetre* fenetre);
				void MenuSystemeFlotte(char* key, EmpireListe* empireListe, Empire* joueur, SystemeStellaire* systemeStellaires, Camera* camera, Fenetre* fenetre);

				void MenuSystemeEtoile(char *key, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre);

				void MenuSystemePlaneteResume(char *key, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre);

				void MenuSystemePlanetePopulation(char *key, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre);
				
				void MenuSystemePlaneteArmee(char *key, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre);

				void MenuSystemeFlotteDetails(char *key, SystemeStellaire *systemeStellaires, EmpireListe *empireListe, Empire* joueur, Camera *camera, Fenetre *fenetre);

				void MenuSystemeStationResume(char *key, Empire *joueur, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre);
				
				void MenuSystemeStationModules(char *key, Empire *joueur, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre);
				
				void MenuSystemeStationChantier(char *key, Empire *joueur, SystemeStellaire *systemeStellaires, Camera *camera, Fenetre *fenetre);

			void MenuMarche(char *key, Empire* joueur, Marche *marche, Camera *camera, Fenetre *fenetre);

			void MenuListeFLottes(char *key, Empire *joueur, Camera *camera, Fenetre *fenetre);

			void MenuRecherche(char *key, Camera *camera, Fenetre *fenetre);

			void MenuContacts(char *key, EmpireListe *empireListe, Camera *camera, Fenetre *fenetre);

#endif _BOUCLE_INCLUDE