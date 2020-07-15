#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <debug.h>
#include <math.h>

#include <fileioc.h>

void setLangage(char languageNumber){
	localeLanguage = languageNumber;
}

char getLanguage(){
	return localeLanguage;
}

char* getText(int numero){
	
}