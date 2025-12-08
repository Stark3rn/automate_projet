#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "csv_reader.h"

ActionID action_name_to_id(const char *name) {
    if(strcmp(name, "Afficher")== 0) return ACTION_AFFICHER;
    if(strcmp(name, "Attendre")== 0) return ACTION_ATTENDRE;
    if(strcmp(name, "Executer_Cmd")== 0) return ACTION_EXECUTER_CMD;
    if(strcmp(name, "Log")== 0) return ACTION_LOG;
    if(strcmp(name, "Bloquer_IP")== 0) return ACTION_BLOQUER_IP;
    if(strcmp(name, "Limiter_Usage")== 0) return ACTION_LIMITER_USAGE;
    return(ACTION_AUCUNE);
}


AFD *load_all_transitions(const char *filename) {
    FILE *f = fopen(filename, "r");
    if(f == NULL) {
        fprintf(stderr,"ERR : ouverture csv");
        return(NULL);
    }

    AFD *afd = (AFD *)malloc(sizeof(AFD));
    if(afd == NULL) {
        fprintf(stderr,"ERR : allocation memoire afd");
        fclose(f);
        return(NULL);
    }
    afd->transition_count = 0;
    afd->current_state_id=0;
    afd->rule_count = 0;

    char line[256];

    if(fgets(line, sizeof(line),f) == NULL) {
        fprintf(stderr,"ERR : CSV vide");
        fclose(f);
        free(afd);
        return(NULL);
    }

    while(fgets(line,sizeof(line),f) != NULL && afd->transition_count < MAX_TRANSITIONS) {
        if(strlen(line)<=1) continue;

        TransitionConfig *config = &afd->transitions[afd->transition_count];
        char *token;
        int col_index = 0;

        char temp_line[256];
        strcpy(temp_line,line);

        token = strtok(temp_line,";");
        int tokens_read = 0;
        
        while(token !=NULL) {
            if(token[0] == '"') token++;
            if(token[strlen(token)-1]=='"' || token[strlen(token)-2]=='"') {
                if(token[strlen(token)-1] == '\n') {
                    token[strlen(token)-2]='\0';
                } else {
                    token[strlen(token)-1]='\0';
                }
            };

            token[strcspn(token,"\r\n")] = 0;

            switch (col_index) {
                case 0: config->id_etat_source = atoi(token); break;
                case 2: strncpy(config->evenement,token,49); config->evenement[49] = '\0'; break;
                case 3:
                    if(strcmp(token,"Q")==0) {
                        config->id_etat_suivant = END_STATE_ID;
                    } else {
                        config->id_etat_suivant = atoi(token);
                    } break;
                case 4: config->action_id = action_name_to_id(token); break;
                case 5: strncpy(config->action_params,token,99); config->action_params[99] = '\0'; break;
            }
            token = strtok(NULL,";");
            col_index++;
            tokens_read++;
        }

        if(tokens_read >=6) {
            afd->transition_count++;
        }
    }
    fclose(f);
    return(afd);
}

int load_decision_rules(AFD *afd, const char *filename) {
    FILE *f = fopen(filename, "r");
    if(f == NULL) {
        fprintf(stderr,"ERR : ouverture DecisionTree csv\n");
        return(-1);
    }

    afd->rule_count = 0;
    char line[256];

    if(fgets(line, sizeof(line), f) == NULL) {
        fprintf(stderr,"ERR : DecisionTree CSV vide\n");
        fclose(f);
        return(-1);
    }

    while(fgets(line, sizeof(line), f) != NULL && afd->rule_count < MAX_DECISION_RULES) {
        if(strlen(line) <= 1) continue;

        DecisionRule *rule = &afd->decision_rules[afd->rule_count];
        char *token;
        int col_index = 0;
        char temp_line[256];
        strcpy(temp_line, line);

        token = strtok(temp_line, ";");
        while(token != NULL) {
            token[strcspn(token, "\r\n")] = 0;

            switch (col_index) {
                case 0: rule->id_etat = atoi(token); break;
                case 1: strncpy(rule->critere, token, 49); rule->critere[49] = '\0'; break;
                case 2: strncpy(rule->operateur, token, 4); rule->operateur[9] = '\0'; break;
                case 3: strncpy(rule->valeur, token, 49); rule->valeur[49] = '\0'; break;
                case 4: rule->action_id = action_name_to_id(token); break;
                case 5: strncpy(rule->action_params, token, 254); rule->action_params[254] = '\0'; break;
            }
            token = strtok(NULL, ";");
            col_index++;
        }
        
        if(col_index >= 6) {
            afd->rule_count++;
        }
    }
    fclose(f);
    return(0);
}