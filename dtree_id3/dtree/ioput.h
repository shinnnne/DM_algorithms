#ifndef IOPUT_H
#define IOPUT_H

#include<iostream>
#include<cstring>
#include<vector>
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



#endif