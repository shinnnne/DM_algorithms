#ifndef IOPUT_H
#define IOPUT_H

#include<iostream>
#include<cstring>
#include<vector>
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



#endif