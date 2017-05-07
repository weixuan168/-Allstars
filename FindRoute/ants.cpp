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
#include <iostream>

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
double q_0;  
short numOfReqNodes = 0;

bool isNodesRequire[ARRAYINF];	
short startNode, endNode;
std::map<unsigned short, unsigned short> mustGoToReqNode;
double  *prob_of_selection;	
float smell[ARRAYINF];

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
		ant[i].requireLine=0;
		ant[i].avoidLine=0;
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
 //计算当前节点到下一节点转移的概率
float Transition(int i, int j)
{
	if (i != j)
	{
		if(j==nodeNum-1){
		  return 1;
		}
		return (pow(pheromone[i][j],1) * pow(1.0/distan[j][nodeNum-1],-1) * pow(smell[j],gama));
		//return (pow(pheromone[i][j],1) * pow(smell[j],gama));
	}
	else
	{
		return 0.0;
	}	
}
bool neighbour_choose_and_move_to_next(Ant &k,vector<vector<int>> link) {  
	int nextCity=-1;
	int count=1;

	do{
		double q = rand()/(double)RAND_MAX;
		//如果 q <= q0,按先验知识，否则则按概率转移，
		if (q <= qzero)
		{
			double probability = -1.0;//转移到下一节点的概率
			int linkNum=link[k.curNode].size();
			for(int i = 0; i < linkNum; i++)
			{   
				
				double prob = Transition(k.curNode, link[k.curNode][i]);
				if (prob  > probability)
				{
					nextCity = link[k.curNode][i];
					probability = prob;
				}
			}
		}
		else
		{
			//按概率转移			
			double p = rand()/(double)RAND_MAX;//生成一个随机数,用来判断落在哪个区间段
			double sum = 0.0;			
			float probability = 0.0;//概率的区间点，p 落在哪个区间段，则该点是转移的方向
			//计算概率公式的分母的值
			int linkNum=link[k.curNode].size();
			for(int i = 0; i < linkNum; i++)
			{			
				sum += Transition(k.curNode, link[k.curNode][i]);
			} 
			for(int j = 0; j < linkNum; j++)
			{
								
				probability += Transition(k.curNode, link[k.curNode][j])/sum;
				
				if (probability >= p || (p > 0.9999 && probability > 0.9999))
				{
					nextCity = link[k.curNode][j];
					break;
				}
			}	
		}	
		for(int i=0;i<mustEdge_num;i++){
			if(k.curNode==mustEdges[i*2]&&nextCity==mustEdges[i*2+1] || nextCity==mustEdges[i*2]&&k.curNode==mustEdges[i*2+1]){
				k.requireLine++;
			}
		}
		for(int i=0;i<avoidEdge_num;i++){
			if(k.curNode==avoidEdges[i*2]&&nextCity==avoidEdges[i*2+1]){
				k.avoidLine++;
			}
		}
		k.tour_length += mapLen[k.curNode][nextCity];
		k.curNode = nextCity;
		k.tour.push_back(nextCity);
		for(int i=0;i<mustNode_num;i++){
			if(mustNodes[i]==nextCity){
				k.requireCnt++;
			}
		}
		count++;
	}while(nextCity!=(nodeNum-1)&&count!=nodeNum);
	//for(int i=0;i<k.tour.size();i++){
	//	cout<<k.tour[i]<<" ";
	//}
	//cout<<endl;
	if(nextCity!=(nodeNum-1)&&count==nodeNum){
		return false;
	}
	return true;
}

 int find_best() {
    int   min = INF;
    int   k, k_min = -1;
	int   require = 0;
	int   maxrequire = 0;
    for( k = 0 ; k < n_ants ; k++ ) {	
		//cout<<ant[k].requireCnt<<" "<<ant[k].requireLine<<" "<<ant[k].avoidLine<<endl;
		require = ant[k].requireCnt + ant[k].requireLine + ant[k].avoidLine;
		
		if((ant[k].curNode == nodeNum-1) && require > maxrequire) {
			//cout<<"require:"<<require<<" maxrequire:"<<maxrequire<<endl;
			maxrequire = require;
			k_min = k;
			min = ant[k].tour_length;
		}
		/*if((ant[k].curNode == nodeNum-1) && ant[k].requireCnt >= numOfReqNodes && ant[k].tour_length < min ) {
			min = ant[k].tour_length;
			k_min = k;
		}*/
    }
	cout<<"best tour length  is:"<<k_min<<endl;
    return k_min;
}

  bool copy_from_to(Ant *a1, Ant *a2) {
	 if(a1 == a2 || a1 == NULL || a2 == NULL) return false;
	 a2->req_node_idx.clear();
	 a2->req_node_idx.insert(a2->req_node_idx.end(), 
		 a1->req_node_idx.begin(), a1->req_node_idx.end());
	 a2->curNode = a1->curNode;
	 a2->requireCnt = a1->requireCnt;
	 a2->stop = a1->stop;
	 a2->tour_length = a1->tour_length;
	 a2->tour.clear();
	 a2->tour.insert(a2->tour.begin(), a1->tour.begin(), a1->tour.end());
	 return true;
	 /*
	 for(int i = 0; i < MAX_NODES; i++) {
		 a2.visited[i] = a1.visited[i];
	 }
	 */
 }

 void local_acs_pheromone_update(Ant *k) {  
double weight = n_ants;
	//double weight = 1.0;
    //printf("best_ant_weight=%f\n", best_ant_weight);
    double d_tau = weight / (double) k->tour_length;
	unsigned short from, to;
    for (int i = 0 ; i < k->tour.size()-1; i++ ) {
	  from = k->tour[i];
	  to = k->tour[i+1];
	  if(pheromone[from][to] <= trail_max){
		pheromone[from][to] = (1 - 0.5) * pheromone[from][to] + 0.5 * d_tau;		
	  }
	}
	
}