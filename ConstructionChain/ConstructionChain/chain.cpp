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
	int target;  //Ŀ�궥��  ������ʼ����͵��ﶥ��
	int transfer; //���� ��ʾΪ����id
	Graph(int _target, int _transfer){
		target = _target;
		transfer = _transfer;
	}
};
class EdgeInfo{
public:
	int time; // ʱ�� ��������ʱ��ͳ���ʱ��
	int transfer; // ���� ��ʾΪ����id
	int target; //
	//int duration; //����ʱ��
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
	int index; //��¼id
};
vector<degree> deg;
bool cmp(degree d1, degree d2);


int vertex_num, edge_num;
int chainNum;
vector<vector<Graph>> v;  //�����ĳ�����Ϣ��¼
vector<vector<Graph>> rv; //�����������Ϣ��¼
vector<int> ind;
vector<int> outd;
vector<bool> visited;
vector <bool> vertexInQueue;
vector<int> originPath; //��ԭʼ��op
vector<vector<int>> linMapper;
vector<vector<int>> loutMapper;
//vector<int> iTopoOrigin; //���˺����
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

	//���㻻�ˡ���ѯ��
	//vertexInQueue.resize(vertex_num);
	visited.resize(vertex_num);
	vertexInQueue.resize(vertex_num);
	//visited.resize(chainNum);

	vector<vector<Graph>> tmp_rv; //�����������Ϣ��¼
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
}  //�Ѳ���

vector <int> inOut;
vector <int> pNext;
vector<vector<EdgeInfo>> startT, arrivalT;
vector< int > curSize, sumSize;
vector< int > originalId;
vector<int> vectorTr; //����transfer 


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

	// ��������
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
		//�����Ż�,�˴�ע��
		//arrivalT[i].resize(_size);
		for (int j = 0; j < _size; j++){
			_read >> _time;
			_read >> _transfer;
			_read >> _target;
			//_read >> _duration;
			//_duration = 1;

			//�Ż�
			EdgeInfo ei(_time, _transfer,_target);
			arrivalT[i].push_back(ei);

			//���Ż�
			//arrivalT[i][j].time = _time;
			//arrivalT[i][j].transfer = _transfer;
		}
	}
	for (int i = 0; i < chainNum; i++){
		_read >> _id >> _size;
		//�����Ż� �˴�ע��
		//startT[i].resize(_size);
		for (int j = 0; j < _size; j++){
			_read >> _time;
			_read >> _transfer;
			_read >> _target;
			//_read >> _duration;
			//_duration = 1;

			//�Ż�
			EdgeInfo e(_time, _transfer,_target);
			startT[i].push_back(e);  // startT��duration��ʵ������

			//���Ż�
			//startT[i][j].time = _time;
			//startT[i][j].transfer = _transfer;
		}
	}
	_read.close();

	//for (int i = 0; i< chainNum; i++){
	//	// ��������i��ʱ�����timeId
	//	for (int j = 0; j<arrivalT[i].size(); j++){
	//		//timeId.push_back(arrivalT[i][j].time);
	//		vectorTr.push_back(arrivalT[i][j].transfer);
	//		
	//	}
	//	//�����i�ĳ���ʱ�����timeId
	//	for (int j = 0; j<startT[i].size(); j++){
	//		//timeId.push_back(startT[i][j].time);
	//		vectorTr.push_back(startT[i][j].transfer);
	//	}
	//}

	cout << "chain info already load done" << endl;
} //����֤


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

	//ԭ��
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

	// toPost�����д洢����ת����sccͼ�ĵ��������Ϣ��
	/*
	���� ��a,1) -> (b,2) ->(b,3 )
	��ô toPost[0]��ֵΪ1
	toPost[3]��ֵΪ2
	toPost{4]��ֵΪ3

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
//��������
vector<int> topoOrder;
//����������
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
	//���� in_degree or out_degree Ϊ0�ĵ�
	for (int i = vertex_num - 1; i >= 0; i--){

		if (ind[i] == 0)
		{
			// stack1 �洢 in_degree = 0
			stack1.push(i);
			Ldown[i] = 0;
		}

		if (outd[i] == 0)
		{
			// stack3 �洢 out_degree = 0�ĵ�
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
		������˺������˵Ĺ���
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
//�������ݱ���.�����������ֵ
//void pathPreservation(){
//	vector<int>::iterator it, it1;
//	for (it = ++originPath.begin(); it != originPath.end();){
//		it1 = find(originPath.begin(), it, *it);
//		if (it1 != it)
//			it = originPath.erase(it1); //����
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
// Ŀ����Ϊ�˻�ȡ��չ����id
// ���Դ��ݵ��� ��չ������id �� position
//vector<vector<int>> Lin_trans; //��¼��������
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
pair<int, int> iCalTransferByBFS(int st, int et, int stime, int etime);//����ʱ��������ȷ����ʼ���Լ�ʱ�䷶Χ
pair<int, int> myCalTransferByBFS(int st, int et, int stime, int etime);
pair<int, int>  vCalTransferByBFS(int st, int et, int stime, int etime);
pair<int, int>  loutCalTransferByBFS(int st, int et, int stime, int etime);
pair<int, int> linCalTransfer(int st, int et, int stime, int etime);
vector<vector<int>> rec_v; //��¼Linÿ����ǩ��Ӧ����չ��scc���id
vector<vector<int>>end_v;
vector<vector<int>>inNeighbor;
vector<vector<int>>outNeighbor;
vector<vector<int>> linTranst;
vector<vector<int>>loutTranst;
//����
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

	int ST, ET; //��ʱ ��ʼʱ�� �� ��ֹʱ��
	double endtime;
	// ------------------------------
	//testRankOrder();

	//vector<bool> stl; //= true;//�����ڱ� true������Ч ����һ��
	//stl.resize(k, true);

	//Lin
	ST = clock();
	for (int i = 0; i < vertex_num; i++){


		vector<int> icount; //���ڼ���

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
		//toChain��val ��ʾ����ԭʼͼ�е�id
		// Lin�ĵ�һ������ �洢����ԭʼ������չͼ�е�rankֵ
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

		


		// vr[tmp].size() -> ��tmp����ȵ����     i  --- > j
		for (int j = 0; j < rv[tmp].size(); j++)
		{

			/*int add = 0;
			bool alreadAdd = false;*/
			int nin = rv[tmp][j].target;
			//cout << "current: " << tmp << "  �� " << j << " �� �������ھ� "<<nin<<" ==============================" << endl;

			// ��ͬȦ,���Ҵ��ڻ��˵����
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

					// ͬȦ B \C ��ͬ����
					//if (curtranst != -1 && alreadAdd != true && v[tmp].size() != 0 && v[nin].size() != 0 && v[rec_v[nin][p2]].size() != 0 && curtranst != v[nin][0].transfer && originalId[nin] == originalId[tmp]){
					//	
					//	// A B ͬ���� -> A C��ͬ����
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

					//// ͬȦ B \C ��ͬ����
					//if (curtranst != -1 && alreadAdd != true && v[tmp].size() != 0 && v[nin].size() != 0 && v[rec_v[nin][p2]].size() != 0 && curtranst != v[nin][0].transfer && originalId[nin] == originalId[tmp]){

					//	// A B ͬ���� -> A C��ͬ����
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
						// ͬȦ B \C ��ͬ����
						//if (curtranst != -1 && alreadAdd != true && v[tmp].size() != 0 && v[nin].size() != 0 && v[rec_v[nin][p2]].size() != 0 && curtranst != v[nin][0].transfer && originalId[nin] == originalId[tmp]){

						//	// A B ͬ���� -> A C��ͬ����
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
	cout << "Lin��ǩ�����ʱ�� " << endtime << " s" << endl;

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
		vector<int> icount; //���ڼ���

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
					//// ͬȦ B \C ��ͬ����
					//if (curtranst != -1 && alreadAdd != true && v[tmp].size() != 0 && v[nin].size() != 0 && v[end_v[nin][p2]].size() != 0 && curtranst != v[nin][0].transfer && originalId[nin] == originalId[tmp]){

					//	// A B ͬ���� -> A C��ͬ����
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
					//// ͬȦ B \C ��ͬ����
					//if (curtranst != -1 && alreadAdd != true && v[tmp].size() != 0 && v[nin].size() != 0 && v[end_v[nin][p2]].size() != 0 && curtranst != v[nin][0].transfer && originalId[nin] == originalId[tmp]){

					//	// A B ͬ���� -> A C��ͬ����
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
						// ͬȦ B \C ��ͬ����
						//if (curtranst != -1 && alreadAdd != true && v[tmp].size() != 0 && v[nin].size() != 0 && v[end_v[nin][p2]].size() != 0 && curtranst != v[nin][0].transfer && originalId[nin] == originalId[tmp]){

						//	// A B ͬ���� -> A C��ͬ����
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
	cout << "Lout��ǩ�����ʱ�� " << endtime << " s" << endl;

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
/*����BFS����·�������㻻��*/

/*
����
*/
int TEST_ST;
int TEST_ET;
double TEST_RESULT;
//vector<int>::iterator it;
//����pair����
// time������ʱ�䣬�������յ���ʱ�� -> ͨ��ʱ����������ҵ���Ӧ����չ����
void vCalTransferByBFS(){
	queue <int> bfsqueue;
	vector<int> path;
	stack<int> tmp;
	vector<int> op;//Original path
	int u = 23548; //ԭʼĿ���
	//int u = 7681; 	//�����ĵ�
	//int target = originalId[et]; //�յ�Ŀ���
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
		visited[u] = true; //������
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
	cout << "��ϴǰ����:  " <<path.size()<< endl;
	sort(path.begin(), path.end());
	path.erase(unique(path.begin(), path.end()), path.end());
	cout << "��ϴ������:  " << path.size() << endl;
	c.close();
}
//pair<int, int>  loutCalTransferByBFS(int st, int et, int stime, int etime){
//	queue <int> bfsqueue;
//	queue<int> path;
//	stack<int> tmp;
//	vector<int> op;//Original path
//	int u = originalId[st]; //ԭʼĿ���
//	int target = originalId[et]; //�յ�Ŀ���
//	int trans = 0;
//	int ist = st; //��չ��ĳ�����
//	int iet = et; //��չ���Ŀ���
//	int _v;
//	int currentTrans = -1; //������һ�εĻ��˵� ��ʼ��-1
//	int currentTime = etime;
//	vector<int>::iterator uit = find(originPath.begin(), originPath.end(), u); //�ҵ���ʼ��
//	vector<int>::iterator vit = find(originPath.begin(), originPath.end(), target); //�ҵ��յ�
//
//	// �����������Ϊ�� ����Χ�ڵĵ�ȫ��ѹ��op��
//
//
//	//�Ӻ��濪ʼ��.��Ϊ���һ��ֵ�ǵ���
//	if (uit != originPath.end() && vit != originPath.end()){
//		int uindex = uit - originPath.begin();
//		int vindex = vit - originPath.begin(); //����
//		if (uindex < vindex){
//			for (int i = uindex; i <= vindex; i++)
//				op.push_back(originPath[i]); //���ҵ��ĵ�ѹ��ջ��
//		}
//		else{
//			// vindexֵ��uindexֵС
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
//		//	op.push_back(u); //��¼��� 
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
//	// ��ŵ� ��һ���ǵ��� or ��ʼ��  ��ӳ��� ��Linʱ��һ���ǳ�����
//	while (op.size() != 0){
//		u = op[op.size() - 1]; //�����
//		op.pop_back();
//		if (op.size() == 0)
//			break;
//		for (int i = arrivalT[u].size()-1 ; i >=0; i--)
//		{
//			if (arrivalT[u][i].time <= endTime) //�ҵ��õ�
//			{
//				index = i;
//				v = arrivalT[u][i].target; //�ҵ���һ����
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
//		//֮ǰ��������ʱ��Ϊ1 ��� ����ʱ�� + 1 = ����ʱ��
//		for (int i = startT[v].size()-1; i >=0; i--){
//			if (startT[v][i].target == u && startT[v][i].time <= arrivalT[u][index].time - arrivalT[u][index].duration){ //�ҵ��õ�
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
	int u = originalId[st]; //ԭʼĿ���
	int target = originalId[et]; //�յ�Ŀ���
	int beginTime = stime;
	int trans = 0;
	int ist = st; //��չ��ĳ�����
	int iet = et; //��չ���Ŀ���
	int _v;
	int currentTrans = -1; //������һ�εĻ��˵� ��ʼ��-1
	int currentTime = etime;
	vector<int>::iterator uit = find(originPath.begin(), originPath.end(), u); //�ҵ���ʼ��
	vector<int>::iterator vit = find(originPath.begin(), originPath.end(), target); //�ҵ��յ�

	// �����������Ϊ�� ����Χ�ڵĵ�ȫ��ѹ��op��
	if (uit != originPath.end() && vit != originPath.end()){
		int uindex = uit - originPath.begin();
		int vindex = vit - originPath.begin();
		for (int i = uindex; i <= vindex; i++)
			op.push_back(originPath[i]); //���ҵ��ĵ�ѹ��ջ��
	}
	else{
		bfsqueue.push(u);
		while (!bfsqueue.empty())
		{
			u = bfsqueue.front();
			op.push_back(u); //��¼��� 
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
		// ��ý��u ע����ȷ���ý���ǳ�����㻹�ǵ�����
		u = op[op.size() - 1];
		op.pop_back();//�ҵ����� ����㵯��


		//��ͷ��ʼ����arrival �жϸýڵ��Ƿ��ǳ������
		for (int i = 0; i < arrivalT[u].size(); i++){

			// ����ýڵ��ǳ������
			if (getVertexID(u, i, 0) == iet)
			{
				// ��ʼ�������еĿ��ܷ�Χ�ڵĵ�
				for (int k = 0; k < op.size(); k++){

					//��� Ŀ�궥����op��,���ҵ��õ� index
					if (op[k] == arrivalT[u][i].target){
						int index = op[k];//��ȡ��ʼ��
						int begin = k; // ����

						//���� startT[index]�е����е�,Ŀ����Ϊ�˲鿴�Ƿ���ڻ���.���������Ҫ���ҵ�����ĳ�����
						for (int j = 0; j < startT[index].size(); j++){

							if (startT[index][j].target == u);//�ҵ�Ŀ��� �� index���Ŀ��� �� u  .ע��������������transfer����ͬ��
							{
								if (currentTrans != -1 && currentTrans != arrivalT[u][i].transfer)// �����ǰֵ��Ϊ��ʼֵ������Ȧ�ڵĵ��ﻻ�� ��= Ȧ�ڵĳ�������
									trans++;
								currentTrans = startT[index][j].transfer; //��¼����ʱ�Ļ��˵� ����һ�εĵ������бȶ�

								
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

				// ����ýڵ��ǳ������
				if (getVertexID(u, i, 1) == iet)
				{
					// ��ʼ�������еĿ��ܷ�Χ�ڵĵ�
					for (int k = 0; k < op.size(); k++){

						//��� Ŀ�궥����op��,���ҵ��õ� index
						if (op[k] == startT[u][i].target){
							int index = op[k];//��ȡ��ʼ��
							int begin = k; // ����
							//int t = startT[u][i].duration; // ��ȡ�ó̵ĳ���ʱ��
							int t = 1;
							//���� startT[index]�е����е�,Ŀ����Ϊ�˲鿴�Ƿ���ڻ���.���������Ҫ���ҵ�����ĳ�����
							for (int j = 0; j < arrivalT[index].size(); j++){

								if (arrivalT[index][j].target == u);//�ҵ�Ŀ��� �� index���Ŀ��� �� u  .ע��������������transfer����ͬ��
								{
									if (currentTrans != -1 && currentTrans != startT[u][i].transfer)// �����ǰֵ��Ϊ��ʼֵ������Ȧ�ڵĵ��ﻻ�� ��= Ȧ�ڵĳ�������
										trans++;
									currentTrans = arrivalT[index][j].transfer; //��¼����ʱ�Ļ��˵� ����һ�εĵ������бȶ�


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
//	int u = originalId[st]; //ԭʼĿ���
//	int target = originalId[et]; //�յ�Ŀ���
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
//			op.push_back(u); //��¼��� 
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
//	//ȷ����Ҫ����Щ�����ܹ�����
//	bool senior = false;
//
//	// op��u���������������ֱ����vɨ�赽ֹͣ
//	// ���п��ܰ���������� �м���w �޷�����v
//	// ע�⣺������v��һ��ѹ��op������
//	int startTrans; //����start��Ļ���
//	int arrivalTrans; //���� arrival��Ļ���
//	int currentTrans = -1; //������һ�εĻ��˵� ��ʼ��-1
//	if (_u == 0 && target == 11)
//		cout << endl;
//	/*
//	60000 ->
//	
//	*/
//	while (op.size() != 0){
//		u = op[op.size() - 1];
//		op.pop_back();
//		//���絽��
//		for (int i = arrivalT[u].size()-1; i >=0; i--){
//			if (arrivalT[u][i].time > currentTime) //ȷ������ʱ��
//				continue;
//			//ɨ�� ����u�����ԭʼ������op��
//			it = find(op.begin(), op.end(), arrivalT[u][i].target);
//			if (it != op.end()){
//				int index = it - op.begin();
//				for (int j = 0; j < startT[op[index]].size(); j++){
//					// ������㻻������
//					startTrans = startT[op[index]][j].transfer;
//					arrivalTrans = arrivalT[u][i].transfer;
//					// ǰһ�κͺ�һ�εĻ�����ͬ ���ҳ���ʱ���С�ڵ���ʱ�� ��currentTime)
//					if (startTrans == arrivalTrans && startT[op[index]][j].time < currentTime){
//						if (currentTrans != -1 && currentTrans != arrivalTrans) //һ����Ҫ����
//							trans++;
//						currentTrans = startTrans; //��¼����ʱ�Ļ��˵� ����һ�εĵ������бȶ�
//						currentTime = arrivalT[u][i].time - arrivalT[u][i].duration; //�洢�Ķ��ǵ���ʱ��
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
//	//�������ı���
//	//����ʱ��Խ��,����Խ��
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
	// lin�ص� st��һ����startT��. �ʿ��Ը���startT�����ҵ�Ŀ�궥��
	int transt = 0;
	int currentTranst; //��ǰ����
	int u = originalId[st]; //��ȡԭʼ���chainId
	int index = -1 ;
	int currentTime;
	int target;
	for (int i = 0; i < startT[u].size(); i++)
	{
		if (startT[u][i].time == stime && queryTranst(getVertexID(u, i, 1), et)) //�ҵ�������
		{
			index = i;
			currentTranst = startT[u][i].transfer;
			target = startT[u][index].target; //�ҵ���һ��chain��arrival��
			linMapper[et].push_back(target);
			currentTime = startT[u][index].time;
			break;
		}
	}
	//cout << index << endl;
	if (index == -1) 
		cout << "û���ҵ�������,���ᱨ��" << endl; 

	bool senior = false;
	// u��������� v����ﵽ�� ������洢
	int w; 
	index = 0;
	int enforce = 0;
	vector<int>::iterator it;
	while (enforce<100){
		if (target == originalId[et]) //����õ��Ŀ�����ͬһ������ ֱ������ѭ��
			break;
		//����ȥsearch�����˵����.
		for (int i = 0; i < startT[target].size(); i++){
			if (startT[target][i].transfer == currentTranst && queryTranst(getVertexID(target, i, 1),et)){
				index = i; //��¼��ǰ������λ��
				break;
			}
		}

		//������裺���ܳ����ĵ� ���� ������Time ֮��
		for (int i = index; i < startT[target].size(); i++){ //��ǰ������
			w = startT[target][i].target; //�ҵ���һ���ĳ�����
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
					target = w; //��һ����target
					senior = true;
					break;
				}
				else if (queryTranst(getVertexID(w, k, 1), et)){ //�����һ������������ɴ�������
					linMapper[et].push_back(w);
					//�ҵ���������
					if (startT[target][i].transfer != currentTranst)
						transt++;
					currentTranst = startT[target][i].transfer;
					//currentTime = startT[target][i].time;
					target = w; //��һ����target
					senior = true;
					break;
				}
			}
			if (senior) 
				break;
		}
		//cout << target << endl;

		if (senior == false){ //��������ɴ�
			cout << "����senior == falsel��" << endl;
			transt = 20; //������󻻳��� �����ɴ� �����˳�
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
	// lout�ص� ����㶼��arrival
	int transt = 0;
	int currentTranst; //��ǰ����
	int currentTime;
	int u = originalId[et]; //��ȡԭʼ���chainId
	int index = -1;
	int target;
	for (int i = 0; i < arrivalT[u].size(); i++)
	{
		if (arrivalT[u][i].time == etime) //�ҵ�������
		{
			index = i;
			currentTranst = arrivalT[u][i].transfer;
			currentTime = arrivalT[u][i].time;
			target = arrivalT[u][index].target; //�ҵ���һ��chain��arrival��
			break;
		}
	}
	//cout << index << endl;
	if (index == -1) cout << "error" << endl;

	bool senior = false;
	// u��������� v����ﵽ�� ������洢
	index = 0;
	int enforce = 0;
	int w;
	vector<int>::iterator it;
	while (enforce<100){
		if (target == originalId[st]) //����õ��Ŀ�����ͬһ������ ֱ������ѭ��
			break;
		for (int i = 0; i < arrivalT[target].size(); i++){
			if (arrivalT[target][i].transfer == currentTranst)
			{
				index = i;
				break;
			}
		}
		for (int i = index; i < arrivalT[target].size(); i++){ //����target�����
			w = arrivalT[target][i].target; //������
			if (w == originalId[st]){
				target = w;
				senior = true;
				break;
			}
			for (int k = 0; k < arrivalT[w].size(); k++){
				it = find(loutMapper[st].begin(), loutMapper[st].end(), w);
				if (it != loutMapper[st].end()){ //�����ҵ���
					if (arrivalT[target][i].transfer != currentTranst)
						transt++;
					currentTranst = arrivalT[target][i].transfer;
					currentTime = arrivalT[target][i].time;
					target = w; //��һ����target
					senior = true;
					break;
				}
				else if (queryTranst(st,getVertexID(w, k, 0))){
					loutMapper[st].push_back(w); //���� w���ɴ�linMapper
					//�ҵ���������
					if (arrivalT[target][i].transfer != currentTranst)
						transt++;
					currentTranst = arrivalT[target][i].transfer;
					currentTime = arrivalT[target][i].time;
					target = w; //��һ����target
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

	string strFileOut = ("label.bin");//����ļ�·��
	ofstream writeFile(strFileOut, ios::out | ios::binary);//�����Ƹ�ʽ
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
	string strFileOut = ("Transtlabel.bin");//����ļ�·��
	ofstream writeFile(strFileOut, ios::out | ios::binary);//�����Ƹ�ʽ
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



int MAXTRANS; //��󻻳���

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
	uint8_t *MaxT; //��󻻳� = sum of chian - 2 
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
//���˲�ѯ
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
			//�ɴ�����ػ��˵ĸ���
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
vector<pair<int,int>> answers; //���
//void eaqPrintf(vector<int> arrival);
void eaqPrintf(vector<pair<int, int>> arrival);
void query(string s)
{
	/*
		query�ļ�������
			1.��ʼ����
			2.Ŀ�궥��
			3.��󻻳���
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
	cout << "���ﻹû�б���" << endl;
	for (int i = 0; i < queryNum; i++){
		// ��ʼ�� -> Ŀ��� ��>��ʼʱ�� -> ����ʱ�� -> ��󻻳���
		//read >> earlist_start[i] >> earlist_target[i] >> earlist_st[i] >> earlist_et[i] >> earlist_tr[i];
		read >> tmpst >> tmpet >> earlist_st[i] >> earlist_et[i] >> earlist_tr[i];
		earlist_start[i] = -1;
		//earlist_target[i] = -1; //�洢������չ���
		int time;
		for (int j = 0; j < startT[tmpst].size(); j++){
			if (startT[tmpst][j].time >= earlist_st[i]){
				earlist_start[i] = getVertexID(tmpst, j, 1); //�������
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
			else if (originalId[earlist_start[i]] == originalId[earlist_target[i][j]]) //��ͬһ������ ->�ȴ�ʱ�� �����ڻ���
				arrive = 1;
			else if (reachindex[earlist_start[i]].Ldown >= reachindex[earlist_target[i][j]].Ldown)
				arrive = 0;
			else if (reachindex[earlist_start[i]].flabel1>reachindex[earlist_target[i][j]].flabel1 || reachindex[earlist_start[i]].flabel2>reachindex[earlist_target[i][j]].flabel2)
				arrive = 0;
			else if ((result = intersect(earlist_start[i], earlist_target[i][j])) >= 0){ //���ػ�����
				//earlist_tr ��ʾ�������Ļ�����   -> ���Ѿ��ɴ��� �������󻻳��������������������󻻳˴� -> һ���ɴ�

				if (earlist_tr[i] - result >= 0)
					arrive = 1;
				else
					arrive = 0;
			}
			else
			{
				mark = i + 1;
				result = query(earlist_start[i], earlist_target[i][j]);
				if (result == -1){ //���ɴ�
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
	cout << "��ѯ��ʱ��" << endtime << " s" << endl;
	cout << "reach number: " << reachNum << endl;
	cout << endl;
	cout << "countQuery= " << countQuery << endl;
	eaqPrintf(answers);
	delete[] flag;
}
//--------------------------���絽���ѯ---------------
queue <int> _queue;
const int infinity = 1e9;
vector <int> start_time;

vector<int> art_v;

//void restrictByTR(int from, int to, int tr, int et);
int restrictByTR(int from, int to);
//void buildEarlistArrivalQuery(string s1){ //�������絽����Ϣ
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
			//�Ƚ� id ��� v
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
 �����չ��
 ��ʼ���㣬Ŀ�궥�㣬��󻻳�����������ʱ��
*/
int restrictByTR(int from ,int to){

	/*
	query�ļ�������
	1.��ʼ����
	2.Ŀ�궥��
	3.��󻻳���
	*/


	//read query
	int t1, t2, trans;
	int i = to;
	int id = originalId[to]; //ԭʼ���е�id
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
	//cout << "��  " << from << "  ����  " << i << "  �ɴ����Ϊ  " << arrive << endl;

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
		//	Lin_trans[tmp].push_back(linCalTransfer(rec_v[tmp][j], tmp, toPos[rec_v[tmp][j]], toPos[tmp])); //���ݵ�ʱ��Ϊ����ʱ��
		//}
	}


	int ET = clock();
	double endtime = (double)(ET - ST) / CLOCKS_PER_SEC;
	cout << "Lin�Ļ��˹����ʱ�� " << endtime << " s" << endl;

	//����
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
	cout << "Lout�Ļ��˹����ʱ�� " << endtime << " s" << endl;

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

	//����sccͼ���������
	readGraph(_parameterI, _parameterIII);

	//����Info�ļ��ڵ������Ϣ    
	readGraphInfo(_parameterIII);

	//buildTimeId();

	//����chain�������Ϣ
	findChain();

	//�������ˣ��棩����
	topological();

	//��ǩ�Ĺ���
	lableBuild();

	copyIndex();


	//transt();
	/*
	��ѯ����
	��ʼ�㣬�յ� ����ʼʱ�䣬����ʱ�䣬��󻻳˴���
	*/
	//int ST = clock();
	query(_parameterII);
	//int ET = clock();
	//double endtime = (double)(ET - ST) / CLOCKS_PER_SEC;
	//cout << "��ѯ��ʱ��" << endtime << " s" << endl;

	// drop
	//===================================	
	/*
	 ��ѯ����
	 ��ʼ�㣬�յ� ����ʼʱ�䣬����ʱ�䣬��󻻳˴���
	*/
	//main_earlist_arrival(_parameterV);
	//main_earlist_arrival(_parameterII);
}