#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
#include<string>
#include<stack>
#include<cstdio>
#include<ctime>
#include<fstream>
using namespace std;


class Graph{
public:
	int target;  //目标顶点  包括起始顶点和到达顶点
	int transfer; //换乘 表示为车辆id
	Graph(int _target, int _transfer){
		target = _target;
		transfer = _transfer;
	}
};
class EdgeInfo{
public:
	int time; // 时间 包括到达时间和出发时间
	int transfer; // 换成 表示为车辆id
	int target; //
	//int duration; //持续时间
	EdgeInfo(int _time,int _transfer, int _target){
		time = _time;
		transfer = _transfer;
		target = _target;
	}
};
//class BOOL{
//public:
//	int ibool;
//	BOOL(){
//		ibool = true;
//	}
//};
vector< int > sources;

class degree{
public:
	int d; // sum of in_neighbor and out_neighbor
	int index; //记录id
};
vector<degree> deg;
bool cmp(degree d1, degree d2);


int vertex_num, edge_num;
int chainNum;
vector<vector<Graph>> v;  //代表点的出度信息记录
vector<vector<Graph>> rv; //代表点的入度信息记录
vector<int> ind;
vector<int> outd;
vector<bool> visited;
vector <bool> vertexInQueue;
vector<int> originPath; //最原始的op
vector<vector<int>> linMapper;
vector<vector<int>> loutMapper;
//vector<int> iTopoOrigin; //拓扑后的链
void readGraph(string s,string s2){  
	fstream _read;

	_read.open(s2.c_str(), ios::in);
	_read >> chainNum;
	_read.close();
	//chainNum = 5;

	_read.open(s.c_str(), ios::in);
	_read >> vertex_num >> edge_num;


	//vertex
	v.resize(vertex_num);
	//re vertex
	rv.resize(vertex_num);
	// indata
	ind.resize(vertex_num);
	// outdata
	outd.resize(vertex_num);

	//计算换乘、查询用
	//vertexInQueue.resize(vertex_num);
	visited.resize(vertex_num);
	vertexInQueue.resize(vertex_num);
	//visited.resize(chainNum);

	vector<vector<Graph>> tmp_rv; //代表点的入度信息记录
	tmp_rv.resize(vertex_num);

	int _vId;
	int _size;
	int _target;
	int _transfer;
	int flag = 0;
	cout << "Start getting data" << endl;
	while (_read >> _vId){
		//if (_vId == 1000){
		//	cout << 1 << endl;
		//	break;
		//}
		_read >> _size;
		//v[_vId].resize(_size);
		for (int i = 0; i < _size; i++){
			_read >> _target;
			_read >> _transfer;

			//cout << i <<" _vId = " << _vId << "  " << "target = " << _target << "  " << "transfer = " << _transfer << endl;
			Graph gv(_target, _transfer);
			v[_vId].push_back(gv);
			//v[_vId][i].target = _target;
			//v[_vId][i].transfer = _transfer;

			Graph grv(_vId, _transfer);
			//rv[_target].resize(rv[_target].size() + 1);
			//rv[_target][rv[_target].size()-1].target = _vId;
			//rv[_target][rv[_target].size() - 1].transfer = _transfer;
			rv[_target].push_back(grv);


			outd[_vId]++;
			ind[_target]++;
		}
	}
	cout << ind[625908] << endl;
	cout << ind[625909] << endl;
	cout << ind[671736] << endl;
	_read.close();
	cout << "Graph of scc already load done." << endl;
}  //已测试

vector <int> inOut;
vector <int> pNext;
vector<vector<EdgeInfo>> startT, arrivalT;
vector< int > curSize, sumSize;
vector< int > originalId;
vector<int> vectorTr; //结点的transfer 


vector<int> toChain;
vector<int> toPos;
vector<vector<int> > toV;


//vector<int> iArrival;
void vCalTransferByBFS();
void readGraphInfo(string s){
	inOut.resize(vertex_num);
	pNext.resize(vertex_num);

	fstream _read;
	_read.open(s.c_str(), ios::in);

	// 链的数量
	_read >> chainNum;

	startT = vector< vector<EdgeInfo> >(chainNum, vector<EdgeInfo>());
	arrivalT = vector< vector<EdgeInfo> >(chainNum, vector<EdgeInfo>());
	curSize = vector<int>(chainNum, 0);
	sumSize = vector<int>(chainNum, 0);

	//iTopoOrigin.resize(chainNum);

	originalId.resize(vertex_num);
	toV.resize(vertex_num);
	toChain.resize(vertex_num);
	toPos.resize(vertex_num);
	linMapper.resize(vertex_num);
	loutMapper.resize(vertex_num);

	int _id, _size, _time, _transfer, _target;//_duration;
	for (int i = 0; i < chainNum; i++){
		_read >> _id >> _size;
		//下述优化,此处注解
		//arrivalT[i].resize(_size);
		for (int j = 0; j < _size; j++){
			_read >> _time;
			_read >> _transfer;
			_read >> _target;
			//_read >> _duration;
			//_duration = 1;

			//优化
			EdgeInfo ei(_time, _transfer,_target);
			arrivalT[i].push_back(ei);

			//被优化
			//arrivalT[i][j].time = _time;
			//arrivalT[i][j].transfer = _transfer;
		}
	}
	for (int i = 0; i < chainNum; i++){
		_read >> _id >> _size;
		//下述优化 此处注解
		//startT[i].resize(_size);
		for (int j = 0; j < _size; j++){
			_read >> _time;
			_read >> _transfer;
			_read >> _target;
			//_read >> _duration;
			//_duration = 1;

			//优化
			EdgeInfo e(_time, _transfer,_target);
			startT[i].push_back(e);  // startT的duration无实际意义

			//被优化
			//startT[i][j].time = _time;
			//startT[i][j].transfer = _transfer;
		}
	}
	_read.close();

	//for (int i = 0; i< chainNum; i++){
	//	// 将到达结点i的时间存入timeId
	//	for (int j = 0; j<arrivalT[i].size(); j++){
	//		//timeId.push_back(arrivalT[i][j].time);
	//		vectorTr.push_back(arrivalT[i][j].transfer);
	//		
	//	}
	//	//将结点i的出发时间存入timeId
	//	for (int j = 0; j<startT[i].size(); j++){
	//		//timeId.push_back(startT[i][j].time);
	//		vectorTr.push_back(startT[i][j].transfer);
	//	}
	//}

	cout << "chain info already load done" << endl;
} //已验证


int getVertexID(int u, int offset, int startOrArrival)
{
	//  0 arrival 1 start

	int base = u == 0 ? 0 : sumSize[u - 1];
	if (startOrArrival == 0)
	{
		return base + offset;
	}
	else if (startOrArrival == 1)
	{
		return base + arrivalT[u].size() + offset;
	}

	//原版
	//int base = u == 0 ? 0 : sumSize[u - 1];
	//const vector<EdgeInfo> & arrival = arrivalT[u];

	//if (startOrArrival == 0)
	//{
	//	return base + offset;
	//}
	//else if (startOrArrival == 1)
	//{
	//	return base + arrival.size() + offset;
	//}

}

void textPrint();
void findChain(){
	
	//vCalTransferByBFS();

	for (int i = 0; i < chainNum; i++){
		int cur = startT[i].size() + arrivalT[i].size();
		curSize[i] = cur;
		sumSize[i] = cur + (i>0 ? sumSize[i - 1] : 0);
	}

	// toPost向量中存储的是转化成scc图的到达（出发信息）
	/*
	比如 （a,1) -> (b,2) ->(b,3 )
	那么 toPost[0]的值为1
	toPost[3]的值为2
	toPost{4]的值为3

	*/

	for (int i = 0; i < chainNum; i++){
		int _p1 = 0;
		int _p2 = 0;
		int j = 0;
		
		while (j<arrivalT[i].size()+startT[i].size())
		{
			if (_p1 == arrivalT[i].size()){

				int u = getVertexID(i, _p2, 1);
				originalId[u] = i;

				toPos[u] = startT[i][_p2].time;
				_p2++;
			}
			else if (_p2 == startT[i].size()){
				int u = getVertexID(i, _p1, 0);
				originalId[u] = i;
				toPos[u] = arrivalT[i][_p1].time;

				_p1++;
			}
			else if (arrivalT[i][_p1].time <= startT[i][_p2].time){
				int u = getVertexID(i, _p1, 0);
				originalId[u] = i; 
				toPos[u] = arrivalT[i][_p1].time;
				_p1++;
			}
			else{
				int u = getVertexID(i, _p2, 1);
				originalId[u] = i;
				toPos[u] = startT[i][_p2].time;
				_p2++;
			}
			j++;
		}
	}

	deg.resize(chainNum);
	for (int i = 0; i < chainNum; i++){
		deg[i].d = startT[i].size() + arrivalT[i].size();
		deg[i].index = i;
	}
	//sort(deg.begin(), deg.end(), cmp);
	//textPrint();
}

//void textPrint(){
//	fstream f;
//	f.open("time.txt", ios::out);
//
//	for (int i = 0; i < toPos.size(); i++){
//		f << toPos[i] <<","<< timeId[i] << endl;
//	}
//}
//void pathPreservation();
vector<int> flabel1;
vector<int> flabel2;
//拓扑排序
vector<int> topoOrder;
//逆拓扑排序
vector<int> rtopoOrder;
int maxDown = 0;
int maxUp = 0;
vector<int> Lup;
vector<int> Ldown;
void topological(){

	vector<int> inds1;
	vector<int> inds2;

	stack<int> stack1;
	stack<int> stack2;
	stack<int> stack3;

	inds1.resize(vertex_num);
	inds2.resize(vertex_num);

	flabel1.resize(vertex_num);
	flabel2.resize(vertex_num);

	topoOrder.resize(vertex_num);
	rtopoOrder.resize(vertex_num);

	// down  up	
	Ldown.resize(vertex_num);
	Lup.resize(vertex_num);

	for (int i = 0; i < vertex_num; i++){
		inds1[i] = inds2[i] = ind[i];
	}

	queue<int> q;
	//计入 in_degree or out_degree 为0的点
	for (int i = vertex_num - 1; i >= 0; i--){

		if (ind[i] == 0)
		{
			// stack1 存储 in_degree = 0
			stack1.push(i);
			Ldown[i] = 0;
		}

		if (outd[i] == 0)
		{
			// stack3 存储 out_degree = 0的点
			stack3.push(i);
			Lup[i] = 0;
		}
	}
		int _count = 0;
	while (!stack1.empty()){
		int tmp = stack1.top();
		stack1.pop();
		if (tmp == 625908)
			cout << endl;
		topoOrder[_count] = tmp;
		flabel1[tmp] = _count++;
		for (int i = v[tmp].size() - 1; i >= 0; i--){
			if (v[tmp][i].target == 671736){
				cout << tmp << inds1[v[tmp][i].target] << endl;
			}
			//cout << inds1[v[tmp][i].target] << endl;
			inds1[v[tmp][i].target]--;
			int read_v = v[tmp][i].target; 
			int read_inds1 = inds1[v[tmp][i].target];

			if (v[tmp][i].target == 671736){
				cout << tmp << inds1[v[tmp][i].target] << endl;
			}
			if (inds1[v[tmp][i].target] == 0){
				stack1.push(v[tmp][i].target);
			}
		}
		if (stack1.size() == 0)
			cout << tmp << endl;
	}
	for (int i = 0; i < vertex_num; i++){
		int tmp = topoOrder[i];
		if (rv[tmp].size() >0){
			int _max = Ldown[rv[tmp][0].target];
			for (int j = 0; j < rv[tmp].size(); j++){
				if (Ldown[rv[tmp][j].target] > _max)
					_max = Ldown[rv[tmp][j].target];
			}
			Ldown[tmp] = _max + 1;
			maxDown = max(maxDown, Ldown[tmp]);
		}
	}

	_count = 0;
	while (!stack3.empty()){

		int tmp = stack3.top();
		stack3.pop();

		rtopoOrder[_count++] = tmp;
		for (int i = 0; i < rv[tmp].size(); i++){
			outd[rv[tmp][i].target]--;
			if (outd[rv[tmp][i].target] == 0){
				stack3.push(rv[tmp][i].target);
			}
		}
	}


	for (int i = 0; i < vertex_num; i++){
		int tmp = rtopoOrder[i];
		if (v[tmp].size() >0){
			int _max = Lup[v[tmp][0].target];
			for (int j = 0; j < v[tmp].size(); j++){
				if (Lup[v[tmp][j].target] >_max){
					_max = Lup[v[tmp][j].target];
				}
			}
			Lup[tmp] = _max + 1;
			maxUp = max(maxUp, Lup[tmp]);
		}
	}


	/*
		完成拓扑和逆拓扑的构建
	*/


	_count = 0;
	for (int i = 0; i < vertex_num; i++){
		if (ind[i] == 0)
			stack2.push(i);
	}
	while (!stack2.empty()){
		int tmp = stack2.top();
		stack2.pop();
		flabel2[tmp] = _count++;
		for (int i = 0; i < v[tmp].size(); i++){
			inds2[v[tmp][i].target]--;
			if (inds2[v[tmp][i].target] == 0){
				stack2.push(v[tmp][i].target);
			}
		}
	}

}
//进行数据保存.保留后面的数值
//void pathPreservation(){
//	vector<int>::iterator it, it1;
//	for (it = ++originPath.begin(); it != originPath.end();){
//		it1 = find(originPath.begin(), it, *it);
//		if (it1 != it)
//			it = originPath.erase(it1); //留后
//		else
//			it++;
//	}
//}
vector<vector<pair<int, int> > > Lin;
vector<vector<pair<int, int> > > Lout;
vector<int> rank_order;
vector<int> reverserank_order;
int k = 0;
bool cmp(degree d1, degree d2)
{
	return d1.d > d2.d;
}
// 目的是为了获取扩展后点的id
// 可以传递的是 扩展后链的id 和 position
//vector<vector<int>> Lin_trans; //记录换乘数量
vector<vector<pair<int,int>>> Lin_trans;
void print_Lable();
void print_TranstLabel();
//vector<vector<int>> Lout_trans;
vector<vector<pair<int,int>>> Lout_trans;
void testRankOrder(){
	fstream f;
	f.open("stand.txt", ios::out);
	for (int i = 0; i < rank_order.size(); i++){
		f << rank_order[i] << endl;
	}
}
int CalTransferByBFS(int st, int et);
pair<int, int> iCalTransferByBFS(int st, int et, int stime, int etime);//增加时间标记用来确定起始点以及时间范围
pair<int, int> myCalTransferByBFS(int st, int et, int stime, int etime);
pair<int, int>  vCalTransferByBFS(int st, int et, int stime, int etime);
pair<int, int>  loutCalTransferByBFS(int st, int et, int stime, int etime);
pair<int, int> linCalTransfer(int st, int et, int stime, int etime);
vector<vector<int>> rec_v; //记录Lin每个标签对应的扩展后scc点的id
vector<vector<int>>end_v;
vector<vector<int>>inNeighbor;
vector<vector<int>>outNeighbor;
vector<vector<int>> linTranst;
vector<vector<int>>loutTranst;
//测试
void lableBuild(){
	rec_v.resize(vertex_num);
	end_v.resize(vertex_num);
	inNeighbor.resize(vertex_num);
	outNeighbor.resize(vertex_num);

	Lin.resize(vertex_num);
	Lout.resize(vertex_num);

	linTranst.resize(vertex_num);
	loutTranst.resize(vertex_num);
	
	queue<int> q;
	queue<int> q1;
	Lin_trans.resize(vertex_num);
	Lout_trans.resize(vertex_num);
	for (int i = 0; i < vertex_num; i++)
		toChain[i] = originalId[i];

	cout << " chain num =  " << chainNum << endl;

	rank_order.resize(chainNum);
	sort(deg.begin(), deg.end(), cmp);
	for (int i = 0; i < chainNum; i++){
		rank_order[deg[i].index] = i;
	}

	int ST, ET; //即时 起始时间 — 终止时间
	double endtime;
	// ------------------------------
	//testRankOrder();

	//vector<bool> stl; //= true;//设置哨兵 true代表有效 即第一次
	//stl.resize(k, true);

	//Lin
	ST = clock();
	for (int i = 0; i < vertex_num; i++){


		vector<int> icount; //用于计数

		int tmp = topoOrder[i];
		//bool curAdd = false;
		//int curtranst = -1;
		//if (v[tmp].size() != 0)
		//{
		//	curtranst = v[tmp][0].transfer;
		//}

		vector<pair<int, int>> mid[2];

		vector<vector<int>> rec_mid;

		vector<vector<int>> tlin;		

		vector<vector<int>> track;
		track.resize(k);

		rec_mid.resize(k);
		tlin.resize(k);

		int _toChain = toChain[tmp];
		int _rank_order = rank_order[_toChain];
		int _topos = toPos[tmp];
		//toChain的val 表示点在原始图中的id
		// Lin的第一个参数 存储的是原始点在扩展图中的rank值
		if (tmp == 297)
			cout << endl;
		Lin[tmp].push_back(make_pair(rank_order[toChain[tmp]], toPos[tmp]));
		linTranst[tmp].push_back(0);
		rec_v[tmp].push_back(tmp);


		mid[0].resize(k);
		mid[1].resize(k);

		rec_mid[0].resize(k);
		rec_mid[1].resize(k);

		tlin[0].resize(k);
		tlin[1].resize(k);
		
		track[0].resize(k);
		track[1].resize(k);

		int cur = 0;
		int curSize = 0;
		int linTmp = 1;
		int linTmpSize = Lin[tmp].size();
		mid[linTmp] = Lin[tmp];
		mid[linTmp].resize(k);

		rec_mid[linTmp] = rec_v[tmp];
		rec_mid[linTmp].resize(k);

		tlin[linTmp] = linTranst[tmp];
		tlin[linTmp].resize(k);

		


		// vr[tmp].size() -> 点tmp的入度点个数     i  --- > j
		for (int j = 0; j < rv[tmp].size(); j++)
		{

			/*int add = 0;
			bool alreadAdd = false;*/
			int nin = rv[tmp][j].target;
			//cout << "current: " << tmp << "  第 " << j << " 次 ，出边邻居 "<<nin<<" ==============================" << endl;

			// 不同圈,并且存在换乘的情况
			//if (v[nin].size() != 0 && curtranst != -1 && v[nin][0].transfer != curtranst && originalId[nin] != originalId[tmp]){
			//	add = add + 1;
			//	alreadAdd = true;
			//}
			//merge tmp nin
			int p1 = 0; int p2 = 0;
			curSize = 0;

			track[cur].push_back(nin);
			//mid[cur].resize(k);
			//if (v[tmp].size() != 0){
			//	if (v[tmp][0].transfer != v[nin][0].transfer)
			//		tlin[cur][curSize] += 1;
			//}

			while (p1 < linTmpSize || p2 < Lin[nin].size())
			{

				if (curSize == k)
					break;

				if (p1 == linTmpSize) {

					mid[cur][curSize] = Lin[nin][p2];
					rec_mid[cur][curSize] = rec_v[nin][p2]; 
					track[cur][curSize] = nin;
					tlin[cur][curSize] += linTranst[nin][p2];

					//cout << rec_v[nin][p2] << endl;

					// 同圈 B \C 不同换乘
					//if (curtranst != -1 && alreadAdd != true && v[tmp].size() != 0 && v[nin].size() != 0 && v[rec_v[nin][p2]].size() != 0 && curtranst != v[nin][0].transfer && originalId[nin] == originalId[tmp]){
					//	
					//	// A B 同换乘 -> A C不同换乘
					//	if (v[nin][0].transfer == v[rec_v[nin][p2]][0].transfer)
					//		add++;
					//	else{
					//		if (v[tmp][0].transfer == v[rec_v[nin][p2]][0].transfer)
					//			add--;
					//	}
					//	alreadAdd = true;
					//}

					//tlin[cur][curSize] = add + linTranst[nin][p2];

					curSize++;
					p2++;
					continue;

				}
				if (p2 == Lin[nin].size()) {

					mid[cur][curSize] = mid[linTmp][p1];
					rec_mid[cur][curSize] = rec_mid[linTmp][p1];

					tlin[cur][curSize] = tlin[linTmp][p1];
					track[cur][curSize] = track[linTmp][p1];
				//	cout << rec_mid[linTmp][p1]<<endl;

					curSize++;
					p1++;
					continue;

				}

				if (mid[linTmp][p1].first < Lin[nin][p2].first) { 
					mid[cur][curSize] = mid[linTmp][p1]; 
					rec_mid[cur][curSize] = rec_mid[linTmp][p1];

					tlin[cur][curSize] = tlin[linTmp][p1];
					track[cur][curSize] = track[linTmp][p1];

				//	cout << rec_mid[linTmp][p1] << endl;


					curSize++;
					p1++; 
				}
				else if (mid[linTmp][p1].first > Lin[nin][p2].first) { 
					
					mid[cur][curSize] = Lin[nin][p2];
					rec_mid[cur][curSize] = rec_v[nin][p2];

					tlin[cur][curSize] += linTranst[nin][p2];
					track[cur][curSize] = nin;
				//	cout << rec_v[nin][p2] << endl;

					//// 同圈 B \C 不同换乘
					//if (curtranst != -1 && alreadAdd != true && v[tmp].size() != 0 && v[nin].size() != 0 && v[rec_v[nin][p2]].size() != 0 && curtranst != v[nin][0].transfer && originalId[nin] == originalId[tmp]){

					//	// A B 同换乘 -> A C不同换乘
					//	if (v[nin][0].transfer == v[rec_v[nin][p2]][0].transfer)
					//		add++;
					//	else{
					//		if (v[tmp][0].transfer == v[rec_v[nin][p2]][0].transfer)
					//			add--;
					//	}
					//	alreadAdd = true;
					//}
					//tlin[cur][curSize] = add + linTranst[nin][p2];

					curSize++;
					p2++; 
				}
				else {
					if (mid[linTmp][p1] >= Lin[nin][p2]){
						mid[cur][curSize] = mid[linTmp][p1];
						rec_mid[cur][curSize] = rec_mid[linTmp][p1];
						tlin[cur][curSize] = tlin[linTmp][p1];
						track[cur][curSize] = track[linTmp][p1];
					//	cout << rec_mid[linTmp][p1] << endl;

					}
					else{
						mid[cur][curSize] = Lin[nin][p2];
						rec_mid[cur][curSize] = rec_v[nin][p2];
						tlin[cur][curSize] += linTranst[nin][p2];
						track[cur][curSize] = nin;
						//cout << rec_v[nin][p2] << endl;
						// 同圈 B \C 不同换乘
						//if (curtranst != -1 && alreadAdd != true && v[tmp].size() != 0 && v[nin].size() != 0 && v[rec_v[nin][p2]].size() != 0 && curtranst != v[nin][0].transfer && originalId[nin] == originalId[tmp]){

						//	// A B 同换乘 -> A C不同换乘
						//	if (v[nin][0].transfer == v[rec_v[nin][p2]][0].transfer)
						//		add++;
						//	else{
						//		if (v[tmp][0].transfer == v[rec_v[nin][p2]][0].transfer)
						//			add--;
						//	}
						//	alreadAdd = true;
						//}
						//tlin[cur][curSize] = add + linTranst[nin][p2];
					}

					//mid[cur][curSize] = max(mid[linTmp][p1], Lin[nin][p2]);
					//rec_mid[cur][curSize] = max(rec_mid[linTmp][p1], rec_v[nin][p2]);
					curSize++;
					p1++;
					p2++;
				}
			}
			cur = 1 - cur;
			linTmp = 1 - linTmp;
			linTmpSize = curSize;
		}
		Lin[tmp] = mid[linTmp];
		Lin[tmp].resize(linTmpSize);

		rec_v[tmp] = rec_mid[linTmp];
		rec_v[tmp].resize(linTmpSize);

		linTranst[tmp] = tlin[linTmp];
		linTranst[tmp].resize(linTmpSize);

		inNeighbor[tmp] = track[linTmp];
		inNeighbor[tmp].resize(linTmpSize);

		if (v[tmp].size() != 0){
			for (int m = 0; m < inNeighbor[tmp].size(); m++){
				if (v[inNeighbor[tmp][m]][0].transfer != v[tmp][0].transfer && rec_v[tmp][m] != tmp)
					linTranst[tmp][m]++;
			}
		}

	}

	ET = clock();
	endtime = (double)(ET - ST) / CLOCKS_PER_SEC;
	cout << "Lin标签构造费时： " << endtime << " s" << endl;

	int space = 0;
	fstream cs("linTranst.txt", ios::out);
	for (int i = 0; i < linTranst.size(); i++){
		for (int j = 0; j < linTranst[i].size(); j++){
			cs << linTranst[i][j] << "   ";
		}
		cs << endl;
	}

	//Lout
	ST = clock();
	for (int i = 0; i < vertex_num; i++)
	{
		vector<int> icount; //用于计数

		int tmp = rtopoOrder[i];
		int curtranst = -1;
		//bool isNULL = false;
		bool curAdd = false;
		//if (v[tmp].size() != 0)
		//{
		//	curtranst = v[tmp][0].transfer;
		//	//icount.push_back(v[tmp][0].transfer);
		//	//curAdd = true;
		//}

		//IP construct
		vector<pair<int, int> > mid[2];
		Lout[tmp].push_back(make_pair(rank_order[toChain[tmp]], toPos[tmp]));
		loutTranst[tmp].push_back(0);
		end_v[tmp].push_back(tmp);

		vector<vector<int>> end_mid;
		vector<vector<int>> tlout;
		vector<vector<int>> track;
		track.resize(k);

		end_mid.resize(k);
		tlout.resize(k);
		mid[0].resize(k);
		mid[1].resize(k);

		end_mid[0].resize(k);
		end_mid[1].resize(k);

		tlout[0].resize(k);
		tlout[1].resize(k);


		track[0].resize(k);
		track[1].resize(k);

		int cur = 0;
		int curSize = 0;
		int loutTmp = 1;
		int loutTmpSize = Lout[tmp].size();
		mid[loutTmp] = Lout[tmp];

		end_mid[loutTmp] = end_v[tmp];
		end_mid[loutTmp].resize(k);

		tlout[loutTmp] = loutTranst[tmp];
		tlout[loutTmp].resize(k);

		mid[loutTmp].resize(k);

		int add = 0;
		for (int j = 0; j < v[tmp].size(); j++)
		{
			int nin = v[tmp][j].target;
			track[cur].push_back(nin);
			//bool alreadAdd = false;
			//if (v[nin].size() != 0 && curtranst != -1 && v[nin][0].transfer != curtranst && originalId[nin] != originalId[tmp]){
			//	add = add + 1;
			//	alreadAdd = true;
			//}
			//merge tmp nin
			int p1 = 0; int p2 = 0;
			curSize = 0;
			//mid[cur].resize(k);
			while (p1 < loutTmpSize || p2 < Lout[nin].size())
			{
				//cout << "p1 p2: " << p1 << " " << p2 << endl;
				if (curSize == k)
					break;

				if (p1 == loutTmpSize) {
					mid[cur][curSize] = Lout[nin][p2];
					end_mid[cur][curSize] = end_v[nin][p2];
					tlout[cur][curSize] += loutTranst[nin][p2];
					track[cur][curSize] = nin;
					//// 同圈 B \C 不同换乘
					//if (curtranst != -1 && alreadAdd != true && v[tmp].size() != 0 && v[nin].size() != 0 && v[end_v[nin][p2]].size() != 0 && curtranst != v[nin][0].transfer && originalId[nin] == originalId[tmp]){

					//	// A B 同换乘 -> A C不同换乘
					//	if (v[nin][0].transfer == v[end_v[nin][p2]][0].transfer)
					//		add++;
					//	else{
					//		if (v[tmp][0].transfer == v[end_v[nin][p2]][0].transfer)
					//			add--;
					//	}
					//	alreadAdd = true;
					//}
					//tlout[cur][curSize] = add + loutTranst[nin][p2];

					curSize++;
					p2++;
					continue;
				}
				if (p2 == Lout[nin].size()) {
					mid[cur][curSize] = mid[loutTmp][p1];
					end_mid[cur][curSize] = end_mid[loutTmp][p1];
					tlout[cur][curSize] = tlout[loutTmp][p1];
					track[cur][curSize] = track[loutTmp][p1];
					curSize++;
					p1++;
					continue;
				}

				if (mid[loutTmp][p1].first < Lout[nin][p2].first) {
					mid[cur][curSize] = mid[loutTmp][p1];
					end_mid[cur][curSize] = end_mid[loutTmp][p1];
					tlout[cur][curSize] = tlout[loutTmp][p1];
					track[cur][curSize] = track[loutTmp][p1];
					curSize++;
					p1++;
				}
				else if (mid[loutTmp][p1].first > Lout[nin][p2].first) {
					mid[cur][curSize] = Lout[nin][p2];
					end_mid[cur][curSize] = end_v[nin][p2];
					tlout[cur][curSize] += loutTranst[nin][p2];
					track[cur][curSize] = nin;
					//// 同圈 B \C 不同换乘
					//if (curtranst != -1 && alreadAdd != true && v[tmp].size() != 0 && v[nin].size() != 0 && v[end_v[nin][p2]].size() != 0 && curtranst != v[nin][0].transfer && originalId[nin] == originalId[tmp]){

					//	// A B 同换乘 -> A C不同换乘
					//	if (v[nin][0].transfer == v[end_v[nin][p2]][0].transfer)
					//		add++;
					//	else{
					//		if (v[tmp][0].transfer == v[end_v[nin][p2]][0].transfer)
					//			add--;
					//	}
					//	alreadAdd = true;
					//}
					//tlout[cur][curSize] = add + loutTranst[nin][p2];
					curSize++;
					p2++;
				}
				else {
					if (mid[loutTmp][p1] <= Lout[nin][p2]){
						mid[cur][curSize] = mid[loutTmp][p1];
						end_mid[cur][curSize] = end_mid[loutTmp][p1];
						tlout[cur][curSize] = tlout[loutTmp][p1];
						track[cur][curSize] = track[loutTmp][p1];
					}
					else{
						mid[cur][curSize] = Lout[nin][p2];
						end_mid[cur][curSize] = end_v[nin][p2];
						tlout[cur][curSize] += loutTranst[nin][p2];
						track[cur][curSize] = nin;
						// 同圈 B \C 不同换乘
						//if (curtranst != -1 && alreadAdd != true && v[tmp].size() != 0 && v[nin].size() != 0 && v[end_v[nin][p2]].size() != 0 && curtranst != v[nin][0].transfer && originalId[nin] == originalId[tmp]){

						//	// A B 同换乘 -> A C不同换乘
						//	if (v[nin][0].transfer == v[end_v[nin][p2]][0].transfer)
						//		add++;
						//	else{
						//		if (v[tmp][0].transfer == v[end_v[nin][p2]][0].transfer)
						//			add--;
						//	}
						//	alreadAdd = true;
						//}
						//tlout[cur][curSize] = add + loutTranst[nin][p2];
					}
					//mid[cur][curSize++] = min(mid[loutTmp][p1], Lout[nin][p2]); 
					//end_mid[cur][curSize - 1] = min(end_mid[loutTmp][p1], end_v[nin][p2]);
					curSize++;
					p1++;
					p2++;
				}
			}
			//Lout[tmp] = mid;
			cur = 1 - cur;
			loutTmp = 1 - loutTmp;
			//mid[loutTmp].resize(curSize);
			loutTmpSize = curSize;
			add = 0;
			//icount.clear();
			//if (curAdd){
			//	icount.push_back(v[tmp][0].transfer);
			//}
		}
		Lout[tmp] = mid[loutTmp];
		Lout[tmp].resize(loutTmpSize);
		end_v[tmp] = end_mid[loutTmp];
		end_v[tmp].resize(loutTmpSize);
		loutTranst[tmp] = tlout[loutTmp];
		loutTranst[tmp].resize(loutTmpSize);
		outNeighbor[tmp] = track[loutTmp];
		outNeighbor[tmp].resize(loutTmpSize);
		for (int m = 0; m < outNeighbor[tmp].size(); m++){
			if (v[outNeighbor[tmp][m]].size() != 0 && v[tmp].size()!= 0 && v[outNeighbor[tmp][m]][0].transfer != v[tmp][0].transfer && end_v[tmp][m] != tmp)
				loutTranst[tmp][m]++;
		}
	}
	ET = clock();
	endtime = (double)(ET - ST) / CLOCKS_PER_SEC;
	cout << "Lout标签构造费时： " << endtime << " s" << endl;

	cout << "Lin and Lout label already done." << endl;
	vector<int>().swap(ind);
	vector<int>().swap(outd);

	//print_Lable();
	//print_TranstLabel();
}


//queue <int> bfsqueue;
//stack<int> path;
//void initBFS(){
//	while (!bfsqueue.empty()){
//		bfsqueue.pop();
//	}
//	op.clear();
//	for (int i = 0; i<vertex_num; i++){
//		visited[i] = false;
//		vertexInQueue[i] = false;
//	}
//	while (!path.empty()){
//		path.pop();
//	}
//
//}
/*利用BFS查找路径，计算换乘*/

/*
测试
*/
int TEST_ST;
int TEST_ET;
double TEST_RESULT;
//vector<int>::iterator it;
//返回pair对象
// time代表到达时间，传入最终到达时间 -> 通过时间的限制来找到相应的扩展顶点
void vCalTransferByBFS(){
	queue <int> bfsqueue;
	vector<int> path;
	stack<int> tmp;
	vector<int> op;//Original path
	int u = 23548; //原始目标点
	//int u = 7681; 	//断链的点
	//int target = originalId[et]; //终点目标点
	int trans = 0;
	int _v;
	
	bfsqueue.push(u);
	while (!bfsqueue.empty())
	{

		u = bfsqueue.front();
		//cout << u << endl;
		path.push_back(u);
		if (u == 404856)
			cout << endl;
	//	cout << u << endl;
		bfsqueue.pop();
		visited[u] = true; //被访问
		vertexInQueue[u] = true;
		for (int i = 0; i < v[u].size(); i++){
			_v = v[u][i].target;
			if (!visited[_v] &&!vertexInQueue[_v]){
				bfsqueue.push(_v);
				vertexInQueue[_v] = true;
			}
		}
	}
	fstream c;
	c.open("bfs.txt", ios::out);
	c << path.size() << endl;
	for (int i = 0; i < path.size(); i++){
		c << path[i] << endl;
	}
	cout << "清洗前数据:  " <<path.size()<< endl;
	sort(path.begin(), path.end());
	path.erase(unique(path.begin(), path.end()), path.end());
	cout << "清洗后数据:  " << path.size() << endl;
	c.close();
}
//pair<int, int>  loutCalTransferByBFS(int st, int et, int stime, int etime){
//	queue <int> bfsqueue;
//	queue<int> path;
//	stack<int> tmp;
//	vector<int> op;//Original path
//	int u = originalId[st]; //原始目标点
//	int target = originalId[et]; //终点目标点
//	int trans = 0;
//	int ist = st; //扩展后的出发点
//	int iet = et; //扩展后的目标点
//	int _v;
//	int currentTrans = -1; //保留上一次的换乘点 初始化-1
//	int currentTime = etime;
//	vector<int>::iterator uit = find(originPath.begin(), originPath.end(), u); //找到起始点
//	vector<int>::iterator vit = find(originPath.begin(), originPath.end(), target); //找到终点
//
//	// 如果两个均不为空 将范围内的点全部压入op中
//
//
//	//从后面开始找.因为最后一个值是到达
//	if (uit != originPath.end() && vit != originPath.end()){
//		int uindex = uit - originPath.begin();
//		int vindex = vit - originPath.begin(); //到达
//		if (uindex < vindex){
//			for (int i = uindex; i <= vindex; i++)
//				op.push_back(originPath[i]); //将找到的点压入栈中
//		}
//		else{
//			// vindex值比uindex值小
//			for (int i = vindex; i <= uindex; i++)   {
//				op.insert(op.begin(), originPath[i]);
//			}
//		}
//	}
//	else{
//		cout << "error" << endl;
//		//bfsqueue.push(u);
//		//while (!bfsqueue.empty())
//		//{
//		//	u = bfsqueue.front();
//		//	op.push_back(u); //记录结点 
//		//	if (u == target)
//		//		break;
//		//	bfsqueue.pop();
//		//	visited[u] = true;
//		//	vertexInQueue[u] = false;
//		//	for (int i = 0; i < startT[u].size(); i++){
//		//		_v = startT[u][i].target;
//		//		if (!visited[_v] && !vertexInQueue[_v]){
//		//			bfsqueue.push(_v);
//		//			vertexInQueue[_v] = true;
//		//			tmp.push(_v);
//		//		}
//		//	}
//		//}
//		//for (int i = 0; i < op.size(); i++){
//		//	visited[op[i]] = false;
//		//}
//		//while (!tmp.empty())
//		//{
//		//	vertexInQueue[tmp.top()] = false;
//		//	tmp.pop();
//		//}
//	}
//	int v;
//	int index;
//	int endTime = etime;
//	bool senior = false;
//	int maxT = 0;
//	// 序号点 不一定是到达 or 起始点  。映射点 在Lin时，一定是出发点
//	while (op.size() != 0){
//		u = op[op.size() - 1]; //到达点
//		op.pop_back();
//		if (op.size() == 0)
//			break;
//		for (int i = arrivalT[u].size()-1 ; i >=0; i--)
//		{
//			if (arrivalT[u][i].time <= endTime) //找到该点
//			{
//				index = i;
//				v = arrivalT[u][i].target; //找到下一个链
//				while (op.size() != 0 && op[op.size() - 1] != v)
//				{
//					op.pop_back();
//				}
//				if (currentTrans != arrivalT[u][i].transfer && currentTrans != -1)
//					trans++;
//				senior = true;
//				break;
//			}
//		}
//
//		//之前假设旅行时间为1 因此 出发时间 + 1 = 到达时间
//		for (int i = startT[v].size()-1; i >=0; i--){
//			if (startT[v][i].target == u && startT[v][i].time <= arrivalT[u][index].time - arrivalT[u][index].duration){ //找到该点
//				endTime = startT[v][i].time;
//				currentTrans = startT[v][i].transfer;
//				break;
//			}
//		}
//		senior = false;
//		maxT++;
//	}
//
//	return make_pair(trans, maxT-2);
//}



pair<int, int> myCalTransferByBFS(int st, int et, int stime, int etime){
	queue <int> bfsqueue;
	queue<int> path;
	stack<int> tmp;
	vector<int> op;//Original path
	int u = originalId[st]; //原始目标点
	int target = originalId[et]; //终点目标点
	int beginTime = stime;
	int trans = 0;
	int ist = st; //扩展后的出发点
	int iet = et; //扩展后的目标点
	int _v;
	int currentTrans = -1; //保留上一次的换乘点 初始化-1
	int currentTime = etime;
	vector<int>::iterator uit = find(originPath.begin(), originPath.end(), u); //找到起始点
	vector<int>::iterator vit = find(originPath.begin(), originPath.end(), target); //找到终点

	// 如果两个均不为空 将范围内的点全部压入op中
	if (uit != originPath.end() && vit != originPath.end()){
		int uindex = uit - originPath.begin();
		int vindex = vit - originPath.begin();
		for (int i = uindex; i <= vindex; i++)
			op.push_back(originPath[i]); //将找到的点压入栈中
	}
	else{
		bfsqueue.push(u);
		while (!bfsqueue.empty())
		{
			u = bfsqueue.front();
			op.push_back(u); //记录结点 
			if (u == target)
				break;
			bfsqueue.pop();
			visited[u] = true;
			vertexInQueue[u] = false;
			for (int i = 0; i < startT[u].size(); i++){
				_v = startT[u][i].target;
				if (!visited[_v] && !vertexInQueue[_v]){
					bfsqueue.push(_v);
					vertexInQueue[_v] = true;
					tmp.push(_v);
				}
			}
		}
		for (int i = 0; i < op.size(); i++){
			//cout << op[i] << endl;
			visited[op[i]] = false;
			//originPath.push_back(op[i]);
		}
		while (!tmp.empty())
		{
			vertexInQueue[tmp.top()] = false;
			tmp.pop();
		}
		//pathPreservation();
	}
	int maxT = op.size() - 2;
	bool senior = false;
	while (op.size() != 0)
	{
		// 获得结点u 注：不确定该结点是出发结点还是到达结点
		u = op[op.size() - 1];
		op.pop_back();//找到结点后 将结点弹出


		//从头开始遍历arrival 判断该节点是否是出发结点
		for (int i = 0; i < arrivalT[u].size(); i++){

			// 如果该节点是出发结点
			if (getVertexID(u, i, 0) == iet)
			{
				// 开始遍历所有的可能范围内的点
				for (int k = 0; k < op.size(); k++){

					//如果 目标顶点在op中,则找到该点 index
					if (op[k] == arrivalT[u][i].target){
						int index = op[k];//获取起始点
						int begin = k; // 索引

						//遍历 startT[index]中的所有点,目的是为了查看是否存在换乘.因此这里需要查找到具体的出发点
						for (int j = 0; j < startT[index].size(); j++){

							if (startT[index][j].target == u);//找到目标点 即 index点的目标点 是 u  .注：理论上两点间的transfer是相同的
							{
								if (currentTrans != -1 && currentTrans != arrivalT[u][i].transfer)// 如果当前值不为初始值，并且圈内的到达换乘 ！= 圈内的出发换乘
									trans++;
								currentTrans = startT[index][j].transfer; //记录出发时的换乘点 和下一次的到达点进行比对

								
								for (int m = arrivalT[index].size()-1; m >=0; m--)
								{
									if (arrivalT[index][m].time <= startT[index][j].time){
										iet = getVertexID(index, m, 0);
										senior = true;
										break;
									}
								}
								if (senior == false)
									iet = getVertexID(index, 0, 0);
								senior = true;

								break;

							}
						}
						if (senior == true){
							int cb;//cut back
							cb = op.size() - begin - 1;
							while (cb > 0)
							{
								op.pop_back();
								cb--;
							}
							break;
						}
					}
				}
				break;
			}
		}
		if (senior == false){
			for (int i = 0; i < startT[u].size(); i++){

				// 如果该节点是出发结点
				if (getVertexID(u, i, 1) == iet)
				{
					// 开始遍历所有的可能范围内的点
					for (int k = 0; k < op.size(); k++){

						//如果 目标顶点在op中,则找到该点 index
						if (op[k] == startT[u][i].target){
							int index = op[k];//获取起始点
							int begin = k; // 索引
							//int t = startT[u][i].duration; // 获取旅程的持续时间
							int t = 1;
							//遍历 startT[index]中的所有点,目的是为了查看是否存在换乘.因此这里需要查找到具体的出发点
							for (int j = 0; j < arrivalT[index].size(); j++){

								if (arrivalT[index][j].target == u);//找到目标点 即 index点的目标点 是 u  .注：理论上两点间的transfer是相同的
								{
									if (currentTrans != -1 && currentTrans != startT[u][i].transfer)// 如果当前值不为初始值，并且圈内的到达换乘 ！= 圈内的出发换乘
										trans++;
									currentTrans = arrivalT[index][j].transfer; //记录出发时的换乘点 和下一次的到达点进行比对


									for (int m = startT[index].size() - 1; m >= 0; m--)
									{
										if (startT[index][m].time <= arrivalT[index][j].time){
											iet = getVertexID(index, m,1);
											senior = true;
											break;
										}
									}
									if (senior == false)
										iet = getVertexID(index, 0,1);
									senior = true;

									break;

								}
							}
							if (senior == true){
								int cb;//cut back
								cb = op.size() - begin - 1;
								while (cb > 0)
								{
									op.pop_back();
									cb--;
								}
								break;
							}
						}
					}
					break;
				}
			}
		}
		senior = false;
	}
	return make_pair(trans, 20);
}
//pair<int,int> iCalTransferByBFS(int st, int et ,int stime,int etime){
//
//	queue <int> bfsqueue;
//	queue<int> path;
//	stack<int> tmp;
//	vector<int> op;//Original path
//	int u = originalId[st]; //原始目标点
//	int target = originalId[et]; //终点目标点
//	int _u = originalId[st];
//	int tar = et;
//	int uid, _v;
//	int currentTime = etime;
//	int beginTime = stime;
//	int trans = 0;
//	vector<int>::iterator uit = find(originPath.begin(), originPath.end(), u);
//	vector<int>::iterator vit = find(originPath.begin(), originPath.end(), target);
//	if (uit != originPath.end() && vit != originPath.end()){
//		int uindex = uit - originPath.begin();
//		int vindex = vit - originPath.begin();
//		for (int i = uindex; i <=vindex; i++)
//			op.push_back(originPath[i]);
//	}
//	else{
//		bfsqueue.push(u);
//		while (!bfsqueue.empty())
//		{
//			u = bfsqueue.front();
//			op.push_back(u); //记录结点 
//			if (u == target)
//				break;
//			bfsqueue.pop();
//			visited[u] = true;
//			vertexInQueue[u] = false;
//			for (int i = 0; i < startT[u].size(); i++){
//				_v = startT[u][i].target;
//				if (!visited[_v] && !vertexInQueue[_v]){
//					bfsqueue.push(_v);
//					vertexInQueue[_v] = true;
//					tmp.push(_v);
//				}
//			}
//		}
//		for (int i = 0; i < op.size(); i++){
//			//cout << op[i] << endl;
//			visited[op[i]] = false;
//			//originPath.push_back(op[i]);
//		}
//		while (!tmp.empty())
//		{
//			vertexInQueue[tmp.top()] = false;
//			tmp.pop();
//		}
//		pathPreservation();
//	}
//	int maxT = op.size() - 2;
//	//if (_u == 0 && target == 11)
//	//	cout << endl;
//	//确定需要走哪些链才能够到达
//	bool senior = false;
//
//	// op中u出发的所有情况，直到点v扫描到停止
//	// 其中可能包含的情况是 中间结点w 无法到达v
//	// 注意：包含点v都一起压入op容器中
//	int startTrans; //保存start点的换乘
//	int arrivalTrans; //保存 arrival点的换乘
//	int currentTrans = -1; //保留上一次的换乘点 初始化-1
//	if (_u == 0 && target == 11)
//		cout << endl;
//	/*
//	60000 ->
//	
//	*/
//	while (op.size() != 0){
//		u = op[op.size() - 1];
//		op.pop_back();
//		//最早到达
//		for (int i = arrivalT[u].size()-1; i >=0; i--){
//			if (arrivalT[u][i].time > currentTime) //确定到达时间
//				continue;
//			//扫描 满足u的入度原始顶点在op中
//			it = find(op.begin(), op.end(), arrivalT[u][i].target);
//			if (it != op.end()){
//				int index = it - op.begin();
//				for (int j = 0; j < startT[op[index]].size(); j++){
//					// 如果满足换乘条件
//					startTrans = startT[op[index]][j].transfer;
//					arrivalTrans = arrivalT[u][i].transfer;
//					// 前一次和后一次的换乘相同 并且出发时间会小于到达时间 （currentTime)
//					if (startTrans == arrivalTrans && startT[op[index]][j].time < currentTime){
//						if (currentTrans != -1 && currentTrans != arrivalTrans) //一定需要换乘
//							trans++;
//						currentTrans = startTrans; //记录出发时的换乘点 和下一次的到达点进行比对
//						currentTime = arrivalT[u][i].time - arrivalT[u][i].duration; //存储的都是到达时间
//						senior = true;
//						break;					
//					}
//				}
//				if (senior == false)
//					continue;
//				//path.push(op[index]);
//				int cb;//cut back
//				cb = op.size() - index - 1;
//				while (cb>0)
//				{
//					op.pop_back();
//					cb--;
//				}
//				break;
//			}
//		}
//		senior = false;
//	}
//	
//	//上面代码的备份
//	//到达时间越晚,换乘越少
//	//for (int i = arrivalT[u].size() - 1; i >= 0; i--){
//	//	it = find(op.begin(), op.end(), arrivalT[u][i].target);
//	//	if (it != op.end()){
//	//		int index = it - op.begin();
//	//		path.push(op[index]);
//	//		int cb;//cut back
//	//		cb = op.size() - index - 1;
//	//		while (cb>0)
//	//		{
//	//			op.pop_back();
//	//			cb--;
//	//		}
//	//		break;
//	//	}
//	//}
//
//	return make_pair(trans,maxT);
//}


bool intersectTranst(const int u1, const int u2);
bool queryTransfer(int from, int to);
bool queryTranst(int from, int to);
void transt();
pair<int, int> linCalTransfer(int st, int et, int stime, int etime){
	// lin特点 st点一定是startT点. 故可以根据startT点来找到目标顶点
	int transt = 0;
	int currentTranst; //当前换乘
	int u = originalId[st]; //获取原始点的chainId
	int index = -1 ;
	int currentTime;
	int target;
	for (int i = 0; i < startT[u].size(); i++)
	{
		if (startT[u][i].time == stime && queryTranst(getVertexID(u, i, 1), et)) //找到出发点
		{
			index = i;
			currentTranst = startT[u][i].transfer;
			target = startT[u][index].target; //找到下一个chain的arrival点
			linMapper[et].push_back(target);
			currentTime = startT[u][index].time;
			break;
		}
	}
	//cout << index << endl;
	if (index == -1) 
		cout << "没有找到出发点,将会报错" << endl; 

	bool senior = false;
	// u代表出发点 v代表达到点 两点均存储
	int w; 
	index = 0;
	int enforce = 0;
	vector<int>::iterator it;
	while (enforce<100){
		if (target == originalId[et]) //如果该点和目标点在同一个链中 直接跳出循环
			break;
		//优先去search不换乘的情况.
		for (int i = 0; i < startT[target].size(); i++){
			if (startT[target][i].transfer == currentTranst && queryTranst(getVertexID(target, i, 1),et)){
				index = i; //记录当前的索引位置
				break;
			}
		}

		//这里假设：可能出发的点 均在 不换乘Time 之后
		for (int i = index; i < startT[target].size(); i++){ //当前出发点
			w = startT[target][i].target; //找到下一个的出发点
			if (w == originalId[et])
			{
				if (startT[target][i].transfer != currentTranst)
					transt++;
				target = w;
				senior = true;
				cout << "if - 1" << endl;
				break;
			}
			for (int k = 0; k < startT[w].size(); k++){ 
				it = find(linMapper[et].begin(), linMapper[et].end(), w);
				if (it != linMapper[et].end()){
					currentTranst = startT[target][i].transfer;
					//currentTime = startT[target][i].time;
					target = w; //下一个链target
					senior = true;
					break;
				}
				else if (queryTranst(getVertexID(w, k, 1), et)){ //如果下一个出发点满足可达性条件
					linMapper[et].push_back(w);
					//找到了这条链
					if (startT[target][i].transfer != currentTranst)
						transt++;
					currentTranst = startT[target][i].transfer;
					//currentTime = startT[target][i].time;
					target = w; //下一个链target
					senior = true;
					break;
				}
			}
			if (senior) 
				break;
		}
		//cout << target << endl;

		if (senior == false){ //代表均不可达
			cout << "这里senior == falsel了" << endl;
			transt = 20; //设置最大换乘数 代表不可达 并且退出
			break;
		}
		senior = false; 
		index = 0;
		//enforce++;
	}
	if (enforce == 100)
		cout << "error" << endl;
	else
		cout << "success" << endl;
	return make_pair(transt, 20);
}
pair<int, int>  loutCalTransferByBFS(int st, int et, int stime, int etime){
	// lout特点 到达点都是arrival
	int transt = 0;
	int currentTranst; //当前换乘
	int currentTime;
	int u = originalId[et]; //获取原始点的chainId
	int index = -1;
	int target;
	for (int i = 0; i < arrivalT[u].size(); i++)
	{
		if (arrivalT[u][i].time == etime) //找到出发点
		{
			index = i;
			currentTranst = arrivalT[u][i].transfer;
			currentTime = arrivalT[u][i].time;
			target = arrivalT[u][index].target; //找到下一个chain的arrival点
			break;
		}
	}
	//cout << index << endl;
	if (index == -1) cout << "error" << endl;

	bool senior = false;
	// u代表出发点 v代表达到点 两点均存储
	index = 0;
	int enforce = 0;
	int w;
	vector<int>::iterator it;
	while (enforce<100){
		if (target == originalId[st]) //如果该点和目标点在同一个链中 直接跳出循环
			break;
		for (int i = 0; i < arrivalT[target].size(); i++){
			if (arrivalT[target][i].transfer == currentTranst)
			{
				index = i;
				break;
			}
		}
		for (int i = index; i < arrivalT[target].size(); i++){ //在链target里出发
			w = arrivalT[target][i].target; //出发点
			if (w == originalId[st]){
				target = w;
				senior = true;
				break;
			}
			for (int k = 0; k < arrivalT[w].size(); k++){
				it = find(loutMapper[st].begin(), loutMapper[st].end(), w);
				if (it != loutMapper[st].end()){ //代表找到了
					if (arrivalT[target][i].transfer != currentTranst)
						transt++;
					currentTranst = arrivalT[target][i].transfer;
					currentTime = arrivalT[target][i].time;
					target = w; //下一个链target
					senior = true;
					break;
				}
				else if (queryTranst(st,getVertexID(w, k, 0))){
					loutMapper[st].push_back(w); //代表 w链可达linMapper
					//找到了这条链
					if (arrivalT[target][i].transfer != currentTranst)
						transt++;
					currentTranst = arrivalT[target][i].transfer;
					currentTime = arrivalT[target][i].time;
					target = w; //下一个链target
					senior = true;
					break;
				}
			}
			if (senior) break;
		}
		if (senior == false){
			transt = 20;
			break;
		}
		senior = false;
		index = 0;
		enforce++;
	}
	return make_pair(transt, 20);
}

void print_Lable(){

	string strFileOut = ("label.bin");//输出文件路径
	ofstream writeFile(strFileOut, ios::out | ios::binary);//二进制格式
	if (writeFile.is_open())
	{
		for (int i = 0; i < Lin.size(); i++){
			int data =i;
			writeFile.write((char *)(&data), sizeof(data));
			for (int j = 0; j < Lin[i].size(); j++){
				data = Lin[i][j].first;
				writeFile.write((char *)(&data), sizeof(data));
				data = Lin[i][j].second;
				writeFile.write((char *)(&data), sizeof(data));
			}
		}
		for (int i = 0; i < Lout.size(); i++){
			int data = i;
			writeFile.write((char *)(&data), sizeof(data));
			for (int j = 0; j < Lout[i].size(); j++){
				data = Lout[i][j].first;
				writeFile.write((char *)(&data), sizeof(data));
				data = Lout[i][j].second;
				writeFile.write((char *)(&data), sizeof(data));
			}
		}
		writeFile.close();
	}

}
void print_TranstLabel(){
	string strFileOut = ("Transtlabel.bin");//输出文件路径
	ofstream writeFile(strFileOut, ios::out | ios::binary);//二进制格式
	if (writeFile.is_open())
	{
		for (int i = 0; i < linTranst.size(); i++){
			int data = i;
			writeFile.write((char *)(&data), sizeof(data));
			for (int j = 0; j < linTranst[i].size(); j++){
				data = linTranst[i][j];
				writeFile.write((char *)(&data), sizeof(data));
			}
		}
		for (int i = 0; i < loutTranst.size(); i++){
			int data = i;
			writeFile.write((char *)(&data), sizeof(data));
			for (int j = 0; j < loutTranst[i].size(); j++){
				data = loutTranst[i][j];
				writeFile.write((char *)(&data), sizeof(data));
			}
		}
		writeFile.close();
	}
}
//copy index



int MAXTRANS; //最大换乘数

#include<stdint.h>
class ReachIndex{
public:
	//uint16_t layerup;
	//uint16_t layerdown;
	uint8_t OutLimit;
	uint8_t InLimit;
	uint8_t HLimit;
	pair<int, int> * Label;
	uint8_t *transfer;
	uint8_t *vexter;
	uint16_t Lup;
	uint16_t Ldown;
	int flabel1;
	int flabel2;
	uint8_t *MaxT; //最大换乘 = sum of chian - 2 
	ReachIndex(){
		Label = NULL;
		transfer = NULL;
		vexter = NULL;
		//MaxT = NULL;
	}

	~ReachIndex(){
		if (Label != NULL)
			delete[] Label;
		if (transfer != NULL)
			delete[] transfer;
		if (vexter != NULL)
			delete[] vexter;
		//if (MaxT != NULL)
		//	delete[] MaxT;
	}
};
vector<ReachIndex> reachindex;
void copyIndex()
{
	reachindex.resize(vertex_num);
	for (int i = 0; i < vertex_num; ++i)
	{
		reachindex[i].Lup = Lup[i];
		reachindex[i].Ldown = Ldown[i];
		reachindex[i].flabel1 = flabel1[i];
		reachindex[i].flabel2 = flabel2[i];

		reachindex[i].OutLimit = Lout[i].size();
		reachindex[i].InLimit = Lin[i].size() + reachindex[i].OutLimit;
		reachindex[i].Label = new pair<int, int>[reachindex[i].InLimit];
		reachindex[i].transfer = new uint8_t[reachindex[i].InLimit];
		reachindex[i].vexter = new uint8_t[reachindex[i].InLimit];
		//reachindex[i].MaxT = new uint8_t[reachindex[i].InLimit];

		for (int j = 0; j < Lout[i].size(); ++j) {
			reachindex[i].Label[j] = Lout[i][j];
			//reachindex[i].transfer[j] = Lout_trans[i][j].first;

		    reachindex[i].transfer[j] = loutTranst[i][j];
			reachindex[i].vexter[j] = end_v[i][j];
			//reachindex[i].MaxT[j] = Lout_trans[i][j].second;
			//reachindex[i].MaxT[j] = 20;
		}
		for (int j = 0; j < Lin[i].size(); ++j){
			reachindex[i].Label[reachindex[i].OutLimit + j] = Lin[i][j];
			reachindex[i].transfer[reachindex[i].OutLimit + j] = linTranst[i][j];
			//reachindex[i].transfer[reachindex[i].OutLimit + j] = Lin_trans[i][j].first;
			reachindex[i].vexter[reachindex[i].OutLimit + j] = rec_v[i][j];
			//reachindex[i].MaxT[reachindex[i].OutLimit + j] = Lin_trans[i][j].second;
			//cout << i << "  " << reachindex[i].OutLimit + j <<" "<< Lin[i][j].first << " " << Lin[i][j].second << endl;
			//reachindex[i].MaxT[reachindex[i].OutLimit + j] = 20;
		}
	}
	cout << "copyIndex already done!" << endl;

}
//换乘查询
int imark;
int *test;
bool queryTranst(int from, int to)
{
	bool arrive;
	if (from == to) arrive = 1;
	else if (reachindex[from].Ldown >= reachindex[to].Ldown) arrive = 0;
	else if (reachindex[from].flabel1>reachindex[to].flabel1 || reachindex[from].flabel2>reachindex[to].flabel2) arrive = 0;
	else if (intersectTranst(from, to)) arrive = 1;
	else
	{
		imark++;
		arrive = queryTransfer(from, to);
	}
	return arrive;
}
bool intersectTranst(const int u1, const int u2)
{
	int p1 = 0;
	int p2 = reachindex[u2].OutLimit;
	while (p1 < reachindex[u1].OutLimit && p2 < reachindex[u2].InLimit)
	{
		if (reachindex[u1].Label[p1].first == reachindex[u2].Label[p2].first && reachindex[u1].Label[p1].second <= reachindex[u2].Label[p2].second) {
			return 1;
		}
		else if (reachindex[u1].Label[p1].first == reachindex[u2].Label[p2].first) {
			p1++; p2++;
		}
		else if (reachindex[u1].Label[p1].first < reachindex[u2].Label[p2].first)
			p1++;
		else
			p2++;
	}
	return 0;
}
bool IPtest(const int u, const int v);
bool queryTransfer(int from, int to)
{
	test[from] = imark;
	if (reachindex[from].flabel1 > reachindex[to].flabel1 || reachindex[from].flabel2 > reachindex[to].flabel2)
		return 0;
	if (IPtest(from, to))
		return 0;
	if (intersectTranst(from, to))
		return 1;

	for (int i = 0; i < v[from].size(); ++i)
	{
		int w = v[from][i].target;
		if (w == to) 
			return 1;
		if (reachindex[w].Ldown<reachindex[to].Ldown && test[w] != imark)
		//if (reachindex[w].Ldown<reachindex[to].Ldown)
		{
			if (queryTransfer(w, to))
				return 1;
		}
	}
	return 0;
}
// --------------------------query --------------------
int intersect(const int u1, const int u2)
{ 
	//p1 ->Lin  p2 ->Lout
	int p1 = 0;
	int p2 = reachindex[u2].OutLimit;
	while (p1 < reachindex[u1].OutLimit && p2 < reachindex[u2].InLimit)
	{
		if (reachindex[u1].Label[p1].first == reachindex[u2].Label[p2].first && reachindex[u1].Label[p1].second <= reachindex[u2].Label[p2].second) {

			if (v[reachindex[u1].vexter[p1]].size() != 0 && v[reachindex[u2].vexter[p2]].size() != 0)
			{
				if (v[reachindex[u1].vexter[p1]][0].transfer != v[reachindex[u2].vexter[p2]][0].transfer)
					return (reachindex[u1].transfer[p1] + reachindex[u2].transfer[p2]+1);
			}
			return (reachindex[u1].transfer[p1] + reachindex[u2].transfer[p2]);
			//可达，即返回换乘的个数
		}
		else if (reachindex[u1].Label[p1].first == reachindex[u2].Label[p2].first) { 
			p1++; p2++; 
		}
		else if (reachindex[u1].Label[p1].first < reachindex[u2].Label[p2].first) 
			p1++;
		else 
			p2++;
	}
	return -1;
}

int mark = 0;
int *flag;

bool IPtest(const int u, const int v)
{

	int it1 = 0, it2 = 0;

	if (reachindex[u].Label[reachindex[u].OutLimit - 1].first > reachindex[v].Label[0].first){
		while (it1 != reachindex[u].OutLimit && it2 != reachindex[v].OutLimit){
			if (reachindex[u].Label[it1].first == reachindex[v].Label[it2].first){
				if (reachindex[u].Label[it1].second > reachindex[v].Label[it2].second) 
					return true;
				it1++;
				it2++;
			}
			else if (reachindex[u].Label[it1].first < reachindex[v].Label[it2].first){
				it1++;
			}
			else
				return true;
		}
	}
	if (reachindex[v].Label[reachindex[v].InLimit - 1].first <= reachindex[u].Label[reachindex[u].OutLimit].first)
		return false;

	it1 = reachindex[v].OutLimit;
	it2 = reachindex[u].OutLimit;

	while (it1 != reachindex[v].InLimit && it2 != reachindex[u].InLimit){
		if (reachindex[v].Label[it1].first == reachindex[u].Label[it2].first){
			if (reachindex[v].Label[it1].second < reachindex[u].Label[it2].second) 
				return true;
			it1++;
			it2++;
		}
		else if (reachindex[v].Label[it1].first < reachindex[u].Label[it2].first){
			it1++;
		}
		else
			return true;
	}

	return false;

}

int insidetest(int from, int to)
{
	//new version
	int l = 0; int r = reachindex[from].OutLimit - 1;
	int m;
	while (l <= r)
	{
		m = (l + r) / 2;
		if (reachindex[from].Label[m].first == rank_order[toChain[to]])
		{
			if (reachindex[from].Label[m].second <= toPos[to]){
				return 1;
			}
			return 2;
		}
		else if (reachindex[from].Label[m].first < rank_order[toChain[to]]){
			l = m + 1;
		}
		else
			r = m - 1;
	}
	return 0;
}

int countQuery = 0;
int recursiondegree = 0;
int query(int from, int to)
{
	countQuery++;
	flag[from] = mark;

	if (reachindex[from].flabel1 > reachindex[to].flabel1 || reachindex[from].flabel2 > reachindex[to].flabel2) 
		return -1;
	if (IPtest(from, to)) 
		return -1;

	int result = intersect(from, to);
	if (result >= 0)
		return result;
	//if (intersect(from, to)) 
	//	return 1;

	for (int i = 0; i < v[from].size(); ++i)
	{
		int w = v[from][i].target;
		if (w == to) {
			if (v[to].size() != 0)
			{
				if (v[w][0].transfer != v[to][0].target)
					return 1;
			}
			return 0;
		}
		if (reachindex[w].Ldown<reachindex[to].Ldown && flag[w] != mark)
		{
			result = query(w, to);
			if (result >= 0)
				return result;
			if (query(w, to)) 
				return 1;
		}
	}
	return -1;
}
bool naiveQuery(int from, int to)
{
	flag[from] = mark;
	if (from == to) return 1;
	for (int i = 0; i < v[from].size(); ++i)
	{
		int w = v[from][i].target;
		if (flag[w] != mark)
		{
			if (naiveQuery(w, to)) 
				return 1;
		}

	}
	return 0;
}
vector<int> earlist_start;
vector<vector<int>> earlist_target;
vector<int> earlist_st;//start time;
vector<int> earlist_et;//end time;
vector<int> earlist_tr;//transfer
vector<pair<int,int>> answers; //结果
//void eaqPrintf(vector<int> arrival);
void eaqPrintf(vector<pair<int, int>> arrival);
void query(string s)
{
	/*
		query文件参数：
			1.起始顶点
			2.目标顶点
			3.最大换乘数
	*/
	flag = new int[vertex_num];
	memset(flag, 0, sizeof(int)*vertex_num);
	fstream read;
	read.open(s.c_str(), ios::in);
	int queryNum;
	read >> queryNum;
	earlist_start.resize(queryNum);
	earlist_target.resize(queryNum);
	earlist_st.resize(queryNum);
	earlist_et.resize(queryNum);
	earlist_tr.resize(queryNum);
	int tmpst, tmpet; 
	cout << "这里还没有报错" << endl;
	for (int i = 0; i < queryNum; i++){
		// 起始点 -> 目标点 —>起始时间 -> 到达时间 -> 最大换乘数
		//read >> earlist_start[i] >> earlist_target[i] >> earlist_st[i] >> earlist_et[i] >> earlist_tr[i];
		read >> tmpst >> tmpet >> earlist_st[i] >> earlist_et[i] >> earlist_tr[i];
		earlist_start[i] = -1;
		//earlist_target[i] = -1; //存储的是扩展后的
		int time;
		for (int j = 0; j < startT[tmpst].size(); j++){
			if (startT[tmpst][j].time >= earlist_st[i]){
				earlist_start[i] = getVertexID(tmpst, j, 1); //最早出发
				time = startT[tmpst][j].time;
				break;
			}
		}
		for (int j = 0; j < arrivalT[tmpet].size(); j++){
			if (arrivalT[tmpet][j].time > time && arrivalT[tmpet][j].time <= earlist_et[i]){
				earlist_target[i] .push_back(getVertexID(tmpet,j, 0)); 
				//break;//
			}
		}
	}
	read.close();
	
	//read.open("vertex.txt", ios::out);
	//for (int i = 0; i < queryNum; i++){
	//	read << earlist_start[i] <<"    " << earlist_target[i] << endl;
	//}
	//read.close();
	cout << "query number: " << earlist_start.size() << endl;

	countQuery = 0;
	int reachNum = 0;
	int result;
	int ST = clock();
	for (int i = 0; i < queryNum; i++)
	{
		bool arrive;
		for (int j = 0; j < earlist_target[i].size(); j++)
		{

			if (earlist_start[i] == -1 || earlist_target[i][j] == -1)
				arrive = 0;
			else if (originalId[earlist_start[i]] == originalId[earlist_target[i][j]]) //是同一个链内 ->等待时间 不存在换乘
				arrive = 1;
			else if (reachindex[earlist_start[i]].Ldown >= reachindex[earlist_target[i][j]].Ldown)
				arrive = 0;
			else if (reachindex[earlist_start[i]].flabel1>reachindex[earlist_target[i][j]].flabel1 || reachindex[earlist_start[i]].flabel2>reachindex[earlist_target[i][j]].flabel2)
				arrive = 0;
			else if ((result = intersect(earlist_start[i], earlist_target[i][j])) >= 0){ //返回换乘数
				//earlist_tr 表示的是最大的换乘数   -> 我已经可达了 ，你的最大换乘数都比我这两个点的最大换乘大 -> 一定可达

				if (earlist_tr[i] - result >= 0)
					arrive = 1;
				else
					arrive = 0;
			}
			else
			{
				mark = i + 1;
				result = query(earlist_start[i], earlist_target[i][j]);
				if (result == -1){ //不可达
					arrive = 0;
				}
				else{
					if (earlist_tr[i] - result >= 0)
						arrive = 1;
					else
						arrive = 0;
				}
			}
			if (arrive == 1)
				break;
		}
		reachNum += arrive;
		answers.push_back(make_pair(arrive, result));
	}
	int ET = clock();
	double endtime = (double)(ET - ST) / CLOCKS_PER_SEC;
	cout << "查询耗时：" << endtime << " s" << endl;
	cout << "reach number: " << reachNum << endl;
	cout << endl;
	cout << "countQuery= " << countQuery << endl;
	eaqPrintf(answers);
	delete[] flag;
}
//--------------------------最早到达查询---------------
queue <int> _queue;
const int infinity = 1e9;
vector <int> start_time;

vector<int> art_v;

//void restrictByTR(int from, int to, int tr, int et);
int restrictByTR(int from, int to);
//void buildEarlistArrivalQuery(string s1){ //读入最早到达信息
//	fstream read;
//	read.open(s1.c_str(), ios::in);
//	int queryNum;
//	read >> queryNum;
//	earlist_start.resize(queryNum);
//	earlist_target.resize(queryNum);
//	earlist_st.resize(queryNum);
//	earlist_et.resize(queryNum);
//	earlist_tr.resize(queryNum);
//
//	flag = new int[vertex_num];
//	memset(flag, 0, sizeof(int)*vertex_num);
//	for (int i = 0; i < queryNum; i++){
//		read >> earlist_start[i] >> earlist_target[i] >> earlist_st[i] >> earlist_et[i] >> earlist_tr[i];
//	}
//}
void init(){

	while (!_queue.empty()){
		_queue.pop();
	}

	for (int i = 0; i<chainNum; i++){
		start_time[i] = infinity;
	}
	art_v.clear();

	for (int i = 0; i<vertex_num; i++){
		visited[i] = false;
		vertexInQueue[i] = false;
	}
}
void earlist_arrival(int from,int to ,int st ,int et,int tr ){
	start_time[from] = st;
	
	int _u, _v, uid,tar;
	for (int j = 0; j < startT[from].size(); j++){
		_u = getVertexID(from, j, 1);
		if (toPos[_u] >= st && toPos[_u] <= et){
			_queue.push(_u);
		}
	}
	while (!_queue.empty()){
		_u = _queue.front();
		_queue.pop();
		visited[_u] = true;
		vertexInQueue[_u] = false;

		uid = originalId[_u];
		if (toPos[_u] < start_time[uid]){
			start_time[uid] = toPos[_u];
			art_v.push_back(_u);
			//cout << _u << endl;
		}
		for (int i = 0; i < v[_u].size(); i++){
			//先进 id 后进 v
			_v = v[_u][i].target;
			if (!visited[_v] && !vertexInQueue[_v]){
				if (toPos[_v] >= st && toPos[_v] <= et){
					_queue.push(_v);
					vertexInQueue[_v] = true;
				}
			}
		}
	}	
}
/*
 针对扩展的
 起始顶点，目标顶点，最大换乘数，最晚到达时间
*/
int restrictByTR(int from ,int to){

	/*
	query文件参数：
	1.起始顶点
	2.目标顶点
	3.最大换乘数
	*/


	//read query
	int t1, t2, trans;
	int i = to;
	int id = originalId[to]; //原始点中的id
	bool arrive;
	/*
	for (i = to; i<sumSize[id]; i++)
	{
		if (from == i)
			arrive = 1;
		else if (reachindex[from].Ldown >= reachindex[i].Ldown)
			arrive = 0;
		else if (reachindex[from].flabel1>reachindex[i].flabel1 || reachindex[from].flabel2>reachindex[i].flabel2)
			arrive = 0;
		else if (intersect(from, i) == 1)
			arrive = 1;
		else
		{
			arrive = query(from, i);
		}
	}
	*/
	if (from == i)
		return 1;
	else if (reachindex[from].Ldown >= reachindex[i].Ldown)
		return 0;
	else if (reachindex[from].flabel1 > reachindex[i].flabel1 || reachindex[from].flabel2 > reachindex[i].flabel2)
		return 0;
	else if (intersect(from, i) == 1)
		return 1;
	else
	{
		return query(from, i);
	}
	//cout << "点  " << from << "  到点  " << i << "  可达情况为  " << arrive << endl;

	//eaqPrintf(arrive);
}
//void main_earlist_arrival(string s1){
//
//	buildEarlistArrivalQuery(s1);
//	start_time.resize(chainNum);
//	art_v.resize(chainNum);
//	for (int i = 0; i < earlist_start.size(); i++){
//		init();
//		earlist_arrival(earlist_start[i], earlist_target[i], earlist_st[i], earlist_et[i], earlist_tr[i]);
//	}
//}
void eaqPrintf(vector<pair<int,int>> arrival){
	fstream write;
	write.open("result.txt", ios::out);
	for (int i = 0; i < arrival.size(); i++){
		//write << arrival[i].first<<"  "<<arrival[i].second << endl;
		write << arrival[i].first << endl;
	}
	write.close();
}
void transt(){


	//test = new int[vertex_num];
	//memset(test, 0, sizeof(int)*vertex_num);
	//imark = 1;
	int ST = clock();
	for (int i = 0; i < rec_v.size(); i++){
		//int tmp = topoOrder[i];
		for (int j = 0; j < rec_v[i].size(); j++){
			if (rec_v[i][j] == i){
				Lin_trans[i].push_back(make_pair(0, 0));
			}
			else{
				test = new int[vertex_num];
				memset(test, 0, sizeof(int)*vertex_num);
				imark = 0;
				Lin_trans[i].push_back(linCalTransfer(rec_v[i][j], i, toPos[rec_v[i][j]], toPos[i]));
				delete[] test;
			}

		}
		//for (int j = 0; j < rec_v[tmp].size(); j++){
		//	if (rec_v[tmp][j] == tmp){
		//		Lin_trans[tmp].push_back(make_pair(0, 0));
		//		continue;
		//	}
		//	Lin_trans[tmp].push_back(linCalTransfer(rec_v[tmp][j], tmp, toPos[rec_v[tmp][j]], toPos[tmp])); //传递的时间为到达时间
		//}
	}


	int ET = clock();
	double endtime = (double)(ET - ST) / CLOCKS_PER_SEC;
	cout << "Lin的换乘构造费时： " << endtime << " s" << endl;

	//测试
	fstream f;
	f.open("Lin_trans.txt", ios::out);
	for (int i = 0; i < Lin_trans.size(); i++){
		for (int j = 0; j < Lin_trans[i].size(); j++){
			f << Lin_trans[i][j].first << "  ";
		}
		f << endl;
	}
	f.close();
	

	ST = clock();
	for (int i = 0; i < end_v.size(); i++){
		//imark = 1;
		int tmp = rtopoOrder[i];
		for (int j = 0; j < end_v[tmp].size(); j++){
			if (end_v[tmp][j] == tmp){
				Lout_trans[tmp].push_back(make_pair(0, 0));
				continue;
			}
			Lout_trans[tmp].push_back(loutCalTransferByBFS(tmp, end_v[tmp][j], toPos[tmp], toPos[end_v[tmp][j]]));
		}
	}
	ET = clock();
	endtime = (double)(ET - ST) / CLOCKS_PER_SEC;
	cout << "Lout的换乘构造费时： " << endtime << " s" << endl;

	f.open("Lout_trans.txt", ios::out);
	for (int i = 0; i < Lout_trans.size(); i++){
		for (int j = 0; j < Lout_trans[i].size(); j++){
			f << Lout_trans[i][j].first << "  ";
		}
		f << endl;
	}
	f.close();
	delete[] test;


	for (int i = 0; i < vertex_num; ++i)
	{
		reachindex[i].transfer = new uint8_t[reachindex[i].InLimit];
		reachindex[i].MaxT = new uint8_t[reachindex[i].InLimit];

		for (int j = 0; j < Lout[i].size(); ++j) {
			reachindex[i].transfer[j] = Lout_trans[i][j].first;
			reachindex[i].MaxT[j] = Lout_trans[i][j].second;
		}
		for (int j = 0; j < Lin[i].size(); ++j){
			reachindex[i].transfer[reachindex[i].OutLimit + j] = Lin_trans[i][j].first;
			reachindex[i].MaxT[reachindex[i].OutLimit + j] = Lin_trans[i][j].second;
		}
	}

}

int main(int argc, char* argv[]){

	string _parameterI = "scc.txt";

	string _parameterII = "query.txt";

	string _parameterIII = "info.txt";

	string _parameterIV = "reachability.txt";

	string  _parameterV = "arrival.txt";

	k = 2;

	//读入scc图的相关数据
	readGraph(_parameterI, _parameterIII);

	//读入Info文件内的相关信息    
	readGraphInfo(_parameterIII);

	//buildTimeId();

	//构建chain的相关信息
	findChain();

	//进行拓扑（逆）排序
	topological();

	//标签的构建
	lableBuild();

	copyIndex();


	//transt();
	/*
	查询次数
	起始点，终点 ，起始时间，到达时间，最大换乘次数
	*/
	//int ST = clock();
	query(_parameterII);
	//int ET = clock();
	//double endtime = (double)(ET - ST) / CLOCKS_PER_SEC;
	//cout << "查询耗时：" << endtime << " s" << endl;

	// drop
	//===================================	
	/*
	 查询次数
	 起始点，终点 ，起始时间，到达时间，最大换乘次数
	*/
	//main_earlist_arrival(_parameterV);
	//main_earlist_arrival(_parameterII);
}