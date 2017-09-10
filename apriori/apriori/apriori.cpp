#include<iostream>
#include<string>
#include<fstream>
#include<cstdio>
#include<time.h>
#include<vector>
using namespace std;

#define maxsize 52
string name[maxsize - 1];
clock_t  start;
clock_t finish;

typedef struct initag_transaction
{
	int tagnum;	 // 每组事务的项目个数
	int tag[maxsize] = {};	  //每组事务的项目
}initag_trans, *pinitag_trans;

typedef struct iniitem_transaction		//项目集设置为带有头结点的链表
{
	initag_trans *item = new initag_trans; //每次产生的集合
	struct iniitem_transaction *next = nullptr;
	int itemnum = 0; //每次产生的集合中事务组的个数
}iniitem_trans, *piniitem_trans;

//项目集的基本格式
typedef struct tag_transaction
{
	int tagnum;	 // 每组事务的项目个数
	vector<int> tag ;	  //每组事务的项目
	int frerate = 0;	
	float cfd = 1;
	float lift = 1;
}tag_trans, *ptag_trans;

typedef struct item_transaction		//项目集设置为带有头结点的链表
{
	tag_trans *item = new tag_trans; //每次产生的集合
	struct item_transaction *next = nullptr;
	struct item_transaction *parent = nullptr;
	int itemnum = 0; //每次产生的集合中事务组的个数

}item_trans, *pitem_trans;

item_trans * initialItem()	//初始化链表结点
{
	item_trans *p;
	p = new item_trans;
	p->next = nullptr;
	p->itemnum = 0;
	return p;
}
//debugged获取初始化数据
iniitem_trans * get_init()
{
	string filename = "123.txt";
	//cout << "text the filename: ";
	//cin >> filename;
	ifstream infile;
	infile.open(filename.data());
	if (!infile.is_open())
	{
		cout << "failed opening text" << endl;
		return nullptr;
	}
	iniitem_trans *ini = new iniitem_trans;
	ini->next = nullptr;
	iniitem_trans *u, *R = ini;
	int i = 1, j;
	while (!infile.eof())
	{
		while (i < maxsize)
		{
			infile >> name[i++];
		}
		u = new iniitem_trans;
		for (j = 0; j < maxsize; j++)
		{
			infile >> u->item->tag[j];
		}
		u->item->tagnum = maxsize - 1;
		u->next = nullptr;
		R->next = u;
		R = u;
	}
	R->next = nullptr;
	ini->item->tagnum = maxsize - 1;

	return ini;
}
//打印初始数据
void print_init(iniitem_trans *ini)
{
	if (ini == nullptr||ini->next == nullptr )
	{
		cout << "initial list is empty!" << endl;
		return;
	}
	iniitem_trans *p = ini->next;
	cout << "***************** initial list *****************" << endl;
	int i = 1, j = 1;
	while (p != nullptr)
	{
		cout << "customer_" << (j++) << ": [";
		for (i = 1; i <= p->item->tagnum; i++)
		{
			if (p->item->tag[i])
				cout << name[i] << " ";
		}
		cout << "]" << endl;
		p = p->next;
	}
	return;
}
//打印频繁集
void print_f(item_trans *f_k)
{
	if (f_k->next == nullptr)
	{
		cout << "empty print" << endl;
		return;
	}
	item_trans *u = f_k->next;
	while (u->next != nullptr)
	{
		cout << "[ ";
		vector<int>::iterator it;
		for (it=u->item->tag.begin();it!=u->item->tag.end();it++)
		{
			cout << name[*it] << " ";
		}
		cout << "] ";
		cout << "fre_rate: " << u->item->frerate << "  confidence: "<<u->item->cfd<<"  lift: "<<u->item->lift<<endl;
		u = u->next;
	}
	cout << endl;
	return;
}
//频繁项计数
item_trans * count_item(iniitem_trans *ini, item_trans *f_next, int* fre_array)  //频繁项计数
{
	if (f_next->item == nullptr || f_next->next == nullptr)
	{
		cout << "the fk list is empty" << endl;
		return nullptr;
	}
	iniitem_trans *item_ini;
	item_trans *item_f = new item_trans;
	item_f = f_next->next;
	item_trans *u;
	while (item_f != nullptr)
	{
		u = new item_trans;
		u = item_f;
		int count = 0;
		item_ini = ini->next;
		while (item_ini != nullptr)		//遍历当前产生的候选集中的每一个商品组合
		{

			int flag = 1;
			vector<int>::iterator it;
			for (it=item_f->item->tag.begin();it!=item_f->item->tag.end();it++)
			{
				if (item_ini->item->tag[*it] == 0)
				{
					flag = 0;
					break;
				}
			}
			if (flag == 1)
			{
				count++;
			}
			item_ini = item_ini->next;
		}
		u->item->frerate = count;
		if (u->parent)
		{
			u->item->cfd = (float)count / (float)u->parent->item->frerate;
			u->item->lift = u->item->cfd / ((float)fre_array[u->item->tag.back()] / (float)(maxsize - 1));
		}

		item_f = u;
		item_f = item_f->next;
	}
	return f_next;
}
//finish
tag_trans *subset(tag_trans *tag_c, int m)	//生成不包含m项的子集
{
	tag_trans *u = new tag_trans;
	int i = 1;
	vector<int>::iterator it;
	for (it=tag_c->tag.begin(); i <= tag_c->tagnum; it++,i++)
	{
		if (i == m)
			continue;
		u->tag.push_back(*it);
	}
	return u;
}
//查找某一项是否在上一频繁项集中存在-
int find_in_f(item_trans *f_k, vector<int> item_search)		//在频繁项集中找某项是否存在
{
	item_trans *item_f = f_k->next;
	while (item_f != nullptr)
	{
		int flag = 1;
		vector<int>::iterator it, pt;
		for(it=item_f->item->tag.begin(),pt=item_search.begin();pt!=item_search.end();it++,pt++)
		{
			if (*it != *pt)
			{
				flag = 0;
				break;
			}
		}
		if (flag == 1)
		{
			return 1; 
		}
		item_f = item_f->next;
	}
	return 0;
}
//删除候选集中不适合的项-devised
item_trans *delete_item(item_trans *c_next, item_trans *fItem)	//实质上为链表的删除操作
{
	if (c_next->item == nullptr || c_next->next == nullptr)
	{
		return nullptr;
	}
	item_trans *p = new item_trans;
	p = c_next;
	while (p->next != fItem)
	{
		p = p->next;
	}
	p->next = fItem->next;
	c_next->itemnum--;
	return c_next;
}
//1-频繁项集
item_trans *f_1(iniitem_trans *ini, int fre_rate,int *fre_array)
{
	item_trans *c_1 = new item_trans;
	item_trans *item_c = new item_trans;
	c_1 = item_c;
	item_c->next = nullptr;
	item_trans *u;

	for (int i = 1; i < maxsize; i++)	//生成1项候选集
	{
		u = new item_trans;
		u->item->tag.push_back(i);
		c_1->itemnum++;
		u->item->tagnum = 1;
		u->next = nullptr;
		item_c->next = u;
		item_c = u;
	}
	c_1 = count_item(ini, c_1,fre_array);
	item_trans *item = c_1->next;
	while (item != nullptr)	//剪枝操作
	{
		fre_array[item->item->tag.front()] = item->item->frerate;
		if (item->item->frerate < fre_rate)
		{
			c_1 = delete_item(c_1, item);
		}
		item = item->next;
	}
	return c_1;
}

//2-频繁项集
item_trans *f_2(iniitem_trans *ini, item_trans *f_1, int fre_rate, int* fre_array)
{
	item_trans *c_2 = initialItem();
	item_trans *item_c = new item_trans, *item_f1 = f_1->next;
	item_trans *u;
	item_c = c_2;
	while (item_f1 != nullptr)
	{
		item_trans *item_cmp = item_f1->next;
		while (item_cmp != nullptr)
		{
			u = new item_trans;
			u->item->tag.push_back(item_f1->item->tag.front());
			u->item->tag.push_back(item_cmp->item->tag.front());
			u->item->tagnum = 2;
			u->parent = item_f1;
			c_2->itemnum++;
			item_c->next = u;
			item_c = u;
			item_cmp = item_cmp->next;
		}
		item_f1 = item_f1->next;
	}
	c_2 = count_item(ini, c_2,fre_array);
	item_trans *f = c_2->next;
	while (f != nullptr) 	//剪枝
	{
		if (f->item->frerate < fre_rate)
			c_2 = delete_item(c_2, f);
		f = f->next;
	}
	return c_2;
}

//k-项频繁集生成k+1-项集候选集
item_trans *gen_trans(item_trans *f_k, int k, int fre_rate, iniitem_trans *ini, int* fre_array)
{
	if (k < 2)
		return nullptr;
	item_trans *c_next = new item_trans;
	c_next->item->tagnum = k + 1;
	item_trans *fItem = f_k->next, *cnext_item = new item_trans, *u;
	cnext_item = c_next;
	while (fItem->next != nullptr)
	{
		item_trans *fItem_cmp = fItem->next;
		//生成未剪枝的k+1-候选集
		while (fItem_cmp != nullptr)
		{
			u = new item_trans;
			if (fItem->item->tag.back() != fItem_cmp->item->tag.back())
			{
				int flag = 1;
				vector<int>::iterator it,pt;
				for (it=fItem->item->tag.begin(),pt=fItem_cmp->item->tag.begin();	//判断前k-1项是否相等，相等则生成新的
						it!=fItem->item->tag.end()-1;it++,pt++)
				{
					if (*it != *pt)
					{
						flag = 0;
						break;
					}
					if (*it < *pt)
						flag = 2;
					u->item->tag.push_back(*it);
				}
				if (flag == 1)	//如果前k-1位相同
				{
					u->item->tag.push_back(*it);
					u->item->tag.push_back(*pt);		//将k和k+1位设置为两个频繁集分别的末尾两位
					u->item->tagnum = k + 1;
					u->parent = fItem;
					u->next = nullptr;
					cnext_item->next = u;
					cnext_item = u;
					c_next->itemnum++;
				}
				if (flag == 2)
					break;
			}
			fItem_cmp = fItem_cmp->next;
		}
		fItem = fItem->next;
	}
	//将生成的候选集剪枝（根据生成的子集判断）
	item_trans *d = new item_trans;
	d = c_next->next;
	while (d != nullptr)
	{
		for (int j = 1; j <= fItem->item->tagnum; j++)	//每个k+1-项目集生成不同（不含第j项的）k-子集
		{
			tag_trans *sub_set = new tag_trans;
			sub_set = subset(d->item, j);
			if (find_in_f(f_k, sub_set->tag) == 0)		//如果生成的子集在上一个频繁项集中找不到则立刻在k+1-候选项中删除该项并结束此项的生成子集
			{
				c_next = delete_item(c_next, d);
				break ;
			}
		}
		d = d->next;
	}
	if (c_next->itemnum == 0)
		return nullptr;
	c_next = count_item(ini, c_next, fre_array);
	d = c_next->next;
	while (d != nullptr)
	{
		if (d->item->frerate < fre_rate)
			c_next = delete_item(c_next, d);
		d = d->next;
	}
	if (c_next->itemnum == 0)
		return nullptr;
	return c_next;
}

int main()
{
	iniitem_trans *ini = get_init();
	int fre_array[maxsize] = {};
	if (ini != nullptr)
	{
		print_init(ini);
		cout << endl << "decide a requested fre_rate: ";
		int k = 1, fre_rate = 1;
		cin >> fre_rate;
		item_trans *fk = new item_trans;
		fk = f_1(ini, fre_rate,fre_array);
		start = clock();
		item_trans *u = new item_trans;
		while (fk != nullptr)
		{
			cout << "***************** fre-" << k++ << " list ******************" << endl;
			print_f(fk);
			u = new item_trans;
			if (k == 2)
				u = f_2(ini, fk, fre_rate, fre_array);
			else if (k > 2)
				u = gen_trans(fk, k - 1, fre_rate, ini, fre_array);
			fk = u;
		}
		finish = clock();
		double dur = (double)(finish - start);
		cout << endl << "done!" << endl;
		cout << "use time: " << (dur / CLOCKS_PER_SEC)<<endl;

	}
	cout << "done!" << endl;
	system("pause");
	return 0;
}
