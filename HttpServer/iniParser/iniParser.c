#include "./iniParser.h"


void initiateConfiguration(Configuration *conf) {
    conf->api = 0;
    conf->kpi = 0;
}

int isLetter(char letter) {
    if (letter >= 97 && letter <= 122) {
        return 1;
    } else if (letter >= 65 && letter <= 90)
    {
        return 1;
    }
    return 0;
}

void parseIniLigne(char *ligne, Configuration *conf) {
    initiateConfiguration(conf);
    if (isLetter(ligne[0] ))
    {
        printf("%s\n", "wesh faut faire un truc");
    }
}

int parseIni(Configuration *conf) {
    initiateConfiguration(conf);
    FILE *fichier = fopen("./configuration.ini", "r");
    
    if (fichier == NULL) {
        perror("fail opening configuration file");
        return EXIT_FAILURE;
    }
    
    char ligne[9999];
    
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        printf("%s", ligne);
        parseIniLigne(ligne, conf);
    }
    
    fclose(fichier);
    return 0;
}