#ifndef INIT_H
#define INIT_H

#include "stdafx.h"
#include <string>
#include <vector>
using namespace std;


extern int nodeNum; 
extern int edgeNum;
extern int mustNode_num;
extern int mustEdge_num;
extern int avoidEdge_num;

extern vector<vector<int>> link;
extern vector<int> mustNodes;
extern vector<vector<double>> traffic;
extern vector<vector<int>> distan;

void readTxt(string file);
#endif