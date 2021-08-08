#ifndef _LOCALE_INCLUDE

#define _LOCALE_INCLUDE


enum _localeLanguageEnum {
    LC_FR, LC_EN
};
typedef enum _localeLanguageEnum localeLanguageEnum;

enum _local_keys {
    lc_none, lc_load, lc_new_game, lc_settings, lc_exit, lc_save, lc_back, lc_create_galaxie
};

typedef enum _local_keys local_keys;

#define LOCALE_ERROR 0

#define MAX_VALUE_LENGTH 100


void setLanguage(char languageNumber);

char getLanguage();
char initializeLanguage();
char* gettext(local_keys numero);

#endif