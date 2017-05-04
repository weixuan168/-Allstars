#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <set>
#include <map>
#include <math.h>
#include <assert.h>
#include "ACS.h"
#include "timer.h"
#include "stdlib.h"
#include "ants.h"
#include <string.h>
#include "defines.h"
#include "init.h"

using namespace std;
//
//
//

int found_best;		
int n_tours;		
int iteration;		
int max_tours = 0;		
double max_time;		
double punish_fac_for_zero;		
double punish_fac_for_notend;
int lastBest = 0;
int sameBestCnt = 0;
int rhoIdx = 9;
double rhos[10] = {0.1, 0.2, 0.3, 0.4, 0.9, 0.6, 0.7, 0.8, 0.5, 0.65};
int rhos_length = 10;
//
void place_ant_to(Ant &k, unsigned short node){
	k.curNode = node;
	k.tour.clear();
	k.tour.push_back(node);
	//k.visited[node] = true;
	// in case the require nodes include the startNode
	if(mustNodes[node]) {
		k.requireCnt++;
	}
}

void construct_solutions(){
    int k;        /* counter variable */    
    for ( k = 0 ; k < n_ants ; k++) {
		ant_empty_memory(ant[k]);
    }        
	for ( k = 0 ; k < n_ants ; k++ ){
		place_ant_to(ant[k], startNode);
	}
	int stopAnts = 0; 
    while ( stopAnts < n_ants ) {		
		for ( k = 0 ; k < n_ants ; k++ ) {
			if(ant[k].stop) continue;
			bool stop = neighbour_choose_and_move_to_next(ant[k]);			
			if(stop) {
				stopAnts++;
			}
		}		
    }  
    n_tours += 1;
}
//
void set_default_parameters() {    
    ignoreWeightMode = true;
	//n_ants         = nodeNum/1.5 + 0.5;    /* number of ants (-1 means instance size) */    
    n_ants = 100;
	alpha          = 2.0;
    beta           = 2.0;
	gama = 1.0;
    rhoIdx         =  rhos_length - 1;
    rho            = rhos[rhoIdx];
    q_0            = 0.1;
    if(nodeNum <= 5) {
		max_tries = 1;
        max_tours = 10;
    }else if(nodeNum <= 10){
		  max_tries = 2;
          max_tours = 30;
    }else{
		max_tries = 9;
        max_tours = 90;
    }    
    max_time       = 9.92;
	trail_max = 1.0 / MAX_NODES;
	trail_min = trail_max / ( 2. * nodeNum);
	trail_0 = trail_max;

	punish_fac_for_zero = 0.4;
	punish_fac_for_notend = 0.7;

	printf("ants number=%d\n", n_ants);	
	printf("set default parameters");	
}
//

//
void dfs_smell(const unsigned short& max_range,const unsigned short pre_node, 
			   const unsigned short cur_node, unsigned short cur_range, bool *visited,
			   const unsigned short& req_node) {
			
				   if(visited[cur_node]) return;
				   if(cur_node == startNode || cur_node == endNode || cur_range >= max_range) {
					  
					   visited[cur_node] = true;
				
					   int cost = distan[cur_node][pre_node];
					  double weight = 1.0 / (cur_range+0.0001);
					   map<unsigned short, double>& smell_info = smell[cur_node];
					   if(smell_info.find(req_node) != smell_info.end()) {
						   
						   smell_info[req_node] += weight;
					   }else{
						   smell_info[req_node] = weight;
					   }					  		   
					   return;
				   }				  
				   visited[cur_node] = true;
				   for(int i=0;i<nodeNum;i++){
					   int next_node = i;
					   if(distan[cur_node][i]!=0 && visited[i] == false){
					      int cost = (int)distan[cur_node][i];
						  if(cur_range + cost <= 20){
						    dfs_smell(max_range, cur_node, next_node, cur_range+cost, visited, req_node);
						  }
					   }
				   }

				   if(cur_node != pre_node) {
					   
					   int cost = distan[cur_node][pre_node];
					  // double weight = 1.0 / (cost + 0.0001);
					   double weight = 1.0 / (cur_range + cost + 0.0001);
					   map<unsigned short, double>& smell_info = smell[cur_node];
					   if(smell_info.find(req_node) != smell_info.end()) {
		
						   smell_info[req_node] += weight;
					   }else{
						   smell_info[req_node] = weight;
					   }		
				   }

}

void init_smell() {
	
	double init_smell = 0.01;
	unsigned short max_range = 25;
	bool *visited = new bool[MAX_NODES];

	
	for(int n = 0; n < mustNode_num; n++){
		for(int i=0;i<nodeNum;i++){
		  visited[i] = false;
		}		
		unsigned short cur_node = mustNodes[n];
	
		dfs_smell(max_range, cur_node, cur_node, 0, visited, cur_node);	
	
	}
	delete[] visited;

}
//

void initProgram() {			
	set_default_parameters();
	allocate_ants();	
	init_smell();
}

 void begin(){
	 initProgram();
			for (int n_try = 0; n_try <= max_tries ; n_try++ ) {
				        //init_try(n_try%10);	
						//rewardForShortPath();
						construct_solutions();						
						//update_statistics();
						//pheromone_trail_update();  
						//punishAndReward();
						//rewardForDijkstraShortPath();
						//search_control_and_statistics();
						//iteration++;	
			}	
	
		//if(elapsed_time(VIRTUAL) < max_time) {
		//	search_with_single_ant(100);		
		//}
		//	printf("\n--------- best=%d\n", best_so_far_ant->tour_length);	
		//	  int tourSize = best_so_far_ant->tour.size();
		//	  if(best_so_far_ant->isForward){
  //                 for (int i = 0; i < tourSize - 1; i++) {
  //                                              //printf("i=%d\n", i);
  //                      unsigned short from = best_so_far_ant->tour[i],
  //                      to = best_so_far_ant->tour[i+1];
  //                      //printf("%d %d \n", from, to);
  //                       record_result(matrix[from][to].linkid);
  //                 }   
  //             }else{
  //  
  //                for (int i = tourSize-1; i > 0; i--) {
  //                     //printf("i=%d\n", i);
  //                      unsigned short from = best_so_far_ant->tour[i],
  //                       to = best_so_far_ant->tour[i-1];
  //                      // printf("%d %d \n", from, to);
  //                       record_result(matrix[from][to].linkid);
  //                }   
  //          }   
	//delete[] isNodesRequire;
	//delete[] ant;
	//delete best_so_far_ant;
	//delete iteration_best_ant;
	//delete best_reverse_ant;
	//delete sub_opt_so_far_ant;
	//delete[] visited_for_ls;

 }
//
// 
///* initilialize variables appropriately when starting a trial */
//void init_try(const int& try_cnt){
//    //best_ant_weight = best_ant_weight_arrays[ (try_cnt%best_ant_weight_arrays_length)];
//	if(ignoreWeightMode){
//		alpha = alphas[try_cnt];
//		beta = betas[try_cnt];
//	}else{
//		alpha = alphas_weight[try_cnt];
//		beta = betas_weight[try_cnt];
//	}
//    //printf("init try,alpha=%.5f beta=%.5f\n", alpha, beta);  
//    rhoIdx = rhos_length - 1;
//    rho = rhos[rhoIdx];
//    sameBestCnt = 0;
//    /* Initialize variables concerning statistics etc. */
//    n_tours      = 0;
//    iteration    = 1;       
//    found_best   = 0;
//    init_pheromone_trails(trail_0);
//  
//}
//void update_statistics() {
//	if( best_so_far_ant->requireCnt >= numOfReqNodes) {
//		double p_x = exp(log(0.05)/nodes.size()); 
//		trail_min = 1. * (1. - p_x) / (p_x * (double)((8) / 2));
//		//trail_min = 0.01;
//		trail_max = 1. / (best_so_far_ant->tour_length );
//		trail_0 = trail_max;
//		trail_min = trail_max * trail_min; 
//	}
//	int iteration_best_ant = find_best(); /* iteration_best_ant is a global variable */
//	if(iteration_best_ant == -1) {
//		return;
//	}	
//    if ( ant[iteration_best_ant].tour_length < best_so_far_ant->tour_length ) {	
//		copy_from_to( &ant[iteration_best_ant], best_so_far_ant );
//		found_best = iteration;		
//    }
//
//}
//
//void pheromone_trail_update() {   
//
//	// 信息素挥发
//	set<unsigned short>::const_iterator fromIt = nodes.begin();
//	set<unsigned short>::const_iterator toIt = nodes.begin();
//	for( ; fromIt != nodes.end(); fromIt++) {
//		for( ; toIt != nodes.end(); toIt++) {
//			unsigned short from = *fromIt, to = *toIt;
//			if(from == to) continue;
//			if(matrix[from][to].cost <= 20) {
//				 pheromone[from][to] = (1 - rho) * pheromone[from][to];
//				 if(pheromone[from][to] <= trail_min) {
//					 pheromone[from][to] = trail_min;
//				 }
//			}
//		}
//	}
//	//printf("pheromone_trail_update\n");
//	acs_global_update();
//
//}
//
// void global_acs_pheromone_update(Ant *k) {  
//	 if(k->tour_length == INFTY) return;
//	// printf("acs_global_update, best length:%d\n", best_so_far_ant->tour_length);
//	double weight = n_ants;
//	//double weight = 1.0;
////	printf("best_ant_weight=%f\n", best_ant_weight);
//    double d_tau = weight / (double) k->tour_length;
//	unsigned short from, to;
//	if(k->isForward) {
//		for (int i = 0 ; i < k->tour.size()-1; i++ ) {
//			from = k->tour[i];
//			to = k->tour[i+1];
//			if(pheromone[from][to] <= trail_max){
//				pheromone[from][to] = (1. - rho) * pheromone[from][to] + rho * d_tau;		
//			}
//		}
//	}else{
//		for (int i = k->tour.size()-1 ; i > 0 ; i--) {
//			from = k->tour[i];
//			to = k->tour[i-1];
//			if(pheromone[from][to] <= trail_max){
//				pheromone[from][to] = (1. - rho) * pheromone[from][to] + rho * d_tau;		
//			}else{
//				//pheromone[from][to] = trail_max;
//			}
//		}
//	}
//}
//
// void punishAndReward() {
//	double bestR = best_so_far_ant->requireCnt / (double)best_so_far_ant->tour.size();
//	for(int i = 0; i < n_ants; i++) {
//		if(ant[i].isForward == false)continue;
//		if(ant[i].requireCnt == 0) {
//			//printf("punish ant:%d for zero\n", i);
//			unsigned short from = 0, to = 0;
//			for(int j = 0; j < ant[i].tour.size()-1; j++) {
//				from = ant[i].tour[j];
//				to = ant[i].tour[j+1];
//				if(pheromone[from][to] > trail_min) {
//						pheromone[from][to] *= punish_fac_for_zero;
//						//total[from][to] = pow(pheromone[from][to], alpha) * pow(heuristic(from,to),beta);
//				}else{
//					pheromone[from][to] = trail_min;
//				}									
//			}
//		}
//		if(ant[i].curNode != endNode) {
//			//printf("punish ant:%d for not end node\n", i);
//			unsigned short from = 0, to = 0;
//			for(int j = 0; j < ant[i].tour.size()-1; j++) {
//				from = ant[i].tour[j];
//				to = ant[i].tour[j+1];
//				if(isNodesRequire[to] == false) {
//					if(pheromone[from][to] > trail_min) {
//						pheromone[from][to] *= punish_fac_for_notend;
//						//total[from][to] = pow(pheromone[from][to], alpha) * pow(heuristic(from,to),beta);
//					}else{
//						pheromone[from][to] = trail_min;
//					}					
//				}
//			}
//		}
//		if(best_so_far_ant->requireCnt < numOfReqNodes && ant[i].curNode == endNode && ant[i].requireCnt > 0) {
//			//printf("reward for the ant:%d right path\n", i);
//			unsigned short from = 0, to = 0;
//			//double weight = ant[i].requireCnt;
//			double weight = 1.0;
//			int antTourLength = ant[i].tour_length;
//			if(antTourLength < best_so_far_ant->tour_length) {
//				antTourLength = best_so_far_ant->tour_length;
//			}
//			for(int j = 0; j < ant[i].tour.size()-1; j++) {
//				from = ant[i].tour[j];
//				to = ant[i].tour[j+1];
//				if(to == endNode || isNodesRequire[to]) {
//					if(pheromone[from][to] < trail_max){
//						pheromone[from][to] += (weight / antTourLength) * rho;
//						//total[from][to] = pow(pheromone[from][to], alpha) * pow(heuristic(from,to),beta);
//					}else{
//					//	pheromone[from][to] = trail_max;
//					}
//				}
//			}
//		}
//	}
//}