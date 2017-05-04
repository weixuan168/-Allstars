// FindRoute.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <iostream>
#include "init.h"

using namespace std;


void main()
{
	cout<<"请输入要读取的文件：";
	string file;
	cin>>file;
    readTxt(file);
	system("pause");

}

