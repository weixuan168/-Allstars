#include "stdafx.h"
#include "stdafx.h"
#include "ants.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <map>
//#include "direct_search.h"
#include "init.h"
#include "ants.h"
#include "defines.h"
#include <set>

using namespace std;

Ant *ant;
Ant *best_so_far_ant;   /* struct that contains the best-so-far ant */
int n_ants;
double  pheromone[ARRAYINF][ARRAYINF];
double  trail_0;         /* initial pheromone trail level in ACS */
double trail_max;
double trail_min;
double max_tries;
bool ignoreWeightMode;
double alpha;
double beta;
double gama;
double rho;
double q_0;  

bool isNodesRequire[MAX_NODES];	
short startNode, endNode;
std::map<unsigned short, unsigned short> mustGoToReqNode;
double  *prob_of_selection;	
std::map<unsigned short,double> smell[MAX_NODES];

void ant_empty_memory(Ant &k){
	k.stop =false;
	k.requireCnt = 0;
	k.tour.clear();
	k.tour_length = 0;
}
 void allocate_ants() {	     
    if((ant = new Ant[n_ants]) == NULL){
		printf("Out of memory, exit.");
		exit(1);
    }
	for(int i = 0; i < n_ants; i++) {
		ant[i].curNode = startNode;
		ant[i].requireCnt = 0;
		ant[i].stop = false;
		ant[i].tour_length = 0;

	}
	if((best_so_far_ant = new Ant) == NULL){
		printf("Out of memory, exit.");
		exit(1);
    }
	 best_so_far_ant->tour_length = INF;
	 best_so_far_ant->requireCnt = 0;
	 best_so_far_ant->curNode = 900;
	 best_so_far_ant->isForward = true;
 }

void init_pheromone_trails(const double &initial_trail ) {
    //printf(" init trails with %.5f\n",initial_trail);
    /* Initialize pheromone trails */
	for(int i=0;i<nodeNum;i++){
		for(int j=0;j<nodeNum;j++){
			pheromone[i][j]=initial_trail;
		}
	}
}

 bool neighbour_choose_and_move_to_next(Ant &k) {  
  int nextCity = -1;		
	double q = rand()/(double)RAND_MAX;
	//如果 q <= q0,按先验知识，否则则按概率转移，
	if (q <= qzero)
	{
		double probability = -1.0;//转移到下一节点的概率
		for(int i = 0; i < N; i++)
		{
			//去掉禁忌表中已走过的节点,从剩下节点中选择最大概率的可行节点
			if (1 == allowed[i])
			{
				double prob = antColony->Transition(cururentCity, i);
				if (prob  > probability)
				{
					nextCity = i;
					probability = prob;
				}
			}
		}
	}
	else
	{
		//按概率转移			
		double p = rand()/(double)RAND_MAX;//生成一个随机数,用来判断落在哪个区间段
		double sum = 0.0;			
		double probability = 0.0;//概率的区间点，p 落在哪个区间段，则该点是转移的方向
		//计算概率公式的分母的值
		for(int i = 0; i < N; i++)
		{
			if (1 == allowed[i])
			{
				sum += antColony->Transition(cururentCity, i);
			}
		}
		for(int j = 0; j < N; j++)
		{
			if (1 == allowed[j] && sum > 0)
			{
				probability += antColony->Transition(cururentCity, j)/sum;
				if (probability >= p || (p > 0.9999 && probability > 0.9999))
				{
					nextCity = j;
					break;
				}
			}
		}	
	}	
	return nextCity;
}

