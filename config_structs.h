#ifndef CONFIG_STRUCTS_H
#define CONFIG_STRUCTS_H

#include "action_types.h"
#include <limits.h>

#define END_STATE_ID 9999
#define MAX_TRANSITIONS 100

typedef struct {
    int id_etat_source;
    char evenement[50];
    int id_etat_suivant;
    ActionID action_id;
    char action_params[255];
} TransitionConfig;

typedef struct {
    TransitionConfig transitions[MAX_TRANSITIONS];
    int count;
    int current_state_id;
} AFD;

#endif