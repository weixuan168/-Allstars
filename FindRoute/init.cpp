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
	infile.open(file.data());   //���ļ����������ļ��������� 
	assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 

	string s;
	getline(infile,s);
	string str1,str2,str3,str4,str5;
	istringstream is(s);
	is>>str1>>str2>>str3>>str4>>str5;
	nodeNum=atoi(str1.c_str());           //�õ��ܽڵ���
	edgeNum=atoi(str2.c_str());           //�õ��ܱ���
	mustNode_num=atoi(str3.c_str());      //�õ��ؾ�����
	mustEdge_num=atoi(str4.c_str());      //�õ��ؾ�����
	avoidEdge_num=atoi(str5.c_str());     //�õ����ܾ�������

	int node1;
	int node2;
	int edgeLen;
	for(int i=0;i<edgeNum;i++){                     //ѭ����ȡ�ߣ���1����2���߾�
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

	getline(infile,s);                            //��ȡ�ؾ���
	istringstream is1(s);
	for(int i=0;i<mustNode_num;i++){
		mustNodes.push_back(atoi(s.substr(0,s.find(" ")).c_str()));
		s=s.substr(s.find(" ")+1);
	}

	for(int i=0;i<mustEdge_num;i++){              //ѭ����ȡ�ؾ���
		getline(infile,s);
		node1=atoi(s.substr(0,s.find(" ")).c_str());
		node2=atoi(s.substr(s.find(" ")+1).c_str());
		traffic[node1][node2]=1;
	}

	for(int i=0;i<avoidEdge_num;i++){            //ѭ����ȡ���ɾ����ı�
		getline(infile,s);
		node1=atoi(s.substr(0,s.find(" ")).c_str());
		node2=atoi(s.substr(s.find(" ")+1).c_str());
		traffic[node1][node2]=0;
	}
	infile.close();             //�ر��ļ������� 

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
