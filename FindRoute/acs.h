
#ifndef ACS_H
#define ACS_H
#include "stdafx.h"
#include "ants.h"
#include "timer.h"
#include <vector>

void update_statistics();
void global_acs_pheromone_update(Ant *k);
void punishAndReward();
extern void begin(vector<vector<int>>);

#endif