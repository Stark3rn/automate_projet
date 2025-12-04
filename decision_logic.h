#ifndef DECISION_LOGIC_H
#define DECISION_LOGIC_H

#include <stdbool.h>
#include "config_structs.h"

bool evaluate_condition(const DecisionRule *rule);

#endif