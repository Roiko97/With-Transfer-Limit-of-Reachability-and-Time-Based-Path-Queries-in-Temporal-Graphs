#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
using namespace std;
class Graph{
public:
	int target;//目标结点
	int startT;//出发时间
	int dur;// 持续时间
	int neiId; //出边邻居的id
	int mark;//车辆班次唯一标识符
	Graph(){
		target = 0;
		startT = 0;
		dur = 0;
		neiId = 0;
		mark = 0;
	}
};
bool cmp(Graph g1, Graph g2){
	return g1.target < g2.target;
}

bool compare(Graph g1, Graph g2){
	return g1.startT < g2.startT;
}
vector<vector<Graph>> route;
//void insertSort(int i, int from, int to)//直接插入排序
//{
//	int i, j;
//	Graph *x;
//	for (i = from+1; i<to; i++)
//	{
//		x = route[i];
//		for (j = i - 1; j >= 0 && route[j].startT>x.startT; j--)
//			data[j + 1] = data[j];
//		data[j + 1] = *x;
//	}
//}
void insertSort(int index, int from, int to)//直接插入排序
{
	int i, j;
	Graph x;
	int tmp =from == 0 ? 1 : from+1;
	for (i = tmp; i<to; i++)
	{
		//x = data[i];
		x = route[index][i];
		for (j = i - 1; j >= from && route[index][j].dur>x.dur; j--)
			route[index][j + 1] = route[index][j];
		route[index][j + 1] = x;
	}
}


void insertSortByStartT(int index, int from, int to)//直接插入排序
{
	int i, j;
	Graph x;
	int tmp = from == 0 ? 1 : from + 1;
	for (i = tmp; i<to; i++)
	{
		//x = data[i];
		x = route[index][i];
		for (j = i - 1; j >= from && route[index][j].startT>x.startT; j--)
			route[index][j + 1] = route[index][j];
		route[index][j + 1] = x;
	}
}

void main(int argc ,char** argv)
{
	int MARK = 1; //车辆班次唯一标识符

	char graph_name[100];
	strcpy(graph_name, "austin.txt");
	//strcpy(graph_name,argv[1]);
	fstream read;
	read.open(graph_name, ios::in);
	/*
		第一行 所有的站点数 所有的线路条数 所有线路的站点和 总的数据量
		第二行 线路id
		第三行 该条线路途径站点id
		第四行 站点id
		第五行 该条线路一共有10班车
	*/

	/*
		输出要求：
		所有的结点书
		结点的编号 -> 出边邻居的个数 -> 出边邻居i的id -> 有多少边去邻居点j 旅程所花费的时间 图中的权值（出发时间） 车辆唯一id
	*/
	int Size;
	read >> Size;
	vector<int> index; //记录每个点的出边邻居个数
	route.resize(Size);
	index.resize(Size);
	int loop;
	read >> loop; //一共有loop条线路
	int tmp;
	read >> tmp;
	read >> tmp;
	//    第一行数据读完
	for (int i = 0; i < loop; i++){
		if (i == 12)
			cout << endl;
		int id;
		read >> id;
		//	第二行数据读完
		int site; // 站点数
		read >> site;
		//	第三行数据读完
		vector<int> tmpSite;
		//tmpSite.resize(site);
		vector<int> index;
		bool origin = false;
		int count = 0;
		for (int j = 0; j < site; j++){
			int site_id;
			read >> site_id;
			
			//如果出现重复的,就剔除,并且记录下标
			for (int k = 0; k < tmpSite.size(); k++){
				if (tmpSite[k] == site_id){
					index.push_back(j);
					origin = true;
					break;
				}
			}
			if (origin == true){
				origin = false;
				continue;
			}
			tmpSite.push_back(site_id);
			//	第四行数据读完
			//  保存了所有的id编号
		}
		int orignSite = site;
		site = tmpSite.size();

		fstream f;
		f.open("site.txt", ios::out | ios::app);
		f << site << "   "; 
		for (int j = 0; j < tmpSite.size(); j++)
			f << tmpSite[j]<<"   ";
		f << endl<<endl;
		f.close();
		int fre;// 一共有fre个班次
		read >> fre;
		//cout << endl;
		//	第五行数据读完
		for (int j = 0; j < fre; j++){ // 10躺

			for (int n = 0; n < site - 1; n++){ //初始化向量大小,并且将出度id赋值给该点
				int extendSize = route[tmpSite[n]].size() == 0 ? 1 : route[tmpSite[n]].size() + 1;
				route[tmpSite[n]].resize(extendSize);
				//cout << tmpSite[n] << endl;
				route[tmpSite[n]][extendSize - 1].neiId = tmpSite[n + 1];
			}



			int arrival, start;
			count = 0;
			read >> arrival; //开始点的到达无用
			int plus = 0;
			bool stop = true;


			for (int k = 0; k < orignSite - 1; k++){
					if (k == 83)
						cout << endl;
				if (stop && index.size() != 0 && index[count] == k+1){ //无效点
					read >> start;
					read >> arrival;
					count++;
					if (count == index.size())
						stop = false;
				}
				else{
					read >> start; //出发点
					int routeIndex = route[tmpSite[plus]].size() - 1;
					route[tmpSite[plus]][routeIndex].startT = start;
					route[tmpSite[plus]][routeIndex].target = tmpSite[plus + 1];
					read >> arrival; //第二行的初
					route[tmpSite[plus]][routeIndex].dur = arrival - start;
					route[tmpSite[plus]][routeIndex].mark = MARK;
					plus = plus + 1;

				}
			}
			read >> start;
			MARK++;
			//for (int k = 0; k < orignSite-1; k++){ //site站数
			//	if (k == 83)
			//		cout << endl;
			//	if (plus == site)
			//		break;
			//	if (stop && index.size()!=0 && index[count] == k){
			//		read >> start;//过滤该点的出发点
			//		read >> arrival;//过滤该点的到达点
			//		count++;
			//		if (count == index.size())
			//			stop = false; //保护不越界
			//		continue;
			//	}
			//	read >> start; //第一行的末
			//	int key;
			//	for (int m = 0; m < route[tmpSite[plus]].size(); m++)
			//	{
			//		if (route[tmpSite[plus]][m].mark == 0)
			//		{
			//			key = m;
			//			break;
			//		}
			//	}
			//	route[tmpSite[plus]][key].startT = start;
			//	//cout << tmpSite[k] << "  " << start << endl;
			//	route[tmpSite[plus]][key].target = tmpSite[plus + 1];
			//	read >> arrival; //第二行的初
			//	//read.seekp(-1, ios_base::cur);
			//	route[tmpSite[plus]][key].dur = arrival - start;
			//	//cout << route[tmpSite[k]][route[tmpSite[k]].size() - 1].dur << endl;
			//	route[tmpSite[plus]][key].mark = MARK;
			//	plus = plus + 1;

			//}
			////没有计算最后一个到达点（最后一个到达点信息可以表示在入读点中）
			//read >> start;
			//MARK++;
		}
	}
	cout << "building down!" << endl;
	read.close();

	
	for (int i = 0; i < route.size(); i++){
		sort(route[i].begin(),route[i].end(),cmp);
	}

	cout << " sort down!" << endl;
	fstream write;
	write.open("example.txt", ios::out);

	write << Size <<endl;
	for (int i = 0; i < Size; i++){
		write <<i << "  ";//输出结点编号
		//write << route[i].size() << "  ";//输出邻居结点的个数
		//for (int j = 0; j < route[i].size(); j++){
		//	write << route[i][j].target<<"  "<<
		//}
		vector<int> sumSize; //分界点 计算每个最末出边邻居的下标
		int neiNum = 0; //计算出边邻居的个数
		vector<int> neiId;//出边邻居的id
		sumSize.resize(1);
		neiId.resize(1);
		if (route[i].size() == 0){
			write << 0 << endl;
			cout << 1 << endl;
			continue;
		}
		for (int j = 0; j < route[i].size() -1 ; j++){
			if (route[i][j].target != route[i][j + 1].target)
			{
				neiNum++;
				sumSize[sumSize.size() - 1] = j+1;
				sumSize.resize(sumSize.size() + 1);
				
				neiId[neiId.size() - 1] = route[i][j].target;
				neiId.resize(neiId.size() + 1);
			}
		}
		if (neiNum == 0){ //代表只有一个出边情况
			neiNum++;
			sumSize[0] = route[i].size();
			neiId[0] = route[i][0].target;
		}
		else if (neiNum != 0){ //代表出现不同，将存储最后一个结点
			neiNum++;
			sumSize[sumSize.size() - 1] = route[i].size();
			neiId[neiId.size() - 1] = route[i][route[i].size() - 1].target;
		}
		write << neiNum << "  "; //输出邻居的个数


		// step1. 按照出度点进行排序
		//for (int j = 0; j < neiNum; j++){
		//	int fromIndex = j == 0 ? 0 : sumSize[j-1];
		//	insertSort(i, fromIndex, sumSize[j]);
		//	//sort(route[i][2].dur, route[i][1].dur+2, compare);
		//}
		
		//这里是假设duration为1进行相关操作，因此将step.1 和 step.2相关内容注解了
		for (int j = 0; j < neiNum; j++){
			insertSortByStartT(i, j == 0 ? 0 : sumSize[j - 1],sumSize[j]);
		}


		//step.2 在持续时间相同的情况下，按照时间进行排序.
		//vector<int>sumDur;
		//sumDur.resize(1);
		//for (int j = 0; j < route[i].size() - 1; j++){
		//	if (route[i][j].dur != route[i][j + 1].dur){ // j -> j+1进行分界
		//		sumDur[sumDur.size() - 1] = j+1;
		//		sumDur.resize(sumDur.size() + 1);
		//		//if (j + 1 == route[i].size() - 1){ //如果是最后一个也不同
		//		//	sumDur.resize(sumDur.size() + 1);
		//		//	sumDur[sumDur.size() - 1] = j+2;
		//		//}
		//	}
		//}
		//sumDur[sumDur.size() - 1] = route[i].size();
		//for (int j = 0; j < sumDur.size(); j++){
		//	insertSortByStartT(i, j==0?0:sumDur[j-1], sumDur[j]);
		//}





		//sort(A, A + 100, greater<int>());//降序排列
		//sort(A, A + 100, less<int>());//升序排列

		for (int j = 0; j < neiNum; j++){
			write << neiId[j] << "  ";//输出邻居结点的id
			int sum = j == 0 ? 0 : sumSize[j - 1];
			int goNei = sumSize[j] - sum;
			write << goNei << "  "; //有多少边去邻居点
			/*
				这里将会存在二义性
			*/
			//step3. 输出持续时间
			//write << route[i][sum].dur << "  "; 
			
			//假设duration时间为1,因此将step3.的内容进行注解
			write << 1 << "  ";

			for (int k = sum; k < sumSize[j]; k++){
				//cout << route[i][k].startT << "  " << route[i][k].mark << "  " << route[i][k].dur << "  " << endl;
				write << route[i][k].startT << "  " << route[i][k].mark << "  " << route[i][k].dur<<"  ";
				// 路程所花费的时间 图中边的权值 车辆的唯一id
			}
		}
		write << endl;
	}
	
}
/*
	每一条线路 可以增加站点i的条数为 1 × 趟数
*/
