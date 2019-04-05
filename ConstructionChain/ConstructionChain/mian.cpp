#include"TemporalG.h"
#include<ctime>
int main(){
	//char* option = argv[1];

	double TS = clock();
	char* s1 = "example.txt";
	char* s2_temp = "result";
	InitGraph g(s1);
	TemporalG gT(g);

	string s2 = s2_temp;
	s2 += s1;
	gT.output(s2);
	cout << "transform done! " << endl;
	double ES = clock();
	double endtime = (double)(ES - TS) / CLOCKS_PER_SEC;
	cout << "Total time:" << endtime <<" s"<< endl;
	//char * s3 = "arrival.txt";
	//gT.initial_query();
	//gT.main_earlist_arrival(s3);
	//cout << "earlist_arrival done!" << endl;
	return 0;
}