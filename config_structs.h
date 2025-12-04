#ifndef CONFIG_STRUCTS_H
#define CONFIG_STRUCTS_H

#include "action_types.h"
#include <limits.h>

#define END_STATE_ID 9999
#define MAX_TRANSITIONS 100
#define MAX_DECISION_RULES 50

typedef struct {
    int id_etat_source;
    char evenement[50];
    int id_etat_suivant;
    ActionID action_id;
    char action_params[255];
} TransitionConfig;

typedef struct {
    int id_etat;
    char critere[50];
    char operateur[5];
    char valeur[50];
    ActionID action_id;
    char action_params[255];
} DecisionRule;

typedef struct {
    TransitionConfig transitions[MAX_TRANSITIONS];
    int transition_count;
    int current_state_id;
    DecisionRule decision_rules[MAX_DECISION_RULES];
    int rule_count;
} AFD;

#endif