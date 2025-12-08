#ifndef ACTION_TYPES_H
#define ACTION_TYPES_H

typedef enum {
    ACTION_AFFICHER = 1,
    ACTION_ATTENDRE = 2,
    ACTION_EXECUTER_CMD = 3,
    ACTION_LOG = 4,
    ACTION_BLOQUER_IP = 5,
    ACTION_LIMITER_USAGE = 6,
    ACTION_AUCUNE = 99
} ActionID;

typedef struct {
    ActionID id;
    char *parameters;
} Action;

#endif