#ifndef INIT_H
#define INIT_H

#include "stdafx.h"
#include <string>
#include <vector>
#include "defines.h"
using namespace std;


extern int nodeNum; 
extern int edgeNum;
extern int mustNode_num;
extern int mustEdge_num;
extern int avoidEdge_num;

extern vector<vector<int>> link;
extern vector<int> mustNodes;
extern vector<int> mustEdges;
extern vector<int> avoidEdges;


extern int distan[ARRAYINF][ARRAYINF];
extern int mapLen[ARRAYINF][ARRAYINF];

vector<vector<int>> readTxt(string file);
#endif