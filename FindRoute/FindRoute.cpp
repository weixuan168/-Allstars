// FindRoute.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <iostream>
#include "init.h"
#include "acs.h"

using namespace std;


void main()
{
	cout<<"请输入要读取的文件：";

    
	begin(readTxt("e://test.txt"));
	system("pause");

}

