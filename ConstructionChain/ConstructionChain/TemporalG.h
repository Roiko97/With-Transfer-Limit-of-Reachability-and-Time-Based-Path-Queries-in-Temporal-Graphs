#ifndef TemporalG_Jung
#define TemporalG_Jung
#define MAXTIME 1e7
#include"InitGraph.h"
#include<fstream>
#include <algorithm>
#include<string>
#include<stack>
#include<queue>
#include<ctime>
#endif
class edgeInfo{
public:
	int id;
	int t;
	int tar;
	//int dur;
	//出发时间 车辆id 目标id  持续时间
	edgeInfo(int t, int id,int tar){
		this->id = id;  
		this->t = t;
		this->tar = tar;
		//this->dur = dur;
	}
	edgeInfo(){

	}
};
class TemporalG{
public:
	TemporalG(const InitGraph &g);
	int getVertexId(int u, int index, int startOrArrival);
	void make_of_unique(vector<edgeInfo> & data);
	int vertex;
	int edge;
	vector<vector<int>> adjcency;

	vector<vector<edgeInfo>> startT, arrivalT;

	vector<int> all_vertex_size;
	//vector<vector<int>> timeAndId;
	//vector<vector<int>> originalId;
	vector<int> timeAndId;
	vector<int>originalId;
	void add_edge(int u, int v, int id);
	void output(string s);
	int initialVertex;

	//---------------
	void initial_query();
	vector<int> sources;
	void main_earlist_arrival(const char* file);
	void init();
	queue<int> _queue;
	vector<bool> vertexInQueue;
	vector<bool>visited;
	vector<int> start_time;
	void earlist_arrival(int source, FILE * file);
	int t_start;
	vector< int > timeId;
	int t_end;
	void print_arrival(const int source, const vector<int> & time, FILE * file);

	//--------------
	vector<vector<int>> temp;
	vector<vector<int>> temp2;

};
vector<vector<bool>> connect;
TemporalG::TemporalG(const InitGraph &g){
	startT = vector<vector<edgeInfo>>(g.vertex, vector<edgeInfo>());
	arrivalT = vector<vector<edgeInfo>>(g.vertex, vector<edgeInfo>());
	connect.resize(g.vertex);
	all_vertex_size = vector<int>(g.vertex, 0);
	initialVertex = g.vertex;

	temp.resize(initialVertex);
	temp2.resize(initialVertex);

	int tmp;
	for (int i = 0; i < g.vertex; i++){
		for (int j = 0; j < g.adjcency[i].size(); j++){
			int v = g.adjcency[i][j].v;
			int weight = g.adjcency[i][j].weight;
			for (int k = 0; k < g.adjcency[i][j].interval.size(); k++){
				int t = g.adjcency[i][j].interval[k][0]; //边中的出发时间
				int id = g.adjcency[i][j].interval[k][1]; //边的id
				int dur = g.adjcency[i][j].interval[k][2]; //边中的持续时间 即旅程时间
				edgeInfo start(t, id, v);//出发边的行走时间
				startT[i].push_back(start);
				temp[i].push_back(t);
				/*
				edgeInfo arri(t + weight, id, i,dur); //到达
				arrivalT[v].push_back(arri);
				temp2[v].push_back(t + weight);
				// 在列表中 ,每一行.每一列存放的都是edgeInfo
				*/
				edgeInfo arri(t + dur, id, i);
				arrivalT[v].push_back(arri);
				temp2[v].push_back(t + dur);
			}
		}
	}
	//以上已测试
	for (int i = 0; i < g.vertex; i++){
		make_of_unique(startT[i]);
		make_of_unique(arrivalT[i]);
		sort(temp[i].begin(), temp[i].end());
		sort(temp2[i].begin(), temp2[i].end());
		connect[i].resize(startT[i].size(), false);
		int index = startT[i].size() + arrivalT[i].size();
		all_vertex_size[i] = index + (i>0 ? all_vertex_size[i - 1] : i );
	}
	//for (int i = 0; i < g.vertex; i++){
	//	for (int j = 0; j < arrivalT[i].size(); j++){
	//		timeId.push_back(arrivalT[i][j].t);
	//	}
	//	for (int j = 0; j < startT[i].size(); j++){
	//		timeId.push_back(startT[i][j].t);
	//	}
	//}
	//以上已测试
	vertex = all_vertex_size.back();
	adjcency.resize(vertex);
	edge = 0;
	start_time.resize(vertex);
	//originalId.resize(vertex);
	visited.resize(vertex);
	vertexInQueue.resize(vertex);

	//TS = clock();
	double TS = clock();
	for (int i = 0; i < g.vertex; i++){
		//tin ->tin
		//int counter = 0;
		for (int j = 1; j < arrivalT[i].size(); j++){
			int u1 = getVertexId(i, j - 1, 0);
			int u2 = getVertexId(i, j, 0);
			//int experiment = arrivalT[i][counter].id;		
			add_edge(u1, u2, arrivalT[i][j-1].id);
			//counter++;
		}
		// tout ->tout
		//counter = 0;
		for (int j = 1; j < startT[i].size(); j++){
			int v1 = getVertexId(i, j - 1, 1);
			int v2 = getVertexId(i, j, 1);
			//int textid = startT[i][j-1].id;
			//int experiment =startT[i][counter].id;
			add_edge(v1, v2, startT[i][j-1].id);
			//counter++;
		}


		int end = -1;
		//counter = 0;
		//same chain tin -> tout (到达 连接 出发）
		queue<int> a; //同一个链 未进行链接的
		for (int j = int(arrivalT[i].size()) - 1; j >= 0; j--)
		{
			int flag = false;
			int u1 = getVertexId(i, j, 0);
			for (int k = startT[i].size()-1; k >=0; k--){
				if (arrivalT[i][j].id  == startT[i][k].id){
					int u2 = getVertexId(i, k, 1);
					//cout << startT[i][k].t << "   " << arrivalT[i][j].t << endl;
					//int experiment = startT[i][counter].id;
					add_edge(u1, u2, arrivalT[i][j].id);
				//	counter++;
					connect[i][k] = true;
					flag = true;
					break;
				}
			}
			if (flag == false){
				a.push(j); //把元素下标入栈
			}

			//if (flag == false){
			//	for (int k = 0; k < startT[i].size(); k++){
			//		if (startT[i][k].t >= arrivalT[i][j].t && connect[i][k] == false){
			//			int u2 = getVertexId(i, k, 1);
			//			add_edge(u1, u2, arrivalT[i][j].id);	
			//			connect[i][k] = true;
			//			break;
			//		}
			//	}
			//}

			//int u1 = getVertexId(i, j, 0);
			//vector<int> ::iterator it = lower_bound(temp[i].begin(), temp[i].end(), arrivalT[i][j].t);
			//if (it != temp[i].end()){
			//	int index = it - temp[i].begin();

			//	if (index == end)
			//		continue;

			//	end = index;
			//	
			//	int u2 = getVertexId(i, index, 1);

			//	//int experiment = startT[i][counter].id;
			//	add_edge(u1, u2, arrivalT[i][j].id);
			////	counter++;
			//}
		}
		
		
		for (int j = 0; j < a.size(); j++){
			int index = a.front(); // j
			a.pop();
			int u1 = getVertexId(i, index, 0);

			for (int k = 0; k < startT[i].size(); k++){
				if (arrivalT[i][index].t <= startT[i][k].t && connect[i][k] == false){
					int u2 = getVertexId(i, k, 1);
					add_edge(u1, u2, arrivalT[i][j].id);
					connect[i][k] = true;
					break;
				}
			}


		}


		//difference chain tou ->tin
		int flag = -1;
	//	counter = 0;
		for (int j = 0; j < g.adjcency[i].size(); j++){
			for (int k = 0; k < g.adjcency[i][j].interval.size(); k++){


				int n = g.adjcency[i][j].interval.size();
				int v = g.adjcency[i][j].v;
				int w = g.adjcency[i][j].weight; //默认是1 
				int t = g.adjcency[i][j].interval[k][0];
				int id = g.adjcency[i][j].interval[k][1];
				int dur = g.adjcency[i][j].interval[k][2];

			/*	int index_u = lower_bound(temp[i].begin(), temp[i].end(), t) - temp[i].begin();
				int index_v = lower_bound(temp2[v].begin(), temp2[v].end(), t + w) - temp2[v].begin();*/

				//代码块
				int index_u;
				int index_v;
				for (int m = 0; m < startT[i].size(); m++){
					//if (startT[i][m].t >= t && startT[i][m].id == id)	
					//{
					//	index_u = m;
					//	break;
					//}
					if (startT[i][m].id == id)
					{
						index_u = m;
						break;
					}
				}

				for (int m = 0; m < arrivalT[v].size(); m++){
					//if (arrivalT[v][m].t >= t + dur && arrivalT[v][m].id == id){
					//	index_v = m;
					//	break;
					//}
					if (arrivalT[v][m].id == id){

						index_v = m;
						break;
					}
				}
				int _u = getVertexId(i, index_u, 1);
				int _v = getVertexId(v, index_v, 0);    
				//cout << i << "   " << _u << "   " << v <<"   "<<_v<< "  "<<id<< endl;
				//int experiment = startT[i][counter].id;
				add_edge(_u, _v, startT[i][index_u].id);
			}
		}
		//ES = clock();
		//cout << "step 5-4.的耗时  -> " << (double)(ES - TS) << " ms" << endl;
	
		//cout << "目前正在运行第 " << i << "次" << endl;
	}
	

	//vector<int> asset = adjcency[641949];
	double ES = clock();
	double endtime = (double)(ES - TS) / CLOCKS_PER_SEC;
	cout << "图转换耗时： " << endtime << " s" << endl;
}
bool cmp(edgeInfo a, edgeInfo b){
	return b.t > a.t;
	//return b.t > a.t && b.id > a.id;
}
void TemporalG::make_of_unique(vector<edgeInfo> &data){
	sort(data.begin(), data.end(),cmp);
	
	//for (int i = 1; i < data.size(); i++){
	//	if (data[i - 1].t == data[i].t){
	//		for (int j = i; j < data.size() - 1; j++){
	//			data[j] = data[j + 1];
	//		}
	//		i = 0;
	//		data.resize(data.size() - 1);
	//	}
	//}
	
	//data.erase(unique(data.begin(), data.end()), data.end());
}
int TemporalG::getVertexId(int u, int index, int startOrArrival){
	int base = u == 0 ? 0 : all_vertex_size[u - 1];
	if (startOrArrival == 0){
		return base + index;
	}
	else{
		return base + arrivalT[u].size() + index;
	}
}
void TemporalG::add_edge(int u, int v,int id){
	edge++;
	adjcency[u].push_back(id);
	adjcency[u].push_back(v);
}
void TemporalG::output(string s){

	fstream out;
	string data ="scc.txt";
	string info = "info.txt";
	cout << "正在进行输出" << data << "ing..." << endl;

	out.open(data.c_str(), ios::out);

	out << vertex << "\t" << edge << endl;

	for (int i = 0; i < vertex; i++){
		out << i << "  " << adjcency[i].size()/2 << "  ";
		for (int j = 0; j < adjcency[i].size(); j++){
			int id = adjcency[i][j];
			//out << adjcency[i][j] << "  ";
			j++;
			int v = adjcency[i][j];
			//out << adjcency[i][j] << "  ";
			out << v << "  ";
			out << id << "  ";
		}
		out << endl;
	}
	out.close();

	cout << "正在进行输出" << info << "ing.." << endl;
	out.open(info.c_str(), ios::out);
	out << initialVertex << endl;
	for (int i = 0; i < initialVertex; i++){
		out << i << "  " << arrivalT[i].size() << "  ";
		for (int j = 0; j < arrivalT[i].size(); j++)
		{
			out << arrivalT[i][j].t << "  " << arrivalT[i][j].id << "   " << arrivalT[i][j].tar << "   ";
		}
		out << endl;

	}
	for (int i = 0; i < initialVertex; i++){
		out << i << "   " << startT[i].size() << "   ";
		for (int j = 0; j < startT[i].size(); j++){
			out << startT[i][j].t << "   " << startT[i][j].id << "   " << startT[i][j].tar << "   ";
		}
		out << endl;
	}
	out.close();
}
