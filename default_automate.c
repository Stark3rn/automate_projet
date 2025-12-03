#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_reader.h"
#include "action_executor.h"

void default_exec(AFD *afd) {
    if(afd == NULL || afd->count == 0) return;

    while(afd->current_state_id != END_STATE_ID) {
        int current_id= afd->current_state_id;
        int transition_found = 0;

        for(int i=0;i<afd->count;i++) {
            TransitionConfig *t = &afd->transitions[i];

            if(t->id_etat_source == current_id) {
                Action current_action;
                current_action.id = t->action_id;
                strcpy(current_action.parameters,t->action_params);
                execute_action(current_action);

                afd->current_state_id = t->id_etat_suivant;

                transition_found = 1;
                break;
            }
        }
        
        if(!transition_found && afd->current_state_id !=END_STATE_ID) {
            fprintf(stderr,"ERR : blocage (etat %d = pas de transition)\n",current_id);
        }

        if(afd->current_state_id == END_STATE_ID) {
            printf("[END] Etat final.\n");
            exit(0);
        }
    }
}