#ifndef CSV_READER_H
#define CSV_READER_H

#include "config_structs.h"

AFD* load_all_transitions(const char *filename);

int load_decision_rules(AFD *afd, const char *filename);

#endif