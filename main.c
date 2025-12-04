#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_reader.h"
#include "action_executor.h"
#include "default_automate.h"

void read_afd(AFD *afd) {
    if(afd == NULL || afd->transition_count == 0) return;

    for(int i=0;i<afd->transition_count;i++) {
        TransitionConfig *t = &afd->transitions[i];
        printf("Transition n°%d\n",i);
        printf("Id : %d\tEvent = %s\tId_Etat_suivant = %d\taction_id = %d\taction_params = %s\t\n",t->id_etat_source,t->evenement,t->id_etat_suivant,t->action_id,t->action_params);
    }

}

void test_afd(AFD *afd) {
    if(afd == NULL || afd->transition_count == 0) return;

    for(int i=0;i<afd->transition_count;i++) {
        TransitionConfig *t = &afd->transitions[i];
        Action action = {t->action_id,t->action_params};
        printf("Transition n°%d\n",i);
        printf("Id : %d\tEvent = %s\tId_Etat_suivant = %d\taction_id = %d\taction_params = %s\t\n",t->id_etat_source,t->evenement,t->id_etat_suivant,t->action_id,t->action_params);
        execute_action(action);
    }

}

int main(int argc, char **argv) {
    char *csv_file = malloc(sizeof(char)*255);
    if(argc == 2) {
        strcpy(csv_file,argv[1]);    
    } else {
        strcpy(csv_file,"automates/afd.csv");
    }
    
    printf("Loading transitions...\n");
    AFD *afd = load_all_transitions(csv_file);

    if(afd == NULL) {
        fprintf(stderr,"ERR : chargement automate");
        free(csv_file);
        return(1);
    }

    printf("Loading decision rules...\n");
    if(load_decision_rules(afd, "decision_trees/DecisionTree.csv") == -1) {
       fprintf(stderr,"ERR : chargement DecisionTree echoue...\n");
    }

    printf("Executing default automate...\n");
    default_exec(afd);

    free(afd);
    return(0);
}