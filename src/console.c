#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <debug.h>
#include <math.h>
#include <errno.h>

#include <graphx.h>
#include <fileioc.h>
#include <fontlibc.h>

#include "gfx/gfx.h"

#include "main.h"
#include "boucle.h"
#include "map.h"
#include "nouvelle_partie.h"
#include "sauvegarde.h"
#include "flottes.h"
#include "updating.h"
#include "console.h"

#include "locale/locale.h"

void AfficherConsole(Console *console, char *key, Fenetre *fenetre, EmpireListe *empireListe, Camera *camera, Date *date){
	int largeur = 200, hauteur = 10;
	Ligne *ligneDeCommande = NULL;
	char character = '.';
	char *resultat = NULL;
	int x = 0, y = 40;
	gfx_SetColor(0);
	ligneDeCommande = console->premiereLigne;
	x += 5;
	while(ligneDeCommande != NULL){
		gfx_FillRectangle_NoClip(x, y, largeur, hauteur);
		gfx_SetTextXY(x, y);
		gfx_PrintString("> ");
		gfx_PrintString(ligneDeCommande->commande);
		y += 10;
		gfx_FillRectangle_NoClip(x, y, largeur, hauteur);
		gfx_PrintStringXY(ligneDeCommande->reponse, x, y);
		y += 10;
		ligneDeCommande = ligneDeCommande->commandeSuivante;
	}
	gfx_FillRectangle_NoClip(x, y, largeur, hauteur);
	gfx_SetTextXY(x, y);
	gfx_PrintString("> ");
	gfx_PrintString(console->deniereCommande);

	if(*key != 0){
		if(console->nombreActive == false){
			switch(*key){
				case sk_Math:
					character = 'a';
					break;
				case sk_Apps:
					character = 'b';
					break;
				case sk_Prgm:
					character = 'c';
					break;
				case sk_Recip:
					character = 'd';
					break;
				case sk_Sin:
					character = 'e';
					break;
				case sk_Cos:
					character = 'f';
					break;
				case sk_Tan:
					character = 'g';
					break;
				case sk_Power:
					character = 'h';
					break;
				case sk_Square:
					character = 'i';
					break;
				case sk_Comma:
					character = 'j';
					break;
				case sk_LParen:
					character = 'k';
					break;
				case sk_RParen:
					character = 'l';
					break;
				case sk_Div:
					character = 'm';
					break;
				case sk_Log:
					character = 'n';
					break;
				case sk_7:
					character = 'o';
					break;
				case sk_8:
					character = 'p';
					break;
				case sk_9:
					character = 'q';
					break;
				case sk_Mul:
					character = 'r';
					break;
				case sk_Ln:
					character = 's';
					break;
				case sk_4:
					character = 't';
					break;
				case sk_5:
					character = 'u';
					break;
				case sk_6:
					character = 'v';
					break;
				case sk_Sub:
					character = 'w';
					break;
				case sk_Store:
					character = 'x';
					break;
				case sk_1:
					character = 'y';
					break;
				case sk_2:
					character = 'z';
					break;
				case sk_0:
					character = ' ';
					break;
				case sk_DecPnt:
					character = ' ';
					break;
				case sk_2nd:
					fenetre->commandPrompt = false;
					camera->bloque = false;
					FermerConsole(console);
					break;
				case sk_Alpha:
					console->nombreActive = true;
					break;
			}
		}	
		else{
			console->nombreActive = true;
			switch(*key){
				case sk_1:
					character = '1';
					break;
				case sk_2:
					character = '2';
					break;
				case sk_3:
					character = '3';
					break;
				case sk_4:
					character = '4';
					break;
				case sk_5:
					character = '5';
					break;
				case sk_6:
					character = '6';
					break;
				case sk_7:
					character = '7';
					break;
				case sk_8:
					character = '8';
					break;
				case sk_9:
					character = '9';
					break;
				case sk_0:
					character = '0';
					break;
				case sk_DecPnt:
					character = ' ';
					break;
				case sk_Alpha:
					console->nombreActive = false;
					break;
				case sk_2nd:
					fenetre->commandPrompt = false;
					camera->bloque = false;
					FermerConsole(console);
					break;
			}
		}
		if((((character != '.') && (*key)) &&  ((29 >= console->cursor) && (*key != sk_Del))) && ((*key != sk_Alpha) && (*key != sk_Enter))){
			console->deniereCommande[console->cursor] = character;
			console->cursor++;
		}
		else if((*key == sk_Del) && (console->cursor > 0)){
			console->cursor--;
			console->deniereCommande[console->cursor]= ' ';
		}
		else if(*key == sk_Enter){
			RechercherCommande(console->deniereCommande, console, empireListe, date);
			NouvelleLigneConsole(console->deniereCommande, console->reponse, console);
			memset(console->deniereCommande, 0, sizeof(console->deniereCommande));
			console->cursor = 0;
			memset(console->reponse, 0, sizeof(console->reponse));
		}
		*key = 0;
	}
}

void NouvelleLigneConsole(char *commande, char *reponse, Console *console){
	Ligne *ligneDeCommande = NULL;
	ligneDeCommande = console->premiereLigne;
	if(ligneDeCommande == NULL){
		console->premiereLigne = malloc(sizeof(Ligne));
		ligneDeCommande = console->premiereLigne;
	}
	else{
		while(ligneDeCommande->commandeSuivante != NULL){
			ligneDeCommande = ligneDeCommande->commandeSuivante;
		}
		ligneDeCommande->commandeSuivante = malloc(sizeof(Ligne));
		ligneDeCommande = ligneDeCommande->commandeSuivante;
	}
	memset(ligneDeCommande, 0, sizeof(Ligne));
	ligneDeCommande->commandeSuivante = NULL;
	strcpy(ligneDeCommande->commande, commande);
	strcpy(ligneDeCommande->reponse, reponse);
}

void FermerConsole(Console *console){
	Ligne *ligneDeCommandePrecedent = NULL, *ligneDeCommande = console->premiereLigne;
	while(ligneDeCommande != NULL){
		ligneDeCommandePrecedent = ligneDeCommande;
		ligneDeCommande = ligneDeCommande->commandeSuivante;
		free(ligneDeCommande);
	}
	console->premiereLigne = NULL;
}

void RechercherCommande(char *commande, Console *console, EmpireListe *empireListe, Date *date){
	char *resultat = NULL;
	char *fin = NULL;
	int i = 0, nombre = 0;
	char nombreChar[10];
	int jour = 0, mois = 0, annee = 0;
	resultat = strstr(commande, "cash");
	if(resultat == commande){
		strcpy(console->reponse, "added ");

		nombre = strtol(&(commande[5]), &fin, 10);

		if(nombre > 99999){
			strcpy(console->reponse, "number must be inferior to 99999");
			return;
		}
		else if(nombre == 0){
			goto syntax_err;
		}
		else{
			sprintf(nombreChar, "%d", nombre);
			strcat(console->reponse, nombreChar);
			strcat(console->reponse, " cash");
			empireListe->premier->credits += nombre;
			return;
		}
	}

	resultat = strstr(commande, "minerals");
	if(resultat == commande){
		strcpy(console->reponse, "added ");

		nombre = strtol(&(commande[9]), &fin, 10);

		if(nombre > 99999){
			strcpy(console->reponse, "number must be inferior to 99999");
			return;
		}
		else if(nombre == 0){
			goto syntax_err;
		}
		else{
			sprintf(nombreChar, "%d", nombre);
			strcat(console->reponse, nombreChar);
			strcat(console->reponse, " minerals");
			empireListe->premier->minerais += nombre;
			return;
		}
	}

	resultat = strstr(commande, "alloys");
	if(resultat == commande){
		strcpy(console->reponse, "added ");

		nombre = strtol(&(commande[7]), &fin, 10);

		if(nombre > 99999){
			strcpy(console->reponse, "number must be inferior to 99999");
			return;
		}
		else if(nombre == 0){
			goto syntax_err;
		}
		else{
			sprintf(nombreChar, "%d", nombre);
			strcat(console->reponse, nombreChar);
			strcat(console->reponse, " alloys");
			empireListe->premier->acier += nombre;
			return;
		}
	}

	resultat = strstr(commande, "foods");
	if(resultat == commande){
		strcpy(console->reponse, "added ");

		nombre = strtol(&(commande[6]), &fin, 10);

		if(nombre > 99999){
			strcpy(console->reponse, "number must be inferior to 99999");
			return;
		}
		else if(nombre == 0){
			goto syntax_err;
		}
		else{
			sprintf(nombreChar, "%d", nombre);
			strcat(console->reponse, nombreChar);
			strcat(console->reponse, " food");
			empireListe->premier->nourriture += nombre;
			return;
		}
	}

	resultat = strstr(commande, "time");
	if(resultat == commande){

		resultat = strstr(&commande[5], "set"); //time set dd mm yyyy
		if(resultat == &commande[5]){
			jour = strtol(&(commande[9]), &fin, 10);
			if((&(commande[9]) == fin) || !((jour > 0) && (jour <= 30))){
				goto syntax_err;
			}
			mois = strtol(&(commande[12]), &fin, 10);
			if((&(commande[12]) == fin) || !((mois > 0) && (mois <= 12))){
				goto syntax_err;
			}
			annee = strtol(&(commande[15]), &fin, 10);
			if((&(commande[15]) == fin) || !((annee > 2200) && (annee <= 2500))){
				goto syntax_err;
			}
			date->jour = jour;
			date->mois = mois;
			date->annee = annee;
			strcpy(console->reponse, "time sat");
			return;
		}
	}


	syntax_err:
		strcpy(console->reponse, "syntax error");
}
