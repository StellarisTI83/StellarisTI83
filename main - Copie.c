	case 5:
					systemeStellaires[k].planete5->habitable = 0;
					systemeStellaires[k].planete5->rayonOrbite = randInt(62, 70);
					systemeStellaires[k].planete5->x = randInt(240 - systemeStellaires[k].planete5->rayonOrbite, 240 + systemeStellaires[k].planete5->rayonOrbite); //aleatoire de x
					systemeStellaires[k].planete5->y = sqrt(pow((double)systemeStellaires[k].planete5->rayonOrbite, 2.0) - pow((double)(systemeStellaires[k].planete5->x - 240), 2.0)) + 320; //calcule de y pour ce x
					
					systemeStellaires[k].planete5->type = randInt(1, 100);
					if(systemeStellaires[k].planete5->type <= 60) {
						systemeStellaires[k].planete5->type = 14; //geante gazeuse
						systemeStellaires[k].planete5->habitable = 0;
					}
					else if (systemeStellaires[k].planete5->type <= 90){
						systemeStellaires[k].planete5->type = 11; //monde aride froid
						systemeStellaires[k].planete5->habitable = 0;
					}
					else {
						systemeStellaires[k].planete5->type = 12; //monde brisé
						systemeStellaires[k].planete5->habitable = 0;
					}
					
					systemeStellaires[k].planete5->taille = randInt(1, 100);
					if(systemeStellaires[k].planete5->type == 14) {
						systemeStellaires[k].planete5->taille = 5;
					}
					if(systemeStellaires[k].planete5->taille <= 10) {
						systemeStellaires[k].planete5->taille = 2;
					}
					else if(systemeStellaires[k].planete5->taille <= 70) {
						systemeStellaires[k].planete5->taille = 3;
					}
					else if(systemeStellaires[k].planete5->taille <= 90) {
						systemeStellaires[k].planete5->taille = 4;
					}
					else {
						systemeStellaires[k].planete5->taille = 5;
					}
					
					if(randInt(0, 1) == 1) {
						systemeStellaires[k].planete5->y = 240 - (systemeStellaires[k].planete5->y - 240);
					}
					systemeStellaires[k].planete5->population = 0;
				
				case 4:
					systemeStellaires[k].planete4->habitable = 0;
					systemeStellaires[k].planete4->rayonOrbite = randInt(50,56);
					systemeStellaires[k].planete4->x = randInt(240 - systemeStellaires[k].planete4->rayonOrbite, 240 + systemeStellaires[k].planete4->rayonOrbite);
					systemeStellaires[k].planete4->y = sqrt(pow((double)systemeStellaires[k].planete4->rayonOrbite, 2.0) - pow((double)(systemeStellaires[k].planete4->x - 240), 2.0)) + 320;	
					
					systemeStellaires[k].planete4->type = randInt(1, 100);
					if(systemeStellaires[k].planete4->type <= 30) {
						systemeStellaires[k].planete4->type = 14; //geante gazeuse
						systemeStellaires[k].planete4->habitable = 0;
					}
					else if (systemeStellaires[k].planete4->type <= 70){
						systemeStellaires[k].planete4->type = 11; //monde aride froid
						systemeStellaires[k].planete4->habitable = 0;
					}
					else if (systemeStellaires[k].planete4->type <= 75){
						systemeStellaires[k].planete4->type = 4; //monde Alpine habitable
						systemeStellaires[k].planete4->habitable = 1;
					}
					else if (systemeStellaires[k].planete4->type <= 80){
						systemeStellaires[k].planete4->type = 5; //monde Arctic froid
						systemeStellaires[k].planete4->habitable = 1;
					}
					else if (systemeStellaires[k].planete4->type <= 85){
						systemeStellaires[k].planete4->type = 6; //monde Tundra habitable
						systemeStellaires[k].planete4->habitable = 1;
					}
					else if (systemeStellaires[k].planete4->type <= 90){
						systemeStellaires[k].planete4->type = 7; //monde continental froid
						systemeStellaires[k].planete4->habitable = 1;
					}
					else {
						systemeStellaires[k].planete4->type = 12; //monde brisé
						systemeStellaires[k].planete4->habitable = 0;
					}
					
					if(systemeStellaires[k].planete4->habitable == 1) {
						strcpy(systemeStellaires[k].planete4->nom, nomPlanetes[randInt(0, (sizeof(nomPlanetes)/sizeof(nomPlanetes[0])) - 1 )]);
						// strcpy(systemeStellaires[k].planete4->nom, "habitabl");
					}
					
					systemeStellaires[k].planete4->taille = randInt(1, 100);
					if(systemeStellaires[k].planete4->taille <= 10) {
						systemeStellaires[k].planete4->taille = 2;
					}
					else if(systemeStellaires[k].planete4->taille <= 70) {
						systemeStellaires[k].planete4->taille = 3;
					}
					else if(systemeStellaires[k].planete4->taille <= 90) {
						systemeStellaires[k].planete4->taille = 4;
					}
					else {
						systemeStellaires[k].planete4->taille = 5;
					}
					if(randInt(0, 1) == 1)
					{
						systemeStellaires[k].planete4->y = 240 - (systemeStellaires[k].planete4->y - 240);
					}
					systemeStellaires[k].planete4->population = 0;
					
				case 3:
					systemeStellaires[k].planete3->habitable = 0;
					systemeStellaires[k].planete3->rayonOrbite = randInt(40, 46);
					systemeStellaires[k].planete3->x = randInt(240 - systemeStellaires[k].planete3->rayonOrbite, 240 + systemeStellaires[k].planete3->rayonOrbite);
					systemeStellaires[k].planete3->y = sqrt(pow((double)systemeStellaires[k].planete3->rayonOrbite, 2.0) - pow((double)(systemeStellaires[k].planete3->x - 240), 2.0)) + 320;
					
					systemeStellaires[k].planete3->type = randInt(1, 100);
					if(systemeStellaires[k].planete3->type <= 5) {
						systemeStellaires[k].planete3->type = 14; //geante gazeuse
						systemeStellaires[k].planete3->habitable = 0;
					}
					else if (systemeStellaires[k].planete3->type <= 20){
						systemeStellaires[k].planete3->type = 16; //monde toxique
						systemeStellaires[k].planete3->habitable = 0;
					}					
					else if (systemeStellaires[k].planete3->type <= 60){
						systemeStellaires[k].planete3->type = 11; //monde aride froid
						systemeStellaires[k].planete3->habitable = 0;
					}
					else if (systemeStellaires[k].planete3->type <= 70){
						systemeStellaires[k].planete3->type = 4; //monde Alpine habitable
						systemeStellaires[k].planete3->habitable = 1;
					}
					else if (systemeStellaires[k].planete3->type <= 74){
						systemeStellaires[k].planete3->type = 5; //monde Arctic froid
						systemeStellaires[k].planete3->habitable = 1;
					}
					else if (systemeStellaires[k].planete3->type <= 78){
						systemeStellaires[k].planete3->type = 6; //monde Tundra habitable
						systemeStellaires[k].planete3->habitable = 1;
					}
					else if (systemeStellaires[k].planete3->type <= 82){
						systemeStellaires[k].planete3->type = 7; //monde continental froid
						systemeStellaires[k].planete3->habitable = 1;
					}
					else if (systemeStellaires[k].planete3->type <= 86){
						systemeStellaires[k].planete3->type = 8; //monde ocean
						systemeStellaires[k].planete3->habitable = 1;
					}
					else {
						systemeStellaires[k].planete3->type = 12; //monde brisé
						systemeStellaires[k].planete3->habitable = 0;
					}
					
					if(systemeStellaires[k].planete3->habitable == 1){
						strcpy(systemeStellaires[k].planete3->nom, nomPlanetes[randInt(0, (sizeof(nomPlanetes)/sizeof(nomPlanetes[0])) - 1 )]);
						//strcpy(systemeStellaires[k].planete3->nom, "habitabl");
					}
					
					systemeStellaires[k].planete3->taille = randInt(1, 100);
					if(systemeStellaires[k].planete3->taille <= 10) {
						systemeStellaires[k].planete3->taille = 2;
					}
					else if(systemeStellaires[k].planete3->taille <= 70) {
						systemeStellaires[k].planete3->taille = 3;
					}
					else if(systemeStellaires[k].planete3->taille <= 90) {
						systemeStellaires[k].planete3->taille = 4;
					}
					else {
						systemeStellaires[k].planete3->taille = 5;
					}
					if(randInt(0, 1) == 1)
					{
						systemeStellaires[k].planete3->y = 240 - (systemeStellaires[k].planete3->y - 240);
					}
					systemeStellaires[k].planete3->population = 0;
					
				case 2:
					systemeStellaires[k].planete2->habitable = 0;
					systemeStellaires[k].planete2->rayonOrbite =  randInt(30, 36);
					systemeStellaires[k].planete2->x = randInt(240 - systemeStellaires[k].planete2->rayonOrbite, 240 + systemeStellaires[k].planete2->rayonOrbite);
					systemeStellaires[k].planete2->y = sqrt(pow((double)systemeStellaires[k].planete2->rayonOrbite, 2.0) - pow((double)(systemeStellaires[k].planete2->x - 240), 2.0)) + 320;	
					
					systemeStellaires[k].planete2->type = randInt(1, 100);
					if(systemeStellaires[k].planete2->type <= 2) {
						systemeStellaires[k].planete2->type = 14; //geante gazeuse
						systemeStellaires[k].planete2->habitable = 0;
					}
					else if (systemeStellaires[k].planete2->type <= 30){
						systemeStellaires[k].planete2->type = 16; //monde toxique
						systemeStellaires[k].planete2->habitable = 0;
					}					
					else if (systemeStellaires[k].planete2->type <= 80){
						systemeStellaires[k].planete2->type = 10; //monde arid 
						systemeStellaires[k].planete2->habitable = 0;
					}
					else if (systemeStellaires[k].planete2->type <= 83){
						systemeStellaires[k].planete2->type = 7; //monde continental habitable
						systemeStellaires[k].planete2->habitable = 1;
					}
					else if (systemeStellaires[k].planete2->type <= 86){
						systemeStellaires[k].planete2->type = 8; //monde ocean habitable
						systemeStellaires[k].planete2->habitable = 1;
					}
					else if (systemeStellaires[k].planete2->type <= 89){
						systemeStellaires[k].planete2->type = 9; //monde tropical habitable
						systemeStellaires[k].planete2->habitable = 1;
					}
					else {
						systemeStellaires[k].planete2->type = 12; //monde brisé
						systemeStellaires[k].planete2->habitable = 0;
					}
					
					if(systemeStellaires[k].planete2->habitable == 1){
						strcpy(systemeStellaires[k].planete2->nom, nomPlanetes[randInt(0, (sizeof(nomPlanetes)/sizeof(nomPlanetes[0])) - 1 )]);
						//strcpy(systemeStellaires[k].planete2->nom, "habitabl");
					}
					
					systemeStellaires[k].planete2->taille = randInt(1, 100);
					if(systemeStellaires[k].planete2->taille <= 10) {
						systemeStellaires[k].planete2->taille = 2;
					}
					else if(systemeStellaires[k].planete2->taille <= 70) {
						systemeStellaires[k].planete2->taille = 3;
					}
					else if(systemeStellaires[k].planete2->taille <= 90) {
						systemeStellaires[k].planete2->taille = 4;
					}
					else {
						systemeStellaires[k].planete2->taille = 5;
					}
					if(randInt(0, 1) == 1)
					{
						systemeStellaires[k].planete2->y = 240 - (systemeStellaires[k].planete2->y - 240);
					}
					systemeStellaires[k].planete2->population = 0;
				
				case 1:
					systemeStellaires[k].planete1->habitable = 0;
					systemeStellaires[k].planete1->rayonOrbite = randInt(20, 26);
					systemeStellaires[k].planete1->x = randInt(240 - systemeStellaires[k].planete1->rayonOrbite, 240 + systemeStellaires[k].planete1->rayonOrbite);
					systemeStellaires[k].planete1->y = sqrt(pow((double)systemeStellaires[k].planete1->rayonOrbite, 2.0) - pow((double)(systemeStellaires[k].planete1->x - 240), 2.0)) + 320;	
					
					systemeStellaires[k].planete1->type = randInt(1, 100);
					if(systemeStellaires[k].planete1->type <= 3) {
						systemeStellaires[k].planete1->type = 1; //monde arid
						systemeStellaires[k].planete1->habitable = 1;
					}
					else if (systemeStellaires[k].planete1->type <= 6){
						systemeStellaires[k].planete1->type = 2; //monde Desert
						systemeStellaires[k].planete1->habitable = 1;
					}					
					else if (systemeStellaires[k].planete1->type <= 9){
						systemeStellaires[k].planete1->type = 3; //monde Savanna
						systemeStellaires[k].planete1->habitable = 1;
					}
					else if (systemeStellaires[k].planete1->type <= 40){
						systemeStellaires[k].planete1->type = 10; //monde aride
						systemeStellaires[k].planete1->habitable = 0;
					}
					else if (systemeStellaires[k].planete1->type <= 50){
						systemeStellaires[k].planete1->type = 12; //monde brisé
						systemeStellaires[k].planete1->habitable = 0;
					}
					else{
						systemeStellaires[k].planete1->type = 15; //monde en fusion
						systemeStellaires[k].planete1->habitable = 0;
					}

					if(systemeStellaires[k].planete1->habitable == 1){
						strcpy(systemeStellaires[k].planete1->nom, nomPlanetes[randInt(0, (sizeof(nomPlanetes)/sizeof(nomPlanetes[0])) - 1 )]);
						//strcpy(systemeStellaires[k].planete1->nom, "habitabl");
					}

					systemeStellaires[k].planete1->taille = randInt(1, 100);
					if(systemeStellaires[k].planete1->taille <= 10) {
						systemeStellaires[k].planete1->taille = 2;
					}
					else if(systemeStellaires[k].planete1->taille <= 70) {
						systemeStellaires[k].planete1->taille = 3;
					}
					else if(systemeStellaires[k].planete1->taille <= 90) {
						systemeStellaires[k].planete1->taille = 4;
					}
					else {
						systemeStellaires[k].planete1->taille = 5;
					}
					systemeStellaires[k].planete1->population = 0;
			}