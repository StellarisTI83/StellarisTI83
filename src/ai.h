#ifndef ARTIFICIAL_INTELLIGENCE_H_INCLUDED
#define ARTIFICIAL_INTELLIGENCE_H_INCLUDED

EmpireListe* EmpireListeCreer();
void EmpireListeSupprimer(EmpireListe*);
Empire* EmpireNumero(EmpireListe*, int numero);
Empire* EmpireAjouter(EmpireListe*);
void EmpireSupprimer(EmpireListe*, int numero);

#endif