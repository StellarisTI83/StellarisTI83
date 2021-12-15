#ifndef H_ORDRES
#define H_ORDRES

/* types =============================================================== */
typedef struct OrdreElement Ordre;
typedef struct OrdreFileStruct OrdreFile;

/* internal public functions =========================================== */

/**
 * Cree une file d'ordre
 * 
 * @return File d'ordres
 */
OrdreFile *CreerFileOrdres();

/**
 * Supprime une file d'ordre
 * 
 * @param *ordreFile File à supprimer
 */
void SupprimerFileOrdres(OrdreFile *ordreFile);

/**
 * Crèe d'ordre
 * 
 * \param ordreFile File où ajouter l'ordre
 * \param ordre id de l'ordre
 * \param empire empire qui effectue l'ordre
 * \param tempsTotal Temps total que dure l'ordre
 * \param info1 info supplementaire 1
 * \param info2 info supplementaire 2
 * \param prix Prix à rendre en cas d'annulation de l'ordre
 */
void order_New(OrdreFile* ordreFile, int ordre, int empire, int tempsTotal, int info1, int info2, int prix);

/**
 * Fini le dernier ordre
 */
void FinirOrdre(OrdreFile *ordreFile);

/**
 * Recupere le dernier ordre
 */
Ordre* RecupererOrdre(OrdreFile *ordreFile);

/**
 * Renvoi le nombre d'ordre dans une file d'ordres
 */
int NombredOrdres(OrdreFile *ordreFile);

int GetOrder(OrdreFile *ordreFile);
int GetOrderEmpire(OrdreFile *ordreFile);
int GetOrderProgress(OrdreFile *ordreFile);
int GetOrderTotalTime(OrdreFile *ordreFile);
void UnincrementOrderProgress(OrdreFile *ordreFile);

int GetOrderInfo1(OrdreFile *ordreFile);
int GetOrderInfo2(OrdreFile *ordreFile);
int GetOrderPrice(OrdreFile *ordreFile);
int GetOrderProgressPourcent(Ordre *ordre);

#endif