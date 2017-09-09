#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<algorithm>
#include<cmath>
#include<fstream>
using namespace std;
#define MAXLEN 6 //输入每行的数据个数

struct Node {				//决策树结点
	string attribute;	    //属性值
	string arrived_value;	//到达的属性值
	vector<Node* > childs;	//所有的孩子
	Node() {
		attribute ="";
		arrived_value = "";
	}
} * root;

vector<vector<string>>state; //实例集
vector<string>attribute_row; //保存首行即属性行数据
string yes("yes");
string no("no");
string blank("");
map<string, vector<string>>map_attribute_values;		//储存属性对应的所有值
int tree_size = 0;

void input() {
	string filename = "input.txt";
	ifstream infile;
	infile.open(filename.data());
	if (!infile.is_open()) {
		cout << "failed opening text" << endl;
		return;
	}
	string temp_string;
	vector<string> temp_row;
	while (!infile.eof()) {
		int i = 0;
		while (i < MAXLEN) {
			
			infile >> temp_string;
			temp_row.push_back(temp_string);
			i++;
		}
		state.push_back(temp_row);
		temp_row.erase(temp_row.begin(), temp_row.end());
	}
	for (int j = 0; j < MAXLEN; j++) {		//将第一行所有的索引，属性，最后结果记录下来作为表头
		attribute_row.push_back(state[0][j]);
	}
}

//根据数据实例，构成每个属性及其对应所有可能取值的map
void ComputeMapFrom2DVector() {
	unsigned int i, j, k;
	bool exited = false;			//哨兵值
	vector<string>values;
	for (i = 1; i < MAXLEN - 1; i++) {		//遍历所有列（横向）
		for (j = 1; j < state.size(); j++) {		//遍历所有行(纵向）
			for (k = 0; k < values.size(); k++) {
				if (!values[k].compare(state[j][i])) {
					exited = true;	//若values中已经收集过该值，就设立哨兵值为真,并立即退出
			//		break;
				}
			}
			if (!exited)		values.push_back(state[j][i]);		//按列（相同的属性）收集同一属性的所有可能取值
			exited = false;		//恢复默认哨兵值
		}
		map_attribute_values[state[0][i]] = values;		//在map中对相应属性添加values即记录下实例中出现的所有取值
		values.erase(values.begin(), values.end());		//清空该属性的所有值，并继续下一属性的值的收集
	}
}

//根据具体属性和值来计算熵
double ComputeEntropy(vector<vector<string>>remain_state, string attribute, string value, bool ifparent) {
	vector<int>count(2);	
	unsigned int i, j;
	bool done_flag = false;
	//先根据表头找到要计算的属性，再遍历行计算这一行即这一属性的熵
	for (j = 1; j < MAXLEN; j++) {
		if (done_flag) break;
		if (!attribute_row[j].compare(attribute)) {
			for (i = 1; i < remain_state.size(); i++) {
				if ((!ifparent && !remain_state[i][j].compare(value)) || (ifparent)) {		//若是父节点直接计算熵，否则需要先判断是否满足指定的值再计算熵
					if (!remain_state[i][MAXLEN - 1].compare(yes)) {
						count[0]++;
					}
					else count[1]++;
				}
			}
			done_flag = true;
		}
	}
	if (count[0] == 0 || count[1] == 0)		return 0;
	//具体计算熵
	double sum = count[0] + count[1];
	double entropy = -count[0] / sum*log(count[0] / sum) / log(2.0) - count[1] / sum*log(count[1] / sum) / log(2.0);
	return entropy;
}

//complete
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

void printv(vector<vector<string>> remain_state) {
	for (int i = 0; i < state.size(); i++) {
		for (int j = 0; j < MAXLEN; j++) {
			cout << remain_state[i][j] << " ";
		}
		cout << endl;
	}
}

//检测样例的结果是否都为label
bool AllTheSameLabel(vector<vector<string>> remain_state, string label) {
	int count = 0;
	for (unsigned int i = 0; i < remain_state.size(); i++) {
		if (!remain_state[i][MAXLEN - 1].compare(label))		count++;
	}
	if (count == remain_state.size() - 1)		return true;
	else return false;
}

//在没有可用于判断的属性时，找出在数量上最普遍的label
string MostCommonLabel(vector<vector<string>> remain_state) {
	int p = 0, n = 0;
	for (unsigned int i = 0; i < remain_state.size(); i++) {
		if (!remain_state[i][MAXLEN - 1].compare(yes))	  p++;
		else n++;
	}
	if (p >= n)		return yes;
	else		return no;
}


//计算按照属性attribute划分当前剩余实例的信息增益
double ComputeGain(vector<vector<string>> remain_state, string attribute) {
	unsigned int j, k, m;
	//首先求不做划分时的熵
	double parent_entropy = ComputeEntropy(remain_state, attribute, blank, true);
	double children_entropy = 0;
	//然后求做划分后各个值的熵
	vector<string>values = map_attribute_values[attribute];
	vector<double>ratio;
	vector<int>count_values;
	int tempint;
	//一一计算当前属性中每个值的数量,按序保存在count_values中
	for (m = 0; m < values.size(); m++) {
		tempint = 0;
		for (k = 1; k < MAXLEN - 1; k++) {
			if (!attribute_row[k].compare(attribute)) {
				for (j = 1; j < remain_state.size(); j++) {
					if (!remain_state[j][k].compare(values[m])) {
						tempint++;
					}
				}
			}
		}
		count_values.push_back(tempint);
	}
	//一一计算每个值出现的概率，保存在ratio中
	for (j = 0; j < values.size(); j++) {
		ratio.push_back((double)count_values[j] / double(remain_state.size() - 1));
	}
	double temp_entropy;
	for (j = 0; j < values.size(); j++) {
		temp_entropy = ComputeEntropy(remain_state, attribute, values[j], false);
		children_entropy += ratio[j] * temp_entropy;
	}
	return (parent_entropy - children_entropy);
}

int FindAttributeNumByName(string attri) {
	for (int i = 0; i < MAXLEN; i++) {
		if (!state[0][i].compare(attri))	return i;
	}
	cout << "no exist" << endl;
	return 0;
}
//重点部分，
//开始计算信息增益，DFS构建决策树
//current_node为当前结点
//remain_state为剩下待分类的样例
//remain_attribute为剩余还没有考虑的属性
//返回根节点指针
Node* BuildDecisionTreeDFS(Node* p, vector<vector<string>> remain_state, vector<string> remain_attribute){
	//if (remain_state.size() > 0) {
	//	printv(remain_state);
	//}
	if (p == NULL) {
		p = new Node();
	}
	//先看搜索到的树叶的情况
	if (AllTheSameLabel(remain_state, yes)) {
		p->attribute = yes;
		return p;
	}
	if (AllTheSameLabel(remain_state, no)) {
		p->attribute = no;
		return p;
	}
	if (remain_attribute.size() == 0) {	//如果所有的属性都已考虑完，还存在剩余的样例没有分配，就找最普通（多）的样例
		string label =  MostCommonLabel(remain_state);
		p->attribute = label;
		return p;
	}

	//以上两种特殊情况考虑完，继续进行一般情况
	double max_gain = 0, temp_gain;
	vector<string>::iterator max_it = remain_attribute.begin();
	vector<string>::iterator it1;
	for (it1 = remain_attribute.begin(); it1 < remain_attribute.end(); it1++) {
		temp_gain = ComputeGain(remain_state, (*it1));
		if (temp_gain > max_gain) {		//找到剩余中最大的那个属性
			max_gain = temp_gain;
			max_it = it1;
		}
	}
	//下面根据max_it指向的属性，来划分当前样例，更新样例集和属性集
	vector<string> new_attribute;
	vector<vector<string>> new_state;
	for (vector<string>::iterator it2 = remain_attribute.begin(); it2 < remain_attribute.end(); it2++) {
		if ((*it2).compare(*max_it))		new_attribute.push_back(*it2);		//非熵最大的属性保留下来说明需要作继续分类，故建立新的属性集合，用以递归分类。
	}
	//确定了最佳划分属性，注意保存
	p->attribute = *max_it;
	vector<string>values = map_attribute_values[*max_it];
	int attribute_num = FindAttributeNumByName(*max_it);
	new_state.push_back(attribute_row);
	for (vector<string>::iterator it3 = values.begin(); it3 < values.end(); it3++) {
		for (unsigned int i = 1; i < remain_state.size(); i++) {
			if (!remain_state[i][attribute_num].compare(*it3)) {
				new_state.push_back(remain_state[i]);
			}
		}
		Node * new_node = new Node();
		new_node->arrived_value = *it3;
		if (new_state.size() == 0) {
			new_node->attribute = MostCommonLabel(remain_state);
		}
		else
			BuildDecisionTreeDFS(new_node, new_state, new_attribute);
		//递归函数返回即回溯时需要将新节点加入父节点孩子容器，然后清楚new_state容器
		p->childs.push_back(new_node);
		new_state.erase(new_state.begin() + 1, new_state.end());
		//注意先清空new_state中前一个取值的样例，准备遍历下一个取值样例

	}
	return p;
}

void PrintTree(Node* p, int depth){
	for (int i = 0; i < depth; i++) {
		cout << "\t";
	}
	if (!p->arrived_value.empty()) {		//若该结点的属性存在有效值，依次输出
		cout << p->arrived_value << endl;
		for (int i = 0; i < depth + 1; i++) {
			cout << "\t";
		}
	}
	cout << "->" <<p->attribute << endl;
	for (vector<Node* >::iterator it = p->childs.begin(); it != p->childs.end(); it++) {
		PrintTree(*it, depth + 1);
	}
}

void FreeTree(Node* p)
{
	if (p == NULL)
		return;
	for (vector<Node* >::iterator it = p->childs.begin(); it != p->childs.end(); it++) {
		FreeTree(*it);
	}
	delete p;
	tree_size++;
	return;
}

//输入结果预测
vector<string> input_test_items() {
	vector<string> items(MAXLEN - 2);
	for (int i = 0; i < MAXLEN - 2; i++) {
		cin >> items[i];
	}
	return items;
}

//预测
string forecast(Node *p, vector<string> values, int index) {
	if (p->arrived_value != blank && !values[index - 1].compare(p->arrived_value) && (p->childs.size() == 0)) {
		cout << endl << p->attribute << endl;
		return p->attribute;
	}
	if (p->arrived_value == blank) {
		for (vector<Node*>::iterator it = p->childs.begin(); it != p->childs.end(); it++) {
		//	index = FindAttributeNumByName((*it)->attribute);
			forecast(*it, values, index);
		}
		return " ";
	}
	else if(values[index-1].compare(p->arrived_value)){
		return " ";
	}
	else {

		if (p->childs.size() != 0) {	
			index = FindAttributeNumByName(p->attribute);
			for (vector<Node*>::iterator it2 = p->childs.begin(); it2 != p->childs.end(); it2++) {
				forecast(*it2, values, index);
			}
			return " ";
		}
	}
}

int main()
{
	input();
	/*
	for (int i = 0; i <state.size(); i++) {
		for (int j = 0; j < MAXLEN; j++) {
			cout << state[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	system("pause");*/
	vector<string>remain_attribute = {"outlook", "temperature", "humidity", "wind"};
	vector<vector<string>>remain_state;		//创建一个能记录本实例中出现的所有属性的集合
	for (unsigned int i = 0; i < state.size(); i++){
		remain_state.push_back(state[i]);
	}
	ComputeMapFrom2DVector();		//将所有实例输入后，将其中所有的属性的出现的所有取值记录下来，以map形式保存
	root = BuildDecisionTreeDFS(root, remain_state, remain_attribute);		//**重点
	cout << "the decision tree is :" << endl;
	//print(root);
	PrintTree(root, 0);	
	
	int test_nums = 0;
	while (test_nums<1) {
		vector<string> get_test_items = input_test_items();
		forecast(root, get_test_items, FindAttributeNumByName(root->attribute));
		//system("pause");
		test_nums++;
	}

	FreeTree(root); 
	cout << endl;
	cout << "tree_size: " << tree_size << endl;

	system("pause");
	return 0;

}




