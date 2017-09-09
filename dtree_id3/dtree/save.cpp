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
	string attr = NULL;			//节点本身表示的属性；
	int arrive_value = 0;	//到达该节点需要满足的值（连接该节点的路径）；
	vector<int> attr_values;		//本节点属性的所有值
	vector<Node*> childs;	//孩子结点；
} * root;

vector<vector<string>> init_items;
vector<string> attr_head;	//输入的表头，即所有索引，属性名，结果名字
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
	for (int j = 0; j < maxsize; j++) {		//将第一行所有的索引，属性，最后结果记录下来作为表头
		attr_head.push_back(init_items[0][j]);
	}
}

//根据数据实例，构成每个属性及其对应所有可能取值的map_index
void make_attr_index() {
	bool existed = false;//哨兵值
	vector<string> values;
	for (int i = 0; i < init_items.size(); i++) {//遍历所有列（横向）
		for (int j = 0; j < maxsize; j++) {//遍历所有行(纵向）
			//用于判断是否已经收集过该值
			for (int k = 0; k < values.size(); k++) {
				if (!values[k].compare(init_items[j][i])) {
					existed = true;//若values中已经收集过该值，就设立哨兵值为真,并立即退出
					break;
				}
			}
			if (!existed) {
				values.push_back(init_items[j][i]);//按列（相同的属性）收集同一属性的所有可能取值
			}
			existed = false;
		}
		attr_value_index[init_items[0][i]] = values;//在map中对相应属性添加values即记录下实例中出现的所有取值
		values.erase(values.begin(), values.end()); //清空该属性的所有值，并继续下一属性的值的收集
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
	vector<string> item(MAXLEN);		//每行实例
	string end("end");		//结束标志
	cin >> s;
	while (s.compare(end) != 0) {		//若检测到输入为"end"则表示输入结束
		item[0] = s;
		for (int i = 1; i < MAXLEN; i++) {
			cin >> item[i];
		}
		state.push_back(item);	//将首行信息也输入进去，即属性
		cin >> s;
	}
	for (int j = 0; j < MAXLEN; j++) {		//将第一行所有的索引，属性，最后结果记录下来作为表头
		attribute_row.push_back(state[0][j]);
	}
}

int main() {
	input();	//


	return 0;
}