#ifndef TemporalG_Jung
#define TemporalG_Jung
#define MAXTIME 1e7
#include"InitGraph.h"
#include<fstream>
#include <algorithm>
#include<ctime>
#include<string>
#endif
class edgeInfo{
public:
	int id;
	int t;
	edgeInfo(int t, int id){
		this->id = id;
		this->t = t;
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
	vector<vector<int>> temp;
	vector<vector<int>> temp2;
};

TemporalG::TemporalG(const InitGraph &g){
	double TS, ES;
	startT = vector<vector<edgeInfo>>(g.vertex, vector<edgeInfo>());
	arrivalT = vector<vector<edgeInfo>>(g.vertex, vector<edgeInfo>());
	all_vertex_size = vector<int>(g.vertex, 0);
	initialVertex = g.vertex;
	cout <<"TemporalG Begin :"<< g.vertex<<endl;

	// step 1.
	TS = clock();
	for (int i = 0; i < g.vertex; i++){
		for (int j = 0; j < g.adjcency[i].size(); j++){
			int v = g.adjcency[i][j].v;
			int weight = g.adjcency[i][j].weight;
			for (int k = 0; k < g.adjcency[i][j].interval.size(); k++){
				int t = g.adjcency[i][j].interval[k][0]; //边中的出发时间
				int id = g.adjcency[i][j].interval[k][1]; //边的id

				edgeInfo start(t, id);
				startT[i].push_back(start);
				edgeInfo arri(t+weight, id);
				arrivalT[v].push_back(arri);

				// 在列表中 ,每一行.每一列存放的都是edgeInfo
			}
		}
		//cout << "标记1：目前正在运行第 " << i << "次" << endl;
	}
	ES = clock();
	cout << "step 1.的耗时  -> " << (double)(ES-TS) << " ms" << endl;
	//以上已测试

	// step 2.
	TS = clock();
	for (int i = 0; i < g.vertex; i++){
		make_of_unique(startT[i]);
		make_of_unique(arrivalT[i]);
		int index = startT[i].size() + arrivalT[i].size();
		all_vertex_size[i] = index + (i>0 ? all_vertex_size[i - 1] : 0 );
	}
	ES = clock();
	cout << "step 2.的耗时  -> " << (double)(ES - TS) << " ms" << endl;

	// step 3,
	TS = clock();
	for (int i = 0; i < g.vertex; i++){
		for (int j = 0; j < arrivalT[i].size(); j++){
			timeId.push_back(arrivalT[i][j].t);
		}
		for (int j = 0; j < startT[i].size(); j++){
			timeId.push_back(startT[i][j].t);
		}
	}
	ES = clock();
	cout << "step 3.的耗时  -> " << (double)(ES - TS) << " ms" << endl;


	//以上已测试
	vertex = all_vertex_size.back();
	adjcency.resize(vertex);
	edge = 0;
	start_time.resize(vertex);
	originalId.resize(vertex);
	visited.resize(vertex);
	vertexInQueue.resize(vertex);

	temp.resize(vertex);
	temp2.resize(vertex);
	// step 4.
	TS = clock();
	for (int i = 0; i < g.vertex; i++){
		for (int j = 0; j < arrivalT[i].size(); j++){
			int u = getVertexId(i, j, 0);
			originalId[u] = i;
		}
		for (int j = 0; j < startT[i].size(); j++){
			int u = getVertexId(i, j, 1);
			originalId[u] = i;
		}
		
	}
	ES = clock();
	cout << "step 4.的耗时  -> " << (double)(ES - TS) << " ms" << endl;

	// step 5.
	for (int i = 0; i < g.vertex; i++){

		//tin ->tin

		// step 5-1.
		TS = clock();
		int counter = 0;
		for (int j = 1; j < arrivalT[i].size(); j++){
			int u1 = getVertexId(i, j - 1, 0);
			int u2 = getVertexId(i, j, 0);

		//	int experiment = arrivalT[i][counter].id;		
			add_edge(u1, u2, arrivalT[i][counter].id);
			counter++;
		}
	//	ES = clock();
		//cout << "step 5-1.的耗时  -> " << (double)(ES - TS) << " ms" << endl;
		// tout ->tout

		counter = 0;
		// step 5-2.
		TS = clock();
		for (int j = 1; j < startT[i].size(); j++){
			int v1 = getVertexId(i, j - 1, 1);
			int v2 = getVertexId(i, j, 1);
			//int textid = startT[i][j-1].id;
			//int experiment =startT[i][counter].id;
			add_edge(v1, v2, startT[i][counter].id);
			counter++;
		}
		//ES = clock();
		//cout << "step 5-2.的耗时  -> " << (double)(ES - TS) << " ms" << endl;
		int end = -1;
		counter = 0;
		//same chain tin ->tout

		// step 5-3.

		//TS = clock();

		for (int n = 0; n < startT[i].size(); n++)
			temp[i].push_back(startT[i][n].t);
		for (int j = (arrivalT[i].size() - 1); j >= 0; j--)
		{
			int u1 = getVertexId(i, j, 0);

			//vector<vector<int>> temp;
			//temp.resize(vertex);

			//for (int n = 0; n < startT[i].size(); n++)
			//	temp[i].push_back(startT[i][n].t);
			vector<int> ::iterator it = lower_bound(temp[i].begin(), temp[i].end(), arrivalT[i][j].t);
		
			if (it != temp[i].end()){

				int index = it - temp[i].begin();

				if (index == end)
					continue;
				end = index;	
				int u2 = getVertexId(i, index, 1);
				//int experiment = startT[i][counter].id;

				add_edge(u1, u2, startT[i][counter].id);
				counter++;
			}
			// 这里存在效率上的限制
		}

		//difference chain tou ->tin
		int flag = -1;
		counter = 0;
		// step 5-4.
		TS = clock();
		for (int j = 0; j < g.adjcency[i].size(); j++){
			for (int k = 0; k < g.adjcency[i][j].interval.size(); k++){
				//int n = g.adjcency[i][j].interval.size();
				int u = i;
				int v = g.adjcency[i][j].v;
				int w = g.adjcency[i][j].weight;
				int t = g.adjcency[i][j].interval[k][0];
				int id = g.adjcency[i][j].interval[k][1];

				//vector<vector<int>> temp;
				//temp.resize(vertex);

				//for (int n = 0; n < startT[i].size();n++)
				//	temp[i].push_back(startT[i][n].t);

				int index_u = lower_bound(temp[i].begin(), temp[i].end(), t) - temp[i].begin();

				//vector<vector<int>> temp2;
				//temp2.resize(vertex);

				for (int n = 0; n < arrivalT[v].size(); n++)
					temp2[v].push_back(arrivalT[v][n].t);

				int index_v = lower_bound(temp2[v].begin(), temp2[v].end(), t + w) - temp2[v].begin();

				int _u = getVertexId(u, index_u, 1);
				int _v = getVertexId(v, index_v, 0);

				//if (flag == _u)
				//	_u++;
				//flag = _u;
				//int experiment = startT[i][counter].id;
				add_edge(_u, _v, startT[i][counter].id);
				counter++;
			}
			//counter = 0;
			//flag = -1;
		}
		ES = clock();
		cout << "step 5-4.的耗时  -> " << (double)(ES - TS) << " ms" << endl;
		cout << "目前正在运行第 " << i << "次" << endl;
	}
	cout << "TemporalG运行完成" << endl;
}
bool cmp(edgeInfo a, edgeInfo b){
	return b.t < a.t && b.id>a.id;
}
void TemporalG::make_of_unique(vector<edgeInfo> &data){
	sort(data.begin(), data.end(),cmp);
	/*
	for (int i = 1; i < data.size(); i++){
		if (data[i - 1].t == data[i].t){
			for (int j = i; j < data.size() - 1; j++){
				data[j] = data[j + 1];
			}
			i = 0;
			data.resize(data.size() - 1);
		}
	}
	*/
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
	string data = s + "_scc.txt";
	string info = s + "_info.txt";
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
			out << arrivalT[i][j].t << "  " << arrivalT[i][j].id << "   ";
		}
		out << endl;

	}
	for (int i = 0; i < initialVertex; i++){
		out << i << "   " << startT[i].size() << "   ";
		for (int j = 0; j < startT[i].size(); j++){
			out << startT[i][j].t << "   " << startT[i][j].id << "   ";
		}
		out << endl;
	}
	out.close();




}