#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
using namespace std;
class Graph{
public:
	int target;//Ŀ����
	int startT;//����ʱ��
	int dur;// ����ʱ��
	int neiId; //�����ھӵ�id
	int mark;//�������Ψһ��ʶ��
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
//void insertSort(int i, int from, int to)//ֱ�Ӳ�������
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
void insertSort(int index, int from, int to)//ֱ�Ӳ�������
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


void insertSortByStartT(int index, int from, int to)//ֱ�Ӳ�������
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
	int MARK = 1; //�������Ψһ��ʶ��

	char graph_name[100];
	strcpy(graph_name, "austin.txt");
	//strcpy(graph_name,argv[1]);
	fstream read;
	read.open(graph_name, ios::in);
	/*
		��һ�� ���е�վ���� ���е���·���� ������·��վ��� �ܵ�������
		�ڶ��� ��·id
		������ ������·;��վ��id
		������ վ��id
		������ ������·һ����10�೵
	*/

	/*
		���Ҫ��
		���еĽ����
		���ı�� -> �����ھӵĸ��� -> �����ھ�i��id -> �ж��ٱ�ȥ�ھӵ�j �ó������ѵ�ʱ�� ͼ�е�Ȩֵ������ʱ�䣩 ����Ψһid
	*/
	int Size;
	read >> Size;
	vector<int> index; //��¼ÿ����ĳ����ھӸ���
	route.resize(Size);
	index.resize(Size);
	int loop;
	read >> loop; //һ����loop����·
	int tmp;
	read >> tmp;
	read >> tmp;
	//    ��һ�����ݶ���
	for (int i = 0; i < loop; i++){
		if (i == 12)
			cout << endl;
		int id;
		read >> id;
		//	�ڶ������ݶ���
		int site; // վ����
		read >> site;
		//	���������ݶ���
		vector<int> tmpSite;
		//tmpSite.resize(site);
		vector<int> index;
		bool origin = false;
		int count = 0;
		for (int j = 0; j < site; j++){
			int site_id;
			read >> site_id;
			
			//��������ظ���,���޳�,���Ҽ�¼�±�
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
			//	���������ݶ���
			//  ���������е�id���
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
		int fre;// һ����fre�����
		read >> fre;
		//cout << endl;
		//	���������ݶ���
		for (int j = 0; j < fre; j++){ // 10��

			for (int n = 0; n < site - 1; n++){ //��ʼ��������С,���ҽ�����id��ֵ���õ�
				int extendSize = route[tmpSite[n]].size() == 0 ? 1 : route[tmpSite[n]].size() + 1;
				route[tmpSite[n]].resize(extendSize);
				//cout << tmpSite[n] << endl;
				route[tmpSite[n]][extendSize - 1].neiId = tmpSite[n + 1];
			}



			int arrival, start;
			count = 0;
			read >> arrival; //��ʼ��ĵ�������
			int plus = 0;
			bool stop = true;


			for (int k = 0; k < orignSite - 1; k++){
					if (k == 83)
						cout << endl;
				if (stop && index.size() != 0 && index[count] == k+1){ //��Ч��
					read >> start;
					read >> arrival;
					count++;
					if (count == index.size())
						stop = false;
				}
				else{
					read >> start; //������
					int routeIndex = route[tmpSite[plus]].size() - 1;
					route[tmpSite[plus]][routeIndex].startT = start;
					route[tmpSite[plus]][routeIndex].target = tmpSite[plus + 1];
					read >> arrival; //�ڶ��еĳ�
					route[tmpSite[plus]][routeIndex].dur = arrival - start;
					route[tmpSite[plus]][routeIndex].mark = MARK;
					plus = plus + 1;

				}
			}
			read >> start;
			MARK++;
			//for (int k = 0; k < orignSite-1; k++){ //siteվ��
			//	if (k == 83)
			//		cout << endl;
			//	if (plus == site)
			//		break;
			//	if (stop && index.size()!=0 && index[count] == k){
			//		read >> start;//���˸õ�ĳ�����
			//		read >> arrival;//���˸õ�ĵ����
			//		count++;
			//		if (count == index.size())
			//			stop = false; //������Խ��
			//		continue;
			//	}
			//	read >> start; //��һ�е�ĩ
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
			//	read >> arrival; //�ڶ��еĳ�
			//	//read.seekp(-1, ios_base::cur);
			//	route[tmpSite[plus]][key].dur = arrival - start;
			//	//cout << route[tmpSite[k]][route[tmpSite[k]].size() - 1].dur << endl;
			//	route[tmpSite[plus]][key].mark = MARK;
			//	plus = plus + 1;

			//}
			////û�м������һ������㣨���һ���������Ϣ���Ա�ʾ��������У�
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
		write <<i << "  ";//��������
		//write << route[i].size() << "  ";//����ھӽ��ĸ���
		//for (int j = 0; j < route[i].size(); j++){
		//	write << route[i][j].target<<"  "<<
		//}
		vector<int> sumSize; //�ֽ�� ����ÿ����ĩ�����ھӵ��±�
		int neiNum = 0; //��������ھӵĸ���
		vector<int> neiId;//�����ھӵ�id
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
		if (neiNum == 0){ //����ֻ��һ���������
			neiNum++;
			sumSize[0] = route[i].size();
			neiId[0] = route[i][0].target;
		}
		else if (neiNum != 0){ //������ֲ�ͬ�����洢���һ�����
			neiNum++;
			sumSize[sumSize.size() - 1] = route[i].size();
			neiId[neiId.size() - 1] = route[i][route[i].size() - 1].target;
		}
		write << neiNum << "  "; //����ھӵĸ���


		// step1. ���ճ��ȵ��������
		//for (int j = 0; j < neiNum; j++){
		//	int fromIndex = j == 0 ? 0 : sumSize[j-1];
		//	insertSort(i, fromIndex, sumSize[j]);
		//	//sort(route[i][2].dur, route[i][1].dur+2, compare);
		//}
		
		//�����Ǽ���durationΪ1������ز�������˽�step.1 �� step.2�������ע����
		for (int j = 0; j < neiNum; j++){
			insertSortByStartT(i, j == 0 ? 0 : sumSize[j - 1],sumSize[j]);
		}


		//step.2 �ڳ���ʱ����ͬ������£�����ʱ���������.
		//vector<int>sumDur;
		//sumDur.resize(1);
		//for (int j = 0; j < route[i].size() - 1; j++){
		//	if (route[i][j].dur != route[i][j + 1].dur){ // j -> j+1���зֽ�
		//		sumDur[sumDur.size() - 1] = j+1;
		//		sumDur.resize(sumDur.size() + 1);
		//		//if (j + 1 == route[i].size() - 1){ //��������һ��Ҳ��ͬ
		//		//	sumDur.resize(sumDur.size() + 1);
		//		//	sumDur[sumDur.size() - 1] = j+2;
		//		//}
		//	}
		//}
		//sumDur[sumDur.size() - 1] = route[i].size();
		//for (int j = 0; j < sumDur.size(); j++){
		//	insertSortByStartT(i, j==0?0:sumDur[j-1], sumDur[j]);
		//}





		//sort(A, A + 100, greater<int>());//��������
		//sort(A, A + 100, less<int>());//��������

		for (int j = 0; j < neiNum; j++){
			write << neiId[j] << "  ";//����ھӽ���id
			int sum = j == 0 ? 0 : sumSize[j - 1];
			int goNei = sumSize[j] - sum;
			write << goNei << "  "; //�ж��ٱ�ȥ�ھӵ�
			/*
				���ｫ����ڶ�����
			*/
			//step3. �������ʱ��
			//write << route[i][sum].dur << "  "; 
			
			//����durationʱ��Ϊ1,��˽�step3.�����ݽ���ע��
			write << 1 << "  ";

			for (int k = sum; k < sumSize[j]; k++){
				//cout << route[i][k].startT << "  " << route[i][k].mark << "  " << route[i][k].dur << "  " << endl;
				write << route[i][k].startT << "  " << route[i][k].mark << "  " << route[i][k].dur<<"  ";
				// ·�������ѵ�ʱ�� ͼ�бߵ�Ȩֵ ������Ψһid
			}
		}
		write << endl;
	}
	
}
/*
	ÿһ����· ��������վ��i������Ϊ 1 �� ����
*/
