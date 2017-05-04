#include "stdafx.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <cassert>
#include <string>
#include <sstream>
#include "defines.h"
#include "init.h"

using namespace std;
int nodeNum; 
int edgeNum;
int mustNode_num;
int mustEdge_num;
int avoidEdge_num;

vector<vector<int>> link;
vector<int> mustNodes;
vector<vector<double>> traffic;
vector<vector<int>> distan;

extern void readTxt(string file)
{
	 

	ifstream infile; 
	infile.open(file.data());   //将文件流对象与文件连接起来 
	assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 

	string s;
	getline(infile,s);
	string str1,str2,str3,str4,str5;
	istringstream is(s);
	is>>str1>>str2>>str3>>str4>>str5;
	nodeNum=atoi(str1.c_str());           //得到总节点数
	edgeNum=atoi(str2.c_str());           //得到总边数
	mustNode_num=atoi(str3.c_str());      //得到必经点数
	mustEdge_num=atoi(str4.c_str());      //得到必经边数
	avoidEdge_num=atoi(str5.c_str());     //得到不能经过边数

	int node1;
	int node2;
	int edgeLen;
	for(int i=0;i<edgeNum;i++){                     //循环读取边：点1，点2，边距
		getline(infile,s);
		node1=atoi(s.substr(0,s.find(" ")).c_str());
		string st=s.substr(s.find(" ")+1);
		node2=atoi(st.substr(0,st.find(" ")).c_str());
		edgeLen=atoi(st.substr(st.find(" ")+1).c_str());
		link[node1].push_back(node2);
		link[node2].push_back(node1);
		distan[node1][node2]=edgeLen;
	}

	for(int i=0;i<nodeNum;i++){
		for(int j=0;j<nodeNum;j++){
			if(distan[i][j]==0&&i!=j){
				distan[i][j]=INF;
			}
		}
	}

	getline(infile,s);                            //读取必经点
	istringstream is1(s);
	for(int i=0;i<mustNode_num;i++){
		mustNodes.push_back(atoi(s.substr(0,s.find(" ")).c_str()));
		s=s.substr(s.find(" ")+1);
	}

	for(int i=0;i<mustEdge_num;i++){              //循环读取必经边
		getline(infile,s);
		node1=atoi(s.substr(0,s.find(" ")).c_str());
		node2=atoi(s.substr(s.find(" ")+1).c_str());
		traffic[node1][node2]=1;
	}

	for(int i=0;i<avoidEdge_num;i++){            //循环读取不可经过的边
		getline(infile,s);
		node1=atoi(s.substr(0,s.find(" ")).c_str());
		node2=atoi(s.substr(s.find(" ")+1).c_str());
		traffic[node1][node2]=0;
	}
	infile.close();             //关闭文件输入流 

	for(int i=0;i<nodeNum;i++){
		int tmp=link[i].size();
		for (int j = 0; j < tmp; j++)
		{
			cout<<link[i][j];
			cout<<" ";
		}
		cout<<""<<endl;
	}
	cout<<endl;
	for(int i=0;i<nodeNum;i++){
		for (int j = 0; j < nodeNum; j++)
		{
			cout<<traffic[i][j];
			cout<<" ";
		}
		cout<<""<<endl;
	}
	cout<<endl;
	for(int i=0;i<nodeNum;i++){
		for (int j = 0; j < nodeNum; j++)
		{
			cout<<distan[i][j];
			cout<<" ";
		}
		cout<<""<<endl;
	}
}
