#ifndef InitGraph_Jung
#define InitGraph_Jung


#include<stdio.h>
#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
#include<fstream>
using namespace std;

class Edge{
public:
	int v;
	int weight;
	vector<vector<int>> interval;
	/*
	1.v 到达顶点
	2.weight 边的权重
	3.size 到达v的入度个数
	*/
	void init(int v, int weight, int size){
		this->v = v;
		this->weight = weight;
		this->interval.resize(size);
	}
	void init(int time, int size){

	}
};
class InitGraph{
public:
	InitGraph(const char* filePath);
	vector<vector<Edge>> adjcency;
	int vertex;
};

//  假设节点构成为 u -> v
InitGraph::InitGraph(const char*filePath){


	fstream read;
	read.open(filePath, ios::in);
	read >> vertex; //读取的vertex
	adjcency.resize(vertex);
	for (int i = 0; i < vertex; i++){
		int out_neighbor_num;
		int u;
		read >> u >> out_neighbor_num;
		adjcency[u].resize(out_neighbor_num);
		for (int j = 0; j < out_neighbor_num; j++){
			int v;
			int in_degree_num; //v的入度数目
			int spend; //默认为1
			read >> v >> in_degree_num >> spend;
			adjcency[i][j].init(v, spend, in_degree_num);
			for (int k = 0; k < in_degree_num; k++){
				int time;
				int id;
				int dur;
				read >> time >> id >> dur;
				adjcency[i][j].interval[k].push_back(time); //权值
				adjcency[i][j].interval[k].push_back(id); //id
				adjcency[i][j].interval[k].push_back(dur); //持续时间

			}
		}
	}

	cout << "InitGraph运行完成" << endl;
	read.close();



	//FILE * file = fopen(filePath, "r");

	////读取顶点
	//fscanf(file, "%d", &vertex);

	////初始化所有顶点
	//adjcency.resize(vertex);
	//for (int i = 0; i < vertex; i++){
	//	int out_neighbor_num;
	//	int u;
	//	fscanf(file, "%d %d", &u, &out_neighbor_num);
	//	adjcency[u].resize(out_neighbor_num);
	//	for (int j = 0; j < out_neighbor_num; j++){
	//		int v;
	//		int in_degree_num; //v的入度数目
	//		int spend; //默认为1
	//		fscanf(file, "%d %d %d", &v, &in_degree_num, &spend);
	//		adjcency[i][j].init(v, spend, in_degree_num);
	//		for (int k = 0; k < in_degree_num; k++){
	//			int time;
	//			int id;
	//			int dur;
	//			fscanf(file, "%d %d %d", &time, &id,&dur);// 持续时间表示 点i - > j 所耗费的时间
	//			//adjcency[i][j].interval[k].reserve(3);
	//			adjcency[i][j].interval[k].push_back(time); //权值
	//			adjcency[i][j].interval[k].push_back(id); //id
	//			adjcency[i][j].interval[k].push_back(dur); //持续时间
	//			
	//		}
	//	}

	//}
	//cout << "InitGraph运行完成" << endl;
	//fclose(file);

}
#endif
