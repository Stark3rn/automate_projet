#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "action_executor.h"

void action_afficher(const char *message) {
    printf("%s\n",message);
}

void action_attendre(const char *duree_str) {
    int duree = atoi(duree_str);
    if(duree > 0) {
        printf("Sleep %d secondes...\n",duree);
        sleep(duree);
    }
}

int action_executer_cmd(const char *command) {
    printf("Execution : %s\n",command);
    int status = system(command);

    if(status == -1) {
        fprintf(stderr,"ERR : executer_cmd");
    }
    return(status);
}

int action_log(const char *mess) {
    FILE *f = fopen("logs.log","a");
    if(f == NULL) {
        fprintf(stderr,"ERR : ouverture log file");
        return(-1);
    }
    if(fprintf(f,mess)!=0) {
        fprintf(stderr,"ERR : ecriture fichier log");
        return(-1);
    }
    fclose(f);
    return(0);
}

void execute_action(Action action) {
    switch (action.id) {
        case ACTION_AFFICHER:
            action_afficher(action.parameters);
            break;
        case ACTION_ATTENDRE:
            action_attendre(action.parameters);
            break;
        case ACTION_EXECUTER_CMD:
            action_executer_cmd(action.parameters);
            break;
        case ACTION_LOG:
            action_log(action.parameters);
            break;
        case ACTION_AUCUNE:
            break;
        default:
            fprintf(stderr,"ERR : action ID %d inconnu\n",action.id);
    }
}