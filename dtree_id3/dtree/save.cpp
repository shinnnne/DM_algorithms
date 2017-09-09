#include<iostream>
#include<string>
#include<vector>
#include<cmath>
#include<fstream>
#include<algorithm>
#include<map>
using namespace std;
#define maxsize 6


struct Node {
	string attr = NULL;			//�ڵ㱾���ʾ�����ԣ�
	int arrive_value = 0;	//����ýڵ���Ҫ�����ֵ�����Ӹýڵ��·������
	vector<int> attr_values;		//���ڵ����Ե�����ֵ
	vector<Node*> childs;	//���ӽ�㣻
} * root;

vector<vector<string>> init_items;
vector<string> attr_head;	//����ı�ͷ�����������������������������
map<string, vector<string>>attr_value_index;

void input() {
	string filename = "123.txt";
	ifstream infile;
	infile.open(filename.data());
	if (!infile.is_open()) {
		cout << "failed opening text" << endl;
		return;
	}
	while (!infile.eof()) {
		int i = 0;
		string temp_string;
		vector<string> temp_row;
		while (i < maxsize) {
			infile >> temp_string;	
			temp_row.push_back(temp_string);
			i++;
		}
		init_items.push_back(temp_row);
		
	}
	for (int j = 0; j < maxsize; j++) {		//����һ�����е����������ԣ��������¼������Ϊ��ͷ
		attr_head.push_back(init_items[0][j]);
	}
}

//��������ʵ��������ÿ�����Լ����Ӧ���п���ȡֵ��map_index
void make_attr_index() {
	bool existed = false;//�ڱ�ֵ
	vector<string> values;
	for (int i = 0; i < init_items.size(); i++) {//���������У�����
		for (int j = 0; j < maxsize; j++) {//����������(����
			//�����ж��Ƿ��Ѿ��ռ�����ֵ
			for (int k = 0; k < values.size(); k++) {
				if (!values[k].compare(init_items[j][i])) {
					existed = true;//��values���Ѿ��ռ�����ֵ���������ڱ�ֵΪ��,�������˳�
					break;
				}
			}
			if (!existed) {
				values.push_back(init_items[j][i]);//���У���ͬ�����ԣ��ռ�ͬһ���Ե����п���ȡֵ
			}
			existed = false;
		}
		attr_value_index[init_items[0][i]] = values;//��map�ж���Ӧ�������values����¼��ʵ���г��ֵ�����ȡֵ
		values.erase(values.begin(), values.end()); //��ո����Ե�����ֵ����������һ���Ե�ֵ���ռ�
	}
}





void PrintTree(Node* p, int depth) {		//
	for (int i = 0; i < depth; i++) {
		cout << "\t";
	}
	if (!p->arrive_value) {

	}
}
void Input() {
	string s;
	vector<string> item(MAXLEN);		//ÿ��ʵ��
	string end("end");		//������־
	cin >> s;
	while (s.compare(end) != 0) {		//����⵽����Ϊ"end"���ʾ�������
		item[0] = s;
		for (int i = 1; i < MAXLEN; i++) {
			cin >> item[i];
		}
		state.push_back(item);	//��������ϢҲ�����ȥ��������
		cin >> s;
	}
	for (int j = 0; j < MAXLEN; j++) {		//����һ�����е����������ԣ��������¼������Ϊ��ͷ
		attribute_row.push_back(state[0][j]);
	}
}

int main() {
	input();	//


	return 0;
}