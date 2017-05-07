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
double rho = 0.5;
//
void place_ant_to(Ant &k, unsigned short node){
	k.curNode = node;
	k.tour.clear();
	k.tour.push_back(node);
	//k.visited[node] = true;
	// in case the require nodes include the startNode
	for(int i=0;i<mustNode_num;i++){
		if(node == mustNodes[i]){
			k.requireCnt++;
		}
	}
}

void construct_solutions(vector<vector<int>> link){
    int k;        /* counter variable */    
    for ( k = 0 ; k < n_ants ; k++) {
		ant_empty_memory(ant[k]);
    }        
	for ( k = 0 ; k < n_ants ; k++ ){
		place_ant_to(ant[k], startNode);
	}
	for ( k = 0 ; k < n_ants ; k++ ) {
	  bool stop = neighbour_choose_and_move_to_next(ant[k],link);
	  local_acs_pheromone_update( &ant[k]);
    }  
 //   n_tours += 1;
}
//
void set_default_parameters() {    
    ignoreWeightMode = true;
	//n_ants         = nodeNum/1.5 + 0.5;    /* number of ants (-1 means instance size) */    
    n_ants = 50;
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
	printf("set default parameters\n");	
}
//

//
void bfs_smell(int start, int next, unsigned short& max_range,bool flag){ //flag=true,增加权重，flag=false，减少权重
	vector<int> b(nodeNum);
	vector<int> que(nodeNum);
	int i =0;
	bool *visited = new bool[MAX_NODES];
	for(int i=0;i<nodeNum;i++){
	  visited[i] = false;
	}
	int s=0,e=0;//e:入队，s：出队
	visited[start] = true;
	que[e++] = start;//起始顶点入队
	if(next == -1){
	  smell[start] += 1;
	} else {
		if(flag==true){
		    smell[start] += 1;
	        smell[next] += 1;
		}else{
		    smell[start] += -1;
	        smell[next] += -1;
		} 
	  visited[next] = true;
	  que[e++] = next;
	}
	while (s<e)
	{
		for(int j=0;j<nodeNum;j++){
			if(mapLen[que[s]][j]!=0 && !visited[j]){
				que[e++] = j;
				visited[j] = true;
				int len = distan[start][j];
				if(next!=-1 &&  distan[start][j] > distan[next][j]){
					len =  distan[next][j];
				}
				if(len < max_range){
				    double weight = 1.0 / (len + 0.0001);
					if(flag==true){
					 smell[j] += weight;
					}else{
					  smell[j] -= weight;
					}
					/*if(j==0){
					  cout<<start<<"点对"<<j<<"点的影响："<<weight<<endl;
					  cout<<"此时smell值为:"<<smell[j]<<endl;
					}*/
				}
			}
		}
		s++;
	}
	delete[] visited;
}

void init_smell() {
	
	double init_smell = 0.01;
	unsigned short max_range = 25;

	for(int n = 0; n < mustNode_num; n++){	
		unsigned short cur_node = mustNodes[n];
			bfs_smell( mustNodes[n],-1,max_range,true);
	}
	for(int n = 0; n < mustEdge_num*2; n+=2){		
		unsigned short cur_node = mustEdges[n];
			bfs_smell( mustEdges[n],mustEdges[n+1],max_range,true);	
	}
	for(int n = 0; n < avoidEdge_num*2; n+=2){		
		unsigned short cur_node = avoidEdges[n];
		bfs_smell(avoidEdges[n],avoidEdges[n+1],max_range,false);
	}
	//for(int n=0;n<nodeNum;n++){
	//   smell[n]= smell[n]/(mustNode_num+mustEdge_num+avoidEdge_num);
	//   cout<<n<<"点 smell值:"<<smell[n]<<endl;
	//}


}
//

void initProgram() {	
	init_pheromone_trails(1);
	set_default_parameters();
	allocate_ants();	
	init_smell();
}

void begin(vector<vector<int>> link){
	 initProgram();
			for (int n_try = 0; n_try <= 5 ; n_try++ ) { //max_tries
			//	        //init_try(n_try%10);	
			//			//rewardForShortPath();
						construct_solutions(link);						
						update_statistics();
			//			//global_acs_pheromone_update( best_so_far_ant );
						punishAndReward();
						int len = best_so_far_ant->tour.size();
						cout<<"so far best tour is: ";
						for(int i=0;i<len;i++){
							cout<<" "<<best_so_far_ant->tour[i];
						}
						cout<<endl;
			/*			for(int i=0;i<nodeNum;i++){
							for(int j=0;j<nodeNum;j++){
								cout<<pheromone[i][j]<<" ";
							}
							cout<<endl;
						}*/
			//			//rewardForDijkstraShortPath();
			//			//search_control_and_statistics();
			//			//iteration++;	
			}	

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
void update_statistics() {
	//if( best_so_far_ant->requireCnt >= numOfReqNodes) {
	//	double p_x = exp(log(0.05)/nodes.size()); 
	//	trail_min = 1. * (1. - p_x) / (p_x * (double)((8) / 2));
	//	//trail_min = 0.01;
	//	trail_max = 1. / (best_so_far_ant->tour_length );
	//	trail_0 = trail_max;
	//	trail_min = trail_max * trail_min; 
	//}
	int iteration_best_ant = find_best(); /* iteration_best_ant is a global variable */
	if(iteration_best_ant == -1) {
		return;
	}	
	//int len = ant[iteration_best_ant].tour_length;
	//cout<<"so far best tour is: ";
	//for(int i=0;i<len;i++){
	//	cout<<" "<<ant[iteration_best_ant].tour[i];
	//}
		copy_from_to( &ant[iteration_best_ant], best_so_far_ant );
		found_best = iteration;
}
//
//void pheromone_trail_update() {   
//	// 信息素挥发
//	for(int i=0;i<nodeNum;i++){
//		for(int j=0;j<nodeNum;j++){
//			if(i==j) continue;
//			pheromone[i][j] = (1-rho) * pheromone[i][j];
//			if(pheromone[i][j] <= trail_min) {
//			   pheromone[i][j] = trail_min;
//		    }
//		}
//	}
//	//printf("pheromone_trail_update\n");
//	global_acs_pheromone_update( best_so_far_ant );
//
//}
//
 void global_acs_pheromone_update(Ant *k) {  
	 if(k->tour_length == INF) return;
	// printf("acs_global_update, best length:%d\n", best_so_far_ant->tour_length);
	double weight = n_ants;
	//double weight = 1.0;
    //printf("best_ant_weight=%f\n", best_ant_weight);
    double d_tau = weight / (double) k->tour_length;
	unsigned short from, to;
    for (int i = 0 ; i < k->tour.size()-1; i++ ) {
	  from = k->tour[i];
	  to = k->tour[i+1];
	  if(pheromone[from][to] <= trail_max){
		pheromone[from][to] = (1 - rho) * pheromone[from][to] + rho * d_tau;		
	  }
	}
}
//
 void punishAndReward() {
	double bestR = best_so_far_ant->requireCnt / (double)best_so_far_ant->tour.size();
	//cout<<"start punish and reward:"<<endl;
	for(int i = 0; i < n_ants; i++) {
		if(ant[i].requireCnt == 0&&ant[i].requireLine==0) {
			//printf("punish ant:%d for zero requireCnt\n", i);
			unsigned short from = 0, to = 0;
			for(int j = 0; j < ant[i].tour.size()-1; j++) {
				from = ant[i].tour[j];
				to = ant[i].tour[j+1];
				if(pheromone[from][to] > trail_min) {
						pheromone[from][to] *= punish_fac_for_zero;
						//total[from][to] = pow(pheromone[from][to], alpha) * pow(heuristic(from,to),beta);
				}else{
					pheromone[from][to] = trail_min;
				}									
			}
		}
		if(ant[i].curNode != nodeNum-1||ant[i].avoidLine>0) {
			//cout<<ant[i].curNode;
			//printf("punish ant:%d for not end node\n", i);
			unsigned short from = 0, to = 0;
			bool isPunish=true;
			for(int j = 0; j < ant[i].tour.size()-1; j++) {
				from = ant[i].tour[j];
				to = ant[i].tour[j+1];
				
				if(isNodesRequire[to] == false) {
					for(int i=0;i<mustEdge_num;i++){
						if(from==mustEdges[i*2]&&to==mustEdges[i*2+1]||from==mustEdges[i*2+1]&&to==mustEdges[i*2] ){
							isPunish=false;
							break;
						}
					}
					if(isPunish && pheromone[from][to] > trail_min) {
							pheromone[from][to] *= punish_fac_for_notend;
							//total[from][to] = pow(pheromone[from][to], alpha) * pow(heuristic(from,to),beta);
					}
					isPunish = true;
				}
				for(int i=0;i<avoidEdge_num;i++){
					if(from==avoidEdges[i*2]&&to==avoidEdges[i*2+1]||from==avoidEdges[i*2+1]&&to==avoidEdges[i*2]){
						pheromone[from][to] *= punish_fac_for_notend;
					}
				}
			}
		}

		else if( ant[i].requireCnt > 0||ant[i].requireLine>0) {
			//printf("reward for the ant:%d right path\n", i);
			unsigned short from = 0, to = 0;
			//double weight = ant[i].requireCnt;
			double weight = ant[i].requireCnt+ant[i].requireLine;
			int antTourLength = ant[i].tour_length;
			if(antTourLength < best_so_far_ant->tour_length) {
				antTourLength = best_so_far_ant->tour_length;
			}
			for(int j = 0; j < ant[i].tour.size()-1; j++) {
				from = ant[i].tour[j];
				to = ant[i].tour[j+1];
					if(pheromone[from][to] < trail_max){
						pheromone[from][to] += (weight / antTourLength) * 0.5;
						//total[from][to] = pow(pheromone[from][to], alpha) * pow(heuristic(from,to),beta);
					}else{
					//	pheromone[from][to] = trail_max;
					}
			}
		}
	}
}