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
    afd->count = 0;
    afd->current_state_id=0;

    char line[256];

    if(fgets(line, sizeof(line),f) == NULL) {
        fprintf(stderr,"ERR : CSV vide");
        fclose(f);
        free(afd);
        return(NULL);
    }

    while(fgets(line,sizeof(line),f) != NULL && afd->count < MAX_TRANSITIONS) {
        if(strlen(line)<=1) continue;

        TransitionConfig *config = &afd->transitions[afd->count];
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
            afd->count++;
        }
    }
    fclose(f);
    return(afd);
}