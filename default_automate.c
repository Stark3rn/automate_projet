#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_reader.h"
#include "action_executor.h"
#include "decision_logic.h"

void default_exec(AFD *afd) {
    if(afd == NULL || afd->transition_count == 0) return;

    while(afd->current_state_id != END_STATE_ID) {

        int current_id= afd->current_state_id;
        int transition_found = 0;

        for(int i=0;i<afd->transition_count;i++) {  
            TransitionConfig *t = &afd->transitions[i];

            if(t->id_etat_source == current_id) {
                int found_rule = 0;
                for(int j=0;j<afd->rule_count;j++) {
                    if(afd->decision_rules[j].id_etat == t->id_etat_source) {
                        found_rule = 1;
                        DecisionRule *rule = &afd->decision_rules[j];
                        if (evaluate_condition(rule)) {
                            
                            printf("%s %s %s : VRAI\n", rule->critere,
                                                    rule->operateur,
                                                    rule->valeur);

                            Action current_action;
                            current_action.id = rule->action_id;
                            current_action.parameters = rule->action_params;
                            
                            printf("Decision rule MET: executing action %d...\n", rule->action_id);
                            execute_action(current_action);
                            afd->current_state_id = t->id_etat_suivant;

                            transition_found = 1;
                            break;
                        } else {
                            printf("%s %s %s : FAUX. Decision annulee.\n",rule->critere,rule->operateur,rule->valeur);
                        }
                    }
                }
                Action current_action;
                current_action.id = t->action_id;
                current_action.parameters = t->action_params;

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