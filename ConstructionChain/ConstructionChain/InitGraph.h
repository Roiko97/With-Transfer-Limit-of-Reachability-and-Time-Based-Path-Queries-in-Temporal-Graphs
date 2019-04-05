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
	1.v ���ﶥ��
	2.weight �ߵ�Ȩ��
	3.size ����v����ȸ���
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

//  ����ڵ㹹��Ϊ u -> v
InitGraph::InitGraph(const char*filePath){


	fstream read;
	read.open(filePath, ios::in);
	read >> vertex; //��ȡ��vertex
	adjcency.resize(vertex);
	for (int i = 0; i < vertex; i++){
		int out_neighbor_num;
		int u;
		read >> u >> out_neighbor_num;
		adjcency[u].resize(out_neighbor_num);
		for (int j = 0; j < out_neighbor_num; j++){
			int v;
			int in_degree_num; //v�������Ŀ
			int spend; //Ĭ��Ϊ1
			read >> v >> in_degree_num >> spend;
			adjcency[i][j].init(v, spend, in_degree_num);
			for (int k = 0; k < in_degree_num; k++){
				int time;
				int id;
				int dur;
				read >> time >> id >> dur;
				adjcency[i][j].interval[k].push_back(time); //Ȩֵ
				adjcency[i][j].interval[k].push_back(id); //id
				adjcency[i][j].interval[k].push_back(dur); //����ʱ��

			}
		}
	}

	cout << "InitGraph�������" << endl;
	read.close();



	//FILE * file = fopen(filePath, "r");

	////��ȡ����
	//fscanf(file, "%d", &vertex);

	////��ʼ�����ж���
	//adjcency.resize(vertex);
	//for (int i = 0; i < vertex; i++){
	//	int out_neighbor_num;
	//	int u;
	//	fscanf(file, "%d %d", &u, &out_neighbor_num);
	//	adjcency[u].resize(out_neighbor_num);
	//	for (int j = 0; j < out_neighbor_num; j++){
	//		int v;
	//		int in_degree_num; //v�������Ŀ
	//		int spend; //Ĭ��Ϊ1
	//		fscanf(file, "%d %d %d", &v, &in_degree_num, &spend);
	//		adjcency[i][j].init(v, spend, in_degree_num);
	//		for (int k = 0; k < in_degree_num; k++){
	//			int time;
	//			int id;
	//			int dur;
	//			fscanf(file, "%d %d %d", &time, &id,&dur);// ����ʱ���ʾ ��i - > j ���ķѵ�ʱ��
	//			//adjcency[i][j].interval[k].reserve(3);
	//			adjcency[i][j].interval[k].push_back(time); //Ȩֵ
	//			adjcency[i][j].interval[k].push_back(id); //id
	//			adjcency[i][j].interval[k].push_back(dur); //����ʱ��
	//			
	//		}
	//	}

	//}
	//cout << "InitGraph�������" << endl;
	//fclose(file);

}
#endif
