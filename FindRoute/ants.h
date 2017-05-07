
#ifndef ANTS_H
#define ANTS_H

#include "stdafx.h"
#include "init.h"
#include "defines.h"
#include <vector>
#include <map>
using namespace std;

#define MAX_NODES 615

extern bool isNodesRequire[];	
extern short startNode, endNode;
extern std::map<unsigned short, unsigned short> mustGoToReqNode;
extern double  *prob_of_selection;	
extern float smell[];


struct Ant{
   std::vector<unsigned short>  tour;	// the path
   std::vector<unsigned short> req_node_idx;
   unsigned short curNode;			// the node this ant currently in
   unsigned short requireCnt;		// how many require nodes this ant has passed
   unsigned short requireLine;
   unsigned short avoidLine;
   int  tour_length;
   bool visited[MAX_NODES];
   bool stop;				// whether this ant is in stop state
};
void ant_empty_memory(Ant &k);
void allocate_ants();

extern Ant *ant;      /* this (array of) struct will hold the colony */
extern Ant *best_so_far_ant;   /* struct that contains the best-so-far ant */
extern int n_ants;
extern double  pheromone[ARRAYINF][ARRAYINF];
extern double  trail_0;         /* initial pheromone trail level in ACS */
extern double trail_max;
extern double trail_min;
extern double max_tries;
extern bool ignoreWeightMode;
extern double alpha;
extern double beta;
extern double gama;
extern double q_0;  

extern void init_pheromone_trails (const double &initial_trail);
extern bool neighbour_choose_and_move_to_next( Ant &k,vector<vector<int>>);
extern int find_best();
extern bool copy_from_to(Ant *a1, Ant *a2);
extern void local_acs_pheromone_update(Ant *k);

#endif