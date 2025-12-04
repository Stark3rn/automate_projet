#ifndef DECISION_TYPES_H
#define DECISION_TYPES_H

typedef enum {
    DECISION_UTILISATEUR = 1,
    DECISION_TEMPS_ATTENTE = 2,
    DECISION_CPU_USAGE = 3,
} DecisionID;

typedef struct {
    DecisionID critere;
    char comparator[5];
    char valeur[50];
} Decision;

#endif