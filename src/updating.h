#ifndef H_UPDATING
#define H_UPDATING

/* entry points ======================================================== */
void UpdatePlayersData(char appliquer, EmpireListe *empireListe, StarSystem **systemeStellaires, NotificationList *notificationList);
int UpdateGame(char *key, EmpireListe *empireListe, StarSystem **systemeStellaires, Time *date, Camera *camera, Window *fenetre, NotificationList *notificationList, Settings *parametres);

#endif