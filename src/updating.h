#ifndef H_UPDATING
#define H_UPDATING

/* entry points ======================================================== */
void UpdatePlayersData(char appliquer, EmpireListe *empireListe, SystemeStellaire **systemeStellaires, NotificationList *notificationList);
int UpdateGame(char *key, EmpireListe *empireListe, SystemeStellaire **systemeStellaires, Date *date, Camera *camera, Fenetre *fenetre, NotificationList *notificationList, Parametres *parametres);

#endif