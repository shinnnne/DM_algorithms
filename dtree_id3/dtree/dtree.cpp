#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<algorithm>
#include<cmath>
#include<fstream>
using namespace std;
#define MAXLEN 6 //����ÿ�е����ݸ���

struct Node {				//���������
	string attribute;	    //����ֵ
	string arrived_value;	//���������ֵ
	vector<Node* > childs;	//���еĺ���
	Node() {
		attribute ="";
		arrived_value = "";
	}
} * root;

vector<vector<string>>state; //ʵ����
vector<string>attribute_row; //�������м�����������
string yes("yes");
string no("no");
string blank("");
map<string, vector<string>>map_attribute_values;		//�������Զ�Ӧ������ֵ
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
	for (int j = 0; j < MAXLEN; j++) {		//����һ�����е����������ԣ��������¼������Ϊ��ͷ
		attribute_row.push_back(state[0][j]);
	}
}

//��������ʵ��������ÿ�����Լ����Ӧ���п���ȡֵ��map
void ComputeMapFrom2DVector() {
	unsigned int i, j, k;
	bool exited = false;			//�ڱ�ֵ
	vector<string>values;
	for (i = 1; i < MAXLEN - 1; i++) {		//���������У�����
		for (j = 1; j < state.size(); j++) {		//����������(����
			for (k = 0; k < values.size(); k++) {
				if (!values[k].compare(state[j][i])) {
					exited = true;	//��values���Ѿ��ռ�����ֵ���������ڱ�ֵΪ��,�������˳�
			//		break;
				}
			}
			if (!exited)		values.push_back(state[j][i]);		//���У���ͬ�����ԣ��ռ�ͬһ���Ե����п���ȡֵ
			exited = false;		//�ָ�Ĭ���ڱ�ֵ
		}
		map_attribute_values[state[0][i]] = values;		//��map�ж���Ӧ�������values����¼��ʵ���г��ֵ�����ȡֵ
		values.erase(values.begin(), values.end());		//��ո����Ե�����ֵ����������һ���Ե�ֵ���ռ�
	}
}

//���ݾ������Ժ�ֵ��������
double ComputeEntropy(vector<vector<string>>remain_state, string attribute, string value, bool ifparent) {
	vector<int>count(2);	
	unsigned int i, j;
	bool done_flag = false;
	//�ȸ��ݱ�ͷ�ҵ�Ҫ��������ԣ��ٱ����м�����һ�м���һ���Ե���
	for (j = 1; j < MAXLEN; j++) {
		if (done_flag) break;
		if (!attribute_row[j].compare(attribute)) {
			for (i = 1; i < remain_state.size(); i++) {
				if ((!ifparent && !remain_state[i][j].compare(value)) || (ifparent)) {		//���Ǹ��ڵ�ֱ�Ӽ����أ�������Ҫ���ж��Ƿ�����ָ����ֵ�ټ�����
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
	//���������
	double sum = count[0] + count[1];
	double entropy = -count[0] / sum*log(count[0] / sum) / log(2.0) - count[1] / sum*log(count[1] / sum) / log(2.0);
	return entropy;
}

//complete
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

void printv(vector<vector<string>> remain_state) {
	for (int i = 0; i < state.size(); i++) {
		for (int j = 0; j < MAXLEN; j++) {
			cout << remain_state[i][j] << " ";
		}
		cout << endl;
	}
}

//��������Ľ���Ƿ�Ϊlabel
bool AllTheSameLabel(vector<vector<string>> remain_state, string label) {
	int count = 0;
	for (unsigned int i = 0; i < remain_state.size(); i++) {
		if (!remain_state[i][MAXLEN - 1].compare(label))		count++;
	}
	if (count == remain_state.size() - 1)		return true;
	else return false;
}

//��û�п������жϵ�����ʱ���ҳ������������ձ��label
string MostCommonLabel(vector<vector<string>> remain_state) {
	int p = 0, n = 0;
	for (unsigned int i = 0; i < remain_state.size(); i++) {
		if (!remain_state[i][MAXLEN - 1].compare(yes))	  p++;
		else n++;
	}
	if (p >= n)		return yes;
	else		return no;
}


//���㰴������attribute���ֵ�ǰʣ��ʵ������Ϣ����
double ComputeGain(vector<vector<string>> remain_state, string attribute) {
	unsigned int j, k, m;
	//������������ʱ����
	double parent_entropy = ComputeEntropy(remain_state, attribute, blank, true);
	double children_entropy = 0;
	//Ȼ���������ֺ����ֵ����
	vector<string>values = map_attribute_values[attribute];
	vector<double>ratio;
	vector<int>count_values;
	int tempint;
	//һһ���㵱ǰ������ÿ��ֵ������,���򱣴���count_values��
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
	//һһ����ÿ��ֵ���ֵĸ��ʣ�������ratio��
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
//�ص㲿�֣�
//��ʼ������Ϣ���棬DFS����������
//current_nodeΪ��ǰ���
//remain_stateΪʣ�´����������
//remain_attributeΪʣ�໹û�п��ǵ�����
//���ظ��ڵ�ָ��
Node* BuildDecisionTreeDFS(Node* p, vector<vector<string>> remain_state, vector<string> remain_attribute){
	//if (remain_state.size() > 0) {
	//	printv(remain_state);
	//}
	if (p == NULL) {
		p = new Node();
	}
	//�ȿ�����������Ҷ�����
	if (AllTheSameLabel(remain_state, yes)) {
		p->attribute = yes;
		return p;
	}
	if (AllTheSameLabel(remain_state, no)) {
		p->attribute = no;
		return p;
	}
	if (remain_attribute.size() == 0) {	//������е����Զ��ѿ����꣬������ʣ�������û�з��䣬��������ͨ���ࣩ������
		string label =  MostCommonLabel(remain_state);
		p->attribute = label;
		return p;
	}

	//��������������������꣬��������һ�����
	double max_gain = 0, temp_gain;
	vector<string>::iterator max_it = remain_attribute.begin();
	vector<string>::iterator it1;
	for (it1 = remain_attribute.begin(); it1 < remain_attribute.end(); it1++) {
		temp_gain = ComputeGain(remain_state, (*it1));
		if (temp_gain > max_gain) {		//�ҵ�ʣ���������Ǹ�����
			max_gain = temp_gain;
			max_it = it1;
		}
	}
	//�������max_itָ������ԣ������ֵ�ǰ���������������������Լ�
	vector<string> new_attribute;
	vector<vector<string>> new_state;
	for (vector<string>::iterator it2 = remain_attribute.begin(); it2 < remain_attribute.end(); it2++) {
		if ((*it2).compare(*max_it))		new_attribute.push_back(*it2);		//�����������Ա�������˵����Ҫ���������࣬�ʽ����µ����Լ��ϣ����Եݹ���ࡣ
	}
	//ȷ������ѻ������ԣ�ע�Ᵽ��
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
		//�ݹ麯�����ؼ�����ʱ��Ҫ���½ڵ���븸�ڵ㺢��������Ȼ�����new_state����
		p->childs.push_back(new_node);
		new_state.erase(new_state.begin() + 1, new_state.end());
		//ע�������new_state��ǰһ��ȡֵ��������׼��������һ��ȡֵ����

	}
	return p;
}

void PrintTree(Node* p, int depth){
	for (int i = 0; i < depth; i++) {
		cout << "\t";
	}
	if (!p->arrived_value.empty()) {		//���ý������Դ�����Чֵ���������
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

//������Ԥ��
vector<string> input_test_items() {
	vector<string> items(MAXLEN - 2);
	for (int i = 0; i < MAXLEN - 2; i++) {
		cin >> items[i];
	}
	return items;
}

//Ԥ��
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
	vector<vector<string>>remain_state;		//����һ���ܼ�¼��ʵ���г��ֵ��������Եļ���
	for (unsigned int i = 0; i < state.size(); i++){
		remain_state.push_back(state[i]);
	}
	ComputeMapFrom2DVector();		//������ʵ������󣬽��������е����Եĳ��ֵ�����ȡֵ��¼��������map��ʽ����
	root = BuildDecisionTreeDFS(root, remain_state, remain_attribute);		//**�ص�
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




