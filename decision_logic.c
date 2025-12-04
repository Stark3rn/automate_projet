#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include "decision_logic.h"

char *get_current_user() {
    struct passwd *p = getpwuid(getuid());
    
    if (p == NULL) {
        fprintf(stderr, "ERR: getpwuid failed to retrieve user information.\n");
        return strdup("unknown"); 
    }

    size_t name_len = strlen(p->pw_name);
    char *user = malloc(name_len + 1); 
    
    if (user == NULL) {
        perror("malloc");
        return strdup("unknown"); 
    }

    strncpy(user, p->pw_name, name_len);
    user[name_len] = '\0'; 
    
    return user;
}

int get_current_cpu_usage() {
    return 85;
}

int get_current_wait_time() {
    return 10;
}

bool compare_string(const char *current, const char *op, const char *target) {
    int cmp = strcmp(current, target);
    
    if (strcmp(op, "==") == 0) return cmp == 0;
    if (strcmp(op, "!=") == 0) return cmp != 0;
    
    return false; 
}

bool compare_integer(int current, const char *op, const char *target_str) {
    int target = atoi(target_str);

    if (strcmp(op, "==") == 0) return current == target;
    if (strcmp(op, "!=") == 0) return current != target;
    if (strcmp(op, ">") == 0) return current > target;
    if (strcmp(op, "<") == 0) return current < target;
    if (strcmp(op, ">=") == 0) return current >= target;
    if (strcmp(op, "<=") == 0) return current <= target;

    return false;
}

bool evaluate_condition(const DecisionRule *rule) {
    printf("Evaluating Rule: %s %s %s...\n", rule->critere, rule->operateur, rule->valeur);
    bool result = false;
    char *current_user = NULL;

    if (strcmp(rule->critere, "Utilisateur") == 0) {
        current_user = get_current_user();
        result = compare_string(current_user, rule->operateur, rule->valeur);
    } 
    else if (strcmp(rule->critere, "CPU_Usage") == 0) {
        int current_usage = get_current_cpu_usage();
        result = compare_integer(current_usage, rule->operateur, rule->valeur);
    } 
    else if (strcmp(rule->critere, "Temps_Attente") == 0) {
        int current_time = get_current_wait_time();
        result = compare_integer(current_time, rule->operateur, rule->valeur);
    } else {
        fprintf(stderr, "ERR : Critere de decision inconnu: %s\n", rule->critere);
    }

    if (current_user != NULL) {
        free(current_user);
    }
    
    return result;
}
